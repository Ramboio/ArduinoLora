/*

Codigo para utilizar o modulo LoRaWAN da radioenge
O modulo funciona via comandos AT, conforme tabela disponivel em:
https://www.radioenge.com.br/uploads/fe3eaca2f4e3fd565143af8cb9703d7d1560427722-manual-lorawan-v2.1.pdf

Para conectar o modulo ao arduino, o pino de RX do modulo deve ser conectado ao pino de TX do arduino, e vice-versa

ATENCAO: O modulo consome 111mA em 3V3 durante um TX, portanto ele deve ser ligado no pino 5V do arduino,
pois a porta de 3V3 fornece apenas 50mA

O codigo abaixo recebe uma string do monitor serial e a envia para o modulo, e printa no monitor serial tudo
que recebe do modulo, sem tratamento algum

INSTRUCOES PARA ABP:

1. AT+NJM=0               // Define como modo ABP
2. AT+DADDR=44:33:22:11   // Inserir a chave no formado LSB, nesse caso a chave eh 11:22:33:44
3. AT+NWKSKEY=Chave       // Inserir a Network Session Key igual esta na TTN, separando bytes por dois pontos (:)
4. AT+APPSKEY=Chave       // Inserir a App Session Key igual esta na TTN, separando bytes por dois pontos (:)
5. AT+SEND=port:msg       // Enviar uma mensagem na porta, ex. AT+SEND=6:Hello

*/

#include <SoftwareSerial.h>

#define PIN_AT_RX 7
#define PIN_AT_TX 8
#define BAUDRATE 9600
SoftwareSerial radioLora(PIN_AT_TX, PIN_AT_RX);

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
  radioLora.begin(BAUDRATE);
  
}

void loop() {

  String comando = "";
  if(Serial.available())  
  {     
    comando=Serial.readString();
    sendATcommand(comando, 2000);  
  } 

  if (radioLora.available())
  {
    Serial.print("Recebido: ");
    Serial.println(radioLora.readString());
  }
}
