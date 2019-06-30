/*
 * Copyright - Nubos Networks
 *
 * Author - Sri
 *
 */

#ifndef _VTL_IP_HDR_H_
#define _VTL_IP_HDR_H_

#include <stdint.h>
#include <netinet/ip.h>

#ifdef __cplusplus
extern "C" {
#endif

#if 0
struct iphdr {
    uint8_t  verihl;    
    uint8_t  tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t  ttl;
    uint8_t  protocol;
    uint16_t checksum;
    uint32_t saddr;
    uint32_t daddr;
    /* optional fields start from here */
};
#endif


static inline uint8_t get_ip_version(const struct iphdr *ip)
{
    return ip->version;
}

static inline void set_ip_version(struct iphdr *ip, uint8_t ver)
{
    ver &= 0x0f;
    ip->version = ver;
}

static inline uint8_t get_ip_ihl(const struct iphdr *ip)
{
    return ip->ihl; 
}

static inline void set_ip_ihl(struct iphdr *ip, uint8_t ihl)
{
    ihl &= 0x0f;
    ip->ihl = ihl;
}

static inline uint8_t get_ip_dscp(const struct iphdr *ip)
{
    return (ip->tos & 0x3f); 
}

static inline void set_ip_dscp(struct iphdr *ip, uint8_t dscp)
{
    dscp &= 0x3f;
    ip->tos &= 0xc0;
    ip->tos |= dscp;
}

static inline uint8_t get_ip_ecn(const struct iphdr *ip)
{
    return (ip->tos >> 6);
}

static inline void set_ip_ecn(struct iphdr *ip, uint8_t ecn)
{
    ip->tos &= 0x3f;
    ip->tos |= (ecn << 6);
}

static inline uint16_t get_ip_totlen(const struct iphdr *ip)
{
    return ip->tot_len;
}

static inline void set_ip_totlen(struct iphdr *ip, uint16_t tot_len)
{
    ip->tot_len = tot_len;
}

static inline uint16_t get_ip_identification(const struct iphdr *ip)
{
    return ip->id;
}

static inline void set_ip_identification(struct iphdr *ip, uint16_t id)
{
    ip->id = id;
}

#pragma message "Error - frag_off of tcp field does not consider endianess"
#pragma message "Change following code on frag_off to consider big endianess to parse values"
static inline uint8_t get_ip_flags(const struct iphdr *ip)
{
    return (ip->frag_off & 0x07);
}

static inline void set_ip_flags(struct iphdr *ip, uint16_t flags)
{
    flags &= 0x07;
    ip->frag_off &= 0xfff8;
    ip->frag_off |= flags;
}

static inline uint16_t get_ip_fragoffset(const struct iphdr *ip)
{
    return (ip->frag_off >> 3);
}

static inline void set_ip_fragoffset(struct iphdr *ip, uint16_t frag_offset)
{
    ip->frag_off &= 0x07;
    ip->frag_off |= (frag_offset << 3);
}

static inline uint8_t get_ip_ttl(const struct iphdr *ip)
{
    return ip->ttl;
}

static inline void set_ip_ttl(struct iphdr *ip, uint8_t ttl)
{
    ip->ttl = ttl;
}

static inline uint8_t get_ip_protocol(const struct iphdr *ip)
{
    return ip->protocol;
}

static inline void set_ip_protocol(struct iphdr *ip, uint8_t protocol)
{
    ip->protocol = protocol;
}

static inline uint16_t get_ip_checksum(const struct iphdr *ip)
{
    return ip->check;
}

static inline void set_ip_checksum(struct iphdr *ip, uint16_t checksum)
{
    ip->check = checksum;
}

static inline uint32_t get_ip_saddr(const struct iphdr *ip)
{
    return ip->saddr;
}

static inline void set_ip_saddr(struct iphdr *ip, uint32_t saddr)
{
    ip->saddr = saddr;
}

static inline uint32_t get_ip_daddr(const struct iphdr *ip)
{
    return ip->daddr;
}

static inline void set_ip_daddr(struct iphdr *ip, uint32_t daddr)
{
    ip->daddr = daddr;
}

#ifdef __cplusplus
};
#endif

#endif
