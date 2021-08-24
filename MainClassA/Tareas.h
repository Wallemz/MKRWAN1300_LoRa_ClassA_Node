#include "SensorPM.h"
#include "SensorBMP180.h"
#include "LoRa.h"

//**************************************************************************
//***************************  Global Variables  ***************************
//**************************************************************************
TaskHandle_t Handle_TareaPM;
TaskHandle_t Handle_TareaBMP180;
TaskHandle_t Handle_TareaLoRa;
TaskHandle_t Handle_monitorTask;

float pm1 = 0, pm25 = 0, pm4 = 0, pm10 = 0;
double t = 0, p = 0, p0 = 0, a = 0;
String Mensaje = "", Recibido = "";

//**************************************************************************
//***************************        TAREAS      ***************************
//**************************************************************************
//**************************************************************************
//                               TAREA Sensor PM
//**************************************************************************
static void threadPM( void *pvParameters ) 
{
  Serial.println("");
  Serial.println("******************************");
  Serial.println("      Thread PM: Iniciado     ");
  Serial.println("******************************");
  
  while(1)
  {
    Serial.println("******************************");
    Serial.println("Sensor PM");
    MedirPM(pm1, pm25, pm4, pm10);
    Serial.print("PM 1   = ");
    Serial.println(pm1);
    
    Serial.print("PM 2.5 = ");
    Serial.println(pm25);

    Serial.print("PM 4   = ");
    Serial.println(pm4);

    Serial.print("PM 10  = ");
    Serial.println(pm10);

    Serial.println("\n");
    myDelayMs(1000);
  }
}


//**************************************************************************
//                            TAREA Sensor BMP180
//**************************************************************************
static void threadBMP180( void *pvParameters ) 
{
  Serial.println("");
  Serial.println("******************************");
  Serial.println("    Thread BMP180: Iniciado   ");
  Serial.println("******************************");
  while(1)
  {
    Serial.println("******************************");
    Serial.println("Sensor BMP180");
    MedirBMP180(t, p, p0, a);
    Serial.print("Tempertura       = ");
    Serial.println(t);
    
    Serial.print("Presion Absoluta = ");
    Serial.println(p);

    Serial.print("Presion Relativa = ");
    Serial.println(p0);

    Serial.print("Altitud          = ");
    Serial.println(a);
    
    Serial.println("\n");
    myDelayMs(1000);
  }

}

//**************************************************************************
//                               TAREA LoRa
//**************************************************************************
static void threadLoRa( void *pvParameters ) 
{
  Serial.println("");
  Serial.println("******************************");
  Serial.println("     Thread LoRa: Iniciado    ");
  Serial.println("******************************");
  
  while(1)
  {
    Serial.println("******************************");
    Serial.println("LoRa");

    Mensaje = "¿0?" + String(pm25) + "¿1?" + String(pm10) + "¿2?" + String(t) + "¿3?" + String(p) + "¿4?" + String(p0) + "¿5?" + String(a);
    
    Serial.print("Enviando: ");
    Serial.println(Mensaje);
    EnviarRecibirLoRa(Mensaje, Recibido);
    Serial.print("Recibido: ");
    Serial.println(Recibido);
    
    Serial.println("\n");
    myDelayMs(5000);
  }
}




//**************************************************************************
//                               TASK MONITOR
//**************************************************************************
void taskMonitor(void *pvParameters)
{
  int x;
  int measurement;
    
  Serial.println("");
  Serial.println("******************************");
  Serial.println("   Thread Monitor: Iniciado   ");
  Serial.println("******************************");

    // run this task afew times before exiting forever
    for(x=0; x<10; ++x)
    {

      Serial.println("");
      Serial.println("******************************");
      Serial.println("[Stacks Free Bytes Remaining] ");

      measurement = uxTaskGetStackHighWaterMark( Handle_TareaPM );
      Serial.print("Tarea PM: ");
      Serial.println(measurement);
      
      measurement = uxTaskGetStackHighWaterMark( Handle_TareaBMP180 );
      Serial.print("Tarea BMP180: ");
      Serial.println(measurement);

      measurement = uxTaskGetStackHighWaterMark( Handle_TareaLoRa );
      Serial.print("Tarea LoRa: ");
      Serial.println(measurement);
      
      measurement = uxTaskGetStackHighWaterMark( Handle_monitorTask );
      Serial.print("Monitor Stack: ");
      Serial.println(measurement);

      Serial.println("******************************");

      myDelayMs(10000); // print every 10 seconds
    }

    // delete ourselves.
    // Have to call this or the system crashes when you reach the end bracket and then get scheduled.
    Serial.println("Task Monitor: Deleting");
    vTaskDelete( NULL );

}
