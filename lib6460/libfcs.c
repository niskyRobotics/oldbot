///////////////////////////////////////////////////////////////////////////////////
// 	Contains code from: 
//  WaitForStartOrButton.c
//    (c) 2014 William Gardner.
//    Use and modify as you wish.  Please give credit for partial or complete use.
///////////////////////////////////////////////////////////////////////////////////

void waitForStartOrButton()
{
  while (nNxtButtonPressed>=0) {  // wait for button to be released
  }

  while (true)
  {
    getJoystickSettings(joystick);
    nxtDisplayCenteredTextLine(2, "  or Button  ");

    if (!joystick.StopPgm)        // normal FCS start condition
      break;
    if (nNxtButtonPressed>=0) {   // manual bypass start condition

      int buttonToggle;  // holds button state for manual start override with another push
      buttonToggle=0;

      // beeps and waits a few seconds to get out of the way before the program starts
      bDisplayDiagnostics=false;

      eraseDisplay();
      nxtDisplayCenteredBigTextLine(1,"Start");
      nxtDisplayCenteredBigTextLine(3,"In");

      for (int i=3; (i>0)&&(buttonToggle!=2); i--) {
        nxtDisplayCenteredBigTextLine(5,"%d!", i);

        ClearTimer(3);

        PlayImmediateTone(1000,50);

        while (time1[3]<1000) {
          if ((buttonToggle==0)&&(nNxtButtonPressed<0)) {
            buttonToggle=1;
          }
          if ((buttonToggle==1)&&(nNxtButtonPressed>=0)) {
            buttonToggle=2;
            eraseDisplay();
            nxtDisplayCenteredBigTextLine(0,"Abort!");
            nxtDisplayCenteredBigTextLine(2,"Return");
            nxtDisplayCenteredBigTextLine(4,"To");
            nxtDisplayCenteredBigTextLine(6,"Wait!");
            wait1Msec(1000);
            while (nNxtButtonPressed>=0) {  // wait for button to be released
            }
          }
        }
      }

      bDisplayDiagnostics=true;
      eraseDisplay();
      if (buttonToggle!=2)   // start now if no override!
        break;
    }
  }
  return;
}
