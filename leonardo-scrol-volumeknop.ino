// Read the full tutorial at prusaprinters.org   https://blog.prusaprinters.org/3d-print-an-oversized-media-control-volume-knob-arduino-basics_30184/

//https://www.youtube.com/watch?v=LKxvuphif7A

#include <ClickEncoder.h>
#include <TimerOne.h>
#include <HID-Project.h>

#define scrolENCODER_SW 2
#define scrolENCODER_DT 3
#define scrolENCODER_CLK 4 //  

#define volENCODER_SW 5
#define volENCODER_DT 6
#define volENCODER_CLK 7 //  

ClickEncoder *scrollknop; // variable representing the rotary scrollknop
ClickEncoder *volknop; // variable representing the rotary scrollknop

int16_t scrollast, scrolvalue; // variables for current and scrollast rotation scrolvalue
int16_t vollast, volvalue; // variables for current and scrollast rotation scrolvalue

void timerIsr() {
  scrollknop->service();
  volknop->service();
}

void setup() {
  Serial.begin(1152000); // Opens the serial connection used for communication with the PC.
  Consumer.begin(); // Initializes the media keyboard
  scrollknop = new ClickEncoder(scrolENCODER_DT, scrolENCODER_CLK, scrolENCODER_SW); // Initializes the rotary scrollknop with the mentioned pins
  volknop = new ClickEncoder(volENCODER_DT, volENCODER_CLK, volENCODER_SW); // Initializes the rotary scrollknop with the mentioned pins
  Timer1.initialize(1000); // Initializes the timer, which the rotary scrollknop uses to detect rotation
  Timer1.attachInterrupt(timerIsr);
  scrollast = -1;
}


void loop() {
  scrolvalue += scrollknop->getValue();
  volvalue += volknop->getValue();

  ///SCROLL KNOP
  // This part of the code is responsible for the actions when you rotate the scrollknop
  if (scrolvalue != scrollast) { // New scrolvalue is different than the scrollast one, that means to scrollknop was rotated
    if (scrollast < scrolvalue) // Detecting the direction of rotation
      //Consumer.write(MEDIA_VOLUME_UP); // Replace this line to have a different function when rotating counter-clockwise
      Mouse.move(0, 0, -2);

    else
      // Consumer.write(MEDIA_VOLUME_DOWN); // Replace this line to have a different function when rotating clockwise
      Mouse.move(0, 0, 2);

    scrollast = scrolvalue; // Refreshing the "scrollast" varible for the next loop with the current scrolvalue
    // Serial.print("scrol Encoder Value: "); // Text output of the rotation scrolvalue used manily for debugging (open Tools - Serial Monitor to see it)
    // Serial.println(scrolvalue);
  }

  // This next part handles the rotary scrollknop BUTTON
  ClickEncoder::Button b = scrollknop->getButton(); // Asking the button for it's current state
  if (b != ClickEncoder::Open) { // If the button is unpressed, we'll skip to the end of this if block
    // Serial.print("Button: ");
    //#define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      case ClickEncoder::Clicked: // Button was clicked once
        //   Serial.println("scrol click: muisklik");
        //  Serial.println(b);

        Mouse.press(MOUSE_LEFT);
        delay(10);
        Mouse.release(MOUSE_LEFT);



        //Consumer.write(CONSUMER_BROWSER_HOME); // Replace this line to have a different function when clicking button once
        break;

      case ClickEncoder::DoubleClicked: // Button was double clicked
        //    Serial.println("scrol doubleclick:  doe niks ");
        // Consumer.write(CONSUMER_BROWSER_HOME); // Replace this line to have a different function when double-clicking
        Consumer.write(HID_CONSUMER_AL_CALCULATOR); //launch calculator :)

        break;
    }
  }

  ///////////////////////////volume.

  // This part of the code is responsible for the actions when you rotate the vollknop
  if (volvalue != vollast) { // New volvalue is different than the vollast one, that means to vollknop was rotated
    if (vollast < volvalue) // Detecting the direction of rotation
      Consumer.write(MEDIA_VOLUME_UP); // Replace this line to have a different function when rotating counter-clockwise
    // Mouse.move(0, 0, 2);
    else
      Consumer.write(MEDIA_VOLUME_DOWN); // Replace this line to have a different function when rotating clockwise
    // Mouse.move(0, 0, -2);

    vollast = volvalue; // Refreshing the "vollast" varible for the next loop with the current volvalue
    //   Serial.print(" vol Encoder Value: "); // Text output of the rotation volvalue used manily for debugging (open Tools - Serial Monitor to see it)
    //  Serial.println(volvalue);
  }

  // This next part handles the rotary vollknop BUTTON
  ClickEncoder::Button c = volknop->getButton(); // Asking the button for it's current state
  if (c != ClickEncoder::Open) { // If the button is unpressed, we'll skip to the end of this if block
    //  Serial.print("Button: ");
    //#define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (c) {
      case ClickEncoder::Clicked: // Button was clicked once
        //  Serial.println ("vol click: mute or unmute");
        Consumer.write(CONSUMER_BROWSER_BACK); // Replace   MEDIA_VOLUME_MUTE  this line to have a different function when clicking button once CONSUMER_BROWSER_BACK 
        break;

      case ClickEncoder::DoubleClicked: // Button was double clicked
        //    Serial.println(" vol double click : browser ");
        Consumer.write(CONSUMER_BROWSER_HOME); // Replace this line to have a different function when double-clicking
        break;
    }
  }

  delay(10); // Wait 10 milliseconds, we definitely don't need to detect the rotary scrollknop any faster than that
  // The end of the loop() function, it will start again from the beggining (the begginging of the loop function, not the whole document)
}


/*
    This is just a long comment
    Here are some fun functions you can use to replace the default behaviour
    Consumer.write(CONSUMER_BRIGHTNESS_UP);
    Consumer.write(CONSUMER_BRIGHTNESS_DOWN);
    Consumer.write(CONSUMER_BROWSER_HOME);
    Consumer.write(CONSUMER_SCREENSAVER);
    Consumer.write(HID_CONSUMER_AL_CALCULATOR); //launch calculator :)
    Consumer.write(HID_CONSUMER_AC_ZOOM_IN);
    Consumer.write(HID_CONSUMER_AC_SCROLL_UP);
    CONSUMER_SLEEP = 0x32,

    FULL LIST CAN BE FOUND HERE:
    https://github.com/NicoHood/HID/blob/master/src/HID-APIs/ConsumerAPI.h
*/
