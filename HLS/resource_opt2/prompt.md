## Optimize the HLS C++ module to reduce FPGA resource usage

You are an expert in FPGA design and proficient in HLS C++ programming. Revise the HLS C++ code in *.cpp to access the data arrays (`reg[N]` and `auto_corr1_reg[N]`) as dual-port block RAM memories, aiming to reduce FPGA LUT resource usage.

## Prompt for generating HLS C++ code

rockyco: You are an expert in FPGA design and proficient in HLS C++ programming. What kind of prompt should I write to make you generate this HLS C++ code to use arrays (`reg[N]`, `auto_corr1_reg[N]`) as FPGA block RAMs?

GitHub Copilot: To generate HLS C++ code that utilizes arrays as FPGA block RAMs, focus on emphasizing the memory access patterns and the need for efficient hardware implementation. Here's a prompt strategy:

1.  **Describe the Algorithm:** Briefly explain the algorithm you're implementing (e.g., auto-correlation for CFO estimation).
2.  **Highlight Key Data Structures:** Clearly state that you're using arrays (`reg[N]`, `auto_corr1_reg[N]`) to store intermediate data.
3.  **Specify Memory Access Patterns:** Describe how these arrays are accessed (e.g., sequential read/write, random access).  Mention if it's a sliding window or circular buffer.
4.  **Demand Block RAM Inference:** Explicitly request that the HLS compiler infer block RAMs for these arrays.
5.  **Mention Performance Goals:** If you have specific performance requirements (e.g., II=1), include them.
6.  **Provide Context:** Give the context of the surrounding code (e.g., data streaming).

Here's an example prompt you could adapt:

"I need to implement an auto-correlation algorithm for carrier frequency offset (CFO) estimation in HLS C++. The design uses streaming input data.  Two arrays, `reg[N]` and `auto_corr1_reg[N]`, are crucial for storing past samples and auto-correlation values, respectively.  These arrays are accessed in a circular buffer manner, with new data overwriting the oldest data.  It is critical that the HLS compiler infers block RAMs (BRAMs) for both `reg[N]` and `auto_corr1_reg[N]` to achieve high performance.  The target is to achieve an initiation interval (II) of 1.  The code should read data from an input stream, perform the auto-correlation, and output the result to another stream."
