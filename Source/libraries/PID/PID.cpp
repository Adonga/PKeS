#include "PID.h"

PID:: PID(){};
void PID::setPID(float p, float i, float d)
{
	kp = p;
	ki = i;
	kd = d;
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

int PID::compute( int direction )
{

	int timeNow = millis();
	int dTime = timeNow - timePast;
	
	int error = target - direction;
	errSum += ( error * dTime );
	
	int dErr = (error - lastErr) / dTime;

	output = kp * error + ki * errSum + kd * dErr;

	lastErr = error;
	timePast = timeNow;
	Serial.println(output);
	if( output > outMax ) { output = outMax; }
	if( output < outMin ) { output = outMin; }	

	return (int)output;
}
  
void PID::setMotorspeed ( int newSpeed )
{
	speed = newSpeed;
}
void PID::currentSpeed( int L0R1[], int direction )
{
	prevSpeed = speed;	
	speed = prevSpeed + ( ( tarSpeed - prevSpeed ) );

	

	int reg = PID::compute( direction );
//	Serial.println(	reg );
	L0R1[0] = (speed + (reg));
	L0R1[1] = (speed - (reg));
	
	
}
