/*
  basic button actions:
  =====================

  Button B0 (by default pin A0, middle button on the original TonUINO): play+pause
  Button B1 (by default pin A1, right button on the original TonUINO): volume up
  Button B2 (by default pin A2, left button on the original TonUINO): volume down
  Button B3 (by default pin A3, optional): previous track
  Button B4 (by default pin A4, optional): next track

  additional button actions:
  ==========================

  During idle:
  ------------
  Hold B0 for 5 seconds - Enter parents menu

  During playback:
  ----------------
  Hold B0 for 5 seconds - Reset progress to track 1 (story book mode)
  Hold B1 for 2 seconds - Skip to the next track (album, party and story book mode)
  Hold B2 for 2 seconds - Skip to the previous track (album, party and story book mode)

  During parents menu:
  --------------------
  Click B0 - Confirm selection
  Click B1 - Next option
  Click B2 - Previous option
  Double click B0 - Announce current option
  Hold B0 for 2 seconds - Cancel parents menu or any submenu
  Hold B1 for 2 seconds - Jump 10 options forward
  Hold B2 for 2 seconds - Jump 10 options backwards

  During nfc tag setup mode:
  --------------------------
  Click B0 - Confirm selection
  Click B1 - Next folder, mode or track
  Click B2 - Previous folder, mode or track
  Double click B0 - Announce current folder, mode or track number
  Hold B0 for 2 seconds - Cancel nfc tag setup mode
  Hold B1 for 2 seconds - Jump 10 folders or tracks forward
  Hold B2 for 2 seconds - Jump 10 folders or tracks backwards

  During power up:
  ----------------

  Hold B0 + B1 + B2 - Erase all eeprom contents (resets stored progress and preferences)

  ir remote:
  ==========

  If an ir receiver (like TSOP38238 or similar) is connected to pin 5, you can also use
  an ir remote to remote control TonUINO. The remote needs at least seven buttons to be
  able to support all functions. This feature can be enabled by uncommenting the
  '#define IRREMOTE' below.

  Down below you can hard code (multiple sets of) code mappings for remotes which will
  always be recognized. In addition to these hard coded remotes, you can learn in one
  additional remote using the parents menu, which is then stored in EEPROM.

  There is one function, currently only available with the ir remote: Box lock.
  When TonUINO is locked, the buttons on TonUINO as well as the nfc reader are disabled
  until TonUINO is unlocked again. Playback continues while TonUINO is locked and
  you are still able to control TonUINO using the ir remote (great for parents).

  During idle:
  ------------
  center - toggle box lock
  menu - enter parents menu

  During playback:
  ----------------
  center - toggle box lock
  play+pause - toggle playback
  up / down - volume up / down
  left / right - previous / next track (album, party and story book mode)
  menu - reset progress to track 1 (story book mode)

  During parents menu:
  --------------------
  center - announce current option
  play+pause - confirm selection
  up / down - next / previous option
  left / right - jump 10 options backwards / forward
  menu - cancel

  During nfc tag setup mode:
  --------------------------
  center - announce current folder, mode or track number
  play+pause - confirm selection
  up / down - next / previous folder, mode or track
  left / right - jump 10 folders or tracks backwards / forward
  menu - cancel

  pin code:
  =========

  The complete erase of the eeprom contents (hold vol-, play/pause, vol+ during startup)
  as well as the parents menu can be secured with a pin code of variable length. It is
  defined (and can be changed, just make sure button and ir pins are the same length) in
  the configuration section below, the default pin codes are:

  * for the buttons on the box: play/pause, vol-, vol+, play/pause
  * on the IR remote: center, vol-, vol+, center

  Once the pin code entry is triggered, you have (by default) 10s to enter the pin code
  before it times out. It repeats when entered incorrectly. See status led section
  for information about visual feedback. This feature can be enabled by uncommenting
  the '#define PINCODE' below.

  status led:
  ===========

  If a led is connected to pin 6, useful status information is given using that led.
  The led is solid on when TonUINO is playing a track and it is pulsing slowly when
  TonUINO is idle. When TonUINO is doing anything interactive, the led is blinking every
  500ms. If the LOWVOLTAGE feature (see cubiekid section below) is enabled, the led is
  flashing every 100ms when the operating volatage drops below a certain value. It
  bursts 4 times when TonUINO is locked/unlocked or when a wrong pin code was entered.
  This feature can be enabled by uncommenting the '#define STATUSLED' below.

  cubiekid:
  =========

  If you happen to have a CubieKid case and the CubieKid circuit board, this firmware
  supports both shutdown methods. The inactivity shutdown timer is enabled by default,
  the shutdown due to low battery voltage (which can be configured in the shutdown
  section below) can be enabled by uncommenting the '#define LOWVOLTAGE' below.

  The CubieKid case as well as the CubieKid circuit board, have been designed and developed
  by Jens Hackel aka DB3JHF and can be found here: https://www.thingiverse.com/thing:3148200

  data stored on the nfc tags:
  ============================

  On MIFARE Classic (Mini, 1K & 4K) tags:
  ---------------------------------------

  Up to 16 bytes of data are stored in sector 1 / block 4, of which the first 8 bytes
  are currently in use:

  13 37 B3 47 01 02 04 11 00 00 00 00 00 00 00 00
  ----------- -- -- -- --
       |      |  |  |  |
       |      |  |  |  + assigned track (0x01-0xFF, only used in single mode)
       |      |  |  + assigned playback mode (0x01-0x05)
       |      |  + assigned folder (0x01-0x63)
       |      + version (currently always 0x01)
       + magic cookie to recognize that a card belongs to TonUINO (by default 0x13 0x37 0xb3 0x47)

  On MIFARE Ultralight / Ultralight C and NTAG213/215/216 tags:
  -------------------------------------------------------------

  Up to 16 bytes of data are stored in pages 8-11, of which the first 8 bytes
  are currently in use:

   8   13 37 B3 47 - magic cookie to recognize that a card belongs to TonUINO (by default 0x13 0x37 0xb3 0x47)
   9   01 02 04 11 - version, assigned folder, assigned playback mode, assigned track (if any)
  10   00 00 00 00
  11   00 00 00 00

  additional non standard libraries used in this firmware:
  ========================================================

  MFRC522.h - https://github.com/miguelbalboa/rfid
  DFMiniMp3.h - https://github.com/Makuna/DFMiniMp3
  AceButton.h - https://github.com/bxparks/AceButton
  IRremote.h - https://github.com/z3t0/Arduino-IRremote
  Vcc.h - https://github.com/Yveaux/Arduino_Vcc
*/

// uncomment the below line to enable five button support
// #define FIVEBUTTONS

// uncomment the below line to enable ir remote support
// #define IRREMOTE

// uncomment the below line to enable pin code support
// #define PINCODE

// uncomment the below line to enable status led support
// #define STATUSLED

// uncomment the below line to enable low voltage shutdown support
// #define LOWVOLTAGE

// include required libraries
#include <avr/sleep.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <EEPROM.h>
#include <MFRC522.h>
#include <DFMiniMp3.h>
#include <AceButton.h>
using namespace ace_button;

// include additional library if ir remote support is enabled
#ifdef IRREMOTE
#include <IRremote.h>
#endif

// include additional library if low voltage shutdown support is enabled
#ifdef LOWVOLTAGE
#include <Vcc.h>
#endif

// playback modes
enum {NOMODE, STORY, ALBUM, PARTY, SINGLE, STORYBOOK};

// button actions
enum {NOACTION,
      B0P, B1P, B2P, B3P, B4P,
      B0H, B1H, B2H, B3H, B4H,
      B0D, B1D, B2D, B3D, B4D,
      IRU, IRD, IRL, IRR, IRC, IRM, IRP
     };

// button modes
enum {INIT, PLAY, PAUSE, PIN, CONFIG};

// shutdown timer actions
enum {START, STOP, CHECK, SHUTDOWN};

// preference actions
enum {READ, WRITE, RESET, RESET_PROGRESS};

// define general configuration constants
const uint8_t mp3SerialTxPin = 3;                   // mp3 serial tx, wired with 1k ohm to rx pin of DFPlayer Mini
const uint8_t mp3SerialRxPin = 2;                   // mp3 serial rx, wired straight to tx pin of DFPlayer Mini
const uint8_t mp3BusyPin = 4;                       // reports play state of DFPlayer Mini (LOW = playing)
#ifdef IRREMOTE
const uint8_t irReceiverPin = 5;                    // pin used for the ir receiver
#endif
#ifdef STATUSLED
const uint8_t statusLedPin = 6;                     // pin used for status led
#endif
const uint8_t shutdownPin = 7;                      // pin used to shutdown the system
const uint8_t nfcResetPin = 9;                      // used for spi communication to nfc module
const uint8_t nfcSlaveSelectPin = 10;               // used for spi communication to nfc module
const uint8_t button0Pin = A0;                      // middle button
const uint8_t button1Pin = A1;                      // right button
const uint8_t button2Pin = A2;                      // left button
#ifdef FIVEBUTTONS
const uint8_t button3Pin = A3;                      // optional 4th button
const uint8_t button4Pin = A4;                      // optional 5th button
#endif
const uint16_t buttonClickDelay = 1000;             // time during which a button click is still a click (in milliseconds)
const uint16_t buttonShortLongPressDelay = 2000;    // time after which a button press is considered a long press (in milliseconds)
const uint16_t buttonLongLongPressDelay = 5000;     // longer long press delay for special cases, i.e. to trigger erase nfc tag mode (in milliseconds)
const uint32_t debugConsoleSpeed = 9600;            // speed for the debug console

// number of mp3 files in advert folder + number of mp3 files in mp3 folder
const uint16_t msgCount = 570;

// define magic cookie (by default 0x13 0x37 0xb3 0x47)
const uint8_t magicCookieHex[4] = {0x13, 0x37, 0xb3, 0x47};

#ifdef PINCODE
// define pin codes, code length for the ir code needs to be the same as the button code
// allowed enums for pinCodeButtons : B0P, B1P, B2P (plus B3P & B4P if FIVEBUTTONS is enabled)
// allowed enums for pinCodeIR: IRU, IRD, IRL, IRR, IRC, IRP (IRM cancels and is therefore not allowed)
const uint8_t pinCodeButtons[] = {B0P, B2P, B1P, B0P};    // for example play/pause, vol-, vol+, play/pause
const uint8_t pinCodeIR[] = {IRC, IRD, IRU, IRC};         // for example center, vol-, vol+, center
const uint8_t pinCodeLength = sizeof(pinCodeButtons);
const uint64_t enterPinCodeTimeout = 10000;               // time to enter the pin code (in milliseconds)
#endif

// default values for preferences
const uint8_t mp3StartVolumeDefault = 15;
const uint8_t mp3MaxVolumeDefault = 25;
const uint8_t mp3MenuVolumeDefault = 15;
const uint8_t mp3EqualizerDefault = 1;
const uint8_t shutdownMinutesDefault = 10;
const uint16_t irRemoteUserCodesDefault[7] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

/*
  define hard coded (sets of) code mappings for ir remotes.
  one remote per line with the following order of codes for: up, down, left, right, center, menu, play/pause

  when adding multiple remotes, the array needs to look like this (watch the 'commas'!):

  const uint16_t irRemoteCodes[][7] = {
    {...},
    {...},
    {...}
  };
*/
const uint16_t irRemoteCodes[][7] = {
  {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}   // example, replace 0x0000 with respective codes as per above
};
const uint8_t irRemoteCount = sizeof(irRemoteCodes) / 14;
const uint8_t irRemoteCodeCount = sizeof(irRemoteCodes) / (2 * irRemoteCount);

#ifdef LOWVOLTAGE
// define constants for shutdown feature
const float shutdownMinVoltage = 4.4;                        // minimum expected voltage level (in volts)
const float shutdownWarnVoltage = 4.8;                       // warning voltage level (in volts)
const float shutdownMaxVoltage = 5.0;                        // maximum expected voltage level (in volts)
const float shutdownVoltageCorrection = 1.0 / 1.0;           // voltage measured by multimeter divided by reported voltage
#endif

// define strings
const char *playbackModeName[] = {" ", "story", "album", "party", "single", "storybook"};
const char *nfcStatusMessage[] = {" ", "read", "write", "ok", "failed"};
const char *mp3EqualizerName[] = {" ", "normal", "pop", "rock", "jazz", "classic", "bass"};

// this object stores the nfc tag data
struct nfcTagObject {
  uint32_t magicCookie;
  uint8_t version;
  uint8_t assignedFolder;
  uint8_t playbackMode;
  uint8_t assignedTrack;
} nfcTag;

// this object tracks the playback state
struct playbackObject {
  bool isLocked = false;
  bool isPlaying = false;
  bool firstTrack = true;
  bool playListMode = false;
  uint8_t mp3CurrentVolume = 0;
  uint8_t playTrack = 1;
  uint8_t storedTrack = 1;
  uint8_t folderTrackCount = 0;
  uint8_t playList[255];
} playback;

// this object stores the preferences
struct preferenceObject {
  uint32_t magicCookie;
  uint8_t version;
  uint8_t mp3StartVolume;
  uint8_t mp3MaxVolume;
  uint8_t mp3MenuVolume;
  uint8_t mp3Equalizer;
  uint8_t shutdownMinutes;
  uint16_t irRemoteUserCodes[7];
} preference;

// global variables
uint8_t inputEvent = NOACTION;
uint32_t magicCookie = 0;

// ################################################################################################################################################################
// ############################################################### no configuration below this line ###############################################################
// ################################################################################################################################################################

// declare functions
void checkForInput();
void translateButtonInput(AceButton *button, uint8_t eventType, uint8_t /* buttonState */);
void switchButtonConfiguration(uint8_t buttonMode);
void waitPlaybackToFinish(uint16_t statusLedBlinkInterval);
void printModeFolderTrack(uint8_t playTrack, bool cr);
void playNextTrack(uint16_t globalTrack, bool directionForward, bool triggeredManually);
uint8_t readNfcTagData();
uint8_t writeNfcTagData(uint8_t nfcTagWriteBuffer[], uint8_t nfcTagWriteBufferSize);
void printNfcTagData(uint8_t dataBuffer[], uint8_t dataBufferSize, bool cr);
void printNfcTagType(MFRC522::PICC_Type nfcTagType);
void shutdownTimer(uint8_t timerAction);
void preferences(uint8_t preferenceAction);
uint8_t prompt(uint8_t promptOptions, uint16_t promptHeading, uint16_t promptOffset, uint8_t promptCurrent, uint8_t promptFolder, bool promptPreview);
void parentsMenu();
#ifdef PINCODE
bool enterPinCode();
#endif
#ifdef STATUSLED
void statusLedFade();
void statusLedBlink(uint16_t statusLedBlinkInterval);
void statusLedBurst();
#endif

// used by DFPlayer Mini library during callbacks
class Mp3Notify {
  public:
    static void OnError(uint16_t returnValue) {
      switch (returnValue) {
        case DfMp3_Error_Busy:
          Serial.print(F("busy"));
          break;
        case DfMp3_Error_Sleeping:
          Serial.print(F("sleep"));
          break;
        case DfMp3_Error_SerialWrongStack:
          Serial.print(F("serial stack"));
          break;
        case DfMp3_Error_CheckSumNotMatch:
          Serial.print(F("checksum"));
          break;
        case DfMp3_Error_FileIndexOut:
          Serial.print(F("file index"));
          break;
        case DfMp3_Error_FileMismatch:
          Serial.print(F("file mismatch"));
          break;
        case DfMp3_Error_Advertise:
          Serial.print(F("advertise"));
          break;
        case DfMp3_Error_General:
          Serial.print(F("general"));
          break;
        default:
          Serial.print(F("unknown"));
          break;
      }
      Serial.print(F(" error "));
      Serial.println(returnValue);
    }
    static void OnPlayFinished(uint16_t returnValue) {
      playNextTrack(returnValue, true, false);
    }
    static void OnCardOnline(uint16_t returnValue) {
      Serial.print(F("sd online "));
      Serial.println(returnValue);
    }
    static void OnCardInserted(uint16_t returnValue) {
      Serial.print(F("sd in "));
      Serial.println(returnValue);
    }
    static void OnCardRemoved(uint16_t returnValue) {
      Serial.print(F("sd out "));
      Serial.println(returnValue);
    }
};

SoftwareSerial mp3Serial(mp3SerialRxPin, mp3SerialTxPin);                     // create SoftwareSerial instance
MFRC522 mfrc522(nfcSlaveSelectPin, nfcResetPin);                              // create MFRC522 instance
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mp3Serial);                          // create DFMiniMp3 instance
ButtonConfig button0Config;                                                   // create ButtonConfig instance
ButtonConfig button1Config;                                                   // create ButtonConfig instance
ButtonConfig button2Config;                                                   // create ButtonConfig instance
AceButton button0(&button0Config);                                            // create AceButton instance
AceButton button1(&button1Config);                                            // create AceButton instance
AceButton button2(&button2Config);                                            // create AceButton instance
#ifdef FIVEBUTTONS
ButtonConfig button3Config;                                                   // create ButtonConfig instance
ButtonConfig button4Config;                                                   // create ButtonConfig instance
AceButton button3(&button3Config);                                            // create AceButton instance
AceButton button4(&button4Config);                                            // create AceButton instance
#endif

#ifdef IRREMOTE
IRrecv irReceiver(irReceiverPin);                                             // create IRrecv instance
decode_results irReading;                                                     // create decode_results instance to store received ir reading
#endif

#ifdef LOWVOLTAGE
Vcc shutdownVoltage(shutdownVoltageCorrection);                               // create Vcc instance
#endif

void setup() {
  // things we need to do immediately on startup
  pinMode(shutdownPin, OUTPUT);
  digitalWrite(shutdownPin, HIGH);
  magicCookie = (uint32_t)magicCookieHex[0] << 24;
  magicCookie += (uint32_t)magicCookieHex[1] << 16;
  magicCookie += (uint32_t)magicCookieHex[2] << 8;
  magicCookie += (uint32_t)magicCookieHex[3];

  // start normal operation
  Serial.begin(debugConsoleSpeed);
  while (!Serial);
  Serial.println(F("\n\nTonUINO JUKEBOX"));
  Serial.println(F("by Thorsten Voß"));
  Serial.println(F("Stephan Eisfeld"));
  Serial.println(F("---------------"));
  Serial.println(F("flashed"));
  Serial.print(F("  "));
  Serial.println(__DATE__);
  Serial.print(F("  "));
  Serial.println(__TIME__);

  preferences(READ);

  Serial.println(F("init nfc"));
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  Serial.println(F("init mp3"));
  mp3.begin();
  delay(2000);
  Serial.print(F("   start "));
  Serial.println(preference.mp3StartVolume);
  mp3.setVolume(playback.mp3CurrentVolume = preference.mp3StartVolume);
  Serial.print(F("     max "));
  Serial.println(preference.mp3MaxVolume);
  Serial.print(F("    menu "));
  Serial.println(preference.mp3MenuVolume);
  Serial.print(F("      eq "));
  Serial.println(mp3EqualizerName[preference.mp3Equalizer]);
  mp3.setEq((DfMp3_Eq)(preference.mp3Equalizer - 1));
  Serial.print(F("  tracks "));
  Serial.println(mp3.getTotalTrackCount() - msgCount);
  pinMode(mp3BusyPin, INPUT);

  Serial.print(F("init"));
  pinMode(button0Pin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  button0.init(button0Pin, HIGH, 0);
  button1.init(button1Pin, HIGH, 1);
  button2.init(button2Pin, HIGH, 2);
#ifdef FIVEBUTTONS
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);
  button3.init(button3Pin, HIGH, 3);
  button4.init(button4Pin, HIGH, 4);
  Serial.print(F(" 5"));
#else
  Serial.print(F(" 3"));
#endif
  Serial.println(F(" buttons"));
  switchButtonConfiguration(INIT);

  Serial.print(F("init "));
  Serial.print(preference.shutdownMinutes);
  Serial.println(F("m timer"));
  shutdownTimer(START);

#ifdef IRREMOTE
  Serial.println(F("init ir"));
  irReceiver.enableIRIn();
#endif

#ifdef STATUSLED
  Serial.println(F("init led"));
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, HIGH);
#endif

#ifdef LOWVOLTAGE
  Serial.println(F("init lvm"));
  Serial.print(F("  ex-"));
  Serial.print(shutdownMaxVoltage);
  Serial.print(F("V"));
  Serial.print(F(" wa-"));
  Serial.print(shutdownWarnVoltage);
  Serial.print(F("V"));
  Serial.print(F(" sh-"));
  Serial.print(shutdownMinVoltage);
  Serial.print(F("V"));
  Serial.print(F(" cu-"));
  Serial.print(shutdownVoltage.Read_Volts());
  Serial.print(F("V ("));
  Serial.print(shutdownVoltage.Read_Perc(shutdownMinVoltage, shutdownMaxVoltage));
  Serial.println(F("%)"));
#endif

  // hold down all three buttons while powering up: erase the eeprom contents
  if (button0.isPressedRaw() && button1.isPressedRaw() && button2.isPressedRaw()) {
#ifdef PINCODE
    if (enterPinCode()) {
#endif
      Serial.println(F("init eeprom"));
      for (uint16_t i = 0; i <= EEPROM.length(); i++) {
        EEPROM.update(i, 0);
#ifdef STATUSLED
        statusLedBlink(50);
#endif
      }
      preferences(RESET);
      mp3.setVolume(playback.mp3CurrentVolume = preference.mp3StartVolume);
      mp3.setEq((DfMp3_Eq)(preference.mp3Equalizer - 1));
      shutdownTimer(START);
      mp3.playMp3FolderTrack(807);
      waitPlaybackToFinish(50);
#ifdef PINCODE
    }
#endif
  }

  switchButtonConfiguration(PAUSE);
  mp3.playMp3FolderTrack(800);
  Serial.println(F("ready"));
}

void loop() {
  playback.isPlaying = !digitalRead(mp3BusyPin);
  checkForInput();
  shutdownTimer(CHECK);

#ifdef LOWVOLTAGE
  // if low voltage level is reached, store progress and shutdown
  if (shutdownVoltage.Read_Volts() <= shutdownMinVoltage) {
    if (nfcTag.playbackMode == STORYBOOK) EEPROM.update(nfcTag.assignedFolder, playback.playTrack);
    mp3.playMp3FolderTrack(806);
    waitPlaybackToFinish(50);
    Serial.println(F("lv shut"));
    digitalWrite(shutdownPin, LOW);
  }
  else if (shutdownVoltage.Read_Volts() <= shutdownWarnVoltage) {
#ifdef STATUSLED
    statusLedBlink(100);
#endif
  }
  else {
#ifdef STATUSLED
    statusLedFade();
#endif
  }
#else
#ifdef STATUSLED
  statusLedFade();
#endif
#endif

  // ################################################################################
  // # main code block, if nfc tag is detected and TonUINO is not locked do something
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && !playback.isLocked) {
    // if the current playback mode is story book mode, only while playing: store the current progress
    if (nfcTag.playbackMode == STORYBOOK && playback.isPlaying) {
      Serial.print(F("save "));
      printModeFolderTrack(playback.playTrack, true);
      EEPROM.update(nfcTag.assignedFolder, playback.playTrack);
    }
    uint8_t readNfcTagStatus = readNfcTagData();
    // ##############################
    // # nfc tag is successfully read
    if (readNfcTagStatus == 1) {
      // #############################################################################
      // # nfc tag has our magic cookie on it, use data from nfc tag to start playback
      if (nfcTag.magicCookie == magicCookie) {
        switchButtonConfiguration(PLAY);
        shutdownTimer(STOP);

        // prepare playlist for playback
        randomSeed(micros());
        playback.folderTrackCount = mp3.getFolderTrackCount(nfcTag.assignedFolder);
        for (uint8_t i = 0; i < 255; i++) playback.playList[i] = i + 1 <= playback.folderTrackCount ? i + 1 : 0;

        switch (nfcTag.playbackMode) {
          case STORY:
            playback.playTrack = random(1, playback.folderTrackCount + 1);
            break;
          case ALBUM:
            playback.playTrack = 1;
            break;
          case PARTY:
            playback.playTrack = 1;
            // shuffle playlist
            for (uint8_t i = 0; i < playback.folderTrackCount; i++) {
              uint8_t j = random(0, playback.folderTrackCount);
              uint8_t temp = playback.playList[i];
              playback.playList[i] = playback.playList[j];
              playback.playList[j] = temp;
            }
            break;
          case SINGLE:
            playback.playTrack = nfcTag.assignedTrack;
            break;
          case STORYBOOK:
            playback.storedTrack = EEPROM.read(nfcTag.assignedFolder);
            // don't resume from eeprom, play from the beginning
            if (playback.storedTrack == 0 || playback.storedTrack > playback.folderTrackCount) playback.playTrack = 1;
            // resume from eeprom
            else {
              playback.playTrack = playback.storedTrack;
              Serial.print(F("resume "));
            }
            break;
          default:
            break;
        }

        playback.firstTrack = true;
        playback.playListMode = true;
        printModeFolderTrack(playback.playList[playback.playTrack - 1], true);
        mp3.playFolderTrack(nfcTag.assignedFolder, playback.playList[playback.playTrack - 1]);
      }
      // # end - nfc tag has our magic cookie on it
      // ##########################################

      // #####################################################################################
      // # nfc tag does not have our magic cookie on it, start setup to configure this nfc tag
      else if (nfcTag.magicCookie == 0) {
        bool tagSetupSuccessfull = false;
        playback.playListMode = false;

        // set volume to menu volume
        mp3.setVolume(preference.mp3MenuVolume);

        switchButtonConfiguration(CONFIG);
        shutdownTimer(STOP);

        while (true) {
          Serial.println(F("setup tag"));
          Serial.println(F("folder"));
          nfcTag.assignedFolder = prompt(99, 801, 0, 0, 0, true);
          if (nfcTag.assignedFolder == 0) {
            mp3.playMp3FolderTrack(805);
            waitPlaybackToFinish(500);
            break;
          }
          Serial.println(F("mode"));
          nfcTag.playbackMode = prompt(5, 820, 820, 0, 0, false);
          if (nfcTag.playbackMode == 0) {
            mp3.playMp3FolderTrack(805);
            waitPlaybackToFinish(500);
            break;
          }
          else if (nfcTag.playbackMode == SINGLE) {
            Serial.println(F("track"));
            nfcTag.assignedTrack = prompt(mp3.getFolderTrackCount(nfcTag.assignedFolder), 802, 0, 0, nfcTag.assignedFolder, true);
            if (nfcTag.assignedTrack == 0) {
              mp3.playMp3FolderTrack(805);
              waitPlaybackToFinish(500);
              break;
            }
          }
          uint8_t bytesToWrite[] = {magicCookieHex[0],                 // 1st byte of magic cookie (by default 0x13)
                                    magicCookieHex[1],                 // 2nd byte of magic cookie (by default 0x37)
                                    magicCookieHex[2],                 // 3rd byte of magic cookie (by default 0xb3)
                                    magicCookieHex[3],                 // 4th byte of magic cookie (by default 0x47)
                                    0x01,                              // version 1
                                    nfcTag.assignedFolder,             // the folder selected by the user
                                    nfcTag.playbackMode,               // the playback mode selected by the user
                                    nfcTag.assignedTrack,              // the track selected by the user
                                    0x00, 0x00, 0x00, 0x00,            // reserved for future use
                                    0x00, 0x00, 0x00, 0x00             // reserved for future use
                                   };
          uint8_t writeNfcTagStatus = writeNfcTagData(bytesToWrite, sizeof(bytesToWrite));
          if (writeNfcTagStatus == 1) {
            tagSetupSuccessfull = true;
            mp3.playMp3FolderTrack(803);
            waitPlaybackToFinish(500);
          }
          else mp3.playMp3FolderTrack(804);
          waitPlaybackToFinish(500);
          break;
        }

        if (!tagSetupSuccessfull) {
          nfcTag.magicCookie = 0;
          nfcTag.version = 0;
          nfcTag.assignedFolder = 0;
          nfcTag.playbackMode = 0;
          nfcTag.assignedTrack = 0;
        }
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();

        // restore playback volume, can't be higher than maximum volume
        mp3.setVolume(playback.mp3CurrentVolume = min(playback.mp3CurrentVolume, preference.mp3MaxVolume));

        switchButtonConfiguration(PAUSE);
        shutdownTimer(START);
        inputEvent = NOACTION;
      }
      // # end - nfc tag does not have our magic cookie on it
      // ####################################################
    }
    // # end - nfc tag is successfully read
    // ####################################
  }
  // # end - main code block
  // #######################

  // ##################################################################################
  // # handle button and ir remote events during playback or while waiting for nfc tags
  // ir remote center: toggle box lock
  if (inputEvent == IRC) {
    if (!playback.isLocked) {
      Serial.println(F("lock"));
      playback.isLocked = true;
#ifdef STATUSLED
      statusLedBurst();
#endif
    }
    else {
      Serial.println(F("unlock"));
      playback.isLocked = false;
#ifdef STATUSLED
      statusLedBurst();
#endif
    }
  }
  // button 0 (middle) press or ir remote play+pause: toggle playback
  else if ((inputEvent == B0P && !playback.isLocked) || inputEvent == IRP) {
    if (playback.isPlaying) {
      switchButtonConfiguration(PAUSE);
      shutdownTimer(START);
      Serial.println(F("pause"));
      mp3.pause();
      // if the current playback mode is story book mode: store the current progress
      if (nfcTag.playbackMode == STORYBOOK) {
        Serial.print(F("save "));
        printModeFolderTrack(playback.playTrack, true);
        EEPROM.update(nfcTag.assignedFolder, playback.playTrack);
      }
    }
    else {
      if (playback.playListMode) {
        switchButtonConfiguration(PLAY);
        shutdownTimer(STOP);
        Serial.println(F("play"));
        mp3.start();
      }
    }
  }
  // button 1 (right) press or ir remote up while playing: increase volume
  else if (((inputEvent == B1P && !playback.isLocked) || inputEvent == IRU) && playback.isPlaying) {
    if (playback.mp3CurrentVolume < preference.mp3MaxVolume) {
      mp3.setVolume(++playback.mp3CurrentVolume);
      Serial.print(F("volume "));
      Serial.println(playback.mp3CurrentVolume);
    }
  }
  // button 2 (left) press or ir remote down while playing: decrease volume
  else if (((inputEvent == B2P && !playback.isLocked) || inputEvent == IRD) && playback.isPlaying) {
    if (playback.mp3CurrentVolume > 0) {
      mp3.setVolume(--playback.mp3CurrentVolume);
      Serial.print(F("volume "));
      Serial.println(playback.mp3CurrentVolume);
    }
  }
  // button 1 (right) hold for 2 sec or button 5 press or ir remote right, only during album, party and story book mode while playing: next track
  else if ((((inputEvent == B1H || inputEvent == B4P) && !playback.isLocked) || inputEvent == IRR) && (nfcTag.playbackMode == ALBUM || nfcTag.playbackMode == PARTY || nfcTag.playbackMode == STORYBOOK) && playback.isPlaying) {
    Serial.println(F("next"));
    playNextTrack(0, true, true);
  }
  // button 2 (left) hold for 2 sec or button 4 press or ir remote left, only during album, party and story book mode while playing: previous track
  else if ((((inputEvent == B2H || inputEvent == B3P) && !playback.isLocked) || inputEvent == IRL) && (nfcTag.playbackMode == ALBUM || nfcTag.playbackMode == PARTY || nfcTag.playbackMode == STORYBOOK) && playback.isPlaying) {
    Serial.println(F("prev"));
    playNextTrack(0, false, true);
  }
  // button 0 (middle) hold for 5 sec or ir remote menu, only during story book mode while playing: reset progress
  else if (((inputEvent == B0H && !playback.isLocked) || inputEvent == IRM) && nfcTag.playbackMode == STORYBOOK && playback.isPlaying) {
    playback.playTrack = 1;
    Serial.print(F("reset "));
    printModeFolderTrack(playback.playTrack, true);
    EEPROM.update(nfcTag.assignedFolder, 0);
    mp3.playFolderTrack(nfcTag.assignedFolder, playback.playTrack);
  }
  // button 0 (middle) hold for 5 sec or ir remote menu while not playing: parents menu
  else if (((inputEvent == B0H && !playback.isLocked) || inputEvent == IRM) && !playback.isPlaying) {
#ifdef PINCODE
    if (enterPinCode()) parentsMenu();
#else
    parentsMenu();
#endif
    Serial.println(F("ready"));
  }
  // # end - handle button or ir remote events during playback or while waiting for nfc tags
  // #######################################################################################

  mp3.loop();
}

// ################################################################################################################################################################
// ################################################################ functions are below this line! ################################################################
// ################################################################################################################################################################

// checks all input sources and populates the global inputEvent variable
void checkForInput() {
  // clear inputEvent
  inputEvent = NOACTION;

  // check all buttons
  button0.check();
  button1.check();
  button2.check();
#ifdef FIVEBUTTONS
  button3.check();
  button4.check();
#endif

#ifdef IRREMOTE
  uint8_t irRemoteEvent = NOACTION;
  uint16_t irRemoteCode = 0;
  static uint64_t irRemoteOldMillis;

  // poll ir receiver, has precedence over (overwrites) physical buttons
  if (irReceiver.decode(&irReading)) {
    // on NEC encoding 0xFFFFFFFF means the button is held down, we ignore this
    if (!(irReading.decode_type == NEC && irReading.value == 0xFFFFFFFF)) {
      // convert irReading.value from 32bit to 16bit
      irRemoteCode = (irReading.value & 0xFFFF);
      for (uint8_t i = 0; i < irRemoteCount; i++) {
        for (uint8_t j = 0; j < irRemoteCodeCount; j++) {
          //if we have a match, temporally populate irRemoteEvent and break
          if (irRemoteCode == irRemoteCodes[i][j] || irRemoteCode == preference.irRemoteUserCodes[j]) {
            // 16 is used as an offset in the button action enum list - 17 is the first ir action
            irRemoteEvent = 16 + j;
            break;
          }
        }
        // if the inner loop had a match, populate inputEvent and break
        // ir remote key presses are debounced by 250ms
        if (irRemoteEvent != 0 && millis() - irRemoteOldMillis >= 250) {
          irRemoteOldMillis = millis();
          inputEvent = irRemoteEvent;
          break;
        }
      }
    }
    irReceiver.resume();
  }
#endif
}

// translates the various button events into enums
void translateButtonInput(AceButton *button, uint8_t eventType, uint8_t /* buttonState */) {
  switch (button->getId()) {
    // button 0 (middle)
    case 0:
      switch (eventType) {
        case AceButton::kEventClicked:
          inputEvent = B0P;
          break;
        case AceButton::kEventLongPressed:
          inputEvent = B0H;
          break;
        case AceButton::kEventDoubleClicked:
          inputEvent = B0D;
          break;
        default:
          break;
      }
      break;
    // button 1 (right)
    case 1:
      switch (eventType) {
        case AceButton::kEventClicked:
          inputEvent = B1P;
          break;
        case AceButton::kEventLongPressed:
          inputEvent = B1H;
          break;
        default:
          break;
      }
      break;
    // button 2 (left)
    case 2:
      switch (eventType) {
        case AceButton::kEventClicked:
          inputEvent = B2P;
          break;
        case AceButton::kEventLongPressed:
          inputEvent = B2H;
          break;
        default:
          break;
      }
      break;
#ifdef FIVEBUTTONS
    // optional 4th button
    case 3:
      switch (eventType) {
        case AceButton::kEventClicked:
          inputEvent = B3P;
          break;
        default:
          break;
      }
      break;
    // optional 5th button
    case 4:
      switch (eventType) {
        case AceButton::kEventClicked:
          inputEvent = B4P;
          break;
        default:
          break;
      }
      break;
#endif
    default:
      break;
  }
}

// switches button configuration dependig on the state that TonUINO is in
void switchButtonConfiguration(uint8_t buttonMode) {
  switch (buttonMode) {
    case INIT:
      // button 0 (middle)
      button0Config.setEventHandler(translateButtonInput);
      button0Config.setFeature(ButtonConfig::kFeatureClick);
      button0Config.setFeature(ButtonConfig::kFeatureSuppressAfterClick);
      button0Config.setClickDelay(buttonClickDelay);
      button0Config.setFeature(ButtonConfig::kFeatureLongPress);
      button0Config.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
      button0Config.setLongPressDelay(buttonShortLongPressDelay);
      // button 1 (right)
      button1Config.setEventHandler(translateButtonInput);
      button1Config.setFeature(ButtonConfig::kFeatureClick);
      button1Config.setFeature(ButtonConfig::kFeatureSuppressAfterClick);
      button1Config.setClickDelay(buttonClickDelay);
#ifndef FIVEBUTTONS
      // only enable long press on button 1 (right) when in 3 button mode
      button1Config.setFeature(ButtonConfig::kFeatureLongPress);
      button1Config.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
      button1Config.setLongPressDelay(buttonShortLongPressDelay);
#endif
      // button 2 (left)
      button2Config.setEventHandler(translateButtonInput);
      button2Config.setFeature(ButtonConfig::kFeatureClick);
      button2Config.setFeature(ButtonConfig::kFeatureSuppressAfterClick);
      button2Config.setClickDelay(buttonClickDelay);
#ifndef FIVEBUTTONS
      // only enable long press on button 2 (left) when in 3 button mode
      button2Config.setFeature(ButtonConfig::kFeatureLongPress);
      button2Config.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
      button2Config.setLongPressDelay(buttonShortLongPressDelay);
#endif
#ifdef FIVEBUTTONS
      // optional 4th button
      button3Config.setEventHandler(translateButtonInput);
      button3Config.setFeature(ButtonConfig::kFeatureClick);
      button3Config.setFeature(ButtonConfig::kFeatureSuppressAfterClick);
      button3Config.setClickDelay(buttonClickDelay);
      // optional 5th button
      button4Config.setEventHandler(translateButtonInput);
      button4Config.setFeature(ButtonConfig::kFeatureClick);
      button4Config.setFeature(ButtonConfig::kFeatureSuppressAfterClick);
      button4Config.setClickDelay(buttonClickDelay);
#endif
      break;
    case PLAY:
      // button 0 (middle)
      button0Config.clearFeature(ButtonConfig::kFeatureDoubleClick);
      button0Config.clearFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
      button0Config.clearFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick);
      button0Config.setLongPressDelay(buttonLongLongPressDelay);
      break;
    case PAUSE:
      // button 0 (middle)
      button0Config.clearFeature(ButtonConfig::kFeatureDoubleClick);
      button0Config.clearFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
      button0Config.clearFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick);
      button0Config.setLongPressDelay(buttonLongLongPressDelay);
      break;
    case PIN:
      // button 0 (middle)
      button0Config.clearFeature(ButtonConfig::kFeatureDoubleClick);
      button0Config.clearFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
      button0Config.clearFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick);
      button0Config.setLongPressDelay(buttonShortLongPressDelay);
      break;
    case CONFIG:
      // button 0 (middle)
      button0Config.setFeature(ButtonConfig::kFeatureDoubleClick);
      button0Config.setFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
      button0Config.setFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick);
      button0Config.setLongPressDelay(buttonShortLongPressDelay);
      break;
    default:
      break;
  }
}

// waits for current playing track to finish
void waitPlaybackToFinish(uint16_t statusLedBlinkInterval) {
  uint64_t waitPlaybackToStartMillis = millis();

  delay(100);
  while (digitalRead(mp3BusyPin)) {
    if (millis() - waitPlaybackToStartMillis >= 10000) break;
#ifdef STATUSLED
    statusLedBlink(statusLedBlinkInterval);
#endif
  }
  while (!digitalRead(mp3BusyPin)) {
#ifdef STATUSLED
    statusLedBlink(statusLedBlinkInterval);
#endif
  }
}

// prints current mode, folder and track information
void printModeFolderTrack(uint8_t playTrack, bool cr) {
  Serial.print(playbackModeName[nfcTag.playbackMode]);
  Serial.print(F("-"));
  Serial.print(nfcTag.assignedFolder);
  Serial.print(F("-"));
  Serial.print(playTrack);
  Serial.print(F("/"));
  Serial.print(playback.folderTrackCount);
  if (cr) Serial.println();
}

// plays next track depending on the current playback mode
void playNextTrack(uint16_t globalTrack, bool directionForward, bool triggeredManually) {
  static uint16_t lastCallTrack = 0;

  //delay 100ms to be on the safe side with the serial communication
  delay(100);

  // we only advance to a new track when in playlist mode, not during interactive prompt playback (ie. during configuration of a new nfc tag)
  if (!playback.playListMode) return;

  // story mode (1): play one random track in folder
  // single mode (4): play one single track in folder
  // there is no next track in story and single mode, stop playback
  if (nfcTag.playbackMode == STORY || nfcTag.playbackMode == SINGLE) {
    playback.playListMode = false;
    switchButtonConfiguration(PAUSE);
    shutdownTimer(START);
    Serial.print(playbackModeName[nfcTag.playbackMode]);
    Serial.println(F("-stop"));
    mp3.stop();
  }

  // album mode (2): play the complete folder
  // party mode (3): shuffle the complete folder
  // story book mode (5): play the complete folder and track progress
  // advance to the next or previous track, stop if the end of the folder is reached
  if (nfcTag.playbackMode == ALBUM || nfcTag.playbackMode == PARTY || nfcTag.playbackMode == STORYBOOK) {

    // **workaround for some DFPlayer mini modules that make two callbacks in a row when finishing a track**
    // reset lastCallTrack to avoid lockup when playback was just started
    if (playback.firstTrack) {
      playback.firstTrack = false;
      lastCallTrack = 0;
    }
    // check if we automatically get called with the same track number twice in a row, if yes return immediately
    if (lastCallTrack == globalTrack && !triggeredManually) return;
    else lastCallTrack = globalTrack;

    // play next track?
    if (directionForward) {
      // there are more tracks after the current one, play next track
      if (playback.playTrack < playback.folderTrackCount) {
        playback.playTrack++;
        printModeFolderTrack(playback.playList[playback.playTrack - 1], true);
        mp3.playFolderTrack(nfcTag.assignedFolder, playback.playList[playback.playTrack - 1]);
      }
      // there are no more tracks after the current one
      else {
        // if not triggered manually, stop playback (and reset progress)
        if (!triggeredManually) {
          playback.playListMode = false;
          switchButtonConfiguration(PAUSE);
          shutdownTimer(START);
          Serial.print(playbackModeName[nfcTag.playbackMode]);
          Serial.print(F("-stop"));
          if (nfcTag.playbackMode == STORYBOOK) {
            Serial.print(F("-reset"));
            EEPROM.update(nfcTag.assignedFolder, 0);
          }
          Serial.println();
          mp3.stop();
        }
      }
    }
    // play previous track?
    else {
      // there are more tracks before the current one, play the previous track
      if (playback.playTrack > 1) {
        playback.playTrack--;
        printModeFolderTrack(playback.playList[playback.playTrack - 1], true);
        mp3.playFolderTrack(nfcTag.assignedFolder, playback.playList[playback.playTrack - 1]);
      }
    }
  }
}

// reads data from nfc tag
uint8_t readNfcTagData() {
  uint8_t nfcTagReadBuffer[16];
  uint8_t piccReadBuffer[18];
  uint8_t piccReadBufferSize = sizeof(piccReadBuffer);
  bool nfcTagReadSuccess = false;
  MFRC522::StatusCode piccStatus;
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  // decide which code path to take depending on picc type
  if (piccType == MFRC522::PICC_TYPE_MIFARE_MINI || piccType ==  MFRC522::PICC_TYPE_MIFARE_1K || piccType == MFRC522::PICC_TYPE_MIFARE_4K) {
    uint8_t classicBlock = 4;
    uint8_t classicTrailerBlock = 7;
    MFRC522::MIFARE_Key classicKey;
    for (uint8_t i = 0; i < 6; i++) classicKey.keyByte[i] = 0xFF;

    // check if we can authenticate with classicKey
    piccStatus = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, classicTrailerBlock, &classicKey, &mfrc522.uid);
    if (piccStatus == MFRC522::STATUS_OK) {
      // read 16 bytes from nfc tag (by default sector 1 / block 4)
      piccStatus = (MFRC522::StatusCode)mfrc522.MIFARE_Read(classicBlock, piccReadBuffer, &piccReadBufferSize);
      if (piccStatus == MFRC522::STATUS_OK) {
        nfcTagReadSuccess = true;
        memcpy(nfcTagReadBuffer, piccReadBuffer, sizeof(nfcTagReadBuffer));
      }
      else Serial.println(mfrc522.GetStatusCodeName(piccStatus));
    }
    else Serial.println(mfrc522.GetStatusCodeName(piccStatus));
  }
  else if (piccType == MFRC522::PICC_TYPE_MIFARE_UL) {
    uint8_t ultralightStartPage = 8;
    uint8_t ultralightACK[] = {0, 0};
    MFRC522::MIFARE_Key ultralightKey;
    for (uint8_t i = 0; i < 4; i++) ultralightKey.keyByte[i] = 0xFF;

    // check if we can authenticate with ultralightKey
    piccStatus = (MFRC522::StatusCode)mfrc522.PCD_NTAG216_AUTH(ultralightKey.keyByte, ultralightACK);
    if (piccStatus == MFRC522::STATUS_OK) {
      // read 16 bytes from nfc tag (by default pages 8-11)
      for (uint8_t ultralightPage = ultralightStartPage; ultralightPage < ultralightStartPage + 4; ultralightPage++) {
        piccStatus = (MFRC522::StatusCode)mfrc522.MIFARE_Read(ultralightPage, piccReadBuffer, &piccReadBufferSize);
        if (piccStatus == MFRC522::STATUS_OK) {
          nfcTagReadSuccess = true;
          memcpy(nfcTagReadBuffer + ((ultralightPage * 4) - (ultralightStartPage * 4)), piccReadBuffer, 4);
        }
        else {
          nfcTagReadSuccess = false;
          Serial.println(mfrc522.GetStatusCodeName(piccStatus));
          break;
        }
      }
    }
    else Serial.println(mfrc522.GetStatusCodeName(piccStatus));
  }
  // picc type is not supported
  else {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return 0;
  }

  Serial.print(nfcStatusMessage[1]);
  Serial.print(nfcStatusMessage[0]);
  printNfcTagType(piccType);
  // read was successfull
  if (nfcTagReadSuccess) {
    // log data to the console
    Serial.print(nfcStatusMessage[3]);
    printNfcTagData(nfcTagReadBuffer, sizeof(nfcTagReadBuffer), true);

    // convert 4 byte magic cookie to 32bit decimal for easier handling
    uint32_t tempMagicCookie;
    tempMagicCookie  = (uint32_t)nfcTagReadBuffer[0] << 24;
    tempMagicCookie += (uint32_t)nfcTagReadBuffer[1] << 16;
    tempMagicCookie += (uint32_t)nfcTagReadBuffer[2] << 8;
    tempMagicCookie += (uint32_t)nfcTagReadBuffer[3];

    // if cookie is not blank, update ncfTag object with data read from nfc tag
    if (tempMagicCookie != 0) {
      nfcTag.magicCookie = tempMagicCookie;
      nfcTag.version = nfcTagReadBuffer[4];
      nfcTag.assignedFolder = nfcTagReadBuffer[5];
      nfcTag.playbackMode = nfcTagReadBuffer[6];
      nfcTag.assignedTrack = nfcTagReadBuffer[7];
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
    // if magic cookie is blank, clear ncfTag object
    else {
      nfcTag.magicCookie = 0;
      nfcTag.version = 0;
      nfcTag.assignedFolder = 0;
      nfcTag.playbackMode = 0;
      nfcTag.assignedTrack = 0;
    }
    return 1;
  }
  // read was not successfull
  else {
    Serial.println(nfcStatusMessage[4]);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return 0;
  }
}

// writes data to nfc tag
uint8_t writeNfcTagData(uint8_t nfcTagWriteBuffer[], uint8_t nfcTagWriteBufferSize) {
  uint8_t piccWriteBuffer[16];
  bool nfcTagWriteSuccess = false;
  MFRC522::StatusCode piccStatus;
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  // decide which code path to take depending on picc type
  if (piccType == MFRC522::PICC_TYPE_MIFARE_MINI || piccType ==  MFRC522::PICC_TYPE_MIFARE_1K || piccType == MFRC522::PICC_TYPE_MIFARE_4K) {
    uint8_t classicBlock = 4;
    uint8_t classicTrailerBlock = 7;
    MFRC522::MIFARE_Key classicKey;
    for (uint8_t i = 0; i < 6; i++) classicKey.keyByte[i] = 0xFF;

    // check if we can authenticate with classicKey
    piccStatus = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, classicTrailerBlock, &classicKey, &mfrc522.uid);
    if (piccStatus == MFRC522::STATUS_OK) {
      // write 16 bytes to nfc tag (by default sector 1 / block 4)
      memset(piccWriteBuffer, 0, sizeof(piccWriteBuffer));
      memcpy(piccWriteBuffer, nfcTagWriteBuffer, sizeof(piccWriteBuffer));
      piccStatus = (MFRC522::StatusCode)mfrc522.MIFARE_Write(classicBlock, piccWriteBuffer, sizeof(piccWriteBuffer));
      if (piccStatus == MFRC522::STATUS_OK) nfcTagWriteSuccess = true;
      else Serial.println(mfrc522.GetStatusCodeName(piccStatus));
    }
    else Serial.println(mfrc522.GetStatusCodeName(piccStatus));
  }
  else if (piccType == MFRC522::PICC_TYPE_MIFARE_UL) {
    uint8_t ultralightStartPage = 8;
    uint8_t ultralightACK[] = {0, 0};
    MFRC522::MIFARE_Key ultralightKey;
    for (uint8_t i = 0; i < 4; i++) ultralightKey.keyByte[i] = 0xFF;

    // check if we can authenticate with ultralightKey
    piccStatus = (MFRC522::StatusCode)mfrc522.PCD_NTAG216_AUTH(ultralightKey.keyByte, ultralightACK);
    if (piccStatus == MFRC522::STATUS_OK) {
      // write 16 bytes to nfc tag (by default pages 8-11)
      for (uint8_t ultralightPage = ultralightStartPage; ultralightPage < ultralightStartPage + 4; ultralightPage++) {
        memset(piccWriteBuffer, 0, sizeof(piccWriteBuffer));
        memcpy(piccWriteBuffer, nfcTagWriteBuffer + ((ultralightPage * 4) - (ultralightStartPage * 4)), 4);
        piccStatus = (MFRC522::StatusCode)mfrc522.MIFARE_Write(ultralightPage, piccWriteBuffer, sizeof(piccWriteBuffer));
        if (piccStatus == MFRC522::STATUS_OK) nfcTagWriteSuccess = true;
        else {
          nfcTagWriteSuccess = false;
          Serial.println(mfrc522.GetStatusCodeName(piccStatus));
          break;
        }
      }
    }
    else Serial.println(mfrc522.GetStatusCodeName(piccStatus));
  }
  // picc type is not supported
  else {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return 0;
  }

  Serial.print(nfcStatusMessage[2]);
  Serial.print(nfcStatusMessage[0]);
  printNfcTagType(piccType);
  // write was successfull
  if (nfcTagWriteSuccess) {
    // log data to the console
    Serial.print(nfcStatusMessage[3]);
    printNfcTagData(nfcTagWriteBuffer, nfcTagWriteBufferSize, true);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return 1;
  }
  // write was not successfull
  else {
    Serial.println(nfcStatusMessage[4]);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return 0;
  }
}

// prints nfc tag data
void printNfcTagData(uint8_t dataBuffer[], uint8_t dataBufferSize, bool cr) {
  for (uint8_t i = 0; i < dataBufferSize; i++) {
    Serial.print(dataBuffer[i] < 0x10 ? " 0" : " ");
    Serial.print(dataBuffer[i], HEX);
  }
  if (cr) Serial.println();
}

// prints nfc tag type
void printNfcTagType(MFRC522::PICC_Type nfcTagType) {
  switch (nfcTagType) {
    case MFRC522::PICC_TYPE_MIFARE_MINI:
    case MFRC522::PICC_TYPE_MIFARE_1K:
    case MFRC522::PICC_TYPE_MIFARE_4K:
      Serial.print(F("cl"));
      break;
    case MFRC522::PICC_TYPE_MIFARE_UL:
      Serial.print(F("ul|nt"));
      break;
    default:
      Serial.print(F("??"));
      break;
  }
  Serial.print(nfcStatusMessage[0]);
}

// starts, stops and checks the shutdown timer
void shutdownTimer(uint8_t timerAction) {
  static uint64_t shutdownMillis = 0;

  switch (timerAction) {
    case START:
      if (preference.shutdownMinutes != 0) shutdownMillis = millis() + (preference.shutdownMinutes * 60000);
      else shutdownMillis = 0;
      break;
    case STOP:
      shutdownMillis = 0;
      break;
    case CHECK:
      if (shutdownMillis != 0 && millis() > shutdownMillis) {
        Serial.println(F("idle shut"));
        shutdownTimer(SHUTDOWN);
      }
      break;
    case SHUTDOWN:
#ifdef STATUSLED
      digitalWrite(statusLedPin, LOW);
#endif
      digitalWrite(shutdownPin, LOW);
      mfrc522.PCD_AntennaOff();
      mfrc522.PCD_SoftPowerDown();
      mp3.sleep();
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      cli();
      sleep_mode();
      break;
    default:
      break;
  }
}

// reads, writes and resets preferences in eeprom
void preferences(uint8_t preferenceAction) {
  Serial.print(F("prefs "));
  switch (preferenceAction) {
    case READ:
      Serial.println(F("read"));
      EEPROM.get(100, preference);
      if (preference.magicCookie != magicCookie) {
        Serial.println(F("  null"));
        preferences(RESET);
      }
      break;
    case WRITE:
      Serial.println(F("write"));
      EEPROM.put(100, preference);
      break;
    case RESET:
      Serial.println(F("reset"));
      preference.magicCookie = magicCookie;
      preference.version = 1;
      preference.mp3StartVolume = mp3StartVolumeDefault;
      preference.mp3MaxVolume = mp3MaxVolumeDefault;
      preference.mp3MenuVolume = mp3MenuVolumeDefault;
      preference.mp3Equalizer = mp3EqualizerDefault;
      preference.shutdownMinutes = shutdownMinutesDefault;
      memcpy(preference.irRemoteUserCodes, irRemoteUserCodesDefault, 14);
      preferences(WRITE);
      break;
    case RESET_PROGRESS:
      Serial.println(F("reset progress"));
      for (uint16_t i = 1; i < 100; i++) {
        EEPROM.update(i, 0);
#ifdef STATUSLED
        statusLedBlink(50);
#endif
      }
      break;
    default:
      break;
  }
}

// interactively prompts the user for options
uint8_t prompt(uint8_t promptOptions, uint16_t promptHeading, uint16_t promptOffset, uint8_t promptCurrent, uint8_t promptFolder, bool promptPreview) {
  uint8_t promptResult = promptCurrent;

  mp3.playMp3FolderTrack(promptHeading);
  while (true) {
    playback.isPlaying = !digitalRead(mp3BusyPin);
    checkForInput();
    // serial console input
    if (Serial.available() > 0) {
      uint32_t promptResultSerial = Serial.parseInt();
      if (promptResultSerial != 0 && promptResultSerial <= promptOptions) {
        Serial.println(promptResultSerial);
        return (uint8_t)promptResultSerial;
      }
    }
    // button 0 (middle) press or ir remote play+pause: confirm selection
    if ((inputEvent == B0P || inputEvent == IRP) && promptResult != 0) {
      if (promptPreview && !playback.isPlaying) {
        if (promptFolder == 0) mp3.playFolderTrack(promptResult, 1);
        else mp3.playFolderTrack(promptFolder, promptResult);
      }
      else return promptResult;
    }
    // button 0 (middle) double click or ir remote center: announce current folder, track number or option
    else if ((inputEvent == B0D || inputEvent == IRC) && promptResult != 0) {
      if (promptPreview && playback.isPlaying) mp3.playAdvertisement(promptResult);
      else if (promptPreview && !playback.isPlaying) {
        if (promptFolder == 0) mp3.playFolderTrack(promptResult, 1);
        else mp3.playFolderTrack(promptFolder, promptResult);
      }
      else mp3.playMp3FolderTrack(promptResult + promptOffset);
    }
    // button 1 (right) press or ir remote up: next folder, track number or option
    else if (inputEvent == B1P || inputEvent == IRU) {
      promptResult = min(promptResult + 1, promptOptions);
      Serial.println(promptResult);
      if (promptPreview) {
        if (promptFolder == 0) mp3.playFolderTrack(promptResult, 1);
        else mp3.playFolderTrack(promptFolder, promptResult);
      }
      else mp3.playMp3FolderTrack(promptResult + promptOffset);
    }
    // button 2 (left) press or ir remote up: previous folder, track number or option
    else if (inputEvent == B2P || inputEvent == IRD) {
      promptResult = max(promptResult - 1, 1);
      Serial.println(promptResult);
      if (promptPreview) {
        if (promptFolder == 0) mp3.playFolderTrack(promptResult, 1);
        else mp3.playFolderTrack(promptFolder, promptResult);
      }
      else mp3.playMp3FolderTrack(promptResult + promptOffset);
    }
    // button 0 (middle) hold for 2 sec or ir remote menu: cancel
    else if (inputEvent == B0H || inputEvent == IRM) {
      Serial.println(F("cancel"));
      return 0;
    }
    // button 1 (right) hold or ir remote right: jump 10 folders, tracks or options forward
    else if (inputEvent == B1H || inputEvent == IRR) {
      promptResult = min(promptResult + 10, promptOptions);
      Serial.println(promptResult);
      mp3.playMp3FolderTrack(promptResult + promptOffset);
    }
    // button 2 (left) hold or ir remote left: jump 10 folders, tracks or options backwards
    else if (inputEvent == B2H || inputEvent == IRL) {
      promptResult = max(promptResult - 10, 1);
      Serial.println(promptResult);
      mp3.playMp3FolderTrack(promptResult + promptOffset);
    }
#ifdef STATUSLED
    statusLedBlink(500);
#endif
    mp3.loop();
  }
}

// parents menu, offers various settings only parents do
void parentsMenu() {
  playback.playListMode = false;

  // set volume to menu volume
  mp3.setVolume(preference.mp3MenuVolume);

  switchButtonConfiguration(CONFIG);
  shutdownTimer(STOP);

  while (true) {
    Serial.println(F("parents"));
    uint8_t selectedOption = prompt(10, 900, 909, 0, 0, false);
    // cancel
    if (selectedOption == 0) {
      mp3.playMp3FolderTrack(904);
      waitPlaybackToFinish(500);
      break;
    }
    // erase tag
    else if (selectedOption == 1) {
      Serial.println(F("erase tag"));
      mp3.playMp3FolderTrack(920);
      // loop until tag is erased
      uint8_t writeNfcTagStatus = 0;
      while (!writeNfcTagStatus) {
        checkForInput();
        // button 0 (middle) hold for 2 sec or ir remote menu: cancel erase nfc tag
        if (inputEvent == B0H || inputEvent == IRM) {
          Serial.println(F("cancel"));
          mp3.playMp3FolderTrack(923);
          waitPlaybackToFinish(500);
          break;
        }
        // wait for nfc tag, erase once detected
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
          uint8_t bytesToWrite[16];
          for (uint8_t i = 0; i < 16; i++) bytesToWrite[i] = 0x00;
          writeNfcTagStatus = writeNfcTagData(bytesToWrite, sizeof(bytesToWrite));
          if (writeNfcTagStatus == 1) {
            mp3.playMp3FolderTrack(921);
            waitPlaybackToFinish(500);
          }
          else mp3.playMp3FolderTrack(922);
        }
#ifdef STATUSLED
        statusLedBlink(500);
#endif
        mp3.loop();
      }
    }
    // startup volume
    else if (selectedOption == 2) {
      Serial.println(F("start vol"));
      uint8_t promptResult = prompt(30, 930, 0, preference.mp3StartVolume, 0, false);
      if (promptResult != 0) {
        // startup volume can't be higher than maximum volume
        preference.mp3StartVolume = min(promptResult, preference.mp3MaxVolume);
        preferences(WRITE);
        mp3.playMp3FolderTrack(901);
        waitPlaybackToFinish(500);
      }
    }
    // maximum volume
    else if (selectedOption == 3) {
      Serial.println(F("max vol"));
      uint8_t promptResult = prompt(30, 931, 0, preference.mp3MaxVolume, 0, false);
      if (promptResult != 0) {
        preference.mp3MaxVolume = promptResult;
        // startup volume can't be higher than maximum volume
        preference.mp3StartVolume = min(preference.mp3StartVolume, preference.mp3MaxVolume);
        preferences(WRITE);
        mp3.playMp3FolderTrack(901);
        waitPlaybackToFinish(500);
      }
    }
    // parents volume
    else if (selectedOption == 4) {
      Serial.println(F("menu vol"));
      uint8_t promptResult = prompt(30, 932, 0, preference.mp3MenuVolume, 0, false);
      if (promptResult != 0) {
        preference.mp3MenuVolume = promptResult;
        preferences(WRITE);
        mp3.playMp3FolderTrack(901);
        waitPlaybackToFinish(500);
      }
    }
    // equalizer
    else if (selectedOption == 5) {
      Serial.println(F("eq"));
      uint8_t promptResult = prompt(6, 940, 940, preference.mp3Equalizer, 0, false);
      if (promptResult != 0) {
        preference.mp3Equalizer = promptResult;
        mp3.setEq((DfMp3_Eq)(preference.mp3Equalizer - 1));
        preferences(WRITE);
        mp3.playMp3FolderTrack(901);
        waitPlaybackToFinish(500);
      }
    }
    // learn ir remote
    else if (selectedOption == 6) {
#ifdef IRREMOTE
      Serial.println(F("learn remote"));
      for (uint8_t i = 0; i < 7; i++) {
        mp3.playMp3FolderTrack(951 + i);
        waitPlaybackToFinish(500);
        // clear ir receive buffer
        irReceiver.resume();
        // wait for ir signal
        while (!irReceiver.decode(&irReading)) {
#ifdef STATUSLED
          statusLedBlink(500);
#endif
        }
        // on NEC encoding 0xFFFFFFFF means the button is held down, we ignore this
        if (!(irReading.decode_type == NEC && irReading.value == 0xFFFFFFFF)) {
          // convert irReading.value from 32bit to 16bit
          uint16_t irRemoteCode = (irReading.value & 0xFFFF);
          Serial.print(F("ir code: 0x"));
          Serial.print(irRemoteCode <= 0x0010 ? "0" : "");
          Serial.print(irRemoteCode <= 0x0100 ? "0" : "");
          Serial.print(irRemoteCode <= 0x1000 ? "0" : "");
          Serial.println(irRemoteCode, HEX);
          preference.irRemoteUserCodes[i] = irRemoteCode;
        }
        // key was held down on NEC encoding, repeat last question
        else i--;
        mp3.loop();
      }
      preferences(WRITE);
      mp3.playMp3FolderTrack(901);
      waitPlaybackToFinish(500);
#else
      mp3.playMp3FolderTrack(950);
      waitPlaybackToFinish(500);
#endif
    }
    // shutdown timer
    else if (selectedOption == 7) {
      Serial.println(F("timer"));
      uint8_t promptResult = prompt(7, 960, 960, 0, 0, false);
      if (promptResult != 0) {
        switch (promptResult) {
          case 1: preference.shutdownMinutes = 5; break;
          case 2: preference.shutdownMinutes = 10; break;
          case 3: preference.shutdownMinutes = 15; break;
          case 4: preference.shutdownMinutes = 20; break;
          case 5: preference.shutdownMinutes = 30; break;
          case 6: preference.shutdownMinutes = 60; break;
          case 7: preference.shutdownMinutes = 0; break;
        }
        preferences(WRITE);
        mp3.playMp3FolderTrack(901);
        waitPlaybackToFinish(500);
      }
    }
    // reset progress
    else if (selectedOption == 8) {
      preferences(RESET_PROGRESS);
      mp3.playMp3FolderTrack(902);
      waitPlaybackToFinish(50);
    }
    // reset preferences
    else if (selectedOption == 9) {
      preferences(RESET);
      mp3.setVolume(preference.mp3MenuVolume);
      mp3.setEq((DfMp3_Eq)(preference.mp3Equalizer - 1));
      mp3.playMp3FolderTrack(903);
      waitPlaybackToFinish(500);
    }
    // manual box shutdown
    else if (selectedOption == 10) {
      Serial.println(F("manual shut"));
      shutdownTimer(SHUTDOWN);
    }
#ifdef STATUSLED
    statusLedBlink(500);
#endif
    mp3.loop();
  }

  // restore playback volume, can't be higher than maximum volume
  mp3.setVolume(playback.mp3CurrentVolume = min(playback.mp3CurrentVolume, preference.mp3MaxVolume));

  switchButtonConfiguration(PAUSE);
  shutdownTimer(START);
  inputEvent = NOACTION;
}

#ifdef PINCODE
// requests pin code from user via buttons or ir remote
bool enterPinCode() {
  uint8_t pinCodeEntered[pinCodeLength];
  uint8_t pinCodeSlot = 0;
  uint64_t cancelEnterPinCodeMillis = millis() + enterPinCodeTimeout;
  bool pinCodeButtonsMatch = true;
  bool pinCodeIRMatch = true;
  playback.playListMode = false;

  // set volume to menu volume
  mp3.setVolume(preference.mp3MenuVolume);

  switchButtonConfiguration(PIN);
  shutdownTimer(STOP);

  Serial.println(F("pin?"));
  mp3.playMp3FolderTrack(808);
  while (true) {
    checkForInput();
    // record button presses, button 0 (middle) hold for 2 sec or ir remote menu: cancel
    if (inputEvent == B0H || inputEvent == IRM || millis() > cancelEnterPinCodeMillis) {
      Serial.println(F("cancel"));

      // restore playback volume, can't be higher than maximum volume
      mp3.setVolume(playback.mp3CurrentVolume = min(playback.mp3CurrentVolume, preference.mp3MaxVolume));

      switchButtonConfiguration(PAUSE);
      shutdownTimer(START);
      inputEvent = NOACTION;

      return false;
    }
    else if (inputEvent != NOACTION) pinCodeEntered[pinCodeSlot++] = inputEvent;
    // if the complete pin code has been recorded, compare with stored button and ir pin code
    if (pinCodeSlot == pinCodeLength) {
      for (uint8_t i = 0; i < pinCodeLength; i++) {
        if (pinCodeButtons[i] != pinCodeEntered[i]) pinCodeButtonsMatch = false;
        if (pinCodeIR[i] != pinCodeEntered[i]) pinCodeIRMatch = false;
      }
      // we have a match, exit
      if (pinCodeButtonsMatch || pinCodeIRMatch) {
        // restore playback volume, can't be higher than maximum volume
        mp3.setVolume(playback.mp3CurrentVolume = min(playback.mp3CurrentVolume, preference.mp3MaxVolume));

        switchButtonConfiguration(PAUSE);
        shutdownTimer(START);
        inputEvent = NOACTION;

        return true;
      }
      // we don't have a match, repeat
      else {
        Serial.println(F("pin?"));
        cancelEnterPinCodeMillis = millis() + enterPinCodeTimeout;
        pinCodeSlot = 0;
        pinCodeButtonsMatch = true;
        pinCodeIRMatch = true;
#ifdef STATUSLED
        statusLedBurst();
#endif
      }
    }
#ifdef STATUSLED
    statusLedBlink(500);
#endif
    mp3.loop();
  }
}
#endif

#ifdef STATUSLED
// fade in/out status led while beeing idle, set to full brightness during playback
void statusLedFade() {
  static bool statusLedDirection = false;
  static int16_t statusLedValue = 255;
  static uint64_t statusLedOldMillis;

  // TonUINO is playing, set status led to full brightness
  if (playback.isPlaying) {
    statusLedValue = 255;
    digitalWrite(statusLedPin, true);
  }
  // TonUINO is not playing, fade status led in/out
  else {
    if (millis() - statusLedOldMillis >= 100) {
      statusLedOldMillis = millis();
      if (statusLedDirection) {
        statusLedValue += 10;
        if (statusLedValue >= 255) {
          statusLedValue = 255;
          statusLedDirection = !statusLedDirection;
        }
      }
      else {
        statusLedValue -= 10;
        if (statusLedValue <= 0) {
          statusLedValue = 0;
          statusLedDirection = !statusLedDirection;
        }
      }
      analogWrite(statusLedPin, statusLedValue);
    }
  }
}

// blink status led every blinkInterval milliseconds
void statusLedBlink(uint16_t statusLedBlinkInterval) {
  static bool statusLedState = true;
  static uint64_t statusLedOldMillis;

  if (millis() - statusLedOldMillis >= statusLedBlinkInterval) {
    statusLedOldMillis = millis();
    statusLedState = !statusLedState;
    digitalWrite(statusLedPin, statusLedState);
  }
}

// burst status led 4 times
void statusLedBurst() {
  static bool statusLedState = true;

  for (uint8_t i = 0; i < 8; i++) {
    statusLedState = !statusLedState;
    digitalWrite(statusLedPin, statusLedState);
    delay(100);
  }
}
#endif
