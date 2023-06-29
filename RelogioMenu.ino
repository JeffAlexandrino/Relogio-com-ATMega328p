// Atribuição dos pinos:
const int pinos7seg[7] = {19, 18, 17, 16, 15, 14, 13};
const int displayPin[4] = {12, 11, 10, 9};
const int buzzerPin = 0;
const int state_button = 1;
const int inc_button = 2, dec_button = 3;
const int mode_button = 4;
const int alarmLedPin = 6;  
const int adjustLedPin = 7;
const int dotPin = 8;   

bool alarmEnabled = true; // Verifica se o alarme está ativo.
bool dotVisible = true; // Verifica se o ponto decimal está ligado.

// Declaração das variáveis:
int hora = 7, minuto = 00, segundo = 0, alarmHour = 8, alarmMinute = 00, currentMode = 0, currentState = 0;
int alarme_atual, alarme_anterior = 1;
int modo_atual, modo_anterior = 1;
int estado_atual, estado_anterior = 1;
unsigned long tempo;
unsigned long atual;
unsigned long dotTimer;
unsigned long currentDot;
unsigned long lastDebounceTime_inc;
unsigned long lastDebounceTime_dec;

// Função do botão incremento:
void handleButtonInterrupt_inc(){
  unsigned long currentMillis = millis();
   if(currentMillis - lastDebounceTime_inc >= 50){
    lastDebounceTime_inc = currentMillis;
    if(currentMode == 2){
     if(currentState == 0){
         hora++;
       if(hora > 23){
         hora = 00;
    }     
}
     if(currentState == 1){
         minuto++;
       if(minuto > 59){
         minuto = 00;
    }   
   }
  }
 }
}

// Função do botão decremento:
void handleButtonInterrupt_dec(){
  unsigned long currentMillis = millis();
   if(currentMillis - lastDebounceTime_dec >= 50){
    lastDebounceTime_dec = currentMillis;
    if(currentMode == 2){
     if(currentState == 0){
         hora--;
       if(hora < 00){
         hora = 23;
    }     
}
     if(currentState == 1){
         minuto--;
       if(minuto < 00){
         minuto = 59;
    }   
   }
  }
 }
}

// Construção dos dígitos:
int disp7seg[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1},
};

void setup(){
      
      // Definição dos pinos:
      pinMode(buzzerPin, OUTPUT);
      pinMode(alarmLedPin, OUTPUT);  
      pinMode(adjustLedPin, OUTPUT);
      pinMode(dotPin, OUTPUT);  
      pinMode(state_button, INPUT_PULLUP);
      pinMode(inc_button, INPUT_PULLUP);
      pinMode(dec_button, INPUT_PULLUP);
      pinMode(mode_button, INPUT_PULLUP);

      attachInterrupt(digitalPinToInterrupt(inc_button), handleButtonInterrupt_inc, FALLING); // Interrupção externa para o botão incremento
      attachInterrupt(digitalPinToInterrupt(dec_button), handleButtonInterrupt_dec, FALLING); // Interrupção externa para o botão decremento
    
    // Liga o display:
    for(int i = 0; i < 7; i++){
      pinMode(pinos7seg[i], OUTPUT);
      pinMode(displayPin[i], OUTPUT);
      digitalWrite(displayPin[i], HIGH);
    } 
}

void loop(){

       // Alterna entre os modos do relógio:
       modo_atual = digitalRead(mode_button); 
    if(modo_atual != modo_anterior){
      if(modo_atual == LOW){
        currentMode += 1;
    }  
      if(currentMode > 2){
        currentMode = 0;
    }
        modo_anterior = modo_atual;
    }

    // Pisca o ponto decimal a cada 500 ms:
    if(currentMode == 0 || currentMode == 1){
        currentDot = millis();
     if((currentDot - dotTimer) >= 500){
        dotVisible = !dotVisible;
        dotTimer = currentDot;
    }
     if(dotVisible){
        digitalWrite(dotPin, HIGH);
    }else{
        digitalWrite(dotPin, LOW);
    }
}

    // O relógio conta as horas somente no modo normal e no modo do alarme:
    if(currentMode == 0 || currentMode == 1){
      updateClock();
    }

    // Funcionamento dos LEDs para cada modo:
    if(currentMode == 0){
      digitalWrite(alarmLedPin, LOW);
      digitalWrite(adjustLedPin, LOW);
    }
    if(currentMode == 1){
      digitalWrite(alarmLedPin, HIGH);
      digitalWrite(adjustLedPin, LOW);
    }
    if(currentMode == 2){
      digitalWrite(alarmLedPin, LOW);
      digitalWrite(adjustLedPin, HIGH);
      digitalWrite(dotPin, LOW);
}
    
    switch(currentMode){
      
      case 0: // Modo para operação normal do relógio
        
        displayTime();
        break;
        
      case 1: // Modo para operação do alarme

        checkAlarm();
        break;
        
      case 2: // Modo para operação do ajuste do relógio

        adjustClock();
        break;
    } 
}

void displayTime(){ // Função para mostrar os números no display.

    // Primeiro dígito:
    for(int i = 0; i < 7; i++){
          digitalWrite(pinos7seg[i], disp7seg[hora/10][i]);
    }
      digitalWrite(displayPin[0], LOW);
      delay(1);
      digitalWrite(displayPin[0], HIGH);

    // Segundo dígito:
    for(int i = 0; i < 7; i++){
          digitalWrite(pinos7seg[i], disp7seg[hora%10][i]);
    }
      digitalWrite(displayPin[1], LOW);
      delay(1);
      digitalWrite(displayPin[1], HIGH);

    // Terceiro dígito:
    for(int i = 0; i < 7; i++){
          digitalWrite(pinos7seg[i], disp7seg[minuto%1000%100/10][i]);
    }
      digitalWrite(displayPin[2], LOW);
      delay(1);
      digitalWrite(displayPin[2], HIGH);

    // Quarto dígito:
    for(int i = 0; i < 7; i++){
          digitalWrite(pinos7seg[i], disp7seg[minuto%1000%100%10][i]);
    }
      digitalWrite(displayPin[3], LOW);
      delay(1);
      digitalWrite(displayPin[3], HIGH);
}

void checkAlarm(){ // Função para a operação do alarme.

    displayTime();

    // Se o horário atual for igual ao horário do alarme, o buzzer é ativado:
    if(alarmEnabled && hora == alarmHour && minuto == alarmMinute){
      tone(buzzerPin, 10);
    }
    
      // O botão estado desliga o buzzer e o relógio volta ao modo normal:
      alarme_atual = digitalRead(state_button); 
    if(alarme_atual != alarme_anterior){
      if(alarme_atual == LOW){
      alarmEnabled = !alarmEnabled;
      noTone(buzzerPin);
      currentMode = 0;
    }
      alarme_anterior = alarme_atual;
    }      
}

void adjustClock(){ // Função para a operação do ajuste do relógio.

    displayTime();

       // Alterna entre ajuste das horas e ajuste dos minutos:
       estado_atual = digitalRead(state_button); 
    if(estado_atual != estado_anterior){
      if(estado_atual == LOW){
        currentState += 1;
    }  
      if(currentState > 1){
        currentState = 0;
    }
        estado_anterior = estado_atual;
    }
}

void updateClock(){ // Função para a contagem das horas.

       atual = millis();
    if((atual - tempo) >= 1000){
       tempo = tempo + 1000;
        segundo ++;
      if(segundo > 59){
        segundo = 0;
        minuto += 1;
    }
      if(minuto > 59){
        minuto = 00;
        hora += 1;
    }   
      if(hora > 23){
        hora = 00;
    }
        tempo = atual;
    }
}
