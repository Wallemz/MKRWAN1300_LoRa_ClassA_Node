#include <sps30.h>

//**************************************************************************
//***************************        SETUP       ***************************
//**************************************************************************
void setup_PM(){
  s16 ret;
  u8 auto_clean_days = 4;
  u32 auto_clean;
  
  delay(2000);
  
  while (sps30_probe() != 0) {
    Serial.print("SPS sensor probing failed\n");
    delay(500);
  }
  Serial.println("Sensor PM Iniciado Correctamente");

  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret) {
    Serial.print("error setting the auto-clean interval: ");
    Serial.println(ret);
  }
    
  ret = sps30_start_measurement();
  if (ret < 0) {
    Serial.print("error starting measurement\n");
  }
  delay(1000);
}


//**************************************************************************
//*************************** FUNCIÓN PRINCIPAL  ***************************
//**************************************************************************
void MedirPM(float &PM1, float &PM25, float &PM4, float &PM10){
  struct sps30_measurement m;
  char serial[SPS_MAX_SERIAL_LEN];
  u16 data_ready;
  s16 ret;

  do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      Serial.print("error reading data-ready flag: ");
      Serial.println(ret);
    } else if (!data_ready)
      Serial.print("data not ready, no new measurement available\n");
    else
      break;
    delay(100); /* retry in 100ms */
  } while (1);

  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    Serial.print("error reading measurement\n");
  } else {
    PM1  = m.mc_1p0;
    PM25 = m.mc_2p5;
    PM4  = m.mc_4p0;
    PM10 = m.mc_10p0;
  }




  
  
}
