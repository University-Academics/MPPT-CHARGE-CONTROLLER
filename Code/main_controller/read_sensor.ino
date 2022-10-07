short int slider(short int dirc) {
  unsigned int reading = 0;
  reading = (dirc) ? analogRead(CHANGE) : analogRead(SLIDE);
  short int result = 0;
  if (abs(CurrentTime - ButtonPreStart[dirc]) > BouncingTime) {
    if (reading < 1000) result = -1;
    else if (reading > 3000) result = 1;
    else result = 0;
    ButtonPreStart[dirc] = CurrentTime;
  }
  return result;
}

bool select_opt() {
  bool result = false;
  if (abs(CurrentTime - ButtonPreStart_sel) > BouncingTime) {
    result = digitalRead(SELECT) ? false : true;
    ButtonPreStart_sel = CurrentTime;
  }
  return result;
}


// void power_measurements() {
  
// InputCurrent = CurMeaOffset - analogRead(CURRENT_IN);}
