#include <SoftwareSerial.h>

#define PIN_AT_RX 7
#define PIN_AT_TX 8
#define BAUDRATE 9600
SoftwareSerial radioLora(PIN_AT_TX, PIN_AT_RX);

char msg[50];

String sendATcommand(String toSend, unsigned long milliseconds) {
  String result;
  Serial.print("Enviado: ");
  Serial.println(toSend);
  radioLora.println(toSend);
  unsigned long startTime = millis();
  Serial.print("Recebido: ");
  while (millis() - startTime < milliseconds) {
    if (radioLora.available()) {
      char c = radioLora.read();
      Serial.write(c);
      result += c;
    }
  }
  Serial.println();
  return result;
}



void setup() {
  
  Serial.begin(9600);
  radioLora.begin(9600);

  sendATcommand("AT+NJM=?", 500);

}

void loop() {

  String comando = "";
  if(Serial.available())  
  {     
    comando=Serial.readString();
    sendATcommand(comando, 500);  
  } 

  if (radioLora.available())
  {
    Serial.print("Recebido: ");
    Serial.println(radioLora.readString());
  }
}
