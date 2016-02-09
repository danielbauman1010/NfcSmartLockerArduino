#include <AddicoreRFID.h>       //libraries 
#include <SPI.h>        //imported 
// http://www.addicore.com/RFID-AddiKit-with-RC522-MIFARE-Module-RFID-Cards-p/126.htm 


#define  uchar unsigned char       //creating a character called uchar
#define uint  unsigned int      //creating an integer called uint
#define button 2                //button pin
#define led 6                   //led pin
int state;          //creating an integer called state
uchar serNumA[5];       //creating a character array with 5 elements called -
//nerNumA 
int button_state;
uchar fifobytes;        //creating a character called fifobytes
uchar fifoValue;        //creating a character called fifoValue
            //fifo stands for first in first out - an order of -
//operation
AddicoreRFID myRFID; // create AddicoreRFID object to control the nfc module

const int chipSelectPin = 10;       //connecting pin 10  to chipSelectPin
const int NRSTPD = 5;     //connecting pin 5 to NRSTPD


#define MAX_LEN 16        //Maximum length of the array is 16
#include <Servo.h>        //importing servo library

Servo myservo;        //creating a Servo object
void setup() {                      //the setup function:
  Serial.begin(9600);                            // RFID reader SOUT pin connected to Serial RX pin 
  SPI.begin();          //start the SPI
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the- 
  pinMode(button, INPUT);                     //setting button as input
  pinMode(led,OUTPUT);                        //setting led as output
// RFID /ENABLE pin 
    digitalWrite(chipSelectPin, LOW);     // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
    digitalWrite(NRSTPD, HIGH);   // write a positive output to NRSTPD
  myRFID.AddicoreRFID_Init();     //calling the AddicoreRFID_Init function.
  state = 0;          //initializing state to be zero.
  myservo.attach(7);        //attaching Servo to pin 7
  myservo.write(0);       //setting the servo’s position to 0.
  button_state = 0;  
}

void loop()         //the loop function:
{
  
    uchar i, tmp, checksum1;      //setting -
  uchar status;         //chars -
        uchar str[MAX_LEN];     //for -
        uchar RC_size;        //nfc -
        uchar blockAddr;  //Selection operation block address 0 to 63
        String mynum = "";
    if(digitalRead(button) == HIGH) {
      button_state = 1 - button_state;          
      Serial.print("button pressed");
      delay(500);
    }
    if(button_state == 1) {
      
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led,LOW);
    }
  //Find tags, return tag type:
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str); 
  if (status == MI_OK)        //if the correct nfc chip was sensed
  {
            state = 1 - state;      //make the state the opposite of what it used to be
            if(state == 1) {        //if the state is 1, meaning the locker should be -
//open
              myservo.write(45);                 //set the servo’s position to 150
            } else if(state == 0) {     //if it’s 0, meaning the locker should be closed
              myservo.write(0);     //set the servo’s position back to 0
            }
            delay(500);       //delay for half a seconds to protect from repetitions
  }

  //Anti-collision, return tag serial number 4 bytes
  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)        //in case of multiple keys, repeat the last operation.
  {
            if(str[0] == 94)                     
            {
               Serial.print("This Locker Belongs to \n"); //same
              state = 1 - state;        //process
              if(state == 1) {        //as
              myservo.write(45);                   //before
            } else if(state == 0) {       //...
              myservo.write(0);       //...
            }
                
            } else if(str[0] == 212) {      //if it’s not the right key do nothing.
            }
            Serial.println();       //prints an empty line(for debug pupose)
            delay(500);         //delay to prevent repetitions
  }
    
        myRFID.AddicoreRFID_Halt();      //Command tag into hibernation     
        
}


