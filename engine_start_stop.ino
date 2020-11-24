/*
 * Autor: Rafael Lopes Schneider
 * 24 de novembro de 2020
 * Serve para controle de relés de ignição de um veículo normal utilizando um botão para substituir a chave
 * Funcionando perfeitamente - lembra de utilizar o resistor pro GND na conexão do botão com o pino digital
 */

#define starter 52
#define halfkey 53
#define button 51
#define serial 0 // troque para 1 para ativar o debug na porta serial

void setup() {
  if (serial) {
    Serial.begin(9600); 
  }  
  pinMode(starter, OUTPUT);
  pinMode(halfkey, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(starter, LOW);
  digitalWrite(halfkey, LOW);
}

int halfkeyState, starterState, engineRunning = 0;

void loop() {
  if (serial) {
    Serial.print(halfkeyState);
    Serial.print(" ");
    Serial.println(starterState); 
  }  
  if (halfkeyState && engineRunning && digitalRead(button)) {
      halfkeyState = 0;
      engineRunning = 0;
      write(halfkey, LOW);
      while(digitalRead(button)) { }
  }
  else if(halfkeyState) {
    if(digitalRead(button)) {
      for(int i = 0; i < 10; i++) {
        delay(100);
        if(!digitalRead(button)) {
          halfkeyState = 0;
          write(halfkey, LOW);
          return;
        }
      }
      if(digitalRead(button)) {
        starterState = 1;
        write(starter, HIGH);
      }        
      while(digitalRead(button)) { }
      starterState = 0;
      engineRunning = 1;
      write(starter, LOW);
    }
  } 
  else {
    if(digitalRead(button)) {
      halfkeyState = 1;
      write(halfkey, HIGH);
      while(digitalRead(button)) { }
    } 
  }
  delay(50);
}

void write(int port, int value) {
  digitalWrite(port, value);
  delay(1000);
}
