/*
 * Copyright - Nubos Networks
 *
 * Author - Sri 
 *
 */


#ifndef _VTL_MQ_H_
#define _VTL_MQ_H_


#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif


#define VTL_MQ_MAX_MSGS 1024

/* vtl priorties for messages
 * to send on queue
 */
typedef enum vtl_mq_prio {
    LOW_PRIO  = 29,
    MID_PRIO  = 30,
    HIGH_PRIO = 31 
} vtl_mq_prio_t;


#define VTL_MQ_ERR -1
#define VTL_QUEUE_ERR -2  /* queue was full cannot send anymore msgs vice versa for recv */  
#define VTL_MSGSZ_ERR -3  /* sending msg larger than configured size vice versa for recv */
#define VTL_NOMSG_ERR -4  /* msg queue has not messages available */

char *vtl_mq_get_errstr(int err, char *str, int size);

/* vtl_create_mq creates named message queue 
 * to communicate with other vsl process
 * @name : name of the queue to create
 * @fd   : file descriptor of the created
 *         mq
 * @msgsize : size of the message 
 * 
 * returns : 0 if successful otherwise -1
 *
 * Postconditions : returns nonblocking fd
 */
int vtl_create_mq(const char *name, int *fd, int msgsize);


/* vtl_open_mq opens the message queue for the
 * calling process
 *
 * @name : name of the queue
 * @fd   : return value, fd of queue
 *
 * Preconditions : name queue must already
 *                 be created
 *
 * returns : 0 if successful otherwise -1.
 *           -1 can be returned if named
 *           message is not already created
 */
int vtl_open_mq(const char *name, int *fd);



/* vtl_delete_mq deletes named message queue
 * @name : name of the message queue
 *
 * returns : 0 if successful otherwise -1
 *
 * postconditions : should not use file
 * descriptor associated to this mq after
 * successful call 
 */
int vtl_delete_mq(const char *name);


int vtl_close_mq(int fd);

/* vtl_send_mq_msg sends message on given mq fd
 * @fd  : message queue fd
 * @msg : message to be sent on queue
 *
 * returns : 0 if successful otherwise vtl_mq_ret_t
 */
int vtl_send_mq_msg(int fd, void *msg, int msgsize);

/* same as vtl_send_mq_msg_priority() but sends at high priority
 * on given @fd queue
 */
int vtl_send_mq_msg_priority(int fd, void *msg,
                             int msgsize, vtl_mq_prio_t prio);

/* vtl_receive_mq_msg receives message on given mq fd
 * @fd  : message queue fd
 * @msg : pointer reference to receive message
 *        
 * returns : size of msg if successful otherwise vtl_mq_ret_t
 */
int vtl_receive_mq_msg(int fd, void *msg, int msgsize);

/* same as vtl_receive_mq_msg_priority() but sends at high priority
 * on given @fd queue
 * NOTE - msgprio has not meaning in sys v message queues
 */
int vtl_receive_mq_msg_priority(int fd, void *msg, int msgsize, unsigned *msgprio);

/* timedwait to receive message from queue */
int vtl_receive_mq_msg_timedwait(int fd, void *msg, int msgsize, struct timeval val);


#ifdef __cplusplus
};
#endif



#endif
