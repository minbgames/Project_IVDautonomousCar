clear; clc; %PID TEST CODE

goal=0;
p_error=0;
error=0;
error_integral=0;
time=1:1000;
filtering=0;
%initial value setting

cycle_time=0.001;
%1cycle time setting

alpha=0.05;
% filtering alpha value

Kp=6;
Ki=1.5;
Kd=0.01;
% P, I, D setting

for x=1:1000
    input(x)=300*(sin(x/100)+0.5*(rand(1)-0.5));
    %input_value = opencv_lane_value
    %잡음의 크기에 따라 rand 상수 조절

    scale(x) = input(x) / 1000; %scale 변화 상수 조절

    filtering = (1-alpha)*filtering+alpha*scale(x); % filtering

    data(x)=filtering; % 필터링 된 data

    error=data(x)-goal; % present value - 0 % scale 상수 변화

    error_total=Kp*error+Ki*error_integral+Kd*(p_error-error)/cycle_time;
    error_integral=error_integral+error*cycle_time;
    p_error = error;

    steering(x) = 1100 + error_total*400; %scale 변화 상수 조절
end

subplot(3,1,1);
plot(time,input);
legend('input value');
grid on;

subplot(3,1,2);
plot(time,scale,time,data);
legend('scale value','filtering value');
grid on;

subplot(3,1,3);
plot(time,steering);
legend('steering');
grid on;

% depend on 1cycle of time / range of opencv value
