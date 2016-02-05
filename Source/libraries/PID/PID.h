#ifndef PID_H
#define PID_H
#include "Arduino.h"  //for millis

class PID {
	public:
	PID( float p, float d, float i );

		void setTarget( int target, int firstValue ); //target == richtung die man fahren will
    void setP(float p);
    void setD(float d);
    void setI(float i);

    int result( int current );

    int target() const;

	private:
		float m_p;
    float m_d;
    float m_i;

    int m_target    = 0;
    int m_lastValue = 0;
    int m_integral  = 0;
};

//K_p = P	momenatner Wert des Fehlers
//K_i = I	früherer Wert des Fehler
//K_d = D	eventueller zukuenftiger Wert des Fehlers

#endif
