/* Copyright - Nubos Networks
 *
 * Author - Sri
 */

#ifndef __VTL_THREAD_H_
#define __VTL_THREAD_H_

#include <pthread.h>
#include <iostream>
#include <memory>


#define VTL_RTT_PRIO  99
#define VTL_NRT_PRIO  0
typedef void *(*VtlThreadFunc_t)(void *);
typedef void  (*VtlThreadExit_t)(void *);

//generic dummy function in case not passed
//by caller 
void thread_exit_handler(void *data);
/* do not define instance for this class
 * used by other classes. See below
 */
class VtlThread {

public:
    VtlThread(int priority,
              int scope,
              int sched_policy,
              int control):
              mPriority(priority),
              mScope(scope),
              mSchedPolicy(sched_policy),
              mControl(control),
              mDie(0),
              mThdCallBack(NULL),
              mThdData(NULL),
              mThdExitCallBack(thread_exit_handler),
              mThdXData(NULL) {}

    virtual ~VtlThread() { stop(); }

    //definition to avoid instance for this class
    virtual pthread_t getThreadID() = 0;


    //inits the class instance
    int init(); 

    //starts the thread on given function
    // @func : func to callback
    // @data : data to be passed to func callback
    // @crtl : whether the thread control is within in @func or 
    //         to be handled by this class instance
    // @xfunc: exit handler func installed for this thread
    //         which is called when thread is exiting
    // @xdata: exit handler func data to be passed as param
    //
    // NOTE - when @crtl is true @xfunc and @xdata are not 
    //        used
    int start(VtlThreadFunc_t func, 
              void *data, 
              bool crtl,   
              VtlThreadExit_t xfunc,
              void *xdata);

    //main function that controls thread
    void main();

    //stops the thread
    void stop();
    
protected:

    int  mPriority;   /* priority of thread */
    int  mScope;      /* schedule scope, system wide or process wide */
    int  mSchedPolicy;/* schedule policy */
    int  mControl;    /* detach or attach the thread to parent */
    int  mDie;        /* indicates whether thread should die */

    pthread_t mTid; 
    pthread_attr_t mThdAttr;

    //the following variables are used when crtl is FALSE
    VtlThreadFunc_t mThdCallBack;
    void *mThdData;

    VtlThreadExit_t mThdExitCallBack;
    void *mThdXData;
};

typedef std::unique_ptr<VtlThread> VtlThreadPtr_t;

/* please be conservative in creating these RT
 * threads
 *
 * create this instance if needed for real time
 * data processing.
 *
 * usage - calling getVtlRTThreadIns() would return
 *         instantiated instance 
 * Postconditions : thread is created when instance is
 *                  returned by function
 */
class VtlRTThread : public VtlThread {

private:
    //VTL_RTT_PRIO is the highest real time priority
    //schedule contention is system wide
    //realtime schedule algorithm is FIFO
    //create thread to be detached from parent
    VtlRTThread():VtlThread(VTL_RTT_PRIO,
                            PTHREAD_SCOPE_SYSTEM,
                            SCHED_FIFO,
                            PTHREAD_CREATE_DETACHED) {}

public:
    ~VtlRTThread() {}

    pthread_t getThreadID() { return mTid; }

    static VtlThreadPtr_t getVtlRTThreadIns(VtlThreadFunc_t func,
                                            void *data, 
                                            bool crtl,
                                            VtlThreadExit_t xfunc = NULL,
                                            void *xdata = NULL);
};


/* create this thread for normal tasks
 * usage - calling getVtlFairThreadIns() would return
 *         instantiated instance 
 * Postconditions : thread is created when instance is
 *                  returned by function
 */
class VtlFairThread : public VtlThread {

private:
    //VTL_NRT_PRIO priority is start of complete fair scheduling
    //schedule contention is process wide
    //schedule algorithm is CFS - i.e other
    //create thread to be attached to parent
    VtlFairThread():VtlThread(VTL_NRT_PRIO,
                              PTHREAD_SCOPE_SYSTEM,
                              SCHED_OTHER,
                              PTHREAD_CREATE_JOINABLE) {}
public:
    ~VtlFairThread() {}

    pthread_t getThreadID() { return mTid; }

    static VtlThreadPtr_t getVtlFairThreadIns(VtlThreadFunc_t func,
                                              void *data,
                                              bool crtl,
                                              VtlThreadExit_t xfunc = NULL,
                                              void *xdata = NULL);
};


#endif
