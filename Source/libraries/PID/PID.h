#ifndef PID_H
#define PID_H
#include "Arduino.h"  //for millis

class PID {
	public:
	PID();

	/*working variables*/
	int timePast;
	int output, target;
	int errSum,error,lastErr;
	int outMin, outMax;
	float kp,ki,kd; // 
	int speed,prevSpeed,tarSpeed,dSpeed;
 
	void setInput( int tmp );
	void setPID( float p, float i, float d );
	void setTarget(int tmp);
	void setOutputLimits( int Min, int Max );
	int compute( int direction );  

	void setMotorspeed ( int motor );
	void currentSpeed( int L0R1[], int direction );


	private:

};

//K_p = P	momenatner Wert des Fehlers
//K_i = I	früherer Wert des Fehler
//K_d = D	eventueller zukuenftiger Wert des Fehlers

#endif
