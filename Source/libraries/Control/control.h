#ifndef CONTROL_H
#define CONTROL_H
#include "control.h"
#include "PID.h"
class Control
{
public:

    struct LR
    {
        int leftM;
        int rightM;
    };

    Control( int motor );

    void setTargetDirection( int value , int currentDirection = 0 );
    int targetDirection() const;
    void setMotorspeed(int motor , bool instant = true );
    void setP(float p );
    LR currentSpeed( int currentDir );

    PID & getpid() { return m_pid; }

private:

    int m_motorSpeed      = 0;
    int m_prevSpeed       = 0;
    int m_targetSpeed     = 0;
    int m_speedChangeStep = 0;
    PID     m_pid;

};

#endif
