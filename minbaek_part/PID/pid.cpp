#include <iostream>

int main(void){

  float data;
  float scale;
  float error_total;
  float steering
  float goal=0;
  float p_error=0;
  float error=0;
  float error_integral=0;
  float filtering=0;
  //initial value setting

  float cycle_time=0.001;
  //1cycle time setting

  float alpha=0.05;
  //filtering alpha value

  float Kp=6;
  float Ki=1.5;
  float Kd=0.01;
  //P, I, D setting

  float input; // recieve input value

  while(1){
    scale = input;
    filtering = (1-alpha)*filtering+alpha*scale; //filtering

    data=filtering; //필터링 된 data

    error=data-goal; //present value - 0

    error_total=Kp*error+Ki*error_integral+Kd*(p_error-error)/cycle_time;
    error_integral=error_integral+error*cycle_time;
    p_error = error;

    steering = 1100 + error_total*400; //scale 변화 상수 조절 // out input value
  }
}
