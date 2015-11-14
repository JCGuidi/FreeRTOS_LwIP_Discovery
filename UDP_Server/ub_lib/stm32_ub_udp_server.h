//--------------------------------------------------------------
// File     : stm32_ub_udp_server.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_UDP_SERVER_H
#define __STM32F4_UB_UDP_SERVER_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4x7_eth_bsp.h"
#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "lwip/udp.h"
#include <string.h>


//--------------------------------------------------------------
// MAC (02:00:00:02:00:02)
// Se eligió esta por comodidad, podria usarse la que se desee,
// pero hay que tener en cuenta que no la esté usando otro dispositivo
//--------------------------------------------------------------
#define MAC_ADDR0   2
#define MAC_ADDR1   0
#define MAC_ADDR2   0
#define MAC_ADDR3   2
#define MAC_ADDR4   0
#define MAC_ADDR5   2


//--------------------------------------------------------------
// Static IP address 192.168.2.123
//--------------------------------------------------------------
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   2
#define IP_ADDR3   123


//--------------------------------------------------------------
// Mascara de red (255.255.255.0)
//--------------------------------------------------------------
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0


//--------------------------------------------------------------
// Gateway Address (192.168.2.1)
//--------------------------------------------------------------
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   2
#define GW_ADDR3   1


//--------------------------------------------------------------
// UDP-Port Number
//--------------------------------------------------------------
#define  OWN_UDP_PORT       7


//--------------------------------------------------------------
// UDP-Buffer size
//--------------------------------------------------------------
#define  UDP_RX_BUFFER_SIZE   2


//---------------------------------------------------------------
// Definicion de estructuras para las variables de estado del UDP
//---------------------------------------------------------------
typedef enum {
  UDP_SERVER_NOINIT =0,
  UDP_SERVER_NOCONNECT,
  UDP_SERVER_RUNNING
}UDP_SERVER_STATUS_t;

typedef struct {
  UDP_SERVER_STATUS_t status;
  char rxbuf[UDP_RX_BUFFER_SIZE];
  uint32_t rxlen;
}UDP_SERVER_t;

typedef enum {
  UDP_INIT_OK =0,
  UDP_INIT_ETH_MACDMA_ERR,
  UDP_INIT_ETH_PHYINT_ERR
}UDP_INIT_STATUS_t;

typedef enum {
  UDP_CONNECT_OK =0,
  UDP_INIT_ERR,
  UDP_RUNNING,
  UDP_NO_LINK,
  UDP_ERR1,
  UDP_ERR2
}UDP_SERVER_CONNECT_t;

typedef enum {
  UDP_SERVER_OFFLINE =0,
  UDP_REVEICE_EMPTY,
  UDP_RECEIVE_READY
}UDP_RECEIVE_t;
//---------------------------------------------------------------


//--------------------------------------------------------------
// Funciones Globales
//--------------------------------------------------------------
UDP_INIT_STATUS_t UB_UDP_Server_Init(void);
UDP_SERVER_CONNECT_t UB_UDP_Server_Connect(void);
void UB_UDP_Server_Disconnect(void);
ErrorStatus UB_UDP_Server_SendString(char *ptr);
UDP_RECEIVE_t UB_UDP_Server_Do(char *ptr);


//--------------------------------------------------------------
#endif // __STM32F4_UB_UDP_SERVER_H
