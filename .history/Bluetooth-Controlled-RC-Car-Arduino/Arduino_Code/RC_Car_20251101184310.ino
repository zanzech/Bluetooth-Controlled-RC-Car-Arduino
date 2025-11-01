//Bluetooth Control RC Car
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(3,2); // RX, TX
char t = 'S'; // Initialize 't' to 'S' (Stop) to ensure defined behavior at start

#define MLa 8     //left motor 1st pin
#define MLb 9     //left motor 2nd pin
#define MRa 10    //right motor 1st pin
#define MRb 11    //right motor 2nd pin
#define LEDR 6  // The pin the REAR LED is connected to
#define BUZ 5  // The pin the BUZZER is connected to
#define LEDF 7  // The pin the FRONT LED is connected to
 
void setup()
{
  Serial.begin(9600);
bluetooth.begin(9600);
pinMode(MLa,OUTPUT);   //left motors forward
pinMode(MLb,OUTPUT);   //left motors reverse
pinMode(MRa,OUTPUT);   //right motors forward
pinMode(MRb,OUTPUT);   //right motors reverse
pinMode(LEDF, OUTPUT); // Declare the FRONT LED as an output
pinMode(LEDR, OUTPUT); // Declare the REAR LED as an output
pinMode(BUZ, OUTPUT); // Declare the BUZZER as an output

// Ensure the car is stopped at startup
digitalWrite(MLa,LOW);
digitalWrite(MLb,LOW); 
digitalWrite(MRa,LOW);
digitalWrite(MRb,LOW);
}
 
void loop() 
{
// Check for new data ONLY, don't execute the switch if no new data arrived
if(bluetooth.available()>0)
{
  t = bluetooth.read();
  Serial.println(t);
}

// The switch block is now outside the if(bluetooth.available()) check,
// so the last command (stored in 't') is continuously executed.
 switch(t)
 {
  case 'F':
        {            //move forward (all motors rotate in forward direction)
          digitalWrite(MLa,HIGH);
          digitalWrite(MLb,LOW);
          digitalWrite(MRa,HIGH);
          digitalWrite(MRb,LOW);
          // The original logic turned the LED on, waited 10ms, then turned it off *outside* the case.
          // For a constant movement command, it's better to keep the LED ON.
          // If you want a quick flash, the original 'delay(10)' was too short to be noticeable.
          // For now, let's keep it ON with the motor command.
          digitalWrite(LEDF,HIGH);
        }
        // If you uncomment the line below, the LED will flash for 10ms every loop, which is often too fast
        // digitalWrite(LEDF,LOW); 
   break;
   
  case 'B':
        {      //move reverse (all motors rotate in reverse direction)
          digitalWrite(MLa,LOW);
          digitalWrite(MLb,HIGH);
          digitalWrite(MRa,LOW);
          digitalWrite(MRb,HIGH);
          digitalWrite(LEDR,HIGH);
        }
        // digitalWrite(LEDR,LOW); // See comment in case 'F'
   break;
   
  case 'L':
        {      //turn Left (right side motors forward, left side motors stopped)
           // NOTE: The description says "turn right" but the code implements a left turn (right motor on, left motor off).
           // I'm keeping the code logic as-is but noting the discrepancy.
          digitalWrite(MLa,LOW); 
          digitalWrite(MLb,LOW); 
          digitalWrite(MRa,HIGH);
          digitalWrite(MRb,LOW);
          digitalWrite(LEDF,LOW); // Turn off continuous LEDs during turn for cleaner control
          digitalWrite(LEDR,LOW);
        }
   break;
   
  case 'R':
        {      //turn Right (left side motors forward, right side motors stopped)
           // NOTE: The description says "turn left" but the code implements a right turn (left motor on, right motor off).
          digitalWrite(MLa,HIGH); 
          digitalWrite(MLb,LOW); 
          digitalWrite(MRa,LOW);
          digitalWrite(MRb,LOW);
          digitalWrite(LEDF,LOW); // Turn off continuous LEDs during turn for cleaner control
          digitalWrite(LEDR,LOW);
        }
   break;
   
   case 'V':
        {           // BUZZER ON
          digitalWrite(BUZ,HIGH);
                  }
  break;
  
 case 'v':
        {           // BUZZER OFF
          digitalWrite(BUZ,LOW);
                 }
  break;

 case 'W':
        {           // FRONT LED ON
          digitalWrite(LEDF,HIGH);
        }
  break;

  case 'w':
        {           // FRONT LED OFF
          digitalWrite(LEDF,LOW);
        }
  break;

  case 'U':
        {           // REAR LED ON
          digitalWrite(LEDR,HIGH);
        }
  break;

  case 'u':
        {           // REAR LED OFF
          digitalWrite(LEDR,LOW);
        }
  break;
  
    case 'S':
        {           //STOP (all motors stop)
          digitalWrite(MLa,LOW);
          digitalWrite(MLb,LOW); 
          digitalWrite(MRa,LOW);
          digitalWrite(MRb,LOW);
          
          // Also turn off the continuous movement LEDs when stopping
          digitalWrite(LEDF,LOW);
          digitalWrite(LEDR,LOW);
        }
   break; // <--- This was the critical missing break!
        
   // Adding a 'default' case is good practice for unknown commands.
   default:
        // If an unknown character is received, keep the car in the last known state.
        break;
 }
}