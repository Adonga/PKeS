#ifndef PID_H
#define PID_H
#include "Arduino.h"  //for millis

class PID {
	public:
	PID( int p, int d, int i );

		void setTarget( int target, int firstValue ); //target == richtung die man fahren will
    void setP(int p);
    void setD(int d);
    void setI(int i);

    int result( int current );

    int target() const;

	private:
	int m_p;
    int m_d;
    int m_i;

    int m_target    = 0;
    int m_lastValue = 0;
    int m_integral  = 0;
};

//K_p = P	momenatner Wert des Fehlers
//K_i = I	früherer Wert des Fehler
//K_d = D	eventueller zukuenftiger Wert des Fehlers

#endif
