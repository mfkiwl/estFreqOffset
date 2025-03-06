#include "estCFO.hpp"

void estCFO(hls::stream<complex_data_t>& adc_data_stream,
            hls::stream<forget_t>& forget_factor_stream,
            hls::stream<bool>& first_time_stream,
            hls::stream<complex_accum_t>& ac_cfo_in_stream,
            hls::stream<complex_accum_t>& ac_cfo_out_stream) {

    accum_t ac_max_pwd = 0;
    complex_accum_t ac = {0, 0};
    complex_accum_t acmax = {0, 0};
    complex_data_t reg[N] = {complex_data_t{0, 0}};
// #pragma HLS ARRAY_PARTITION variable=reg complete dim=1
    complex_data_t auto_corr1_reg[N] = {complex_data_t{0, 0}};
// #pragma HLS ARRAY_PARTITION variable=auto_corr1_reg complete dim=1

    // // Initialize registers
    // INIT_REG: for (int i = 0; i < N; i++) {
    //     reg[i] = 0;
    //     auto_corr1_reg[i] = 0;
    // }

    // Process data
    PROC_DATA: for (int n = 0; n < 512; n++) {
#pragma HLS PIPELINE II=1
        complex_data_t adc_data = adc_data_stream.read();
        complex_data_t auto_corr1 = adc_data * std::conj(reg[0]);
        complex_data_t rs = auto_corr1 - auto_corr1_reg[0];

        // Update register
        UPDATE_REGS: for (int i = 0; i < N - 1; i++) {
            reg[i] = reg[i + 1];
            auto_corr1_reg[i] = auto_corr1_reg[i + 1];
        }
        reg[N - 1] = adc_data;
        auto_corr1_reg[N - 1] = auto_corr1;

        ac = ac + (complex_accum_t)rs;
        accum_t ac_pwd = ac.real() * ac.real() + ac.imag() * ac.imag();

        if (ac_pwd > ac_max_pwd) {
            ac_max_pwd = ac_pwd;
            acmax = ac;
        }
    }

    bool first_time = first_time_stream.read();
    forget_t forget_factor = forget_factor_stream.read();
    complex_accum_t ac_cfo_in = ac_cfo_in_stream.read();

    complex_accum_t ac_cfo;
    if (first_time) {
        ac_cfo = acmax;
    } else {
        ac_cfo = ac_cfo_in + (complex_accum_t)forget_factor * (acmax - ac_cfo_in);
    }

    ac_cfo_out_stream.write(ac_cfo);
}
