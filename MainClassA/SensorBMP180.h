#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 pressure;

#define ALTITUDE 959.0 // Altitud Bucaramanga

//**************************************************************************
//***************************        SETUP       ***************************
//**************************************************************************
void setup_BMP180(){
 if (pressure.begin())
    Serial.println("BMP180 Iniciado Correctamente");
  else
  {
    Serial.println("BMP180 init fail (disconnected?)\n\n");
  }
}


//**************************************************************************
//*************************** FUNCIÓN PRINCIPAL  ***************************
//**************************************************************************
void MedirBMP180 (double &T, double &P, double &P0, double &A){
  char status;
  //double T,P,p0,a;

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          P0 = pressure.sealevel(P,ALTITUDE);
          A = pressure.altitude(P,P0);
        }
      }
    }
  }
}
