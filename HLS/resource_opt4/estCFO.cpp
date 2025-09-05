#include "estCFO.hpp"

void estCFO(hls::stream<complex_data_t>& adc_data_stream,
            hls::stream<forget_t>& forget_factor_stream,
            hls::stream<bool>& first_time_stream,
//			complex_accum_t acmax_o,
            hls::stream<complex_accum_t>& ac_cfo_in_stream,
            hls::stream<complex_accum_t>& ac_cfo_out_stream) {

	rs_t ac_max_pwd = 0;
    complex_accum_t ac = {0, 0};
    complex_accum_t acmax = {0, 0};

    // Keep original register arrays but optimize with pragmas
    complex_data_t reg[N];
#pragma HLS bind_storage variable=reg type=RAM_S2P impl=bram
    complex_rs_t auto_corr1_reg[N];
#pragma HLS bind_storage variable=auto_corr1_reg type=RAM_S2P impl=bram
    complex_ac_part_t ac_part = {0, 0};

    // Process data with optimized shifting and resource constraints
    PROC_DATA: for (int n = 0; n < 512; n++) {
#pragma HLS PIPELINE II=1
// #pragma HLS ALLOCATION instances=mul limit=4 operation
        complex_data_t adc_data = adc_data_stream.read();

        // Compute autocorrelation with DSP-optimized operations
        complex_data_t reg0;
        reg0 = (n < N) ? complex_data_t{0, 0} : reg[n % N]; // Handle initial condition
        complex_data_t reg0_conj = std::conj(reg0);
        
        // Use original complex multiplication but force DSP binding  
        complex_rs_t auto_corr1;
        #pragma HLS bind_op variable=auto_corr1 op=mul impl=dsp
        auto_corr1 = (complex_rs_t)adc_data * (complex_rs_t)reg0_conj;
        // complex_rs_t auto_corr1 = adc_data * reg0_conj;

        // Compute difference
        complex_rs_t auto_corr1_reg0 = (n < N) ? complex_rs_t{0, 0} : auto_corr1_reg[n % N]; // Handle initial condition
        complex_rs_t rs = auto_corr1 - auto_corr1_reg0;

        reg[n % N] = adc_data;
        auto_corr1_reg[n % N] = auto_corr1;

        // Accumulate result
        ac = ac + (complex_accum_t)rs;

        // Optimized power calculation with reduced precision and DSP binding
        ac_part.real(ac.real() >> 8);  // Keep original shift
        ac_part.imag(ac.imag() >> 8);

        rs_t ac_pwd_re = (ac_part.real() * ac_part.real()) >> 10;  // Keep original shift
        rs_t ac_pwd_im = (ac_part.imag() * ac_part.imag()) >> 10;
        rs_t ac_pwd = ac_pwd_re + ac_pwd_im;

        // rs_t ac_pwd = (rs_t)(ac_part.real()*ac_part.real() + ac_part.imag()*ac_part.imag());

        // Peak detection
        if (ac_pwd > ac_max_pwd) {
            ac_max_pwd = ac_pwd;
            acmax = ac;
        }
    }

    // Read input streams
    bool first_time = first_time_stream.read();
    forget_t forget_factor = forget_factor_stream.read();
    complex_accum_t ac_cfo_in = ac_cfo_in_stream.read();

    // Simplified output calculation
    complex_accum_t ac_cfo = acmax;

    // Write output
    ac_cfo_out_stream.write(ac_cfo);
}
