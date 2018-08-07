#ifndef PWMTRANSITION_H
#define PWMTRANSITION_H

class PwmTransition{
public:
  PwmTransition();  //Constructor
  void fadeTransition(int outputPin, int endValue, int actualValue, int cycleTime);  //TestFunction

private:
  //Variables here
};

#endif
