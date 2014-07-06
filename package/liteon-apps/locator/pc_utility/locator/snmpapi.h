/***********************************************************
	Copyright 1989 by Carnegie Mellon University

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of CMU not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.
******************************************************************/
/*
 * snmp_api.h - API for access to snmp.
 */
#ifdef __cplusplus
  extern "C" {
#endif

#define _OID

#ifndef EIGHTBIT_SUBIDS
typedef u_short	oid;
#define MAX_SUBID   0xFFFF
#else
typedef u_char	oid;
#define MAX_SUBID   0xFF
#endif

#define SNMP_PORT	    161
#define SNMP_TRAP_PORT	    162

#define SNMP_MAX_LEN	    484

#define SNMP_VERSION_1	    0

#define ASN_CONTEXT	    (0x80)
#define ASN_APPLICATION     (0x40)
#define ASN_CONSTRUCTOR	    (0x20)

#define GET_REQ_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x0)
#define GETNEXT_REQ_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x1)
#define GET_RSP_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x2)
#define SET_REQ_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x3)
#define TRP_REQ_MSG	    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x4)

#define ASN_INTEGER	    (0x02)
#define ASN_OCTET_STR	    (0x04)

/* defined types (from the SMI, RFC 1065) */
#define IPADDRESS   (ASN_APPLICATION | 0)
#define COUNTER	    (ASN_APPLICATION | 1)
#define GAUGE	    (ASN_APPLICATION | 2)
#define TIMETICKS   (ASN_APPLICATION | 3)


#define SNMP_ERR_NOERROR    (0x0)
#define SNMP_ERR_TOOBIG	    (0x1)
#define SNMP_ERR_NOSUCHNAME (0x2)
#define SNMP_ERR_BADVALUE   (0x3)
#define SNMP_ERR_READONLY   (0x4)
#define SNMP_ERR_GENERR	    (0x5)

#define SNMP_TRAP_COLDSTART		(0x0)
#define SNMP_TRAP_WARMSTART		(0x1)
#define SNMP_TRAP_LINKDOWN		(0x2)
#define SNMP_TRAP_LINKUP		(0x3)
#define SNMP_TRAP_AUTHFAIL		(0x4)
#define SNMP_TRAP_EGPNEIGHBORLOSS	(0x5)
#define SNMP_TRAP_ENTERPRISESPECIFIC	(0x6)
/*
 * Set fields in session and pdu to the following to get a default or unconfigured value.
 */
#define SNMP_DEFAULT_COMMUNITY_LEN  0	/* to get a default community name */
#define SNMP_DEFAULT_RETRIES	    -1
#define SNMP_DEFAULT_TIMEOUT	    -1
#define SNMP_DEFAULT_REMPORT	    0
#define SNMP_DEFAULT_REQID	    0
#define SNMP_DEFAULT_ERRSTAT	    -1
#define SNMP_DEFAULT_ERRINDEX	    -1
#define SNMP_DEFAULT_ADDRESS	    0
#define SNMP_DEFAULT_PEERNAME	    NULL
#define SNMP_DEFAULT_ENTERPRISE_LENGTH	0
#define SNMP_DEFAULT_TIME	    0

/* Error return values */
#define SNMPERR_GENERR		-1
#define SNMPERR_BAD_LOCPORT	-2  /* local port was already in use */
#define SNMPERR_BAD_ADDRESS	-3
#define SNMPERR_BAD_SESSION	-4
#define SNMPERR_TOO_LONG	-5

#define RECEIVED_MESSAGE   1
#define TIMED_OUT	   2
/* status codes */
#define STAT_SUCCESS	0
#define STAT_ERROR	1
#define STAT_TIMEOUT 2

typedef struct synch_state {
    int	waiting;
    int status;
    int reqid;
    struct snmp_pdu *pdu;
}SYNCH_STATE;

typedef struct sockaddr_in  ipaddr; 

typedef struct snmp_session {
    u_char  *community;	/* community for outgoing requests. */
    int	    community_len;  /* Length of community name. */
    int	    retries;	/* Number of retries before timeout. */
    long    timeout;    /* Number of uS until first timeout, then exponential backoff */
    char    *peername;	/* Domain name or dotted IP address of default peer */
    u_short remote_port;/* UDP port number of peer. */
    u_short local_port; /* My UDP port number, 0 for default, picked randomly */
    /* Authentication function or NULL if null authentication is used */
    u_char    *(*authenticator)();
    int	    (*callback)();  /* Function to interpret incoming data */
    /* Pointer to data that the callback function may consider important */
    void    *callback_magic;
} SESSION, *LPSESSION;

typedef struct snmp_pdu {
    ipaddr  address;	/* Address of peer */

    int	    command;	/* Type of this PDU */

    u_long  reqid;	/* Request id */
    u_long  errstat;	/* Error status */
    u_long  errindex;	/* Error index */

    /* Trap information */
    oid	    *enterprise;/* System OID */
    int	    enterprise_length;
    ipaddr  agent_addr;	/* address of object generating trap */
    int	    trap_type;	/* trap type */
    int	    specific_type;  /* specific type */
    u_long  time;	/* Uptime */

    struct variable_list *variables;
} PDU, *LPPDU;


typedef struct variable_list {
    struct variable_list *next_variable;    /* NULL for last variable */
    oid	    *name;  /* Object identifier of variable */
    int	    name_length;    /* number of subid's in name */
    u_char  type;   /* ASN type of variable */
    union { /* value of variable */
	long	*integer;
	u_char	*string;
	oid	*objid;
    } val;
    int	    val_len;
} VAR_LIST, *LPVAR_LIST;

LPSESSION snmp_open(LPSESSION);
int snmp_close(LPSESSION);
int snmp_send(LPSESSION, LPPDU);
void snmp_read(fd_set *);
void snmp_free_pdu(LPPDU);
LPPDU snmp_pdu_create(int);
void snmp_add_var(LPPDU,oid *, int,u_char, u_char *, int);
void snmp_add_null_var(LPPDU,oid *, int);
int  snmp_synch_response(LPSESSION, LPPDU, LPPDU *);
LPPDU snmp_fix_pdu(LPPDU, int);
char *snmp_errstring(int);
void snmp_synch_setup(LPSESSION);
int	 snmp_get_socketid(LPSESSION);
int snmp_parse(LPSESSION, LPPDU, u_char *, int);
LPPDU snmp_trap_read();
int	snmp_build(LPSESSION, LPPDU, u_char *, int *);
#ifdef __cplusplus
  }
#endif

//extern int snmp_dump_packet;
