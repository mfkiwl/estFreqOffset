#ifndef ESTCFO_HPP
#define ESTCFO_HPP

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

// Data type definitions
typedef ap_fixed<22, 1> data_t;          // Example: 18-bit fixed-point with 2 integer bits
typedef ap_fixed<24, 3> accum_t;         // 24-bit fixed-point for accumulator
typedef ap_fixed<24, 3> forget_t;        // 24-bit fixed-point for forget factor (0 to 1)
typedef std::complex<data_t> complex_data_t;
typedef std::complex<accum_t> complex_accum_t;
typedef std::complex<float> complex_float_t;

// Parameter definition
const int N = 128;                       // Define N as a constant

#define ITER_NUM 48 // Number of iterations for the loop

// Function declaration
void estCFO(hls::stream<complex_data_t>& adc_data_stream,
            hls::stream<forget_t>& forget_factor_stream,
            hls::stream<bool>& first_time_stream,
            hls::stream<complex_accum_t>& ac_cfo_in_stream,
            hls::stream<complex_accum_t>& ac_cfo_out_stream);

#endif
