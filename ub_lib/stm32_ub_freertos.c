//--------------------------------------------------------------
// File     : stm32_ub_freertos.c
// Datum    : 11.01.2014
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : FreeRTOS
// Funktion : FreeRTOS (GCC-Version für ARM Cortex-M4/F)
//            Version 7.6.0 (vom 18.11.2013)
//            Quelle : www.freertos.org
//
// Doku und Beispiele : www.freertos.org
//
// FPU      : Hardware FPU muss in CoIDE aktiviert werden
//
// Heap     : nur EIN File vom "FreeRTOS\Source\portable\MemMang\"
//            Ordner in das Projekt einbinden !! (z.B. "heap_2.c")
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_freertos.h"


//--------------------------------------------------------------
// IDLE
//--------------------------------------------------------------
void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}



//--------------------------------------------------------------
// TICK
//--------------------------------------------------------------
void vApplicationTickHook( void )
{
    /* vApplicationTickHook() will only be called if configUSE_TICK_HOOK is set
    to 1 in FreeRTOSConfig.h.  It is a hook function that will get called during
    each FreeRTOS tick interrupt.  Note that vApplicationTickHook() is called
    from an interrupt context. */
}


//--------------------------------------------------------------
// STACK OVERFLOW
//--------------------------------------------------------------
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}


//--------------------------------------------------------------
// MALLOC FAILED
//--------------------------------------------------------------
void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
