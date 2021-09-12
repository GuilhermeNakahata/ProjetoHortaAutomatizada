#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include <virtuabotixRTC.h>
#include <NewTone.h>

#define pinSH_CP 9   //Pino Clock
#define pinST_CP 10  //Pino Latch
#define pinDS    11  //Pino Data
#define qtdeCI   1
#define pinVRx A2   // JoyStick X
#define pinVRy A3   // JoyStick Y
#define pinSW  3    // JoyStick SW
#define pinSensorA 12 // Sensor Horta
#define pinSensorB 13 // Sensor Horta
#define pinoSensor 2

void ci74HC595Write(byte pino, bool estado);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
virtuabotixRTC myRTCO(7, 6, 5);
DS1302 rtc1(5, 6, 7);
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };
int ValorX;
int ValorY;

bool situation;

void setup() {
   pinMode(pinSH_CP, OUTPUT);
   pinMode(pinST_CP, OUTPUT);
   pinMode(pinDS, OUTPUT);
   pinMode(pinVRx, INPUT);
   pinMode(pinVRy, INPUT);
   pinMode(pinSW,  INPUT_PULLUP);
   pinMode(pinoSensor,INPUT);
   lcd.begin(16,2);
   IniciarRtc();
   pinMode(pinSensorA, INPUT);
   pinMode(pinSensorB, INPUT);
   Serial.begin(9600);
}

void loop() {
 myRTCO.updateTime();
 if(myRTCO.hours == 6 && myRTCO.minutes >= 30 || myRTCO.hours == 11 && myRTCO.minutes >= 30 || myRTCO.hours == 17 && myRTCO.minutes >= 30){
  if(digitalRead(pinSensorA) && digitalRead(pinSensorB)){
     TocarSinfonia();
     Irrigar();
  }
 }
 if(!digitalRead(pinSW)){
    Menu();
 }


 Serial.println(digitalRead(pinoSensor)); 
 if(digitalRead(pinoSensor)){
  if(analogRead(0) < 450){
    ci74HC595Write(2, HIGH);
    situation = true;
  }
 }
 if(situation == true && !digitalRead(pinoSensor)){
    ci74HC595Write(2, LOW);
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
    int y = map(valorVRy,0,1023,1023,0);

    if( x == 88 && y == 1023){
      Luzes();
      break; 
    }
    else if( x == 178 && y == 507){
      SairMenu();
      break;
    }
    if (!statusSW) {
      Irrigar();
      break;
    }
  }
 
}

void SairMenu(){
  lcd.clear();
  lcd.print(">Sair");

  while(true){
  int valorVRx = analogRead(pinVRx);
  int valorVRy = analogRead(pinVRy);
  bool statusSW = digitalRead(pinSW);

  int x = map(valorVRx,0,1023,0,180);
  int y = map(valorVRy,0,1023,1023,0);

    if( x == 0 && y == 507){
      Menu();
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
    int y = map(valorVRy,0,1023,1023,0);

    if( x == 89 && y == 6){
      Menu();
      break;
    }
    if (!statusSW) {
      MenuLuzes();
      break;
    }
  }
}

void MenuLuzes(){
  delay(1000);
  lcd.clear();
  lcd.print(">Acender Apagar");
  while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);
  
    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,1023,0);

    if( x == 88 && y == 1023){
      MenuApagarLuzes();
      break; 
    }
    else if( x == 178 && y == 507){
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
  ci74HC595Write(2, HIGH);
  delay(2000);
}

void ApagarLuzes(){
  lcd.clear();
  lcd.write("Luzes Apagadas!");
  ci74HC595Write(2, LOW);
  delay(2000);
}

void MenuApagarLuzes(){
  lcd.clear();
  lcd.write("Acender >Apagar");
  while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);
  
    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,1023,0);

    if( x == 88 && y == 6){
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
  lcd.write(">Sair");
  while(true){
    int valorVRx = analogRead(pinVRx);
    int valorVRy = analogRead(pinVRy);
    bool statusSW = digitalRead(pinSW);
  
    int x = map(valorVRx,0,1023,0,180);
    int y = map(valorVRy,0,1023,1023,0);

    if( x == 89 && y == 6){
      MenuLuzes();
      break;
    }
    if (!statusSW) {
      Menu();
      break;
    }
  }
}


void ci74HC595Write(byte pino, bool estado) {
static byte ciBuffer[qtdeCI];

bitWrite(ciBuffer[pino / 8], pino % 8, estado);

digitalWrite(pinST_CP, LOW); //Inicia a Transmissão

digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
digitalWrite(pinSH_CP, LOW);

for (int nC = qtdeCI-1; nC >= 0; nC--) {
    for (int nB = 7; nB >= 0; nB--) {

        digitalWrite(pinSH_CP, LOW);  //Baixa o Clock      
        
        digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
        
        digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
        digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
    }  
}

digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão

}

void Irrigar(){
  TocarSinfonia();
  delay(1000);
  lcd.clear();
  lcd.print("Irrigando!");
  ci74HC595Write(1, HIGH);
  delay(4000);
  while(digitalRead(pinSensorA) && digitalRead(pinSensorB)){
    myRTCO.updateTime();
    if(myRTCO.minutes == 30){
      break;
    }
    
    bool statusSW = digitalRead(pinSW);
    if (!statusSW) {
      lcd.clear();
      lcd.print("Parada Acionada");
      break;
    }
    
  }
  ci74HC595Write(1, LOW);
}

void IniciarRtc(){
  rtc1.halt(false);
  rtc1.writeProtect(false);
  //SetarHoras();
}

void SetarHoras(){
  rtc1.setDOW(SATURDAY);       
  rtc1.setTime(20, 27, 01);     
  rtc1.setDate(5, 5, 2018);
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


