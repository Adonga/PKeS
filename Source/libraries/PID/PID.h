#ifndef PID_H
#define PID_H
#include "Arduino.h"

class PID {
	public:
	PID();

	/*working variables*/
	unsigned long lastTime;
	double Input, Output, Setpoint;
	double ITerm, lastInput;
	double kp, ki, kd;
	int SampleTime = 1000; //1 sec
	double outMin, outMax;
	bool inAuto = false;
 
	#define MANUAL 0
	#define AUTOMATIC 1
 
	void Compute();  
	void setInput(int tmp);
	void setpoint(int tmp);
	void SetTunings(double Kp, double Ki, double Kd);
	void SetSampleTime(int NewSampleTime);
	void SetOutputLimits(double Min, double Max);
	void SetMode(int Mode);
	void Initialize();


	private:
};


#endif
