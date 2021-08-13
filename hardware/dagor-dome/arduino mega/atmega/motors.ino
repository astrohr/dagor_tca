void motors_loop()
{
    if (input_buffer.start_goto){
        pid_input = status_buffer.current_azimuth;
        pid_setpoint = input_buffer.target_azimuth;
        dome_pid.Compute();
        if (pid_output < 0){
          input_buffer.dome_speed = -pid_output;
          _motors.rotation = DOWN;
        }
        if (pid_output > 0){
          input_buffer.dome_speed = pid_output;
          _motors.rotation = UP;
        }
        if (abs(input_buffer.target_azimuth - status_buffer.current_azimuth) < 5)
          input_buffer.start_goto = false;
    } else {
      if (input_buffer.direction == UP)
            _motors.rotation = UP;
      if (input_buffer.direction == DOWN)
            _motors.rotation = DOWN;
      if (input_buffer.stop)            
            _motors.rotation = STOP;
    }

    
    if (_motors.rotation == DOWN) {
        setDomeSpeed(input_buffer.dome_speed);
        digitalWrite(motor_up, LOW);
        digitalWrite(motor_down, HIGH);
    } else if (_motors.rotation == UP) {
        setDomeSpeed(input_buffer.dome_speed);
        digitalWrite(motor_up, HIGH);
        digitalWrite(motor_down, LOW);
    } else  {
        setDomeSpeed(0);
        digitalWrite(motor_up, LOW);
        digitalWrite(motor_down, LOW);
    }
}

void setDomeSpeed(int speed_val) {
  float speedPWM = speed_val*255/100;
  analogWrite(motor_speed, round(speedPWM));
}
