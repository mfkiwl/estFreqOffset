open_project proj_estCFO
set_top estCFO
add_files estCFO.cpp
add_files -tb estCFO_tb.cpp
add_files -tb ./acCFOKeep_out.txt
add_files -tb ./data4cfoEst_in.txt
open_solution solution1
set_part {xcvu11p-fsgd2104-2-e  }
create_clock -period 1.25 -name default
set_clock_uncertainty 12.5%
export_design -flow impl -rtl verilog
exit
