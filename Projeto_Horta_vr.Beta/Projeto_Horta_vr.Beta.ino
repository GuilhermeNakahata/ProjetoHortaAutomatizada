#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include <virtuabotixRTC.h>
#include <NewTone.h>
#include <EEPROM.h>

#define pinVRx A2   // JoyStick X
#define pinVRy A3   // JoyStick Y
#define pinSW  3    // JoyStick SW
#define releLuz 13 // rele da luz
#define releValvula 12 // rele da valvula solenoide
#define pinoSensor 9
#define pinoSensorHortaA 10
#define pinoSensorHortaB 11
#define LigarSensores 8

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
virtuabotixRTC myRTCO(7, 6, 5);
DS1302 rtc1(5, 6, 7);
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };
int ValorX;
int ValorY;

int ManhaHorta;
int TardeHorta;

bool situation;

void setup() {
   pinMode(pinVRx, INPUT);
   pinMode(pinVRy, INPUT);
   pinMode(pinSW,  INPUT_PULLUP);
   pinMode(pinoSensor,INPUT);
   lcd.begin(16,2);
   IniciarRtc();
   pinMode(releLuz, OUTPUT);
   pinMode(releValvula, OUTPUT);
   pinMode(pinoSensorHortaA, INPUT);
   pinMode(pinoSensorHortaB, INPUT);
   Serial.begin(9600);
   pinMode(LigarSensores,OUTPUT);
   ManhaHorta = EEPROM.read(0);
   TardeHorta = EEPROM.read(1);
}

void loop() {
 myRTCO.updateTime();
 if((myRTCO.hours == 7 && myRTCO.minutes == 30 && ManhaHorta == 1) || (myRTCO.hours == 13 && myRTCO.minutes == 30 && TardeHorta == 1)){ 
   digitalWrite(LigarSensores,HIGH);
   delay(5000);
     if(digitalRead(pinoSensorHortaA) ||  digitalRead(pinoSensorHortaB)){
      TocarSinfonia();
      Irrigar();
     }
   delay(5000);
   digitalWrite(LigarSensores,LOW); 
 }
 if(!digitalRead(pinSW)){
    Menu();
 }
  if(digitalRead(pinoSensor)){
  if(myRTCO.hours > 18 || myRTCO.hours < 6){
    if(analogRead(0) > 800){
      digitalWrite(releLuz, HIGH);
      situation = true;
      lcd.clear();
      lcd.print("Luz Acesa");
      for(int i = 0; i < 15; i++){
        if(digitalRead(pinoSensor)){
          i = 0;
        }
        delay(1000);
      }
    }
  }
 }
 if(situation == true && !digitalRead(pinoSensor)){
    digitalWrite(releLuz, LOW);
    situation = false;
 }
 
 MostrarData(); 
}

void Menu(){
  delay(1000);
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("Menu");
  lcd.setCursor(0,1);
  lcd.print(">Irrigar Luzes");

  while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);
  
    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    Serial.print("Valor X: ");
    Serial.println(x);
    Serial.print("Valor Y: ");
    Serial.println(y);
    

    if(y >= 0 && y <= 10){
      Luzes();
      break; 
    }
    else if(x >= 175 && x <= 185){
      ConfigMenu();
      break;
    }
    if (!statusSW) {
      Irrigar();
      break;
    }
  }
 
}

void ConfigMenu(){
  lcd.clear();
  lcd.print(">Config Sair");

  while(true){
  int valorVRx = analogRead(pinVRx);
  int valorVRy = analogRead(pinVRy);
  bool statusSW = digitalRead(pinSW);

  int x = map(valorVRx,0,1023,0,180);
  int y = map(valorVRy,0,1023,0,180);

    if(x >= 0 && x <= 10){
      Menu();
      break;
    }
    else if(y >= 0 && y <= 10){
      SairMenu();
      break;
    }
    if (!statusSW) {
      MenuConfiguracoes();
      break;
    }
  }
}

void MenuConfiguracoes(){
    delay(1000);
    lcd.clear();
    lcd.print(">Horario Irriga");
    lcd.setCursor(0,1);
    lcd.print("Sair");

    while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);

    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(y >= 0 && y <= 10){
      ConfigIrrigacao();
      break;
    }
    else if(x >= 175 && x <= 185){
      SairMenuConfig();
      break;
    }
    if (!statusSW) {
      TelaConfigHoras();
      break;
    }
  }
}

void SairMenuConfig(){
    delay(1000);
    lcd.clear();
    lcd.print("Horario Irriga");
    lcd.setCursor(0,1);
    lcd.print(">Sair");

    while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);

    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(x >= 0 && x <= 10){
      MenuConfiguracoes();
      break;
    }
    if (!statusSW) {
      break;
    }
  }
}

void ConfigIrrigacao(){
    delay(1000);
    lcd.clear();
    lcd.print("Horario >Irriga");
    lcd.setCursor(0,1);
    lcd.print("Sair");

    while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);

    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(y >= 175 && y <= 185){
      MenuConfiguracoes();
      break;
    }
    if (!statusSW) {
      ConfiguracaoIrrigacao();
      break;
    }
  }
}

void ConfiguracaoIrrigacao(){
    delay(1000);
    lcd.clear();
    lcd.print(">Manha Tarde");
    lcd.setCursor(0,1);
    lcd.print("Sair");

    while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);

    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(y >= 0 && y <= 10){
      ConfiguracaoIrrigacaoTarde();
      break;
    }
    else if(x >= 175 && x <= 185){
      ConfiguracaoIrrigacaoSair();
      break;
    }
    if (!statusSW) {
      if(ManhaHorta == 0){
        EEPROM.write(0,1);
        ManhaHorta = 1;
        lcd.clear();
        lcd.print("Manha Ligado!");  
      }
      else{
        EEPROM.write(0,0);
        ManhaHorta = 0;
        lcd.clear();
        lcd.print("Manha Desligado!");  
      }
      delay(1000);
      break;
    }
  }
}

void ConfiguracaoIrrigacaoTarde(){
    delay(1000);
    lcd.clear();
    lcd.print("Manha >Tarde");
    lcd.setCursor(0,1);
    lcd.print("Sair");

    while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);

    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(y >= 175 && y <= 185){
      ConfiguracaoIrrigacao();
      break;
    }
    if (!statusSW) {
      if(TardeHorta == 0){
        EEPROM.write(1,1);
        TardeHorta = 1;
        lcd.clear();
        lcd.print("Manha Ligado!");  
      }
      else{
        EEPROM.write(1,0);
        TardeHorta = 0;
        lcd.clear();
        lcd.print("Manha Desligado!");  
      }
      delay(1000);
      break;
    }
  }
}

void ConfiguracaoIrrigacaoSair(){
    delay(1000);
    lcd.clear();
    lcd.print("Manha Tarde");
    lcd.setCursor(0,1);
    lcd.print(">Sair");

    while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);

    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(x >= 0 && x <= 10){
      ConfiguracaoIrrigacao();
      break;
    }
    if (!statusSW) {
      break;
    }
  }
}

void TelaConfigHoras(){ 
  lcd.clear();
  int h = myRTCO.hours;
  int m = myRTCO.minutes;

  ImprimirHorario(h,m,true);
  
  delay(2000);
  while(true){
    
  int valorVRx = analogRead(pinVRx);
  int valorVRy = analogRead(pinVRy);
  bool statusSW = digitalRead(pinSW);

  int x = map(valorVRx,0,1023,0,180);
  int y = map(valorVRy,0,1023,0,180);
  
    if(x >= 0 && x <= 10){
      h += 1;
      if(h == 24){
        h = 0;
      }
      ImprimirHorario(h,m,true);
      delay(1000);
    }
    if(y >= 0 && y <= 10){
      TelaConfigMinutos();
      break;
    }
    else if( x >= 150){
      break;
    }
    if (!statusSW) {     
      rtc1.setTime(h, m, 0);
      break;
    }
  }
  
}

void TelaConfigMinutos(){
  lcd.clear();
  int h = myRTCO.hours;
  int m = myRTCO.minutes;

  ImprimirHorario(h,m,false);
  
  delay(2000);
  while(true){
    
  int valorVRx = analogRead(pinVRx);
  int valorVRy = analogRead(pinVRy);
  bool statusSW = digitalRead(pinSW);

  int x = map(valorVRx,0,1023,0,180);
  int y = map(valorVRy,0,1023,0,180);
  
    if(x >= 0 && x <= 10){
      m += 1;
      if(m == 60){
        m = 0;
      }
      ImprimirHorario(h,m,false);
      delay(1000);
    }
    if(y >= 175 && y <= 185){
      TelaConfigHoras();
      break;
    }
    else if( x >= 150){
      break;
    }
    if (!statusSW) {     
      rtc1.setTime(h, m, 0);
      break;
    }
  }
}

void ImprimirHorario(int h, int m, boolean x){
  lcd.clear();
  if(x){
  lcd.print("  Config Horas  ");
  }
  else{
  lcd.print(" Config Minutos "); 
  }
  if(h < 10){
  lcd.setCursor(5,1);
  lcd.print(0);
  lcd.setCursor(6,1);
  lcd.print(h);
  lcd.setCursor(7,1);
  lcd.print(":");
  if(m < 10){
  lcd.setCursor(8,1);
  lcd.print(0);
  lcd.setCursor(9,1);
  lcd.print(m);  
  }
  else{
  lcd.setCursor(8,1);
  lcd.print(m);
  } 
  }
  else{
  lcd.setCursor(5,1);
  lcd.print(h);
  lcd.setCursor(7,1);
  lcd.print(":");
  if(m < 10){
  lcd.setCursor(8,1);
  lcd.print(0);
  lcd.setCursor(9,1);
  lcd.print(m);  
  }
  else{
  lcd.setCursor(8,1);
  lcd.print(m);
  } 
  }
}

void SairMenu(){
  lcd.clear();
  lcd.print("Config >Sair");

  while(true){
  int valorVRx = analogRead(pinVRx);
  int valorVRy = analogRead(pinVRy);
  bool statusSW = digitalRead(pinSW);

  int x = map(valorVRx,0,1023,0,180);
  int y = map(valorVRy,0,1023,0,180);

    if(y >= 175 && y <= 185){
      ConfigMenu();
      break;
    }
    if (!statusSW) {
      break;
    }
  }
}

void Luzes(){
  
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("Menu");
  lcd.setCursor(0,1);
  lcd.print("Irrigar >Luzes");

  while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);
  
    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(y >= 175 && y <= 185){
      Menu();
      break;
    }
    if (!statusSW) {
      delay(1000);
      lcd.clear();
      lcd.print("Menu luzes");
      lcd.setCursor(0,1);
      lcd.print("Desativado");
      delay(1000);
      break;
    }
  }
}

void MenuLuzes(){
  delay(1000);
  lcd.clear();
  lcd.print(">Acender Apagar");
  lcd.setCursor(0,1);
  lcd.write("Sair");
  while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);
  
    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(y >= 0 && y <= 10){
      MenuApagarLuzes();
      break; 
    }
    else if(x >= 175 && x <= 185){
      SairLuzes();
      break;
    }
    if (!statusSW) {
      AcenderLuzes();
      break;
    }
  }
}

void AcenderLuzes(){
  lcd.clear();
  lcd.write("Luzes Acesas!");
  digitalWrite(releLuz, HIGH);
  delay(2000);
}

void ApagarLuzes(){
  lcd.clear();
  lcd.write("Luzes Apagadas!");
  digitalWrite(releLuz, LOW);
  delay(2000);
}

void MenuApagarLuzes(){
  lcd.clear();
  lcd.write("Acender >Apagar");
  lcd.setCursor(0,1);
  lcd.write("Sair");
  while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);
  
    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(y >= 175 && y <= 185){
      MenuLuzes();
      break;
    }
    if (!statusSW) {
      ApagarLuzes();
      break;
    }
  }
}

void SairLuzes(){
  lcd.clear();
  lcd.clear();
  lcd.print("Acender Apagar");
  lcd.setCursor(0,1);
  lcd.write(">Sair");
  while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);
  
    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,0,180);

    if(x >= 0 && x <= 10){
      MenuLuzes();
      break;
    }
    if (!statusSW) {
      Menu();
      break;
    }
  }
}




void Irrigar(){
  TocarSinfonia();
  delay(1000);
  lcd.clear();
  lcd.print("Irrigando!");
  digitalWrite(releValvula, HIGH);
  delay(4000);
  myRTCO.updateTime();
  int minutoAnterior = myRTCO.minutes;
  Serial.println(minutoAnterior % 60);
  if((minutoAnterior+5) % 60 != 0){
    minutoAnterior = (minutoAnterior+5) % 60;
  }
  while(myRTCO.minutes != minutoAnterior){
    myRTCO.updateTime();  
    bool statusSW = digitalRead(pinSW);
    if (!statusSW) {
     lcd.clear();
     lcd.print("Parada Acionada");
     delay(2000);
     break;
    }
  }
    
  digitalWrite(releValvula, LOW);
}

void IniciarRtc(){
  rtc1.halt(false);
  rtc1.writeProtect(false);
  //SetarHoras();
}

void SetarHoras(){
  rtc1.setDOW(WEDNESDAY);       
  rtc1.setTime(18, 21, 50);     
  rtc1.setDate(2, 1, 2019);
}

void MostrarData(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hora: ");
  lcd.setCursor(6,0);
  lcd.print(rtc1.getTimeStr());
  lcd.setCursor(0,1);
  lcd.print("Data: ");
  lcd.setCursor(6, 1);
  lcd.print(rtc1.getDateStr());
  delay(1000);
  lcd.clear ();
}

void TocarSinfonia(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    NewTone(4, melody[thisNote], noteDuration);
    delay(noteDuration * 4 / 3);
  }
  noNewTone(4); 
}
