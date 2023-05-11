#include <SPI.h>
#include <MFRC522.h>
#include <LCD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> //library for servo moter

#define SS_PIN 10 //RX slave select
#define RST_PIN 9

Servo myservo;  // create servo object to control a servo.
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE);//instance for lcd display

byte card_ID[4]; //card UID size 4byte
byte Name1[4]={0x43,0x85,0x9E,0xAA};//first UID card
byte Name2[4]={0xF3,0xC7,0x1A,0x2E};//second UID card
//byte Name3[4]={0x93,0xE7,0x22,0xA6};//third UID card

//if you want the arduino to detect the cards only once
int NumbCard[10];//this array content the number of cards. in my case i have just three cards.
int j=0;        

// Pins for LEDs and buzzer
const int redLED = 8;
const int greenLED = 4;
const int buzzer = 5;

String Name;//user name
String Department;//user department
long ID;//user number
int n ;//The number of card you want to detect (optional)  

void setup() {

    // Set LEDs and buzzer as outputs
  pinMode(redLED, OUTPUT);  
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  //setup for lcd screen
  lcd.begin(16,2);// initialize the lcd

  myservo.attach(6);  // attaches the servo on pin 6 to the servo object

  
  Serial.println("CLEARSHEET");// clears starting at row 1
  Serial.println("LABEL,Date,Time,Name,ID");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)
  Serial.println("RESETTIMER");

   }   

void servo(){//method for servo motor
   int pos = 0;    // variable to store the servo position
   for (pos = 90; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              
    delay(5);                       
  }
  delay(3000);
   for (pos = 0; pos <= 90; pos += 1) {// goes from 180 degrees to 0 degrees
    myservo.write(pos);             
    delay(5);                       
  }
}

void loop() {
       lcd.home();
       lcd.clear();
       lcd.setCursor (0,0);
       lcd.print(F("Welcome to SMUCT"));
       lcd.setCursor (2,1);
       lcd.print(F("Scan ID Card"));
  //look for new card
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;//got to start of loop if there is no card present
 }
 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
 }
 
 for (byte i = 0; i < mfrc522.uid.size; i++) {
     card_ID[i]=mfrc522.uid.uidByte[i];

       if(card_ID[i]==Name1[i]){  
       Name="Shakil Mahmud";//user name
       lcd.home();
       lcd.clear();
       lcd.setCursor (0,0);
       lcd.print(Name);

       ID=1091;//user number
       lcd.setCursor (0, 3 );
       lcd.print(F("ID:"));
       lcd.setCursor (3, 1 );
       lcd.print(ID);

       Department="CSE";//user number
       lcd.setCursor (8,3);
       lcd.print(F("Dep:"));
       lcd.setCursor (12, 1 );
       lcd.print(Department);
       
        digitalWrite(greenLED, HIGH);
        delay(100);
        digitalWrite(buzzer, HIGH);
        delay(50);
        digitalWrite(buzzer, LOW);
        delay(5);

       j=0;//first number in the NumbCard array : NumbCard[j]
      }
      else if(card_ID[i]==Name2[i]){

       Name="Alamin Ahmed";//user name
       lcd.home();
       lcd.clear();
       lcd.setCursor (0,0);
       lcd.print(Name);

       ID=1070;//user id
       lcd.setCursor (0,3);
       lcd.print(F("ID:"));
       lcd.setCursor (3,1);
       lcd.print(ID);

       Department="CSIT";//user number
       lcd.setCursor (8,3);
       lcd.print(F("Dep:"));
       lcd.setCursor (12, 1);
       lcd.print(Department);

       digitalWrite(greenLED, HIGH);
        delay(100);
        digitalWrite(buzzer, HIGH);
        delay(50);
        digitalWrite(buzzer, LOW);
        delay(5);

       j=1;//Second number in the NumbCard array : NumbCard[j]
      }
      else{
       digitalWrite(redLED, HIGH);
        delay(200);
        digitalWrite(redLED, LOW);
        digitalWrite(buzzer, HIGH);
        delay(200);
        digitalWrite(buzzer, HIGH);
        delay(200);
        digitalWrite(buzzer, HIGH);
        delay(200);
        digitalWrite(buzzer, LOW);

       lcd.clear();
       lcd.setCursor(1,0);
       lcd.print(F("Access Denied"));
       delay(5);
       goto cont;//go directly to line 85
     }
}
      if(NumbCard[j] == 1){//to check if the card already detect
      //if you want to use LCD
      digitalWrite(greenLED,HIGH);
      delay(200);
      digitalWrite(greenLED,LOW);
       lcd.home();
       lcd.clear();
       lcd.setCursor(1,0);
       lcd.print(F("Access Guranted"));
       lcd.setCursor(2,1);
       lcd.print(F("Gate Opening"));
       
       NumbCard[j] = 1;//put 1 in the NumbCard array : NumbCard[j]={1,1} to let the arduino know if the card was detecting 
      n++;//(optional)
      Serial.print("DATA,DATE,TIME," + Name );//send the Name to excel
      Serial.print(",");
      Serial.println(ID); //send the ID to excel
      delay(5);
      digitalWrite(greenLED,HIGH);
      delay(5);
      digitalWrite(redLED,LOW);
      //Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
      servo();
      }
      else{
      NumbCard[j] = 1;//put 1 in the NumbCard array : NumbCard[j]={1,1} to let the arduino know if the card was detecting 
      n++;//(optional)
      Serial.print("DATA,DATE,TIME," + Name );//send the Name to excel
      Serial.print(",");
      Serial.println(ID); //send the ID to excel

      digitalWrite(greenLED,HIGH);
      digitalWrite(redLED,LOW);
      delay(5);
      Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
      }
      delay(5);
cont:
delay(5);
digitalWrite(greenLED,LOW);
digitalWrite(redLED,LOW);
delay(2000);

//if you want to close the Excel when all card had detected and save Excel file in Names Folder. in my case i have just 2 card (optional)
//if(n==2){
    
  //Serial.println("FORCEEXCELQUIT");
 //   }
}
    
