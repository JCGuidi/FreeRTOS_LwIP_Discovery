//--------------------------------------------------------------
//
//            PA1  = RMII_Ref_Clk       PC1 = ETH_MDC
//            PA2  = ETH_MDIO           PC4 = RMII_RXD0
//            PA7  = RMII_CRS_DV        PC5 = RMII_RXD1
//            PB11 = RMII_TX_EN
//            PB12 = RMII_TXD0          NRST = Reset
//            PB13 = RMII_TXD1
//            PB14 = RMII_INT
//
//--------------------------------------------------------------

#include "stm32_ub_udp_server.h"

//--------------------------------------------------------------
// Variables Globales
//--------------------------------------------------------------
volatile uint32_t LocalTime = 0;
extern uint8_t EthLinkStatus;
struct udp_pcb *upcb_server;
struct ip_addr OwnIPaddr;
struct ip_addr HostIPaddr;
u16_t HOST_UDP_PORT;

UDP_SERVER_t UDP_SERVER = {
  UDP_SERVER_NOINIT,"\0",0
};


void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);


UDP_INIT_STATUS_t UB_UDP_Server_Init(void)
{
  UDP_INIT_STATUS_t ret_wert=UDP_INIT_OK;
  uint32_t check=0;

  if(UDP_SERVER.status==UDP_SERVER_NOINIT) {
    // Ethernet init
    check=ETH_BSP_Config();
    if(check==1) ret_wert=UDP_INIT_ETH_MACDMA_ERR;
    if(check==2) ret_wert=UDP_INIT_ETH_PHYINT_ERR;

    // Init LwIP-Stack
    LwIP_Init();

    // Init Server
    UDP_SERVER.status=UDP_SERVER_NOCONNECT;
  }

  return(ret_wert);
}


UDP_SERVER_CONNECT_t UB_UDP_Server_Connect(void)
{
  UDP_SERVER_CONNECT_t ret_wert=UDP_CONNECT_OK;
  err_t err;

  if(UDP_SERVER.status==UDP_SERVER_NOINIT) return(UDP_INIT_ERR);
  if(UDP_SERVER.status==UDP_SERVER_RUNNING) return(UDP_RUNNING);
  if(EthLinkStatus>0) return(UDP_NO_LINK);

  upcb_server = udp_new();

  if (upcb_server==NULL) return(UDP_ERR1);

  IP4_ADDR( &OwnIPaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3 );

  err=udp_bind(upcb_server,&OwnIPaddr,OWN_UDP_PORT);

  if (err != ERR_OK) return(UDP_ERR2);

  udp_recv(upcb_server, udp_receive_callback, NULL);

  UDP_SERVER.status=UDP_SERVER_RUNNING;

  return(ret_wert);
}


void UB_UDP_Server_Disconnect(void)
{
  if(UDP_SERVER.status==UDP_SERVER_RUNNING) {
    udp_remove(upcb_server);
    UDP_SERVER.status=UDP_SERVER_NOCONNECT;
  }
}


ErrorStatus UB_UDP_Server_SendString(char *ptr)
{
  ErrorStatus ret_wert=ERROR;
  struct pbuf *p;

  if(UDP_SERVER.status==UDP_SERVER_RUNNING) {
    p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)ptr), PBUF_POOL);
    if (p == NULL) return(ERROR);

    pbuf_take(p, (char*)ptr, strlen((char*)ptr));

    udp_sendto(upcb_server,p,&HostIPaddr,HOST_UDP_PORT);

    pbuf_free(p);

    ret_wert=SUCCESS;
  }

  return(ret_wert);
}


UDP_RECEIVE_t UB_UDP_Server_Do(char *ptr)
{
  UDP_RECEIVE_t ret_wert=UDP_REVEICE_EMPTY;
  uint32_t n;
  char wert;

  if(UDP_SERVER.status!=UDP_SERVER_NOINIT) {

    if (ETH_CheckFrameReceived())
    {
      LwIP_Pkt_Handle();
    }
    LwIP_Periodic_Handle(LocalTime);

    if(EthLinkStatus>0) {
      if(UDP_SERVER.status==UDP_SERVER_RUNNING) {
        udp_remove(upcb_server);
        UDP_SERVER.status=UDP_SERVER_NOCONNECT;
      }
    }
  }

  if(UDP_SERVER.status!=UDP_SERVER_RUNNING) return(UDP_SERVER_OFFLINE);

  //Copia el mensaje recibido en ptr
  if(UDP_SERVER.rxlen>0) {
    ret_wert=UDP_RECEIVE_READY;
    n=0;
    do {
    	wert=UDP_SERVER.rxbuf[n];
    	ptr[n]=wert;
    	n++;
    }while((n<UDP_RX_BUFFER_SIZE-1)&&(wert!=0));
    ptr[n]=0x00;

    UDP_SERVER.rxlen=0;
  }

  return(ret_wert);
}


void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
  uint32_t n;
  char wert;
  char *ptr;

  HostIPaddr = *addr;
  HOST_UDP_PORT = port;

  ptr=(char*)p->payload;

  n=0;
  do {
    wert=*ptr;
    UDP_SERVER.rxbuf[n]=wert;
    ptr++;
    n++;
  }while((wert!=0) && (n<UDP_RX_BUFFER_SIZE-1));
  UDP_SERVER.rxbuf[n]=0x00;
  UDP_SERVER.rxlen=n;

  pbuf_free(p);

}


void EXTI15_10_IRQHandler(void)
{
  // check welcher Ext-Interrupt (10 bis 15) aufgetreten ist
  if(EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
  {
    // wenn es der Interrupt vom LINK-Status war

    // Interrupt Handler starten
    Eth_Link_ITHandler(DP83848_PHY_ADDRESS);
    // ISR-Flag löschen
    EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);
  }
}
