#include <iostream>
#include <fstream>
#include <complex>
#include "estCFO.hpp"

int main() {
    // Input and output streams
    hls::stream<complex_data_t> adc_data_stream;
    hls::stream<forget_t> forget_factor_stream;
    hls::stream<bool> first_time_stream;
    hls::stream<complex_accum_t> ac_cfo_in_stream;
    hls::stream<complex_accum_t> ac_cfo_out_stream;

    // Load test data from file (replace with your actual data loading)
    std::ifstream dataFile("data4cfoEst_in.txt"); // Assuming data is in a text file
    if (!dataFile.is_open()) {
        std::cerr << "Error opening data file!" << std::endl;
        return 1;
    }
    // Load golden reference data from file
    std::ifstream refFile("acCFOKeep_out.txt"); // Assuming data is in a text file
    if (!refFile.is_open()) {
        std::cerr << "Error opening reference file!" << std::endl;
        return 1;
    }

    // Test parameters
    forget_t forget_factor = 0.5; // Example value
    complex_accum_t ac_cfo_in = {0.0, 0.0}; // Initial CFO estimate
    // Result data array
    complex_float_t ac_cfo_out_ref[ITER_NUM];
    complex_float_t ac_cfo_out_buf[ITER_NUM];

    // Loop through test iterations
    for (int iter = 0; iter < ITER_NUM; ++iter) { //iter<=iter_nums
        bool first_time = (iter == 0);

        // Read ADC data and push to stream
        for (int n = 0; n < 512; ++n) {
            float real_part, imag_part;
            dataFile >> real_part >> imag_part;
            complex_data_t adc_data(real_part, imag_part);
            adc_data_stream.write(adc_data);
        }

        // Push test parameters to streams
        forget_factor_stream.write(forget_factor);
        first_time_stream.write(first_time);
        ac_cfo_in_stream.write(ac_cfo_in);

        // Call the HLS function
        estCFO(adc_data_stream, forget_factor_stream, first_time_stream, ac_cfo_in_stream, ac_cfo_out_stream);

        // Read the output from the stream
        complex_data_t ac_cfo_out = ac_cfo_out_stream.read();
        ac_cfo_out_buf[iter] = {(float)ac_cfo_out.real(), (float)ac_cfo_out.imag()};
        float real_part_ref, imag_part_ref;
        refFile >> real_part_ref >> imag_part_ref;
        ac_cfo_out_ref[iter] = {real_part_ref, imag_part_ref};

        // Print or store the output for verification
        std::cout << "Iteration " << iter << ": ac_cfo_out = " << ac_cfo_out << std::endl;

        // Update ac_cfo_in for the next iteration
        ac_cfo_in = ac_cfo_out;
    }

    // Compare the output with the golden reference
    int err_cnt = 0;
    for (int i = 0; i < ITER_NUM; ++i) {
        if ((std::abs(ac_cfo_out_buf[i].real() - ac_cfo_out_ref[i].real()) > 1e-4) || (std::abs(ac_cfo_out_buf[i].imag() - ac_cfo_out_ref[i].imag()) > 1e-4)) {
            std::cerr << "Mismatch at iteration " << i << ": ac_cfo_out = " << ac_cfo_out_buf[i] << ", expected " << ac_cfo_out_ref[i] << std::endl;
            ++err_cnt;
        }
    }
    if (err_cnt == 0) {
        std::cout << "Test passed!" << std::endl;
    } else {
        std::cout << "Test failed with " << err_cnt << " mismatches!" << std::endl;
    }

    // Close files
    dataFile.close();
    refFile.close();
    return 0;
}
