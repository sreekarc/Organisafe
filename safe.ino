#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Servo.h>
#include <Keypad.h>

//initializing variables
Servo myservo;

int pin = random(8999)+1000;
String kp = "";
bool locked = true;

char auth[] = "I6bn1Uld4-CbQqC_LG8A21NqC9b--5i0";

char ssid[] = "50MiltonPl";
char pass[] = "chilakapati";

char keymap[4][4]= { {'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'0', 'F', 'E', 'D'} };

byte rowPins[4] = {9,8,7,6}; //Rows 0 to 3
byte colPins[4]= {5,4,3,2}; //Columns 0 to 3

Keypad myKeypad= Keypad(makeKeymap(keymap), colPins, rowPins, 4, 4);

#define ESP8266_BAUD 115200
ESP8266 wifi(&Serial);

//timer that changes pin every 30 seconds and send new pin to web server
BlynkTimer timer;
void myTimerEvent(){
  pin = random(8999)+1000;
  Blynk.virtualWrite(V5, pin);
} 

void setup(){
  Serial.begin(9600);
  delay(10); 

  //starting web server
  Serial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);

  //starting timer
  timer.setInterval(30000, myTimerEvent);
  
  myservo.attach(11);
  myservo.write(37);

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop(){
  //running server and timer
  Blynk.run();
  timer.run(); 

  //unlocking door if correct pin is typed
  char keypressed = myKeypad.getKey(); 
  if (keypressed != NO_KEY) {
    kp+=keypressed;
  }
  if(kp.length() == 4){
    Serial.println(kp);
    if(kp.toInt()==pin){
      Serial.println("correct pin");
      if(locked){
        myservo.write(127);
        locked = false;
      }
      else {
        myservo.write(37);
        locked = true;
      }
      digitalWrite(12,HIGH);
      delay(250);
      digitalWrite(12,LOW);
    }
    else{
      digitalWrite(13,HIGH);
      delay(250);
      digitalWrite(13,LOW);
    }
    kp="";
  }
  delay(100);  
} 
