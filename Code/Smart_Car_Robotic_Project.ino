/*******************************************************************
Prject: Wireless samrt robot car
Author: Han Guo
*******************************************************************/
#include <SoftwareSerial.h> 
#include <Wire.h>
#include <BH1750.h>
SoftwareSerial MyBlue(14, 16); // RX | TX pins
BH1750 lightMeter;

//Motor A pins connect to Arduino Nano

# define ENA 9
# define IN1 7
# define IN2 8

//Motor B pins connect to Arduino Nano

# define ENB 10
# define IN3 11
# define IN4 12

// Distance pin define
#define TrigPin1 2
#define EchoPin1 3
#define TrigPin2 4
#define EchoPin2 5


float distance1;
float distance2;

char cIncoming_com = 0;

int LED = 15; 




void setup() 
{   
  Serial.begin(9600); 
  MyBlue.begin(9600); 
  Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 

  lightMeter.begin();
  Serial.println("Running...");
  pinMode(LED,OUTPUT);

  pinMode(ENA, OUTPUT);             //Set the motor pins to output pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT); 
  pinMode(ENB, OUTPUT); 
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);  

  pinMode(TrigPin1,OUTPUT);
  pinMode(EchoPin1,INPUT);
  pinMode(TrigPin2,OUTPUT);
  pinMode(EchoPin2,INPUT);

         digitalWrite(IN1, LOW);
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW); 
  
} 
void loop() 
{ 

   
    if( MyBlue.available())                   //Read the signal command from the phone
    {
      cIncoming_com = MyBlue.read();
      Serial.print(cIncoming_com);
      Serial.print("\n");

    }

   
      if(cIncoming_com == 'F')                    // When "Up" button pressed, the car will go forward
      {
          MoveForward();           
          cIncoming_com = 0;

      }

      else if(cIncoming_com == 'B')                    // When "Down" button pressed, the car will go backward
      {
          MoveBackward();
          cIncoming_com = 0;
      }

      else if(cIncoming_com == 'L')                    // When "Left" button pressed, the car will turn left
      {
          TurnLeft();
          delay(300);
          MoveForward();
          cIncoming_com = 0;
      }

      else if(cIncoming_com == 'R')                    // When "Right" button pressed, the car will turn right
      {
          TurnRight();
          delay(300);
          MoveForward();
          cIncoming_com = 0;

      }

      else if(cIncoming_com == 'S')                    // When "Stop" button pressed, the car will stop
      {
           MotorStop(); 
           cIncoming_com = 0;
      }


    if(cIncoming_com == 'A')                          //When "Auto" button pressed, the car will set to automatic mode
    {
      DistanceMeasure();
      MoveForward();                                  //The car will keep move forward
      if(distance1 && distance2 <= 20)                //The IR sesnor will measure the distance,if the distance is less than 20cm. the car will stop and backward then stop
      {
        MotorStop();
        delay(100);
        MoveBackward();
        delay(300);
        MotorStop();
        if(distance1 >= distance2)                    //if the right sensor is larger than left distance, the car will turn right
        {
          TurnRight();
          delay(300);
          MotorStop();
        }

        else                                          //if the left sensor is larger than right distance, the car will turn left
        {
          TurnLeft();
          delay(300);
          MotorStop();
        }
      }
      cIncoming_com = 'A';
      
    }

      uint16_t lux = lightMeter.readLightLevel();     //Read the light sensor value
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lx");
      delay(1000);
      if(lux < 50)                                    //if the light insensity is less than 50, the LED will turn on
      {
        digitalWrite(LED,HIGH);
      }
      else
      {
        digitalWrite(LED,LOW);
      }

}  

/*****************************************************************************
Function: MoveFoward
Operation: Robot car will go forward,when two motor spin to one direction at 
           same time with same speed
           
*****************************************************************************/
void MoveForward()                                
{

       digitalWrite(IN1, HIGH);
       digitalWrite(IN2, LOW);
       analogWrite(ENA,150);

       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, LOW);
       analogWrite(ENB,150);


       delay(100);
}

/*****************************************************************************
Function: MoveBackward
Operation: Robot car will go Backward,when two motor spin to opposite direction 
           of forward at same time with same speed
           
*****************************************************************************/
void MoveBackward()
{      

       digitalWrite(IN1, LOW);
       digitalWrite(IN2, HIGH);
       analogWrite(ENA,150);

       digitalWrite(IN3, LOW);
       digitalWrite(IN4, HIGH);
       analogWrite(ENB,150);

       delay(100);
}

/*****************************************************************************
Function: TurnLeft
Operation: Robot car will turn left,when right motor spins and left motor stops
           
*****************************************************************************/
void TurnLeft()
{
       digitalWrite(IN1, LOW);
       digitalWrite(IN2, HIGH);
       analogWrite(ENA,100);

       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, LOW);
       analogWrite(ENB,100);

       delay(100);
}

/*****************************************************************************
Function: TurnRight
Operation: Robot car will turn right,when left motor spins and right motor stops
           
*****************************************************************************/
void TurnRight()
{
       digitalWrite(IN1, HIGH);
       digitalWrite(IN2, LOW);
       analogWrite(ENA,100);

       digitalWrite(IN3, LOW);
       digitalWrite(IN4, HIGH);
       analogWrite(ENB,100);

       delay(100);
}

/*****************************************************************************
Function: MotorStop
Operation: Robot car will stop when two motors are all stopping
           
*****************************************************************************/
void MotorStop()
{
       digitalWrite(IN1, LOW);
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW); 
}


/*****************************************************************************
Function: DistanceMeasure
Operation: The function is read the IR sensor values
           Each sensor read 10 times and calculate the aveerage value
           Store the average value to get more accurate data
           
*****************************************************************************/
void DistanceMeasure()
{
      float distancetamp[10];


    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[0] = pulseIn(EchoPin1,HIGH)*0.034/2;

    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[1] = pulseIn(EchoPin1,HIGH)*0.034/2;

    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[2] = pulseIn(EchoPin1,HIGH)*0.034/2;

    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[3] = pulseIn(EchoPin1,HIGH)*0.034/2;

    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[4] = pulseIn(EchoPin1,HIGH)*0.034/2;

    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[5] = pulseIn(EchoPin1,HIGH)*0.034/2;

    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[6] = pulseIn(EchoPin1,HIGH)*0.034/2;

    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[7] = pulseIn(EchoPin1,HIGH)*0.034/2;

    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[8] = pulseIn(EchoPin1,HIGH)*0.034/2;

    digitalWrite(TrigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin1,LOW);
    distancetamp[9] = pulseIn(EchoPin1,HIGH)*0.034/2;

    distance1 = distancetamp[0]+distancetamp[1]+distancetamp[2]+distancetamp[3]+distancetamp[4]+distancetamp[5]+distancetamp[6]+distancetamp[7]+distancetamp[8]+distancetamp[9];
    distance1 = distance1/10;

    Serial.print("Distance1: ");
    Serial.println(distance1);



    float distancetamp2[10];


    digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[0] = pulseIn(EchoPin2,HIGH)*0.034/2;

        digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[1] = pulseIn(EchoPin2,HIGH)*0.034/2;

        digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[2] = pulseIn(EchoPin2,HIGH)*0.034/2;

        digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[3] = pulseIn(EchoPin2,HIGH)*0.034/2;

        digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[4] = pulseIn(EchoPin2,HIGH)*0.034/2;

        digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[5] = pulseIn(EchoPin2,HIGH)*0.034/2;

        digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[6] = pulseIn(EchoPin2,HIGH)*0.034/2;

        digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[7] = pulseIn(EchoPin2,HIGH)*0.034/2;

        digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[8] = pulseIn(EchoPin2,HIGH)*0.034/2;

        digitalWrite(TrigPin2,LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin2,LOW);
    distancetamp2[9] = pulseIn(EchoPin2,HIGH)*0.034/2;

        distance2 = distancetamp2[0]+distancetamp2[1]+distancetamp2[2]+distancetamp2[3]+distancetamp2[4]+distancetamp2[5]+distancetamp2[6]+distancetamp2[7]+distancetamp2[8]+distancetamp2[9];
    distance2 = distance2/10;

    Serial.print("Distance2: ");
    Serial.println(distance2);

}

