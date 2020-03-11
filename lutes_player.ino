/**
 * \using the MP3Shield Arduino driver with a movement detector,
 *
 * \Special thanks to  Michael P. Flaga
 *
 * Player will begin the stream of track000.mp3 through 
 * , begining with 0.
 */

// libraries
#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>
// #include <Bounce2.h> 

/**
 * \breif Macro for the debounced NEXT pin, with pull-up
 */
// #define B_NEXT  A0

/**
 * \breif Macro for the debounced STOP pin, with pull-up
 */
// #define B_STOP  A1

/**
 * \breif Macro for the debounced PLAY pin, with pull-up
 */
// #define B_PLAY  A2

/**
 * \breif Macro for the Debounce Period [milliseconds]
 */
#define BUTTON_DEBOUNCE_PERIOD 20 //ms

#define led0 A0
#define led1 A1
#define led2 A2
#define led3 A3
#define led4 A4
#define led5 A5
#define sensor 5
#define viewled 10 //debug
/**
 * \brief Object instancing the SdFat library.
 *
 * principal object for handling all SdCard functions.
 */
SdFat sd;

/**
 * \brief Object instancing the SFEMP3Shield library.
 *
 * principal object for handling all the attributes, members and functions for the library.
 */
SFEMP3Shield MP3player;

// Bounce b_Play  = Bounce();

int sensor_thres = 50; // sensor threshold
int8_t tracks_num = 6; // total number of tracks
int8_t play_volume = 0; // 40 is -20dB
int8_t pspeed = 1; //debug, normal=1
bool motion = false; //pir motion state
int sensor_val = 0; //pir read
//bool res = 0; //debug
/**
 * \brief Index of the current track playing.
 *
 * Value indicates current playing track, used to populate "x" for playing the 
 * filename of "track00x.mp3" for track000.mp3 through track254.mp3
 */

// First time to power on it will play a loop
int8_t current_track = 0;

//------------------------------------------------------------------------------
/**
 * \brief Setup the Arduino Chip"s feature for our use.
 *
 * After Arduino"s kernel has booted initialize basic features for this
 * application, such as Serial port and MP3player objects with .begin.
 */
void setup() {
  //Serial.begin(115200);
//  pinMode(B_PLAY, INPUT_PULLUP);
//  b_Play.attach(B_PLAY);
//  b_Play.interval(BUTTON_DEBOUNCE_PERIOD);

  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(viewled, OUTPUT);
  digitalWrite(viewled, LOW); //debug
  turnOffLeds();
  
  if(!sd.begin(9, SPI_HALF_SPEED)) sd.initErrorHalt();
  if (!sd.chdir("/")) sd.errorHalt("sd.chdir");

  MP3player.begin();
  MP3player.setVolume(play_volume,play_volume);
  MP3player.setPlaySpeed(pspeed);
  
  //Serial.println(F("Initialising lyres_player_mouseio2..."));
}


//------------------------------------------------------------------------------
/**
 * \brief Main Loop the Arduino Chip
 *
 * This is called at the end of Arduino kernel"s main loop before recycling.
 * And is where the user"s is executed.
 *
 * \note If the means of refilling is not interrupt based then the
 * MP3player object is serviced with the availaible function.
 */
void turnOffLeds() {
  //digitalWrite(led0, HIGH);
  //digitalWrite(led1, HIGH);
  analogWrite(led0, 255);
  analogWrite(led1, 255);
  analogWrite(led2, 255);
  analogWrite(led3, 255);
  analogWrite(led4, 255);
  analogWrite(led5, 255);
}

void loop() {

// debug*******************
/**digitalWrite(test, 0);
res=digitalRead(test);
//Serial.print(res);
delay(300);
digitalWrite(test, 1);
bool res=digitalRead(test);
//Serial.print(res);
delay(300);
**/
// ***********************debug

delay(300);

sensor_val = digitalRead(sensor);
if (sensor_val == HIGH) {
  motion = true;
  digitalWrite(viewled, HIGH);
}
else {
  motion = false;
  digitalWrite(viewled, LOW);
}
//Serial.print(" ");
//Serial.print(sensor_val);    
    
// Below is only needed if not interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) \
    && ( (USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer) \
    ||   (USE_MP3_REFILL_MEANS == USE_MP3_Polled)      )

  MP3player.available();
#endif

  //debug:
  //MP3player.playTrack(5);
  ////Serial.print(current_track);

   //Do something. Have fun with it.
if (current_track > tracks_num) { 
  //Serial.print("Motion state after loop: ");
  //Serial.println(motion);   
  // if the sensor is signaling and player is not playing, initialise player
  if (motion == true) {
//    //Serial.print("Motion state after loop: ");
    //Serial.println("Motion Detected!!!");    
    current_track = 0;
  }
}
else {
    
  if (MP3player.isPlaying() == 1) {
    
    switch(current_track) {
      case 0: 
        // first run
        break;
      case 1:
        //digitalWrite(led0, HIGH);
        analogWrite(led0, 0);
        // //Serial.println("Turning on led0...");
        break;
      case 2:
        //digitalWrite(led1, HIGH);
        analogWrite(led1, 0);
        // //Serial.println("Turning on led1...");
        break;   
      case 3:
        analogWrite(led2, 0);
        // //Serial.println("Turning on led4...");
        break;
      case 4:
        analogWrite(led3, 0);
        // //Serial.println("Turning on led5...");
        break;
      case 5:
        analogWrite(led4, 0);
        // //Serial.println("Turning on led6..."); 
        break;
      case 6:
        analogWrite(led5, 0);
        // //Serial.println("Turning on led7...");
        break;
    }
   
  }
  else {
    turnOffLeds();
    ++current_track;
  
//    if (current_track > 8) {
//      current_track = 1;
//    }
    if (current_track <= tracks_num) {
      MP3player.playTrack(current_track);
      //Serial.print("Playing Track: ");
      //Serial.println(current_track);
    }
  }

}

}
