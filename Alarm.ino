#include <Sim800l.h>
#include <SoftwareSerial.h> //is necesary for the library!! 
#include <PN532_I2C.h>
#include <PN532.h>

#define OPEN false
#define CLOSED true
#define REED_SWITCH 2
#define PIR 3
#define RED_LED 5
#define GREEN_LED 6
#define SIGNAL 7
#define NUMBER_OF_CARDS 2
#define TIME_FOR_UNLOCK 20
#define TIME_BEFORE_HANG 10
PN532_I2C i2c(Wire);
PN532 nfc(i2c);

Sim800l Sim800l;  //to declare the library
char * number = "+48664059986";
bool error; //to catch the response of sendSms
volatile uint8_t card[NUMBER_OF_CARDS][4] = {
                                            {0xF7,0x9F,0xCB, 0xA6},
                                            {0xD9, 0xA7, 0xA4, 0xB2}
                                            };


volatile uint8_t state = OPEN;
volatile bool alarm = false; 
volatile uint8_t alarm_counter = 0;
volatile uint8_t occurance = 0;
void setup() {
  //Outputs setup
  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(SIGNAL,OUTPUT);
  pinMode(REED_SWITCH, INPUT_PULLUP);
  pinMode(PIR, INPUT_PULLUP);
  digitalWrite(RED_LED,HIGH);
  digitalWrite(GREEN_LED,HIGH);
  
  Serial.begin(115200);

  //SIM setup
  Sim800l.begin(); // initializate the library. 
  

  //NFC Setup
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  //Timer Setup
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10); 
  timer(false);
  delay(5000);
  digitalWrite(RED_LED,LOW);

  Serial.println("Setup done!");
}

void loop() {
  bool success,correct = false;
  
  if(state == OPEN)
  {
    digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,HIGH);
    correct = readUID();
      if(correct)
      {
        lock();
      }
  }
  if(state == CLOSED)
  {
    correct = readUID();
    if( (alarm == true) & !(readUID()) )
    {
      timer(true);
      detachInterrupt(digitalPinToInterrupt(REED_SWITCH));
      //detachInterrupt(digitalPinToInterrupt(PIR));
      if( (alarm_counter > TIME_FOR_UNLOCK) && (digitalRead(REED_SWITCH) == HIGH))
        {
          Sim800l.callNumber(number);
          delay(3000);
          if( alarm_counter - TIME_FOR_UNLOCK > TIME_BEFORE_HANG )
          {
            alarm_counter = 0;
            Sim800l.hangoffCall();
            Serial.println("hanged out the call");
          }
        occurance ++;
        Serial.println("ALARM!!!");

        if(occurance > 2 )
        {
          digitalWrite(SIGNAL,HIGH);
        }
        }
      
    }
    if(correct)
    {
      
      timer(false);
      Serial.println("Unlocked");
      detachInterrupt(digitalPinToInterrupt(REED_SWITCH));
      Sim800l.hangoffCall();
      digitalWrite(SIGNAL,LOW);
      delay(2000);
      occurance = 0;
      alarm_counter = 0;
      alarm = false;
      state = OPEN;
      digitalWrite(RED_LED,LOW);
      digitalWrite(GREEN_LED,HIGH);
      delay(5000);
    }
  }

}

bool readUID()
{
  bool success,correct = false;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t checksum = 0;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
      if(success)
      {
        for(uint8_t i = 0; i < NUMBER_OF_CARDS; i++)
        {
          for(uint8_t j = 0; j < 4; j++)
          {
            if( uid[j] == card[i][j] )
            {
              checksum +=1;
            }
            else
            {
              checksum = 0;
              break;
            }
            if (checksum == 4) correct = true;  
          }
        }
      }
  return correct;
}
void lock()
{
  digitalWrite(RED_LED,HIGH);
  digitalWrite(GREEN_LED,HIGH);
  state = CLOSED;
  alarm_counter = 0;
  delay(3000);
  timer(true);
  while( alarm_counter < TIME_FOR_UNLOCK );
  timer(false);
  alarm_counter = 0;
  Serial.println("Locked");
  EIFR = 0x11;
  attachInterrupt(digitalPinToInterrupt(REED_SWITCH),int0, RISING);
  //attachInterrupt(digitalPinToInterrupt(PIR),int0, FALLING);
  digitalWrite(GREEN_LED,LOW);
  delay(1000);
  if( digitalRead(REED_SWITCH) == HIGH ) int0();
  
}
void int0()
{
  digitalWrite(RED_LED,HIGH);
  digitalWrite(GREEN_LED,HIGH);
  detachInterrupt(digitalPinToInterrupt(REED_SWITCH));

 
  alarm_counter = 0;
  if(alarm == false)
  {
   timer(true);
   alarm = true;
  }

}

void timer(bool enable)
{
  if (enable)
  {
    TIMSK1 |= (1 << OCIE1A);
  }
  else TIMSK1 &= ~(1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
  cli();
  alarm_counter ++;
  Serial.println(alarm_counter);
  //if(alarm_counter == TIME_FOR_UNLOCK)
  //{
   // alarm = true;
   // alarm_counter = 0;
  //  TIMSK1 &= ~(1 << OCIE1A);
  sei();
}
