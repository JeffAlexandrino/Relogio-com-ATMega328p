// Atribuição dos pinos:
const int segmentosPin[7] = {19, 18, 17, 16, 15, 14, 13};
const int displaysPin[6] = {12, 11, 10, 9, 8, 7};
const int buzzerPin = 0, alarmLedPin = 5, adjustLedPin = 6;
const int state_button = 1, inc_button = 2, dec_button = 3, mode_button = 4;

// Declaração das variáveis:
int hora = 7, minuto = 0, segundo = 0, horaAlarme = 0, minutoAlarme = 0, segundoAlarme = 0, currentMode = 0, currentState = 0;
int alarme_atual, alarme_anterior = 1, modo_atual, modo_anterior = 1, estado_atual, estado_anterior = 1;
bool alarmEnabled = false, piscar = false;
unsigned long tempo, atual, lastDebounceTime_inc, lastDebounceTime_dec, ultimoPiscar = 0;

// Funções do botão incremento:
void handleButtonInterrupt_inc(){
  unsigned long currentMillis = millis();
  if(currentMillis - lastDebounceTime_inc >= 50){
    lastDebounceTime_inc = currentMillis;
    if(currentMode == 1){
      switch(currentState){
        case 0: 
          horaAlarme = (horaAlarme + 1) % 24;
          break;
        case 1:  
          minutoAlarme = (minutoAlarme + 1) % 60;
          break;
        case 2:  
          segundoAlarme = (segundoAlarme + 1) % 60;
          break;
      }
    }else if(currentMode == 2){
      switch(currentState){
        case 0: 
          hora = (hora + 1) % 24;
          break;
        case 1:  
          minuto = (minuto + 1) % 60;
          break;
        case 2:  
          segundo = (segundo + 1) % 60;
          break;
      }
    }
  }
}

// Funções do botão decremento:
void handleButtonInterrupt_dec(){
  unsigned long currentMillis = millis();
  if(currentMillis - lastDebounceTime_dec >= 50){
    lastDebounceTime_dec = currentMillis;
    if(currentMode == 1){
      switch(currentState){
        case 0: 
          horaAlarme = (horaAlarme + 23) % 24;
          break;
        case 1: 
          minutoAlarme = (minutoAlarme + 59) % 60;
          break;
        case 2:  
          segundoAlarme = (segundoAlarme + 59) % 60;
          break;
      }
    }else if(currentMode == 2){
      switch(currentState){
        case 0: 
          hora = (hora + 23) % 24;
          break;
        case 1: 
          minuto = (minuto + 59) % 60;
          break;
        case 2:  
          segundo = (segundo + 59) % 60;
          break;
      }
    }
  }
}

// Construção dos dígitos:
int digitos[10][7] = {
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
    pinMode(inc_button, INPUT_PULLUP);
    pinMode(dec_button, INPUT_PULLUP);
    pinMode(mode_button, INPUT_PULLUP);
    pinMode(state_button, INPUT_PULLUP);

    // Interrupção para os botões incremento e decremento:
    attachInterrupt(digitalPinToInterrupt(inc_button), handleButtonInterrupt_inc, FALLING);
    attachInterrupt(digitalPinToInterrupt(dec_button), handleButtonInterrupt_dec, FALLING);
    
    // Liga os segmentos:
    for(int i = 0; i < 7; i++){
      pinMode(segmentosPin[i], OUTPUT);
    }
    
    // Liga os displays:
    for(int i = 0; i < 6; i++){
      pinMode(displaysPin[i], OUTPUT);
    } 
}

void loop(){

    // Alterna entre os modos do relógio:
    modo_atual = digitalRead(mode_button); 
    if(modo_atual != modo_anterior){
      if(!modo_atual){
        currentMode = (currentMode + 1) % 3;
        currentState = 0;
        if(hora != horaAlarme || minuto != minutoAlarme){
          alarmEnabled = true;
        }
        if(currentMode == 1){
          horaAlarme = 0;
          minutoAlarme = 0;
          segundoAlarme = 0;
        }
      }  
        modo_anterior = modo_atual;
    }

    // Alterna entre o ajuste das horas, minutos e segundos:
    estado_atual = digitalRead(state_button);
    if(estado_atual != estado_anterior){
      if(!estado_atual){
        currentState = (currentState + 1) % 3;
      }
        estado_anterior = estado_atual;
    }

    // Funcionamento dos LEDs para cada modo:
    digitalWrite(alarmLedPin, currentMode == 1 ? HIGH : LOW);
    digitalWrite(adjustLedPin, currentMode == 2 ? HIGH : LOW);
    
    // Seleção de modo:
    switch(currentMode){     
      case 0: // Modo de operação normal do relógio.
        updateClock();
        displayDigits();
        checkAlarm();
        break;
      case 1: // Modo de ajuste do alarme.
        setAlarm();
        break;       
      case 2: // Modo de ajuste do horário.
        adjustClock();
        break;
    } 
}

void displayDigits(){ // Função para mostrar os dígitos no display.

    int valores[6] = {hora / 10, hora % 10, minuto / 10, minuto % 10, segundo / 10, segundo % 10};
    for(int i = 0; i < 6; i++){
      for(int j = 0; j < 7; j++){
        digitalWrite(segmentosPin[j], digitos[valores[i]][j]);
      }
      digitalWrite(displaysPin[i], LOW);
      delayMicroseconds(100);
      digitalWrite(displaysPin[i], HIGH);
    }
}

void checkAlarm(){ // Função para checar se o alarme está ativado.

    // Se o horário atual for igual ao horário do alarme, o buzzer é ativado:
    if(alarmEnabled && hora == horaAlarme && minuto == minutoAlarme){
      tone(buzzerPin, 10);
    }
    
    // O botão state desativa o buzzer:
    alarme_atual = digitalRead(state_button); 
    if(alarme_atual != alarme_anterior){
      if(alarmEnabled && !alarme_atual){
        alarmEnabled = false;
        noTone(buzzerPin);
      }
        alarme_anterior = alarme_atual;
    }
}

void blinkDigits(int valores[], int startIndex, int endIndex, bool shouldBlink){ // Função para piscar os dígitos.

    // Os dígitos piscam em intervalos de 500 milisegundos:
    if(millis() - ultimoPiscar >= 500){
      piscar = !piscar;
      ultimoPiscar = millis();
    }

    // Lógica para fazer os displays ligarem e desligarem:
    if(shouldBlink && piscar){
      for(int i = startIndex; i < endIndex; i++){
        for(int j = 0; j < 7; j++) {
          digitalWrite(segmentosPin[j], digitos[valores[i]][j]);
        }
        digitalWrite(displaysPin[i], HIGH);
      }
    }else{
      for(int i = startIndex; i < endIndex; i++){
        for(int j = 0; j < 7; j++){
          digitalWrite(segmentosPin[j], digitos[valores[i]][j]);
        }
        digitalWrite(displaysPin[i], LOW);
        delayMicroseconds(100); 
        digitalWrite(displaysPin[i], HIGH);
      }
    }
}

void setAlarm(){ // Função para o ajuste do alarme.

  int valoresAlarme[6] = {horaAlarme / 10, horaAlarme % 10, minutoAlarme / 10, minutoAlarme % 10, segundoAlarme / 10, segundoAlarme % 10};
  
  // Somente os dígitos do estado atual piscam:
  switch(currentState){
    case 0:
      blinkDigits(valoresAlarme, 0, 2, true); 
      blinkDigits(valoresAlarme, 2, 6, false); 
      break;
    case 1:
      blinkDigits(valoresAlarme, 0, 2, false);
      blinkDigits(valoresAlarme, 2, 4, true); 
      blinkDigits(valoresAlarme, 4, 6, false); 
      break;
    case 2:
      blinkDigits(valoresAlarme, 0, 4, false); 
      blinkDigits(valoresAlarme, 4, 6, true); 
      break;
  }
}

void adjustClock(){ // Função para o ajuste do horário.

  int valoresAjuste[6] = {hora / 10, hora % 10, minuto / 10, minuto % 10, segundo / 10, segundo % 10};

  // Somente os dígitos do estado atual piscam:
  switch(currentState){
    case 0:
      blinkDigits(valoresAjuste, 0, 2, true); 
      blinkDigits(valoresAjuste, 2, 6, false); 
      break;
    case 1:
      blinkDigits(valoresAjuste, 0, 2, false);
      blinkDigits(valoresAjuste, 2, 4, true); 
      blinkDigits(valoresAjuste, 4, 6, false); 
      break;
    case 2:
      blinkDigits(valoresAjuste, 0, 4, false); 
      blinkDigits(valoresAjuste, 4, 6, true); 
      break;
  }
}

void updateClock(){ // Função para a contagem do tempo.

    atual = millis();
    if((atual - tempo) >= 1000){
      tempo += 1000;
      segundo ++;
      if(segundo > 59){
        segundo = 0;
        minuto ++;
        if(minuto > 59){
          minuto = 0;
          hora ++; 
          if(hora > 23){
            hora = 0;
          }
        }
      }
      tempo = atual;
    }
}
