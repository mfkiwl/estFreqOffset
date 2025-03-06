close all
N = 128;
forget_factor =1/2;
ac_cfo_in = 0;

ac_keep2 = [];
% load r_to_cfo_test
load r_tb_for_cfo_est
iter_nums = size(r_tb_for_cfo_est,1);
tb_for_cfo_est = r_tb_for_cfo_est.';
tb_for_cfo_est = tb_for_cfo_est / 2^13;
tb_for_cfo_est = reshape(tb_for_cfo_est, [], 1);
writematrix([real(tb_for_cfo_est), imag(tb_for_cfo_est)], 'data4cfoEst_in.txt', 'Delimiter', 'tab');
ac_cfo_keep = zeros(iter_nums,1);
cfo2piTs = zeros(iter_nums,1);
for iter = 1:iter_nums
    adc_data = r_tb_for_cfo_est(iter,:);

    if iter == 1 
        fist_time_or_not = 1;
    else
        fist_time_or_not = 0;
    end

    ac_cfo = estCFO(adc_data,forget_factor,fist_time_or_not,ac_cfo_in);
    ac_cfo_in = ac_cfo;
    ac_cfo_keep(iter,1)  = ac_cfo;

    cfo2piTs(iter,1) = angle(ac_cfo)/N;

end 
acCFOKeep_out = ac_cfo_keep / 2^26;
writematrix([real(acCFOKeep_out), imag(acCFOKeep_out)], 'acCFOKeep_out.txt', 'Delimiter', 'tab');   
