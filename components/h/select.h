/* Copyright Nubos Networks
 *
 * Author - Sri
 */

#ifndef _VTL_SELECT_C_H_
#define _VTL_SELECT_C_H_

#include <sys/select.h>

#ifdef __cplusplus
extern "C" {
#endif


/* @fd   : descriptor on which event was received
 * @data : thread data that will be given to call back
 */
typedef int (*select_func_t)(int fd, void *data);
#define SELECT_RET 999

#define MAX_READ_FDSET_SZ FD_SETSIZE

/* read select 
 * @fd   : read fdset to listen event on
 * @num_fds : number of fds in array. Should not exceed MAX_READ_FDSET_SZ
 * @func : func to call if any event received on fd set
 *         if  this func returns SELECT_RET thread exits from
 *         blocking mode
 * @data : data param to func
 *
 * Postconditions: This is blocking functon call
 *                 converts all fds to nonblocking mode
 * return -1 if error otherwise never returns
 */
int vtl_read_select(int fd[],
                    int num_fds,
                    select_func_t func,
                    void *data);

#ifdef __cplusplus
};
#endif

#endif //end of guard
