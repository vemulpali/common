/*
 * Copyright - Nubos Networks
 *
 *
 * Author - Sri
 *
 */

#ifndef _VTL_TIMER_H_
#define _VTL_TIMER_H_

#include <sys/timerfd.h>

#define TIMER_ONCE 1   /* one time timer */
#define TIMER_CONT 2   /* indefinite timer until it is killed */

/* NOTE this class does not implement absolute 
 * timer it only implements relative timer
 */
class VtlTimer {
public:
    virtual ~VtlTimer() {stopTimer();}

    /* dummy pure virtual function 
     * to get type
     */
    virtual int getTimerType() = 0;  

protected:
    VtlTimer():mTimerDesc(-1){}

    int startTimer(int timer_type, 
                   struct timespec spec);

    void stopTimer();

    int mTimerDesc;
};


#endif
