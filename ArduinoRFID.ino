//add in the servo and rfid reader
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial rSerial(7, 3); // RX, TX


Servo lockServo;

// Pick a PWM pin to put the servo on
const int servoPin = 10;

const int tagLen = 16;
const int idLen = 13;
const int kTags = 4;

// Put your known tags here!
char knownTags[kTags][idLen] = {
             "7A005B308392",
            //the scanned tag values had to be manually placed here to be added to a list that identifies the value as a known tag that lets the servo arm work.
};

// Empty array to hold a new scanned tag
char newTag[idLen];

void setup() {
  // Starts the hardware and software serial ports
   Serial.begin(9600);
   rSerial.begin(9600);

   // Attaches the servo to the pin
   lockServo.attach(servoPin);

   // Put servo in locked position
   lockServo.write(30);

}

  int servoAngle = 30; //Runs once when the arduino is powered 



  
void loop(){ 
  int i = 0;
  int readByte;

  boolean tag = false;
  // starts the servo in a locked position

  if (rSerial.available() == tagLen) {
    tag = true;
//if the scanned card is a known 
  }

  if (tag == true) {
    while (rSerial.available()) {
      // Take each byte out of the serial buffer, one at a time
      readByte = rSerial.read();
      //writes out the scanned tag 

      if (readByte != 2 && readByte!= 13 && readByte != 10 && readByte != 3) {
        newTag[i] = readByte;
        i++;
      }
      if (readByte == 3) {
        tag = false;
      }

    }
  }

  if (strlen(newTag)== 0) {
    return;
  }

  else {
    int total = 0;
    int check = 0;

    for (int ct=0; ct < kTags; ct++){
        total += checkTag(newTag, knownTags[ct]);
    }
    
    if (total > 0) {
      check++;
      Serial.println("Success! Card Number: ");
      Serial.println(check);
      Serial.println(servoAngle);
      if(servoAngle == 30){
        servoAngle = 90;
      } else{
        servoAngle = 30;
      }
      lockServo.write(servoAngle);
      //total = 0;
    }

    else {
        Serial.print("Unknown tag! ");
        Serial.print(newTag);
        Serial.println();
    }
  }

  for (int c=0; c < idLen; c++) {
    newTag[c] = 0;
  }
}

//this checks the code, if the scanned tag is not equal to a known tag in the list defined previously, it does not change the position of the servo, but if it is a known tag value, the servo arm is moved to the open position.
int checkTag(char nTag[], char oTag[]) {
    for (int i = 0; i < idLen; i++) {
      if (nTag[i] != oTag[i]) {
        return 0;
      }
    }
  return 1;
}
