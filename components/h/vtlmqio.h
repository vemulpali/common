/*
 * Copyright - Nubos Networks
 *
 *
 * Author - Sri
 *
 */

#ifndef _VTL_CRTL_MQ_H_
#define _VTL_CRTL_MQ_H_

#include <vtlio.h>
#include <vtlmq.h>

#define VTL_MQ_NAME_SZ  20

/* NOTE - Do not insert this socket instance
 * into eventhandler class
 */

class MqSocket : public Socket {
public:
    ~MqSocket(){deinitMqSocket();}

    std::unique_ptr<Msg> recv();
    int send(void *msg, int size);
            

    static Socket_t getMqSocketCInstance(const char *name, 
                                          int msgsize);

    static Socket_t getMqSocketOInstance(const char *name, 
                                           int msgsize);

    static Socket_t getMqPrioSocketInstance(const char *name,
                                              int msgsize);

private:
    int create(const char *name, 
               int msgsize);

    int open(const char *name,
             int msgsize);

    //dummy for pure virtual function
    int open();

    MqSocket():mPrio(MID_PRIO) {}

    void setMqPrio() { mPrio = HIGH_PRIO; }

    /* deletes queue */
    void deinitMqSocket();

    vtl_mq_prio_t mPrio;
    int mMsgSize;
    char mName[VTL_MQ_NAME_SZ];
};

#endif
