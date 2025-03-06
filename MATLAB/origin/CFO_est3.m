% Calculate auto-correlation and noise power over 2 training sequences for acquisition (coarse synchronisation)
% the data type:  
% adc_data: float
% forget_factor: 24 bits, range from 0 to 1, the max is 32768 match to 1
% fist_time_or_not: one bit
% ac_cfo_in : float
function [ac_cfo] = CFO_est3(adc_data,forget_factor,fist_time_or_not,ac_cfo_in)

    N = 128;

    ac_max_pwd = 0;
    ac = 0;
    reg = zeros(1,2*N+1); % define local buffer
    auto_corr1_reg = zeros(1,1*N+1); % define local buffer
    for n = 1:16*32
        % Shift register 
        reg = [reg(2:end),adc_data(1,n)];

        auto_corr1 = reg(end)*conj(reg(end-N));
        auto_corr1_reg = [auto_corr1_reg(2:end),auto_corr1];
        rs = auto_corr1 - auto_corr1_reg(1);
                
        ac = ac + rs;  
        ac_pwd = ac*conj(ac);
        if (ac_pwd > ac_max_pwd)
            ac_max_pwd = ac_pwd;
            acmax = ac;

        end

    end

    if fist_time_or_not == 1
        ac_cfo = (acmax );   
    else
        ac_cfo = ac_cfo_in + forget_factor*(acmax - ac_cfo_in);   
    end
end
