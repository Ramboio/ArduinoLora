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
2. AT+NWKSKEY=Chave       // Inserir a Network Session Key igual esta na TTN, separando bytes por dois pontos (:)
3. AT+APPSKEY=Chave       // Inserir a App Session Key igual esta na TTN, separando bytes por dois pontos (:)
4. AT+DADDR=45:34:23:12   // Inserir a chave no formado LSB, nesse caso a chave eh 12:23:34:45
5. AT+SEND=port:msg       // Enviar uma mensagem na porta, ex. AT+SEND=6:Hello

AT+CHMASK=FF00:0000:0000:0000:00FF:0000


*/

#include <SoftwareSerial.h>.

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

const int LED = 13;
const int Potenciometro = A5;
int ligado = 0;

void setup() {
  
  Serial.begin(9600);
  radioLora.begin(BAUDRATE);
  
  pinMode(Potenciometro, INPUT);
  pinMode(LED, OUTPUT);
  
}

void loop() {

  int leituraPotenciometro = analogRead(Potenciometro);
  if ( leituraPotenciometro > 512 && ligado == 0 )
  {
    ligado = 1;
    Serial.println("Liga");
    digitalWrite(LED, HIGH);
    sendATcommand("AT+SEND=2:ON", 2000);

  }
  else if ( leituraPotenciometro < 512 && ligado == 1 )
  {
    ligado = 0;
    Serial.println("Desliga");
    digitalWrite(LED, LOW);
    sendATcommand("AT+SEND=2:OFF", 2000);
  }
 
}
