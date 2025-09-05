#ifndef ESTCFO_HPP
#define ESTCFO_HPP

#include <complex>
#include <ap_fixed.h>
#include <hls_stream.h>

// Data type definitions
//typedef ap_fixed<22, 1> data_t;          // Example: 18-bit fixed-point with 2 integer bits
//typedef ap_fixed<24, 3> accum_t;         // 24-bit fixed-point for accumulator

// Data type definitions - Conservative optimization for resource usage
typedef ap_fixed<10, 10> data_t;          // Keep original 10 bits
typedef ap_fixed<19, 19> rs_t;           // Keep original 20 bits for accuracy
typedef ap_fixed<23, 23> accum_t;        // Reduced from 28 to 26 bits
typedef ap_fixed<15, 15> ac_part_t;      // Keep original 18 bits


typedef ap_fixed<24, 3> forget_t;        // 24-bit fixed-point for forget factor (0 to 1)
typedef std::complex<data_t> complex_data_t;
typedef std::complex<rs_t> complex_rs_t;
typedef std::complex<accum_t> complex_accum_t;
typedef std::complex<ac_part_t> complex_ac_part_t;
typedef std::complex<float> complex_float_t;

// Parameter definition - Keep original correlation window for accuracy
const int N = 128;                       // Keep original 128 for algorithm accuracy

#define ITER_NUM 48 // Number of iterations for the loop

// Function declaration
void estCFO(hls::stream<complex_data_t>& adc_data_stream,
            hls::stream<forget_t>& forget_factor_stream,
            hls::stream<bool>& first_time_stream,
            hls::stream<complex_accum_t>& ac_cfo_in_stream,
            hls::stream<complex_accum_t>& ac_cfo_out_stream);

#endif
