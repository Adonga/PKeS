#include "PID.h"

PID::PID( float p, float d, float i ) :
    m_p( p ), m_d( d ), m_i( i )
{

}

void PID::setTarget( int target, int firstValue )
{
    m_lastValue = firstValue;
    m_target    = target;
}

void PID::setP(float p)
{
    m_p = p;
}

void PID::setD(float d)
{
    m_d = d;
}

void PID::setI(float i)
{
    m_i = i;
}

int16_t PID::result( int current )
{
    int16_t diff = m_target - current;

//		Serial.println(diff);
	 
  	m_integral += diff;
    return m_i * m_integral +
           m_d * static_cast<float>( diff - m_lastValue ) +
           m_p * static_cast<float>( diff );
}

int PID::target() const
{
    return m_target;
}
