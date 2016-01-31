#include "PID.h"

PID:: PID(){};
void PID::setPID(int p, int i, int d)
{
	kp = p;
	ki = i;
	kd = d;
}

void PID::setInput(int tmp)
{
	Input = tmp;
}

void PID::setTarget( int tmp )
{
  target = tmp;
}

void PID::setOutputLimits( int Min, int Max )
{
   if(Min > Max) return;
   outMin = Min;
   outMax = Max;
 
   if(output > outMax) output = outMax;
   else if(output < outMin) output = outMin;
}

int PID::compute()
{
	int timeNow = millis();
	int dTime = timeNow - timePast;
	
	int error = target - Input;
	errSum += ( error * dTime );
	
	int dErr = (error - lastErr) / dTime;

	output = kp * error + ki * errSum + kd * dErr;

	lastErr = error;
	timePast = timeNow;
	
	output > outMax ? outMax : output;
	output < outMin ? outMin : output;	

	return output;
}
  
void PID::setMotorspeed ( int newSpeed )
{
	speed = newSpeed;
}
void PID::currentSpeed(int L0R1[])
{
	speed = prevSpeed + ( dSpeed * ( tarSpeed - prevSpeed ) / 10 );
	
	dSpeed < 10 ? ++dSpeed : dSpeed;
	

	int reg = PID::compute();
	L0R1[0] = speed - reg;
	L0R1[1] = speed + reg;


	
}
