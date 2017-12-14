

#include "PID.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
PID::PID()
{
}

PID::~PID()
{
}

int PID::init()
{
	goal = 0.0f;
	p_error = 0.0f;
	error = 0.0f;
	error_integral = 0.0f;
	filtering = 0.0f;
	return 0;
}

int PID::process(float input)
{
    if(!prescale_flag) {
        pre_scale = input;
        prescale_flag = true;
    }

    scale = input;

    if(std::abs(scale-pre_scale)>15){
        scale=pre_scale;
    }


    filtering = (1-alpha)*filtering+alpha*scale; //filtering

    data=filtering; //필터링 된 data

    error=data-goal; //present value - 0

    error_total=Kp*error+Ki*error_integral+Kd*(p_error-error)/cycle_time;
    error_integral=error_integral+error*cycle_time;
    p_error = error;
	std::cout << "err_integ : "<< error_integral << std::endl;
    steering = error_total*coef;
    if(steering > 2000){
        steering = 2000;
    }
    else if(steering < -2000){
        steering = -2000;
    }
    pre_scale = scale;
    return steering; //scale 변화 상수 조절 // out input value
}
