#include <Arduino.h>
#include <PwmTransition.h>

PwmTransition::PwmTransition(){
  Serial.println("PwmTransition class instance running...");
}

void PwmTransition::fadeTransition(int outputPin, int endValue, int actualValue, int cycleTime){
  int difference = abs(endValue-actualValue);
  if(endValue>actualValue){
    while(actualValue!=endValue){
      actualValue++;
      analogWrite(outputPin,actualValue);
      delay(cycleTime);
    }
  }
  else if(endValue<actualValue){
    while(actualValue!=endValue){
      actualValue--;
      analogWrite(outputPin, actualValue);
      delay(cycleTime);
    }
  }
}
