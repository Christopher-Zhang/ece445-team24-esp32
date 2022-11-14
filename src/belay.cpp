#include <Arduino.h>
#include "constants.h"
#include "belay.h"
#include "main.h"
/*
in the belay state, turn on the green LED
*/
void belay() {
  Serial.println("Entered the belaying state!");
  digitalWrite(GPIO_NUM_0, LOW); 
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(GPIO_NUM_4, HIGH); //figure out which pin we want later
  Serial.print("Current button value:");
  Serial.println(digitalRead(GPIO_NUM_23));
  //PWM CONTROL IS HERE (SET THE VOLTAGES AND PINS)

}

/*
in the stopped state, turn on the red LED
*/
void stop(int lower_flag) {
  Serial.println("Entered the stopped state!");
  digitalWrite(GPIO_NUM_0, HIGH); 
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(GPIO_NUM_4, LOW); //figure out which pin we want later
  
  //stop the motors

  if(lower_flag) {
    lower(); //we should only be entering the lower state after stopping
  }
}

/*
in the lower state, turn on the blue LED
*/
void lower() {
  Serial.println("Entered the lower state!");
  digitalWrite(BUILTIN_LED, HIGH); //blue
  digitalWrite(GPIO_NUM_0, LOW); //red
  digitalWrite(GPIO_NUM_4, LOW); //green
  int done = 0;
  while(!done) {
    Serial.println("Lowering!!");
    digitalWrite(BUILTIN_LED, HIGH);
    digitalWrite(GPIO_NUM_0, LOW); 
    digitalWrite(GPIO_NUM_4, LOW); //figure out which pin we want later
    done = digitalRead(GPIO_NUM_23); //button press
    Serial.println(done); 
    //send a signal to servo to turn lever 
    //maybe should light an LED to show it? 
  }
}

/**
 * @brief 
 * might be a good idea to drop this function inside the loop
 */
void checkFeedback() { 
  //check feedback voltage and either call 
  // float voltage = float(digitalRead(GPIO_NUM_)); //some value we read from the feedback pin
  float voltage = analogRead(LEFT_FB); //should enter teh stopped state 
  Serial.println(voltage);
  //currently threshold is just 0
  if(voltage > threshold) {
    stop(0); //just want to enter the regular stopped state
  } else {
    belay();
  }
}