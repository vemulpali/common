/*
 * Copyright - Nubos Networks
 *
 * Author - Sri
 *
 */

#ifndef _VTL_CTXID_DEF_H_
#define _VTL_CTXID_DEF_H_

/* NOTE NOTE -----
 * In VTL, transport id becomes app id for conceptual
 * understanding. All consumer apps that binded to
 * transport to talk to provider of that transport,
 * their identities will be disappeared. So transport
 * becomes apps id. Vice versa to provider apps.
 * logical reasoning to keep ---
 */

/* ctx_id aka virtual network identifier is 24-bits
 * trans_id aka transport id on which app communicates - 12-bits
 * srv_id aka service id to which app belongs as part of service - 10-bits
 * port func aka port role is 2-bits

 * LSB <--------- 32 bits -----------> MSB
 * ----------------------------------------
 * | 2-bits | 12-bits | 10-bits | 00000000|
 * ----------------------------------------
 * |port fun|trans id | srv id  |
 *
 * NOTE - we do not add role to these two
 *        last 2-bits in ctx_id for now.
 */
#define SRV_CTX_SHIFT_LEN    14
#define TRANS_CTX_SHIFT_LEN  2
#define GET_TRANS_ID_VAL(vnid) ((vnid >> TRANS_CTX_SHIFT_LEN) & 0xFFF)
#define GET_SRV_ID_VAL(vnid)   ((vnid >> SRV_CTX_SHIFT_LEN)   & 0x3FF)
#define PREPARE_CTX_ID(trans_id, srv_id) ((srv_id & 0x3FF)   << SRV_CTX_SHIFT_LEN  ) | \
                                         ((trans_id & 0xFFF) << TRANS_CTX_SHIFT_LEN)

#endif
