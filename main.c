//--------------------------------------------------------------------------------------------------||
// Proyecto		: Demo_73 																																						||
// Fecha	    : 24.10.2015																																					||
// Version  	: 2.0																																									||
// Autores    	: Guidi, Mancini																																		||
// EMail    	: guidiutn@hotmail.com.ar - mancini_mauricio@hotmail.com															||
// Materia		: Software de Tiempo Real																															||
// CPU      	: STM32F4 Discovery																																		||
// IDE      	: CooCox CoIDE 1.7.8																																	||
// GCC      	: 4.7 2012q4																																					||
// Modulo   	: DP83848 Ethernet Board																															||
// Funci�n 		: La funci�n de este programa es implementar un UDP echoserver sobre FreeRTOS.				||
//				  Se crearon un total de 4 tareas con distintas frecuencias de las cuales tres						||
//				  de ellas prenden y apagan leds y la cuarta es el echoserver.														||
//				  La IP del dispositivo esta configurada en 192.168.2.123 y el puerto es el 7, 						||
//				  cualquiera de estos parametros puede ser modificado por el usuario.											||
//				  El UDP echoserver responde el caracter que ha recivido al IP y puerto de origen					||
//--------------------------------------------------------------------------------------------------||


#include "main.h"
#include "stm32_ub_freertos.h"
#include "stm32_ub_udp_server.h"
#include "stm32_ub_led.h"


//--------------------------------------------------------------
//Define los delay de las tareas a ejecutar en mseg
//--------------------------------------------------------------
#define DelayRED		1000
#define DelayBLUE		500
#define DelayORANGE		200
#define DelayUDP		10
//--------------------------------------------------------------

//--------------------------------------------------------------
// Prototipos de las tareas del FreeRTOS
//--------------------------------------------------------------
void BlinkRED( void *pvParameters );
void BlinkBLUE( void *pvParameters );
void BlinkORANGE( void *pvParameters );
void UdpTask (void *pvParameters);
//--------------------------------------------------------------


int main(void)
{
	UDP_INIT_STATUS_t init_check;

	//----------------------- Inicializaci�n del sistema ------------------------------
	SystemInit(); 								//Sistema
	UB_Led_Init();								//LEDs
	init_check = UB_UDP_Server_Init();			//UDP Server
    if(init_check==UDP_INIT_OK){
    		if(UB_UDP_Server_Connect()==UDP_CONNECT_OK)
    			UB_Led_On(LED_GREEN);
    }
    else
    	UB_Led_On(LED_RED);


    //--------------------------- Creacion de las tareas del FreeRTOS ---------------------------
    xTaskCreate( BlinkRED, ( signed char * ) "BlinkyRED", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xTaskCreate( BlinkBLUE, ( signed char * ) "BlinkyBLUE", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xTaskCreate( BlinkORANGE, ( signed char * ) "BlinkyORANGE", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xTaskCreate( UdpTask, ( signed char * ) "UDP", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    // FreeRTOS Started
    vTaskStartScheduler();
    //-------------------------------------------------------------------------------------------

    while(1)
    {
    	//Bucle donde se ejecutan las tareas de FreeRTOS de manera periodica
    }

}


//--------------------------------------------------------------
// FreeRTOS-Task "BlinkRED"
//--------------------------------------------------------------
void BlinkRED( void *pvParameters )
{
  while(1)
  {
	  UB_Led_Toggle(LED_RED);						//Togglea led Rojo

	  vTaskDelay(DelayRED/portTICK_RATE_MS);		//Delay de 1seg
  }
}

//--------------------------------------------------------------
// FreeRTOS-Task "BlinkBLUE"
//--------------------------------------------------------------
void BlinkBLUE( void *pvParameters )
{
  while(1)
  {
	  UB_Led_Toggle(LED_BLUE);						//Togglea led Azul

	  vTaskDelay(DelayBLUE/portTICK_RATE_MS);		//Delay de 500mseg
  }
}

//--------------------------------------------------------------
// FreeRTOS-Task "BlinkORANGE"
//--------------------------------------------------------------
void BlinkORANGE( void *pvParameters )
{
	int i;
  while(1)
  {
	  for(i=0;i<1000000;i++);
	  UB_Led_Toggle(LED_ORANGE);					//Togglea led Naranja

	  //vTaskDelay(DelayORANGE/portTICK_RATE_MS);		//Delay de 200mseg
  }
}

//--------------------------------------------------------------
// FreeRTOS-Task "UDP EchoServer"
//--------------------------------------------------------------
void UdpTask (void *pvParameters)
{
	UDP_RECEIVE_t rec_check;
	char rx_buf[UDP_RX_BUFFER_SIZE];

	while(1){

		rec_check=UB_UDP_Server_Do(rx_buf);		//Se copia el mensaje recibido en rx_buf

	    if(rec_check==UDP_SERVER_OFFLINE)
	    	UB_Led_Off(LED_GREEN);

	    else {
	    	UB_Led_On(LED_GREEN);

	    	if(rec_check==UDP_RECEIVE_READY)
	    		UB_UDP_Server_SendString(rx_buf);
	    	else
				if(rec_check==UDP_REVEICE_EMPTY){

					UB_Led_Off(LED_GREEN);
					taskYIELD();

			}

	    	//Se responde con el mismo mensaje recibido
	    }

	    //vTaskDelay(DelayUDP/portTICK_RATE_MS);			//Delay de 10mseg
	}

}
