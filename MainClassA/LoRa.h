#include <MKRWAN.h>
LoRaModem modem;

#include "LoRaKeys.h"
#include "Reset.h"

int DR          = 3;  
//(AS923, AU915, EU868, KR920, IN865, US915, US915_HYBRID)
_lora_band region = US915_HYBRID;

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;


//**************************************************************************
//***************************   CONECTAR AL GWY  ***************************
//**************************************************************************
void ConectarLoRa(int &Conexion){
  Conectar:
  if (!modem.begin(US915_HYBRID)) {
    Serial.println("Fallo al iniciar el modulo");
    delay(1000);
    goto Conectar;
  };
  Conexion = 1;
  modem.configureClass(CLASS_A);
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("ERROR: Conectando");
    delay(1000);
    int tiempo = 1000;
    Reset(tiempo);
  }
  Serial.println("Conectado!");
  modem.configureClass(CLASS_A);
  modem.minPollInterval(1);
  /*
  DataRate  Modulation  SF  BW    bits/s  Max payload
  0            LoRa     10  125    980        19
  1            LoRa     9   125   1.760       61
  2            LoRa     8   125   3.125      133    
  3            LoRa     7   125   5.470      250
  4            LoRa     8   500   12.500     250
  */
  modem.dataRate(DR);
}

//**************************************************************************
//***************************   ENVIO DE DATOS   ***************************
//**************************************************************************
void EnviarRecibirLoRa(String &msg, String &rcv){
  Serial.print("Enviando: " + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++) {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();
  int err;
  modem.dataRate(DR);
  modem.beginPacket();
  modem.print(msg);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Mensaje enviado correctamente!");
  }
  else {
    Serial.println("ERROR: Mensaje no enviado");
  }
  delay(1000);
  if (!modem.available()) {
    Serial.println("No hay mensajes Downlink.");
    //return;
  }
  else {
    //String rcv;
    rcv.reserve(300);
    while (modem.available()) {
      rcv += (char)modem.read();
    }
    Serial.print("Received: " + rcv + " - ");
    for (unsigned int i = 0; i < rcv.length(); i++) {
      Serial.print(rcv[i] >> 4, HEX);
      Serial.print(rcv[i] & 0xF, HEX);
      Serial.print(" ");
    }
    Serial.println();
    }
  Dormir(10000);
}
