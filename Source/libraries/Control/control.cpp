#include "control.h"

#define LIN_ACC_STEPS 10

Control::Control( int motor ) :
    m_pid( 0.5, 0.0, 0.0 )
{
    m_motorSpeed = 0;
    setMotorspeed( motor );
}

void Control::setTargetDirection( int value, int currentDirection )
{
    m_pid.setTarget( value, currentDirection );
}

int Control::targetDirection() const
{
    return m_pid.target();
}

void Control::setMotorspeed( int motor, bool instant )
{
    m_prevSpeed = instant ? motor : m_motorSpeed;
    m_targetSpeed = motor;
    m_speedChangeStep = instant ? LIN_ACC_STEPS : 1;
}

void Control::setP(float p)
{
    m_pid.setP( p );
}

Control::LR Control::currentSpeed( int currentDir )
{
	m_prevSpeed = m_motorSpeed;
  m_motorSpeed = m_prevSpeed + (  ( m_targetSpeed - m_prevSpeed )  );
//currentDir = 0;
    if( m_speedChangeStep < LIN_ACC_STEPS )
        ++m_speedChangeStep;

    LR regulatedMotor;

    int reg = m_pid.result( currentDir );

    regulatedMotor.leftM = m_motorSpeed - reg;
    regulatedMotor.rightM = m_motorSpeed + reg;
	

    return regulatedMotor;

}

