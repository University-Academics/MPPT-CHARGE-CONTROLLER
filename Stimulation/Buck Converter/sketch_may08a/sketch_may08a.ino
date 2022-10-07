// Buck Converter test code

#include <TimerOne.h>
 
void setup() 
{
  // Initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  Timer1.initialize(20); // set a timer of length 8uS
  //Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  
  //Set duty cycle
  //Timer1.pwm(9,256);  // 25% duty cycle 
 //Timer1.pwm(9, 512); // 50% duty cycle
  Timer1.pwm(9, 768);  //  75% duty cycle
}
 
void loop()
{
  // Main code loop
  // TODO: Put your regular (non-ISR) logic here
}
 
/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
    // Toggle LED
    //digitalWrite( 13, digitalRead( 13 ) ^ 1 );
}
