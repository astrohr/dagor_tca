
void pid_loop(){
  double Kp = 0.2; //PID: proportional coefficient
  int desired_dome_speed = (int) Kp*(input_buffer.target_azimuth - status_buffer.current_azimuth);

  //saturation (adjusting for max speed)
  if (desired_dome_speed > 100){
    desired_dome_speed = 100;
  }
  if (desired_dome_speed < -100){
    desired_dome_speed = -100;
  }

  _motors.rotation = desired_dome_speed;
}
