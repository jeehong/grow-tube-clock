/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 * Modifications: Christian Walter <wolti@sil.at>
 */
#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

/* ------------------------ Generic options ------------------------------- */

#define LWIP_DEBUG
#undef  LWIP_DEBUG

#define LWIP_DBG_TYPES_ON				(LWIP_DBG_ON|LWIP_DBG_TRACE|LWIP_DBG_STATE|LWIP_DBG_FRESH|LWIP_DBG_HALT)

/* ------------------------                 ------------------------------- */
#define TCP_LISTEN_BACKLOG              1
//#define LWIP_SO_RCVBUF                  1
#define LWIP_NETCONN                    1
#define LWIP_NETBUF_RECVINFO            1
#define LWIP_NETCONN_SEM_PER_THREAD     1
#define LWIP_NOASSERT
#define LWIP_NOERROR

#ifdef LWIP_DEBUG
	#define LWIP_DBG_MIN_LEVEL		 	0
	#define PPP_DEBUG					LWIP_DBG_OFF
	#define MEM_DEBUG					LWIP_DBG_OFF
	#define MEMP_DEBUG					LWIP_DBG_OFF
	#define PBUF_DEBUG					LWIP_DBG_OFF
	#define API_LIB_DEBUG				LWIP_DBG_OFF
	#define API_MSG_DEBUG				LWIP_DBG_OFF
	#define TCPIP_DEBUG					LWIP_DBG_ON
	#define NETIF_DEBUG					LWIP_DBG_OFF
	#define SOCKETS_DEBUG				LWIP_DBG_OFF
	#define DNS_DEBUG					LWIP_DBG_OFF
	#define AUTOIP_DEBUG				LWIP_DBG_OFF
	#define DHCP_DEBUG					LWIP_DBG_OFF
	#define IP_DEBUG					LWIP_DBG_OFF
	#define IP_REASS_DEBUG				LWIP_DBG_OFF
	#define ICMP_DEBUG					LWIP_DBG_OFF
	#define IGMP_DEBUG					LWIP_DBG_OFF
	#define UDP_DEBUG					LWIP_DBG_OFF
	#define TCP_DEBUG					LWIP_DBG_OFF
	#define TCP_INPUT_DEBUG				LWIP_DBG_OFF
	#define TCP_OUTPUT_DEBUG			LWIP_DBG_OFF
	#define TCP_RTO_DEBUG				LWIP_DBG_OFF
	#define TCP_CWND_DEBUG				LWIP_DBG_OFF
	#define TCP_WND_DEBUG				LWIP_DBG_OFF
	#define TCP_FR_DEBUG				LWIP_DBG_OFF
	#define TCP_QLEN_DEBUG				LWIP_DBG_OFF
	#define TCP_RST_DEBUG				LWIP_DBG_OFF
#endif

/* ------------------------ Core locking -------------------------------- */
#define NO_SYS                          0
#define SYS_LIGHTWEIGHT_PROT            1
#define LWIP_TCPIP_CORE_LOCKING         1
#define LWIP_MPU_COMPATIBLE             0
#define LWIP_TCPIP_CORE_LOCKING_INPUT   1

#define IP_FRAG_MAX_MTU         1500

/* ------------------------ Memory options -------------------------------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT           4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE                (4 * 1024)

#define MEMP_MEM_MALLOC         1
/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#define MEMP_NUM_PBUF           10
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        4
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB        10
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 8
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG        8
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
#define MEMP_NUM_SYS_TIMEOUT    6

/* The following four are used only with the sequential API and can be
   set to 0 if the application only will use the raw API. */
/* MEMP_NUM_NETBUF: the number of struct netbufs. */
#define MEMP_NUM_NETBUF         4
/* MEMP_NUM_NETCONN: the number of struct netconns. */
#define MEMP_NUM_NETCONN        4
/* MEMP_NUM_APIMSG: the number of struct api_msg, used for
   communication between the TCP/IP stack and the sequential
   programs. */
//#define MEMP_NUM_API_MSG        8

/* These two control is reclaimer functions should be compiled
   in. Should always be turned on (1). */
#define MEM_RECLAIM             1
#define MEMP_RECLAIM            1

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE          16

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE       512

/* PBUF_LINK_HLEN: the number of bytes that should be allocated for a
   link level header. */
#define PBUF_LINK_HLEN          16

/* ------------------------ TCP options ----------------------------------- */
#define LWIP_TCP                1
#define TCP_TTL                 255

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         0 
 
/* TCP Maximum segment size. */ 
#define TCP_MSS                 (IP_FRAG_MAX_MTU - 40)
 
/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF             (2 * TCP_MSS)

/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. */
#define TCP_SND_QUEUELEN        (4 * TCP_SND_BUF/TCP_MSS)

/* TCP receive window. */
#define TCP_WND                 (8 * TCP_MSS)


/* Maximum number of retransmissions of data segments. */
#define TCP_MAXRTX              12

/* Maximum number of retransmissions of SYN segments. */
#define TCP_SYNMAXRTX           4

/* ------------------------ ARP options ----------------------------------- */
#define ARP_TABLE_SIZE          10
#define ARP_QUEUEING            1

/* ------------------------ IP options ------------------------------------ */
/* Define IP_FORWARD to 1 if you wish to have the ability to forward
   IP packets across network interfaces. If you are going to run lwIP
   on a device with only one network interface, define this to 0. */
#define IP_FORWARD              1

/* If defined to 1, IP options are allowed (but not parsed). If
   defined to 0, all packets with IP options are dropped. */
#define IP_OPTIONS              1

/* ------------------------ ICMP options ---------------------------------- */
#define ICMP_TTL                255


/* ------------------------ DHCP options ---------------------------------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */
#define LWIP_DHCP               1
#define LWIP_AUTOIP             1
#define LWIP_DHCP_AUTOIP_COOP   1

/* MQTT */
//#define LWIP_ALTCP              1
//#define LWIP_ALTCP_TLS          1

/* 1 if you want to do an ARP check on the offered address
   (recommended). */
#define DHCP_DOES_ARP_CHECK     0

/* ------------------------ UDP options ----------------------------------- */
#define LWIP_UDP                1
#define UDP_TTL                 255

#define LWIP_DNS	            0
#define LWIP_COMPAT_SOCKETS     1
/* ------------------------ Statistics options ---------------------------- */
#define LWIP_STATS				1
#define LWIP_STATS_DISPLAY		0

#if LWIP_STATS
	#define LINK_STATS			1
	#define IP_STATS			1
	#define ICMP_STATS			0//1
	#define IGMP_STATS			0
	#define IPFRAG_STATS		0
	#define UDP_STATS			1
	#define TCP_STATS			1
	#define MEM_STATS			1
	#define MEMP_STATS			1
	#define PBUF_STATS			1
	#define SYS_STATS			1
#endif /* LWIP_STATS */

#define LWIP_PROVIDE_ERRNO      1

/** Define LWIP_COMPAT_MUTEX if the port has no mutexes and binary semaphores
    should be used instead */
#define LWIP_COMPAT_MUTEX       1
#define LWIP_COMPAT_MUTEX_ALLOWED

#define TCPIP_MBOX_SIZE         16

#define TCPIP_THREAD_STACKSIZE  450
#define TCPIP_THREAD_PRIO       4

#define EHTIF_THREAD_STACKSIZE  350
#define EHTIF_THREAD_PRIO       1

#endif /* __LWIPOPTS_H__ */

