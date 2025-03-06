close all
clear all
N = 128;
forget_factor =1/2;
ac_cfo_in = 0;

ac_keep2 = [];
% load r_to_cfo_test
load r_tb_for_cfo_est
for iter = 1: size(r_tb_for_cfo_est,1)
adc_data = r_tb_for_cfo_est(iter,:);

if iter == 1 
    fist_time_or_not = 1;
else
    fist_time_or_not = 0;
end

    ac_cfo = CFO_est3(adc_data,forget_factor,fist_time_or_not,ac_cfo_in);
    ac_cfo_new = estCFO(adc_data,forget_factor,fist_time_or_not,ac_cfo_in);
    if abs(ac_cfo - ac_cfo_new) > 1e-6
        disp('error')
    end
    ac_cfo_in = ac_cfo;
    ac_cfo_keep(iter,1)  = ac_cfo;

cfo2piTs(iter,1) = angle(ac_cfo)/N;

end    


