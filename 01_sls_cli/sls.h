/*
 * Copyright (c) 2016, Zolertia - http://www.zolertia.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
/**
 * \author Vo Que Son <sonvq@hcmut.edu.vn>
 */

#ifndef SLS_
#define SLS_

#define SLS_PAN_ID	 IEEE802154_CONF_PANID

enum {
	SLS_NORMAL_PORT			= 3000,
	SLS_HELLO_PORT			= 3001,
	SLS_EMERGENCY_PORT		= 3002,
};

/*---------------------------------------------------------------------------*/
/* This is the UDP port used to receive data */
/* Response will be echoed back to DST port */
#define UDP_SERVER_LISTEN_PORT   	SLS_NORMAL_PORT
#define UDP_CLIENT_SEND_PORT   		SLS_EMERGENCY_PORT


/*
SLS_CC2538DK_HW = 1 : for compiling to CC2538dk
SLS_CC2538DK_HW = 0 : for compiling to SKY used in Cooja simulation
*/
#define SLS_CC2538DK_HW		0

// #define security_enable (1)
#if (SLS_CC2538DK_HW)
#define SLS_USING_CC2538DK
#else
#define SLS_USING_SKY
#endif

#define	SFD 	0x7F

//redefine led
#define BLUE		LEDS_ORANGE
#define RED			LEDS_GREEN
#define GREEN		LEDS_BLUE

/*------------------------------------------------------------
 * define UART1 *
 * UART1_RX:	PA6	RF2.5
 * UART1_TX:	PA7	RF2.6
 *------------------------------------------------------------*/


#define MAX_CMD_DATA_LEN	16
#define MAX_CMD_LEN	sizeof(cmd_struct_t)

//enum {false=0, true=1} bool;

#define DEFAULT_EMERGENCY_STATUS 1

enum {	
	// msg type
	MSG_TYPE_REQ			= 0x01,
	MSG_TYPE_REP			= 0x02,
	MSG_TYPE_HELLO			= 0x03,
	MSG_TYPE_EMERGENCY		= 0x04,
};


enum {
	//command id
	CMD_LED_OFF				= 0x01,
	CMD_LED_ON				= 0x02,
	CMD_LED_DIM				= 0x03,
	CMD_GET_LED_STATUS 		= 0x04,
	CMD_GET_NW_STATUS 		= 0x05,
	CMD_GET_GW_STATUS 		= 0x06,
	CMD_TIMER_ON 			= 0x07,
	CMD_TIMER_OFF 			= 0x08,
	CMD_SET_APP_KEY			= 0x09,
	CMD_GET_APP_KEY			= 0x0A,
	CMD_LED_HELLO 			= 0x0B,
	CMD_LED_REBOOT			= 0x0C,
	CMD_REPAIR_ROUTE		= 0x0D,
};

enum {
	//status of LED
	STATUS_LED_ON			= 0x01,
	STATUS_LED_OFF			= 0x02,
	STATUS_LED_DIM			= 0x03,	
	STATUS_LED_ERROR		= 0x04,
};


enum {	
	// node status
	NODE_CONNECTED			= 0x01,
	NODE_DISCONNECTED		= 0x02,
	NODE_POWER_ON			= 0x03,
	NODE_POWER_OFF			= 0x04,
	NODE_ERROR				= 0x05,
};

enum{
	//gateway status
	GW_CONNECTED			= 0x01,
	GW_DISCONNECTED			= 0x02,
	GW_POWER_ON				= 0x03,
	GW_POWER_OFF			= 0x04,
	GW_ERROR				= 0x05,
};

enum {
	// error code
	ERR_NORMAL				= 0x00,
	ERR_UNKNOWN_CMD			= 0x01,
	ERR_IN_HELLO_STATE		= 0x02,
};

enum {
	//state machine
	STATE_HELLO				= 0x00,
	STATE_NORMAL			= 0x01,
	STATE_EMERGENCY			= 0x02,
};


/*---------------------------------------------------------------------------*/
struct led_struct_t {
	uint16_t	id;			/*000xxxxx xxxxxxxx */
	uint16_t  	panid;		/* default = 0xABCD */ 
	uint16_t	voltage;
	uint16_t	current;
	uint16_t	power;
	uint16_t	temperature;
	uint16_t	lux;
	uint8_t		dim;	
	uint8_t		status;
};

/*---------------------------------------------------------------------------*/
//	used by gateway
struct gw_struct_t {
	uint16_t	id;			/*001xxxxx xxxxxxxx */
	uint16_t	panid;		
	uint16_t	voltage;
	uint16_t	current;
	uint16_t	power;
	uint16_t	temperature;
	uint16_t	lux;
	uint8_t		status;
};

/*---------------------------------------------------------------------------*/
//	used in the future
struct env_struct_t {
	uint16_t	id;			/*010xxxxx xxxxxxxx */
	uint16_t	panid;		
	uint16_t	temp;
	uint16_t	humidity;
	uint16_t	light;
	uint16_t	pir;
	uint16_t	rain;
	uint8_t		status;
};

/* This data structure is used to store the packet content (payload) */
struct net_struct_t {
	uint8_t			channel;	
	int8_t			rssi;
	int8_t			lqi;
	int8_t			tx_power;
	uint16_t		panid;
	uint16_t		node_addr;
	unsigned char	app_code[16];
};

/*---------------------------------------------------------------------------*/
//	sfd = 0x7F
//	seq: transaction id;
//	type: 	REQUEST/REPLY/HELLO
//	len: 	
//	cmd:	command id
//	err_code: code returned in REPLY, sender check this field to know the REQ status
//	arg[16]: data payload
struct cmd_struct_t {
	uint8_t  	sfd;
	uint8_t 	len;
	uint16_t 	seq;
	uint8_t		type;
	uint8_t		cmd;
	uint16_t	err_code;
	uint8_t 	arg[MAX_CMD_DATA_LEN];
};




typedef struct cmd_struct_t		cmd_struct_t;
typedef struct net_struct_t		net_struct_t;
typedef struct gw_struct_t		gw_struct_t;
typedef struct led_struct_t		led_struct_t;
/*---------------------------------------------------------------------------*/
#endif
