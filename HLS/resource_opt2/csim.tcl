open_project -reset proj_estCFO
set_top estCFO
add_files estCFO.cpp
add_files -tb estCFO_tb.cpp
add_files -tb ./acCFOKeep_out.txt
add_files -tb ./data4cfoEst_in.txt
open_solution -reset solution1
set_part {xcvu11p-fsgd2104-2-e  }
create_clock -period 1.25 -name default
config_compile -pipeline_loops 1
config_interface -m_axi_addr64=0
config_rtl -reset control
config_dataflow -default_channel fifo -fifo_depth 2
csim_design -clean
exit
