#include <Arduino.h>
#include "constants.h"
#include "belay.h"
#include "main.h"
/*
in the belay state, turn on the green LED
*/
void belay() {
  Serial.println("Entered the belaying state!");
  state = CLIMB_STATE;
  // digitalWrite(GPIO_NUM_0, LOW); 
  // digitalWrite(BUILTIN_LED, LOW);
  // digitalWrite(GPIO_NUM_4, HIGH); //figure out which pin we want later
  // Serial.print("Current button value:");
  // Serial.println(digitalRead(GPIO_NUM_23));
  //PWM CONTROL IS HERE (SET THE VOLTAGES AND PINS)

  // SerialBT.println("Belay state " + digitalRead(GPIO_NUM_23));

}

/*
in the stopped state, turn on the red LED
*/
void stop(int lower_flag) {
  Serial.println("Entered the stopped state!");
  state = STOP_STATE;
  // digitalWrite(GPIO_NUM_0, HIGH); 
  // digitalWrite(BUILTIN_LED, LOW);
  // digitalWrite(GPIO_NUM_4, LOW); //figure out which pin we want later
  
  //stop the motors

  if(lower_flag) {
    lower(); //we should only be entering the lower state after stopping
  }
  // SerialBT.println("Belay state " + digitalRead(GPIO_NUM_23));
}

/*
in the lower state, turn on the blue LED
*/
void lower() {
  Serial.println("Entered the lower state!");
  state = LOWER_STATE;
  // digitalWrite(BUILTIN_LED, HIGH); //blue
  // digitalWrite(GPIO_NUM_0, LOW); //red
  // digitalWrite(GPIO_NUM_4, LOW); //green
  int done = 1;
  while(!done) {
    Serial.println("Lowering!!");
    // digitalWrite(BUILTIN_LED, HIGH);
    // digitalWrite(GPIO_NUM_0, LOW); 
    // digitalWrite(GPIO_NUM_4, LOW); //figure out which pin we want later
    //do the actual lowering (servo control)

    // done = digitalRead(GPIO_NUM_23); //button press
    Serial.println(done); 
    //send a signal to servo to turn lever 

    // SerialBT.println("Belay state " + digitalRead(GPIO_NUM_23));
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
  voltage = scale_number(voltage);
  Serial.println(voltage);
  //currently threshold is just 0
  if(voltage > threshold) {
    stop(0); //just want to enter the regular stopped state
  } else {
    belay();
  }
}

/**
 * @brief 
 * Analog pin returns between 0 and 4095, use this function to scale between 0 and 3.3V.
 * Note the pin curve tapers from 0 to 0.1V and 3.2 to 3.3V (the readings aren't super accurate)
 * @param voltage 
 * @return float 
 */
float scale_number(float voltage) {
  float old_min = 0;
  float old_max = 4095;
  float new_min = 0;
  float new_max = 3.3;
  voltage =  (new_max - new_min) * (voltage - old_min) / (old_max - old_min) + new_min;
  return voltage;
}

float send_to_phone() {
  float previous_state = 0;
  float current_state = 0;
  float next_state = 0;

  return 0.;
}

// digiitalWrite(GPIO_NUM_12, HIGH);
// digitalWrite(GPIO_NUM_13, HIGH);
// digitalWrite(GPIO_NUM_14, HIGH);