/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "56WGtcCRoJew8sv4LmeOUfxIf3ytM7bc";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CDG Network";
char pass[] = "wifipassword";

///////Define motor pins//////
const int LM_F = 13; //L298N IN2
const int LM_B = 12; //L298N IN1
const int RM_F = 15; //L298N IN3
const int RM_B = 2; //L298N IN4

//////End of Define Motor Pins//////

///Define PWM Pins for Motor Speed/////
const int ENA = 14;
const int ENB = 0;
////End of Define PWM Pins for Motor Speed////

//Setting PWM properties
const int freq = 5000;
const int PWMChannel1 = 0;
const int PWMChannel2 = 1;
const int resolution = 8;

int pinValue; //Motor Speed Variable from Blynk Slider
int pinValueAdjusted; //Converted Motor Speed Variable from Blynk Slider for turns

void setup()
{
  ledcSetup(PWMChannel1, freq, resolution);
  pinMode(LM_F, OUTPUT);
  pinMode(LM_B, OUTPUT);
  pinMode(RM_F, OUTPUT);
  pinMode(RM_B, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  ledcAttachPin(ENA,PWMChannel1);
  ledcAttachPin(ENB,PWMChannel2);
  ledcWrite(PWMChannel1,255);
  ledcWrite(PWMChannel2,255);
  
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V4) {
 pinValue = param.asInt(); // assigning incoming value from pin V4 to a variable for Motor Speed (0-255)
 pinValueAdjusted = pinValue * 0.6;  //Motor speed reduction for turning side wheels (diagonal turns)
                                 //This value may need to be different for your chassis and wheels

  Serial.print("V4 Slider value is: ");
  Serial.println(pinValue);
//  motorLF->setSpeed(pinValue);
//  motorLR->setSpeed(pinValue);
//  motorRF->setSpeed(pinValue);
//  motorRR->setSpeed(pinValue); 
ledcWrite(PWMChannel1,pinValue);
ledcWrite(PWMChannel2,pinValue);
}

//Read the Joystick//
BLYNK_WRITE(V0) {
  
  const int X_THRESHOLD_LOW = 108; //x: 128 - 20
  const int X_THRESHOLD_HIGH = 148; //x: 128 + 20   

  const int Y_THRESHOLD_LOW = 108; //y: 128 - 20
  const int Y_THRESHOLD_HIGH = 148; //y: 128 + 20
      
  int x_position = param[0].asInt();  //Read the Blynk Joystick x Position 0-255
  int y_position = param[1].asInt();  //Read the Blynk Joystick y Position 0-255

  int x_direction;  //Variable for Direction of Joystick Movement: x= -1, 0, 1
  int y_direction;  //Variable for Direction of Joystick Movement: y= -1, 0, 1
 
  Serial.print("x_position: ");
  Serial.print(x_position);
  Serial.print("  y_position: ");
  Serial.println(y_position);

//Determine the direction of the Joystick Movement

  x_direction = 0;
  y_direction = 0;

  if (x_position > X_THRESHOLD_HIGH) {
    x_direction = 1;
  } else if (x_position < X_THRESHOLD_LOW) {
    x_direction = -1;
  }
  if (y_position > Y_THRESHOLD_HIGH) {
    y_direction = 1;
  } else if (y_position < Y_THRESHOLD_LOW) {
    y_direction = -1;
  }
//if x and y are within the threshold their values then x_direction = 0 and y_direction = 0

//Move the Rover (Rover will move in the direction of the slower wheels)
//0,0(Stop); 0,1(Forward); 0,-1(Backward); 1,1(Right up diagonal); 1,0(Right); 1,-1(Right down diagonal);
//-1,0(Left); -1,1(Left up diagonal); -1,-1(Left down diagonal)

//**********BACKWARD DIAGONAL (SOFT) LEFT**********
//x = -1 and y = -1 
  if (x_direction == -1) 
    if (y_direction == -1) {
        Serial.print("JOYSTICK: left-down DIRECTION: BACKWARD SOFT LEFT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction); 
//
//  motorLF->setSpeed(pinValueAdjusted); //Turning Side slower than opposing side
//  motorLR->setSpeed(pinValueAdjusted); //Turning Side slower than opposing side
//  motorRF->setSpeed(pinValue);
//  motorRR->setSpeed(pinValue);
//  
//        motorLF->run(BACKWARD);
//        motorLR->run(BACKWARD);
//        motorRF->run(BACKWARD); 
//        motorRR->run(BACKWARD);
ledcWrite(PWMChannel1,pinValueAdjusted);
ledcWrite(PWMChannel2,pinValue);
digitalWrite(LM_F,LOW);
digitalWrite(LM_B,HIGH);
digitalWrite(RM_F,LOW);
digitalWrite(RM_B,HIGH);


//**********HARD LEFT TURN**********
//x = -1 and y = 0      
      } else if (y_direction == 0) {
        Serial.print("JOYSTICK: left DIRECTION: HARD LEFT ");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

//  motorLF->setSpeed(pinValue);
//  motorLR->setSpeed(pinValue);
//  motorRF->setSpeed(pinValue);
//  motorRR->setSpeed(pinValue);
//        
//        motorLF->run(RELEASE);
//        motorLR->run(RELEASE);
//        motorRF->run(FORWARD);
//        motorRR->run(FORWARD);
ledcWrite(PWMChannel1,pinValue);
ledcWrite(PWMChannel2,pinValue);
digitalWrite(LM_F,LOW);
digitalWrite(LM_B,HIGH);
digitalWrite(RM_F,HIGH);
digitalWrite(RM_B,LOW);


//FORWARD DIAGONAL (SOFT) LEFT
//x = -1 and y = 1   
      } else {
        //y_direction == 1
        Serial.print("JOYSTICK left-up DIRECTION: FORWARD SOFT LEFT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);  

//  motorLF->setSpeed(pinValueAdjusted); //Turning Side slower than opposing side
//  motorLR->setSpeed(pinValueAdjusted); //Turning Side slower than opposing side
//  motorRF->setSpeed(pinValue);
//  motorRR->setSpeed(pinValue);
//  
//        motorLF->run(FORWARD);
//        motorLR->run(FORWARD);
//        motorRF->run(FORWARD);
//        motorRR->run(FORWARD);
ledcWrite(PWMChannel1,pinValueAdjusted);
ledcWrite(PWMChannel2,pinValue);
digitalWrite(LM_F,LOW);
digitalWrite(LM_B,HIGH);
digitalWrite(RM_F,HIGH);
digitalWrite(RM_B,LOW);

//**********BACKWARD**********
//x = 0 and y = -1 
      } else 
        if (x_direction == 0) 
        if (y_direction == -1) {
        Serial.print("JOYSTICK down DIRECTION BACKWARD");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

//  motorLF->setSpeed(pinValue);
//  motorLR->setSpeed(pinValue);
//  motorRF->setSpeed(pinValue);
//  motorRR->setSpeed(pinValue);
//  
//        motorLF->run(BACKWARD);
//        motorLR->run(BACKWARD);
//        motorRF->run(BACKWARD);
//        motorRR->run(BACKWARD);
ledcWrite(PWMChannel1,pinValue);
ledcWrite(PWMChannel2,pinValue);
digitalWrite(LM_F,LOW);
digitalWrite(LM_B,HIGH);
digitalWrite(RM_F,LOW);
digitalWrite(RM_B,HIGH);

//**********STOP**********
//x = 0 and y = 0 
      } else if (y_direction == 0) {
        Serial.print("JOYSTICK: centered DIRECTION: STOP");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
        
//        motorLF->run(RELEASE);
//        motorLR->run(RELEASE);
//        motorRF->run(RELEASE);
//        motorRR->run(RELEASE);

digitalWrite(LM_F,LOW);
digitalWrite(LM_B,LOW);
digitalWrite(RM_F,LOW);
digitalWrite(RM_B,LOW);

//**********FORWARD**********
//x = 0 and y = 1 
      } else {
        //y_direction == 1
        Serial.print("JOYSTICK: up DIRECTION: FORWARD");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

//  motorLF->setSpeed(pinValue);
//  motorLR->setSpeed(pinValue);
//  motorRF->setSpeed(pinValue);
//  motorRR->setSpeed(pinValue);
//  
//        motorLF->run(FORWARD);
//        motorLR->run(FORWARD);
//        motorRF->run(FORWARD);
//        motorRR->run(FORWARD);
ledcWrite(PWMChannel1,pinValue);
ledcWrite(PWMChannel2,pinValue);
digitalWrite(LM_F,HIGH);
digitalWrite(LM_B,LOW);
digitalWrite(RM_F,HIGH);
digitalWrite(RM_B,LOW);

//**********BACKWARD DIAGONAL (SOFT) RIGHT**********
//x = 1 and y = -1 
      } else 
        //x_direction == 1
        if (y_direction == -1){ 
        Serial.print("JOYSTICK right-down DIRECTION: BACKWARD SOFT RIGHT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
//
//  motorLF->setSpeed(pinValue);
//  motorLR->setSpeed(pinValue);
//  motorRF->setSpeed(pinValueAdjusted);//Turning Side slower than opposing side
//  motorRR->setSpeed(pinValueAdjusted);//Turning Side slower than opposing side
//  
//        motorLF->run(BACKWARD);
//        motorLR->run(BACKWARD);
//        motorRF->run(BACKWARD);
//        motorRR->run(BACKWARD);
ledcWrite(PWMChannel1,pinValue);
ledcWrite(PWMChannel2,pinValueAdjusted);
digitalWrite(LM_F,LOW);
digitalWrite(LM_B,HIGH);
digitalWrite(RM_F,HIGH);
digitalWrite(RM_B,LOW);

////**********HARD RIGHT TURN**********
////x = 1 and y = 0 Right on x-axis 
      } else 
        if (y_direction == 0){ 
        Serial.print("JOYSITCK: right DIRECTION: HARD RIGHT");
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);

//  motorLF->setSpeed(pinValue);
//  motorLR->setSpeed(pinValue);
//  motorRF->setSpeed(pinValue);
//  motorRR->setSpeed(pinValue);
//  
//        motorLF->run(FORWARD);
//        motorLR->run(FORWARD);
//        motorRF->run(RELEASE);
//        motorRR->run(RELEASE); 
ledcWrite(PWMChannel1,pinValue);
ledcWrite(PWMChannel2,pinValue);
digitalWrite(LM_F,HIGH);
digitalWrite(LM_B,LOW);
digitalWrite(RM_F,LOW);
digitalWrite(RM_B,HIGH);

//**********FORWARD DIAGONAL (SOFT) RIGHT**********
//x = 1 and y = 1 
      } else { 
        //y_direction == 1
        Serial.print("JOYSTICK: right-up DIRECTION: SOFT RIGHT"); 
        Serial.print("  x_direction  ");
        Serial.print(x_direction);
        Serial.print("  y_direction  ");
        Serial.println(y_direction);
//        
//  motorLF->setSpeed(pinValue);
//  motorLR->setSpeed(pinValue);
//  motorRF->setSpeed(pinValueAdjusted);//Turning Side slower than opposing side
//  motorRR->setSpeed(pinValueAdjusted);//Turning Side slower than opposing side
//  
//        motorLF->run(FORWARD);
//        motorLR->run(FORWARD);
//        motorRF->run(FORWARD);
//        motorRR->run(FORWARD);      

ledcWrite(PWMChannel1,pinValue);
ledcWrite(PWMChannel2,pinValueAdjusted);
digitalWrite(LM_F,HIGH);
digitalWrite(LM_B,LOW);
digitalWrite(RM_F,LOW);
digitalWrite(RM_B,HIGH);
  }
}


 //**********END Translate the Joystick Position to a Rover Direction********** 
