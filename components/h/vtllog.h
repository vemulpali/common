/*
 * Copyright - Nubos Networks
 *
 * Author - Sri
 *
 */

#ifndef _VTL_LOG_H_
#define _VTL_LOG_H_

#include <string.h>
#include <syslog.h>

/* debug levels */
#define VTL_DETAIL LOG_DEBUG
#define VTL_INFO   LOG_INFO
#define VTL_DEBUG  LOG_NOTICE /* this would be defauly log level    */
#define VTL_ERR    LOG_ERR    /* err that can be handled gracefully */
#define VTL_ALERT  LOG_ALERT  /* system can be recovered if handled */
#define VTL_EMERG  LOG_EMERG  /* system is out of state             */

#define VTL_LOG_MSG_LEN  300


#define vtl_log_detail(format, ...)  {                                \
                char new_format[VTL_LOG_MSG_LEN + 1];                 \
                int n = snprintf(new_format, VTL_LOG_MSG_LEN, "%s:%d:", __FILE__, __LINE__); \
                strncpy(&new_format[n], format, VTL_LOG_MSG_LEN - n); \
                new_format[VTL_LOG_MSG_LEN] = 0;                      \
                syslog(VTL_DETAIL, new_format, ##__VA_ARGS__); }      

#define vtl_log_info(format, ...) {                                   \
                char new_format[VTL_LOG_MSG_LEN + 1];                 \
                int n = snprintf(new_format, VTL_LOG_MSG_LEN, "%s:%d:", __FILE__, __LINE__); \
                strncpy(&new_format[n], format, VTL_LOG_MSG_LEN - n); \
                new_format[VTL_LOG_MSG_LEN] = 0;                      \
                syslog(VTL_INFO,   new_format, ##__VA_ARGS__); }    

#define vtl_log_debug(format, ...) {                                  \
                char new_format[VTL_LOG_MSG_LEN + 1];                 \
                int n = snprintf(new_format, VTL_LOG_MSG_LEN, "%s:%d:", __FILE__, __LINE__); \
                strncpy(&new_format[n], format, VTL_LOG_MSG_LEN - n); \
                new_format[VTL_LOG_MSG_LEN] = 0;                      \
                syslog(VTL_DEBUG,  new_format, ##__VA_ARGS__); }

#define vtl_log_err(format, ...) {                                    \
                char new_format[VTL_LOG_MSG_LEN + 1];                 \
                int n = snprintf(new_format, VTL_LOG_MSG_LEN, "%s:%d:", __FILE__, __LINE__); \
                strncpy(&new_format[n], format, VTL_LOG_MSG_LEN - n); \
                new_format[VTL_LOG_MSG_LEN] = 0;                      \
                syslog(VTL_ERR,    new_format, ##__VA_ARGS__); }   


#define vtl_log_alert(format, ...) {                                  \
                char new_format[VTL_LOG_MSG_LEN + 1];                 \
                int n = snprintf(new_format, VTL_LOG_MSG_LEN, "%s:%d:", __FILE__, __LINE__); \
                strncpy(&new_format[n], format, VTL_LOG_MSG_LEN - n); \
                new_format[VTL_LOG_MSG_LEN] = 0;                      \
                syslog(VTL_ALERT,  new_format, ##__VA_ARGS__); }

#define vtl_log_emerg(format, ...) {                                  \
                char new_format[VTL_LOG_MSG_LEN + 1];                 \
                int n = snprintf(new_format, VTL_LOG_MSG_LEN, "%s:%d:", __FILE__, __LINE__); \
                strncpy(&new_format[n], format, VTL_LOG_MSG_LEN - n); \
                new_format[VTL_LOG_MSG_LEN] = 0;                      \
                syslog(VTL_EMERG,  new_format, ##__VA_ARGS__); }

#endif
