#include <FreeRTOS_SAMD21.h> //samd21
#include "Delays.h"
#include "Tareas.h"

#define  ERROR_LED_PIN  6 //Led Pin: Typical Arduino Board
#define ERROR_LED_LIGHTUP_STATE  LOW

int Conect = 0;

//**************************************************************************
//***************************        SETUP       ***************************
//**************************************************************************
void setup() 
{

  Serial.begin(115200);
  setup_PM();
  setup_BMP180();
  Serial.println("Conectando...");
  ConectarLoRa(Conect);
  if (Conect == 1){
    Serial.println("Conectado!!");
  }
  else{
    Serial.println("ERROR: No Conectado");
  }

  vNopDelayMS(1000); // prevents usb driver crash on startup, do not omit this
  while (!Serial) ;  // Wait for Serial terminal to open port before starting program

  Serial.println("");
  Serial.println("******************************");
  Serial.println("      Inicio de Programa      ");
  Serial.println("******************************");

  // Error Blink Codes:
  //    3 blinks - Fatal Rtos Error, something bad happened. Think really hard about what you just changed.
  //    2 blinks - Malloc Failed, Happens when you couldn't create a rtos object. 
  //               Probably ran out of heap.
  //    1 blink  - Stack overflow, Task needs more bytes defined for its stack! 
  //               Use the taskMonitor thread to help gauge how much more you need
  vSetErrorLed(ERROR_LED_PIN, ERROR_LED_LIGHTUP_STATE);

  
  // Create the threads that will be managed by the rtos
  // Sets the stack size and priority of each task
  // Also initializes a handler pointer to each task, which are important to communicate with and retrieve info from tasks
  xTaskCreate(threadPM,    "Task PM",      256, NULL, tskIDLE_PRIORITY + 4, &Handle_TareaPM);
  xTaskCreate(threadBMP180,"Task BMP180",  256, NULL, tskIDLE_PRIORITY + 3, &Handle_TareaBMP180);
  xTaskCreate(threadLoRa,  "Task LoRa",   1024, NULL, tskIDLE_PRIORITY + 2, &Handle_TareaLoRa);
  xTaskCreate(taskMonitor, "Task Monitor", 256, NULL, tskIDLE_PRIORITY + 1, &Handle_monitorTask);
  

  // Start the RTOS, this function will never return and will schedule the tasks.
  vTaskStartScheduler();

}



//**************************************************************************
//***************************         LOOP       ***************************
//**************************************************************************
void loop() 
{
    // Optional commands, can comment/uncomment below
    //Serial.print("."); //print out dots in terminal, we only do this when the RTOS is in the idle state
    vNopDelayMS(100);
}
