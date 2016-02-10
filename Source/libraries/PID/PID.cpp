#include "PID.h"

PID::PID( int p, int d, int i ) :
    m_p( p ), m_d( d ), m_i( i )
{

}

void PID::setTarget( int target, int firstValue )
{
    m_lastValue = firstValue;
    m_target    = target;
}

void PID::setP(int p)
{
    m_p = p;
}

void PID::setD(int d)
{
    m_d = d;
}

void PID::setI(int i)
{
    m_i = i;
}

int16_t PID::result( int current )
{
    int16_t diff = m_target - current;

//		Serial.println(diff);
	 
  	m_integral += diff;
	
	int res=	m_i * (m_integral) +
				m_d * ( diff - m_lastValue ) +
				m_p * ( diff );
	
    return res/100;
}

int PID::target() const
{
    return m_target;
}
