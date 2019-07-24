#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
Servo myservo;  //creates a servo object
                         //a maximum of eight servo objects can be created

int pos = 0;        //variable to store servo position

//amount of time we give the sensor to calibrate(10-60 secs according to the datasheet)
int calibrationTime = 10;
boolean takeLowTime; 

int pirPin = 12;            //digital pin connected to the PIR's output
int pirPos = 13;           //connects to the PIR's 5V pin

void setup(){
  mySerial.begin(9600);
  myservo.attach(4);    //attaches servo to pin 4
  Serial.begin(9600);    //begins serial communication
  pinMode(pirPin, INPUT);
  pinMode(pirPos, OUTPUT);
  digitalWrite(pirPos, HIGH);

  //give the sensor time to calibrate
  Serial.println("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(calibrationTime - i);
    Serial.print("-");
    delay(1000);
  }
  Serial.println();
  Serial.println("done");
 
  //while making this Instructable, I had some issues with the PIR's output
  //going HIGH immediately after calibrating
  //this waits until the PIR's output is low before ending setup
  while (digitalRead(pirPin) == HIGH) {
    delay(500);
    Serial.print(".");     
  }
  Serial.print("SENSOR ACTIVE");
}


void readSerial(){
  char reply[50];
  int i = 0;
  while (mySerial.available()) {
    reply[i] = mySerial.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  if(strlen(reply) > 0){
    Serial.println(reply);
    if((strcmp(reply,"open")==0)&&(digitalRead(pirPin) == LOW))
   {
     Serial.println("toll opened");
     for(pos = 0; pos < 90; pos += 1)  //goes from 0 to 180 degrees
    {                                                 //in steps of one degree
      myservo.write(pos);                   //tells servo to go to position in variable "pos"
       delay(5);                                //waits for the servo to reach the position
    };
  
    delay(3000);
    for(pos = 90; pos>=0; pos-=1)    //goes from 180 to 0 degrees
    {                               
      myservo.write(pos);                  //to make the servo go faster, decrease the time in delays for
      delay(5);                                  //to make it go slower, increase the number.
    }
   
   }
     else
        Serial.println("access denied");
  }
}

void loop(){

  if(digitalRead(pirPin) == HIGH){  //if the PIR output is HIGH, turn servo

    for(pos = 0; pos < 90; pos += 1)  //goes from 0 to 180 degrees
    {                                                 //in steps of one degree
      myservo.write(pos);                   //tells servo to go to position in variable "pos"
       delay(5);                                //waits for the servo to reach the position
    };
  
    delay(3000);
    for(pos = 90; pos>=0; pos-=1)    //goes from 180 to 0 degrees
    {                               
      myservo.write(pos);                  //to make the servo go faster, decrease the time in delays for
      delay(5);                                  //to make it go slower, increase the number.
    }
      
    takeLowTime = true;
  }

  if(digitalRead(pirPin) == LOW){      

    if(takeLowTime){
      takeLowTime = false;    //make sure this is only done at the start of a LOW phase
      delay(50);
    }
  }
   readSerial();
   delay(500);
}
