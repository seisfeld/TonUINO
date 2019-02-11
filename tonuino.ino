/*-----------------------------------------------------------------------------------------
              MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
              Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
  Signal      Pin          Pin           Pin       Pin        Pin              Pin
  -----------------------------------------------------------------------------------------
  RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
  SPI SS      SDA(SS)      10            53        D10        10               10
  SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
  SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
  SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

  button assignments:
  ===================

  Button B0 (by default pin A0, middle button on the original TonUINO): play+pause
  Button B1 (by default pin A1, right button on the original TonUINO): volume up
  Button B2 (by default pin A2, left button on the original TonUINO): volume down

  additional button actions:
  ==========================

  Hold B0 for 5 seconds during idle: Enter erase nfc tag mode
  Hold B0 for 5 seconds during playback in story book mode: Reset progress to track 1
  Hold B0 for 2 seconds during erase nfc tag mode: Cancel erase nfc tag mode
  Hold B0 for 2 seconds during nfc tag setup mode: Cancel nfc tag setup mode
  Click B0 during nfc tag setup mode: Confirm selection
  Double click B0 during nfc tag setup mode: Announce folder number or track number

  Hold B1 for 2 seconds during playback in album, party and story book mode: Skip to the next track
  Hold B1 for 2 seconds during nfc tag setup mode: Jump 10 folders or 10 tracks forward

  Hold B2 for 2 seconds during playback in album, party and story book mode: Skip to the previous track
  Hold B2 for 2 seconds during nfc tag setup mode: Jump 10 folders or 10 tracks backwards

  Hold B0 + B1 + B2 while powering up: Erase the eeprom contents. (Use with care, eeprom write/erase cycles are limited!)

  ir remote:
  ==========

  If a TSOP38238 is connected to pin 5, you can also use an ir remote to remote
  control TonUINO. There are code mappings for the silver apple remote below, but you can
  change them to other codes to match different remotes. This feature can be enabled by
  uncommenting the define TSOP38238 below.

  There is one function, currently only available with the ir remote: Box lock.
  When TonUINO is locked, the buttons on TonUINO as well as the nfc reader are disabled
  until TonUINO is unlocked again. Playback continues while TonUINO is locked.

  During playback:
  center - toggle box lock
  play+pause - toggle playback
  up / down - volume up / down
  left / right - previous / next track in album, party and story book mode
  menu - reset progress to track 1 in story book mode

  During idle:
  center - toggle box lock
  menu - enter erase nfc tag mode

  During erase nfc tag mode:
  menu - cancel

  During nfc tag setup mode:
  play+pause - confirm
  right - next option
  left - previous option
  up - jump 10 folders or 10 tracks forward
  down - jump 10 folders or 10 tracks backwards
  center - announce folder number or track number
  menu - cancel

  status led:
  ===========

  If a led is connected to pin 6, limited status information is given using that led.
  The led is solid on when TonUINO is playing a track and it is pulsing slowly when
  TonUINO is idle. When TonUINO is in setup new nfc tag or erase nfc tag mode, the
  led is blinking every 500ms. And last but not not least, the led bursts 4 times when
  TonUINO is locked or unlocked. This feature can be enabled by uncommenting the
  define STATUSLED below.

  cubiekid:
  =========

  If you happen to have a CubieKid case and the CubieKid circuit board, this firmware
  supports both shutdown methods. They can be configured in the shutdown section below.
  The inactivity shutdown timer is enabled by default, the shutdown due to low battery
  voltage can be enabled by uncommenting the define LOWVOLTAGE below.

  The CubieKid case as well as the CubieKid circuit board, have been designed and developed
  by Jens Hackel aka DB3JHF and can be found here: https://www.thingiverse.com/thing:3148200

  data stored on the nfc tags:
  ============================

  On MIFARE Classic (Mini, 1k & 4K) tags:
  ---------------------------------------

  Up to 16 bytes of data are stored in sector 1 / block 4, of which the first 8 bytes
  are currently in use.

  13 37 B3 47 01 02 04 11 00 00 00 00 00 00 00 00
  ----------- -- -- -- --
       |      |  |  |  |
       |      |  |  |  + assigned track (0x01-0xFF, only used in single mode)
       |      |  |  + assigned playback mode (0x01-0x05)
       |      |  + assigned folder (0x01-0x63)
       |      + version (currently always 0x01)
       + magic cookie to recognize that a card belongs to TonUINO

  On MIFARE Ultralight / Ultralight C and NTAG213/215/216 tags:
  -------------------------------------------------------------

  Up to 16 bytes of data are stored in pages 8-11, of which the first 8 bytes
  are currently in use.

   8   13 37 B3 47 - magic cookie to recognize that a card belongs to TonUINO
   9   01 02 04 11 - version, assigned folder, assigned playback mode, assigned track
  10   00 00 00 00
  11   00 00 00 00

  non standard libraries used in this firmware:
  =============================================

  MFRC522.h - https://github.com/miguelbalboa/rfid
  DFMiniMp3.h - https://github.com/Makuna/DFMiniMp3
  AceButton.h - https://github.com/bxparks/AceButton
  IRremote.h - https://github.com/z3t0/Arduino-IRremote
  Vcc.h - https://github.com/Yveaux/Arduino_Vcc
*/

// uncomment the below line to enable ir remote support
// #define TSOP38238

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
#if defined(TSOP38238)
#include <IRremote.h>
#endif

// include additional library if low voltage shutdown support is enabled
#if defined(LOWVOLTAGE)
#include <Vcc.h>
#endif

// define general configuration constants
const uint8_t softwareSerialTxPin = 3;              // software serial tx, wired with 1k ohm to rx pin of DFPlayer Mini
const uint8_t softwareSerialRxPin = 2;              // software serial rx, wired straight to tx pin of DFPlayer Mini
const uint8_t mp3BusyPin = 4;                       // reports play state of DFPlayer Mini (LOW = playing)
const uint8_t irReceiverPin = 5;                    // pin used for the ir receiver
const uint8_t statusLedPin = 6;                     // pin used for status led
const uint8_t nfcResetPin = 9;                      // used for spi communication to nfc module
const uint8_t nfcSlaveSelectPin = 10;               // used for spi communication to nfc module
const uint8_t rngSeedPin = A0;                      // used to seed the random number generator
const uint8_t mp3StartVolume = 10;                  // initial volume of DFPlayer Mini
const uint8_t mp3MaxVolume = 25;                    // maximal volume of DFPlayer Mini
const uint8_t button0Pin = A0;                      // middle button
const uint8_t button1Pin = A1;                      // right button
const uint8_t button2Pin = A2;                      // left button
const uint16_t buttonClickDelay = 1000;             // time during which a button click is still a click (in milliseconds)
const uint16_t buttonShortLongPressDelay = 2000;    // time after which a button press is considered a long press (in milliseconds)
const uint16_t buttonLongLongPressDelay = 5000;     // longer long press delay for special cases, i.e. to trigger erase nfc tag mode (in milliseconds)
const uint32_t debugConsoleSpeed = 115200;          // speed for the debug console

// define constants for shutdown feature
const uint8_t shutdownPin = 7;                      // pin used to shutdown the system
const uint8_t shutdownMinutes = 10;                 // minutes until shutdown (to disable, set to 0)
const float shutdownMinVoltage = 4.4;               // minimum expected voltage level (in volts)
const float shutdownMaxVoltage = 5.0;               // maximum expected voltage level (in volts)
const float shutdownVoltageCorrection = 1.0 / 1.0;  // voltage measured by multimeter divided by reported voltage

// define message to mp3 file mappings for spoken feedback
const uint16_t msgSetupNewTag = 305;                // 01
const uint16_t msgSetupNewTagFolderAssigned = 306;  // 02
const uint16_t msgSetupNewTagStoryMode = 310;       // 03
const uint16_t msgSetupNewTagAlbumMode = 320;       // 04
const uint16_t msgSetupNewTagPartyMode = 330;       // 05
const uint16_t msgSetupNewTagSingleMode = 340;      // 06
const uint16_t msgSetupNewTagSingleModeCont = 341;  // 07
const uint16_t msgSetupNewTagStoryBookMode = 350;   // 08
const uint16_t msgSetupNewTagConfirm = 390;         // 09
const uint16_t msgSetupNewTagCancel = 391;          // 10
const uint16_t msgSetupNewTagError = 392;           // 11
const uint16_t msgEraseTag = 405;                   // 12
const uint16_t msgEraseTagConfirm = 490;            // 13
const uint16_t msgEraseTagCancel = 491;             // 14
const uint16_t msgEraseTagError = 492;              // 15
const uint16_t msgWelcome = 505;                    // 16
const uint16_t msgBatteryLow = 510;                 // 17

// used to calculate the total ammount of tracks on the sd card (510 + count from above)
const uint16_t msgCount = 527;

// define codes for ir remotes
// one remote per line with codes for: up, down, left, right, center, menu, play/pause
const uint16_t irRemoteCodes[][7] = {
  {0x5057, 0x3057, 0x9057, 0x6057, 0x3A57, 0xC057, 0xFA57},  // silver apple tv remote
  {0x50BF, 0x30BF, 0x90BF, 0x60BF, 0x3ABF, 0xC0BF, 0xFABF},  // silver apple tv remote
  {0xD0E4, 0xB0E4, 0x10E4, 0xE0E4, 0xBAE4, 0x40E4, 0x7AE4}   // silver apple tv remote
};
const uint8_t irRemoteCount = sizeof(irRemoteCodes) / 14;
const uint8_t irRemoteCodeCount = sizeof(irRemoteCodes) / (2 * irRemoteCount);

// define strings
const char* playbackModeName[] = {" ", "story", "album", "party", "single", "story_book"};
const char* nfcStatusMessage[] = {" ", "read", "write", "ok", "failed"};

// playback modes
enum {NOMODE, STORY, ALBUM, PARTY, SINGLE, STORYBOOK};

// button actions
enum {NOACTION,
      B0P, B1P, B2P,
      B0H, B1H, B2H,
      B0D, B1D, B2D,
      IRU, IRD, IRL, IRR, IRC, IRM, IRP
     };

// button modes
enum {PAUSE, PLAY, CONFIG};

// shutdown timer actions
enum {START, STOP, CHECK};

// this object stores nfc tag data
struct nfcTagObject {
  uint32_t cookie;
  uint8_t version;
  uint8_t assignedFolder;
  uint8_t playbackMode;
  uint8_t assignedTrack;
} nfcTag;

// this object tracks the playback state
struct playbackObject {
  bool firstTrack = true;
  bool playListMode = false;
  uint8_t playTrack = 1;
  uint8_t storedTrack = 1;
  uint8_t folderTrackCount = 0;
  uint8_t playList[255];
} playback;

// global variables
uint8_t inputEvent = NOACTION;
uint8_t mp3CurrentVolume = mp3StartVolume;
bool isLocked = false;

// ############################################################### no configuration below this line ###############################################################

// declare functions
void checkForInput();
void translateButtonInput(AceButton* button, uint8_t eventType, uint8_t /* buttonState */);
void switchButtonConfiguration(uint8_t buttonMode);
void waitPlaybackToFinish();
void printModeFolderTrack(uint8_t playTrack, bool cr);
void playNextTrack(uint16_t globalTrack, bool directionForward, bool triggeredManually);
uint8_t readNfcTagData();
uint8_t writeNfcTagData(uint8_t *nfcTagWriteBuffer, uint8_t nfcTagWriteBufferSize);
void printNfcTagData(uint8_t *dataBuffer, uint8_t dataBufferSize, bool cr);
void printNfcTagType(MFRC522::PICC_Type nfcTagType);
void shutdownTimer(uint8_t timerAction);
#if defined(STATUSLED)
void statusLedFade(bool isPlaying);
void statusLedBlink(uint16_t statusLedBlinkInterval = 500);
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
      Serial.print(F(" error: "));
      Serial.println(returnValue);
    }
    static void OnPlayFinished(uint16_t returnValue) {
      playNextTrack(returnValue, true, false);
    }
    static void OnCardOnline(uint16_t returnValue) {
      Serial.println(F("sd online"));
    }
    static void OnCardInserted(uint16_t returnValue) {
      Serial.println(F("sd in"));
    }
    static void OnCardRemoved(uint16_t returnValue) {
      Serial.println(F("sd out"));
    }
};

SoftwareSerial secondarySerial(softwareSerialRxPin, softwareSerialTxPin);     // create SoftwareSerial instance
MFRC522 mfrc522(nfcSlaveSelectPin, nfcResetPin);                              // create MFRC522 instance
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);                    // create DFMiniMp3 instance
ButtonConfig button0Config;                                                   // create ButtonConfig instance
ButtonConfig button1Config;                                                   // create ButtonConfig instance
ButtonConfig button2Config;                                                   // create ButtonConfig instance
AceButton button0(&button0Config);                                            // create AceButton instance
AceButton button1(&button1Config);                                            // create AceButton instance
AceButton button2(&button2Config);                                            // create AceButton instance

#if defined(TSOP38238)
IRrecv irReceiver(irReceiverPin);                                             // create IRrecv instance
decode_results irReadings;                                                    // create decode_results instance to store received ir readings
#endif

#if defined(LOWVOLTAGE)
Vcc shutdownVoltage(shutdownVoltageCorrection);                               // create Vcc instance
#endif

// checks all input sources and populates the global inputEvent variable
void checkForInput() {
  // clear inputEvent
  inputEvent = NOACTION;

  // check all buttons
  button0.check();
  button1.check();
  button2.check();

#if defined(TSOP38238)
  uint8_t irRemoteEvent = 0;
  uint16_t irRemoteCode = 0;

  // poll ir receiver, has precedence over (overwrites) physical buttons
  if (irReceiver.decode(&irReadings)) {
    irRemoteCode = irReadings.value & 0xFFFF;
    for (uint8_t i = 0; i < irRemoteCount; i++) {
      for (uint8_t j = 0; j < irRemoteCodeCount; j++) {
        //if we have a match, temporally populate irRemoteEvent and break
        if (irRemoteCode == irRemoteCodes[i][j]) {
          irRemoteEvent = 10 + j;
          break;
        }
      }
      // if the inner loop had a match, populate inputEvent and break
      if (irRemoteEvent != 0) {
        inputEvent = irRemoteEvent;
        break;
      }
    }
    irReceiver.resume();
  }
#endif
}

// translates the various button events into enums
void translateButtonInput(AceButton* button, uint8_t eventType, uint8_t /* buttonState */) {
  switch (button->getId()) {
    // botton 0 (middle)
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
    // botton 1 (right)
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
    // botton 2 (left)
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
    default:
      break;
  }
}

// switches button configuration dependig on the state that TonUINO is in
void switchButtonConfiguration(uint8_t buttonMode) {
  // default configuration for all modes
  // button 0
  button0Config.setEventHandler(translateButtonInput);
  button0Config.setFeature(ButtonConfig::kFeatureClick);
  button0Config.setFeature(ButtonConfig::kFeatureLongPress);
  button0Config.setClickDelay(buttonClickDelay);
  button0Config.setLongPressDelay(buttonShortLongPressDelay);
  // button 1
  button1Config.setEventHandler(translateButtonInput);
  button1Config.setFeature(ButtonConfig::kFeatureClick);
  button1Config.setFeature(ButtonConfig::kFeatureLongPress);
  button1Config.setClickDelay(buttonClickDelay);
  button1Config.setLongPressDelay(buttonShortLongPressDelay);
  // button 2
  button2Config.setEventHandler(translateButtonInput);
  button2Config.setFeature(ButtonConfig::kFeatureClick);
  button2Config.setFeature(ButtonConfig::kFeatureLongPress);
  button2Config.setClickDelay(buttonClickDelay);
  button2Config.setLongPressDelay(buttonShortLongPressDelay);

  // non default configuration
  switch (buttonMode) {
    case PAUSE:
      // button 0
      button0Config.clearFeature(ButtonConfig::kFeatureDoubleClick);
      button0Config.clearFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
      button0Config.setLongPressDelay(buttonLongLongPressDelay);
      break;
    case PLAY:
      // button 0
      button0Config.clearFeature(ButtonConfig::kFeatureDoubleClick);
      button0Config.clearFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
      button0Config.setLongPressDelay(buttonLongLongPressDelay);
      break;
    case CONFIG:
      // button 0
      button0Config.setFeature(ButtonConfig::kFeatureDoubleClick);
      button0Config.setFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
      button0Config.setLongPressDelay(buttonShortLongPressDelay);
      break;
    default:
      break;
  }
}

// waits for current playing track to finish
void waitPlaybackToFinish() {
  uint64_t waitPlaybackToStartMillis = millis();
  delay(100);
  while (digitalRead(mp3BusyPin)) if (millis() - waitPlaybackToStartMillis >= 10000) break;
  while (!digitalRead(mp3BusyPin));
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
    // check if we get called with the same track number twice in a row, if yes return immediately
    if (lastCallTrack == globalTrack) return;
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
    MFRC522::MIFARE_Key classicKey;
    for (uint8_t i = 0; i < 6; i++) classicKey.keyByte[i] = 0xFF;

    // check if we can authenticate with classicKey
    piccStatus = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, classicBlock, &classicKey, &(mfrc522.uid));
    if (piccStatus == MFRC522::STATUS_OK) {
      // read 16 bytes from nfc tag (by default sector 1 / block 4)
      piccStatus = (MFRC522::StatusCode)mfrc522.MIFARE_Read(classicBlock, piccReadBuffer, &piccReadBufferSize);
      if (piccStatus == MFRC522::STATUS_OK) {
        nfcTagReadSuccess = true;
        memcpy(nfcTagReadBuffer, piccReadBuffer, 16);
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

    // convert 4 byte cookie to 32bit decimal for easier handling
    uint32_t tempCookie;
    tempCookie  = (uint32_t)nfcTagReadBuffer[0] << 24;
    tempCookie += (uint32_t)nfcTagReadBuffer[1] << 16;
    tempCookie += (uint32_t)nfcTagReadBuffer[2] << 8;
    tempCookie += (uint32_t)nfcTagReadBuffer[3];

    // if cookie is not blank, update ncfTag object with data read from nfc tag
    if (tempCookie != 0) {
      nfcTag.cookie = tempCookie;
      nfcTag.version = nfcTagReadBuffer[4];
      nfcTag.assignedFolder = nfcTagReadBuffer[5];
      nfcTag.playbackMode = nfcTagReadBuffer[6];
      nfcTag.assignedTrack = nfcTagReadBuffer[7];
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
    // if cookie is blank, clear ncfTag object
    else {
      nfcTag = (struct nfcTagObject) {
        0
      };
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
uint8_t writeNfcTagData(uint8_t *nfcTagWriteBuffer, uint8_t nfcTagWriteBufferSize) {
  uint8_t piccWriteBuffer[16];
  uint8_t piccWriteBufferSize = sizeof(piccWriteBuffer);
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
    piccStatus = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, classicTrailerBlock, &classicKey, &(mfrc522.uid));
    if (piccStatus == MFRC522::STATUS_OK) {
      // write 16 bytes to nfc tag (by default sector 1 / block 4)
      memset(piccWriteBuffer, 0, piccWriteBufferSize);
      memcpy(piccWriteBuffer, nfcTagWriteBuffer, 16);
      piccStatus = (MFRC522::StatusCode)mfrc522.MIFARE_Write(classicBlock, piccWriteBuffer, piccWriteBufferSize);
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
        memset(piccWriteBuffer, 0, piccWriteBufferSize);
        memcpy(piccWriteBuffer, nfcTagWriteBuffer + ((ultralightPage * 4) - (ultralightStartPage * 4)), 4);
        piccStatus = (MFRC522::StatusCode)mfrc522.MIFARE_Write(ultralightPage, piccWriteBuffer, piccWriteBufferSize);
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
  //write was not successful
  else {
    Serial.println(nfcStatusMessage[4]);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return 0;
  }
}

// prints nfc tag data
void printNfcTagData(uint8_t *dataBuffer, uint8_t dataBufferSize, bool cr) {
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
      if (shutdownMinutes != 0) shutdownMillis = millis() + (shutdownMinutes * 60000);
      else shutdownMillis = 0;
      break;
    case STOP:
      shutdownMillis = 0;
      break;
    case CHECK:
      if (shutdownMillis != 0 && millis() > shutdownMillis) {
        Serial.println(F("idle shutdown"));
        digitalWrite(statusLedPin, LOW);
        digitalWrite(shutdownPin, LOW);
        mfrc522.PCD_AntennaOff();
        mfrc522.PCD_SoftPowerDown();
        mp3.sleep();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        cli();
        sleep_mode();
      }
      break;
    default:
      break;
  }
}

#if defined(STATUSLED)
// fade in/out status led while beeing idle, during playback set to full brightness
void statusLedFade(bool isPlaying) {
  static bool statusLedDirection = false;
  static int16_t statusLedValue = 255;
  static uint64_t statusLedOldMillis;

  // TonUINO is playing, set status led to full brightness
  if (isPlaying) {
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
void statusLedBlink(uint16_t statusLedBlinkInterval = 500) {
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

void setup() {
  pinMode(shutdownPin, OUTPUT);
  digitalWrite(shutdownPin, HIGH);
  Serial.begin(debugConsoleSpeed);
  while (!Serial);
  Serial.println(F("\n\nTonUINO JUKEBOX"));
  Serial.println(F("by Thorsten Voß"));
  Serial.println(F("Stephan Eisfeld"));
  Serial.println(F("---------------"));
  Serial.println(F("init nfc"));
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  Serial.println(F("init mp3"));
  mp3.begin();
  delay(2000);
  Serial.print(F("  volume "));
  Serial.println(mp3StartVolume);
  mp3.setVolume(mp3StartVolume);
  Serial.print(F("  tracks "));
  Serial.println(mp3.getTotalTrackCount() - msgCount);
  pinMode(mp3BusyPin, INPUT);

  Serial.println(F("init rng"));
  randomSeed(analogRead(rngSeedPin));

  Serial.println(F("init buttons"));
  pinMode(button0Pin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  button0.init(button0Pin, HIGH, 0);
  button1.init(button1Pin, HIGH, 1);
  button2.init(button2Pin, HIGH, 2);
  switchButtonConfiguration(PAUSE);

  Serial.print(F("init timer "));
  Serial.print(shutdownMinutes);
  Serial.println(F("m"));
  shutdownTimer(START);

#if defined(TSOP38238)
  Serial.println(F("init ir"));
  irReceiver.enableIRIn();
#endif

#if defined(STATUSLED)
  Serial.println(F("init led"));
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, HIGH);
#endif

#if defined(LOWVOLTAGE)
  Serial.println(F("init voltage monitor"));
  Serial.print(F("  expected "));
  Serial.print(shutdownMaxVoltage);
  Serial.println(F("V"));
  Serial.print(F("  shutdown "));
  Serial.print(shutdownMinVoltage);
  Serial.println(F("V"));
  Serial.print(F("   current "));
  Serial.print(shutdownVoltage.Read_Volts());
  Serial.print(F("V ("));
  Serial.print(shutdownVoltage.Read_Perc(shutdownMinVoltage, shutdownMaxVoltage));
  Serial.println(F("%)"));
#endif

  // hold down all three buttons while powering up: erase the eeprom contents
  if (digitalRead(button0Pin) == LOW && digitalRead(button1Pin) == LOW && digitalRead(button2Pin) == LOW) {
    Serial.println(F("init eeprom"));
    for (uint16_t i = 0; i < EEPROM.length(); i++) {
      EEPROM.update(i, 0);
#if defined(STATUSLED)
      statusLedBlink(50);
#endif
    }
  }

  Serial.println(F("ready"));
  mp3.playMp3FolderTrack(msgWelcome);
}

void loop() {
  bool isPlaying = !digitalRead(mp3BusyPin);
  checkForInput();
  shutdownTimer(CHECK);

#if defined(STATUSLED)
  statusLedFade(isPlaying);
#endif

#if defined(LOWVOLTAGE)
  // if low voltage level is reached, store progress and shutdown
  if (shutdownVoltage.Read_Volts() <= shutdownMinVoltage) {
    if (nfcTag.playbackMode == STORYBOOK) EEPROM.update(nfcTag.assignedFolder, playback.playTrack);
    mp3.playMp3FolderTrack(msgBatteryLow);
    waitPlaybackToFinish();
    Serial.println(F("low voltage shutdown"));
    digitalWrite(shutdownPin, LOW);
  }
#endif

  // ################################################################################
  // # main code block, if nfc tag is detected and TonUINO is not locked do something
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && !isLocked) {
    // if the current playback mode is story book mode, only while playing: store the current progress
    if (nfcTag.playbackMode == STORYBOOK && isPlaying) {
      Serial.print(F("save "));
      printModeFolderTrack(playback.playTrack, true);
      EEPROM.update(nfcTag.assignedFolder, playback.playTrack);
    }
    uint8_t readNfcTagStatus = readNfcTagData();
    // ##############################
    // # nfc tag is successfully read
    if (readNfcTagStatus == 1) {
      // #######################################################################################################
      // # nfc tag has our magic cookie 0x1337 0xb347 on it (322417479), use data from nfc tag to start playback
      if (nfcTag.cookie == 322417479) {
        switchButtonConfiguration(PLAY);
        shutdownTimer(STOP);
        // prepare playlist for playback
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
      // # end - nfc tag has our magic cookie 0x1337 0xb347 on it (322417479)
      // ####################################################################

      // #######################################################################################################
      // # nfc tag does not have our magic cookie 0x1337 0xb347 on it (0), start setup to configure this nfc tag
      else if (nfcTag.cookie == 0) {
        playback.playListMode = false;
        switchButtonConfiguration(CONFIG);
        shutdownTimer(STOP);
        Serial.println(F(">tag setup"));
        Serial.println(F(">>folder"));
        mp3.playMp3FolderTrack(msgSetupNewTag);
        // loop until folder is assigned
        bool setAssignedFolder = false;
        while (!setAssignedFolder) {
          checkForInput();
          // button 0 (middle) press or ir remote play+pause: confirm selected folder
          if (inputEvent == B0P || inputEvent == IRP) {
            if (nfcTag.assignedFolder == 0) continue;
            else setAssignedFolder = true;
          }
          // button 0 (middle) double click or ir remote center: announce folder number
          else if (inputEvent == B0D || inputEvent == IRC) {
            mp3.playAdvertisement(nfcTag.assignedFolder);
          }
          // button 1 (right) press or ir remote right: next folder
          else if (inputEvent == B1P || inputEvent == IRR) {
            nfcTag.assignedFolder = min(nfcTag.assignedFolder + 1, 99);
            Serial.println(nfcTag.assignedFolder);
            mp3.playFolderTrack(nfcTag.assignedFolder, 1);
          }
          // button 2 (left) press or ir remote left: previous folder
          else if (inputEvent == B2P || inputEvent == IRL) {
            nfcTag.assignedFolder = max(nfcTag.assignedFolder - 1, 1);
            Serial.println(nfcTag.assignedFolder);
            mp3.playFolderTrack(nfcTag.assignedFolder, 1);
          }
          // button 0 (middle) hold for 2 sec or ir remote menu: cancel tag setup
          else if (inputEvent == B0H || inputEvent == IRM) {
            switchButtonConfiguration(PAUSE);
            shutdownTimer(START);
            Serial.println(F("cancel"));
            nfcTag = (struct nfcTagObject) {
              0
            };
            mfrc522.PICC_HaltA();
            mfrc522.PCD_StopCrypto1();
            mp3.playMp3FolderTrack(msgSetupNewTagCancel);
            return;
          }
          // button 1 (right) hold or ir remote up: jump 10 folders forward
          else if (inputEvent == B1H || inputEvent == IRU) {
            nfcTag.assignedFolder = min(nfcTag.assignedFolder + 10, 99);
            Serial.println(nfcTag.assignedFolder);
            mp3.playMp3FolderTrack(nfcTag.assignedFolder);
            waitPlaybackToFinish();
            mp3.playFolderTrack(nfcTag.assignedFolder, 1);
          }
          // button 2 (left) hold or ir remote down: jump 10 folders backwards
          else if (inputEvent == B2H || inputEvent == IRD) {
            nfcTag.assignedFolder = max(nfcTag.assignedFolder - 10, 1);
            Serial.println(nfcTag.assignedFolder);
            mp3.playMp3FolderTrack(nfcTag.assignedFolder);
            waitPlaybackToFinish();
            mp3.playFolderTrack(nfcTag.assignedFolder, 1);
          }
#if defined(STATUSLED)
          statusLedBlink();
#endif
          mp3.loop();
        }
        delay(500);
        Serial.println(F(">>playback mode"));
        mp3.playMp3FolderTrack(msgSetupNewTagFolderAssigned);
        // loop until playback mode is set
        bool setPlaybackMode = false;
        while (!setPlaybackMode) {
          checkForInput();
          // button 0 (middle) press or ir remote play+pause: confirm selected playback mode
          if (inputEvent == B0P || inputEvent == IRP) {
            if (nfcTag.playbackMode == NOMODE) continue;
            else setPlaybackMode = true;
          }
          // button 1 (right) press or ir remote up / right: next playback mode
          else if (inputEvent == B1P || inputEvent == IRU || inputEvent == IRR) {
            nfcTag.playbackMode = min(nfcTag.playbackMode + 1, 5);
            Serial.println(playbackModeName[nfcTag.playbackMode]);
            mp3.playMp3FolderTrack(300 + (10 * nfcTag.playbackMode));
          }
          // button 2 (left) press or ir remote down / left: previous playback mode
          else if (inputEvent == B2P || inputEvent == IRD || inputEvent == IRL) {
            nfcTag.playbackMode = max(nfcTag.playbackMode - 1, 1);
            Serial.println(playbackModeName[nfcTag.playbackMode]);
            mp3.playMp3FolderTrack(300 + (10 * nfcTag.playbackMode));
          }
          // button 0 (middle) hold for 2 sec or ir remote menu: cancel tag setup
          else if (inputEvent == B0H || inputEvent == IRM) {
            switchButtonConfiguration(PAUSE);
            shutdownTimer(START);
            Serial.println(F("cancel"));
            nfcTag = (struct nfcTagObject) {
              0
            };
            mfrc522.PICC_HaltA();
            mfrc522.PCD_StopCrypto1();
            mp3.playMp3FolderTrack(msgSetupNewTagCancel);
            return;
          }
#if defined(STATUSLED)
          statusLedBlink();
#endif
          mp3.loop();
        }
        delay(500);
        // if single mode was selected, let user select the track to assign
        if (nfcTag.playbackMode == SINGLE) {
          Serial.println(F(">>track"));
          mp3.playMp3FolderTrack(msgSetupNewTagSingleModeCont);
          // loop until track is assigned
          bool setAssignedTrack = false;
          while (!setAssignedTrack) {
            checkForInput();
            // button 0 (middle) press or ir remote play+pause: confirm selected track
            if (inputEvent == B0P || inputEvent == IRP) {
              if (nfcTag.assignedTrack == 0) continue;
              else setAssignedTrack = true;
            }
            // button 0 (middle) double click or ir remote center: announce track number
            else if (inputEvent == B0D || inputEvent == IRC) {
              mp3.playAdvertisement(nfcTag.assignedTrack);
            }
            // button 1 (right) press or ir remote right: next track
            else if (inputEvent == B1P || inputEvent == IRR) {
              nfcTag.assignedTrack = min(nfcTag.assignedTrack + 1, 255);
              Serial.println(nfcTag.assignedTrack);
              mp3.playFolderTrack(nfcTag.assignedFolder, nfcTag.assignedTrack);
            }
            // button 2 (left) press or ir remote left: previous track
            else if (inputEvent == B2P || inputEvent == IRL) {
              nfcTag.assignedTrack = max(nfcTag.assignedTrack - 1, 1);
              Serial.println(nfcTag.assignedTrack);
              mp3.playFolderTrack(nfcTag.assignedFolder, nfcTag.assignedTrack);
            }
            // button 0 (middle) hold for 2 sec or ir remote menu: cancel tag setup
            else if (inputEvent == B0H || inputEvent == IRM) {
              switchButtonConfiguration(PAUSE);
              shutdownTimer(START);
              Serial.println(F("cancel"));
              nfcTag = (struct nfcTagObject) {
                0
              };
              mfrc522.PICC_HaltA();
              mfrc522.PCD_StopCrypto1();
              mp3.playMp3FolderTrack(msgSetupNewTagCancel);
              return;
            }
            // button 1 (right) hold for 2 sec or ir remote up: jump 10 tracks forward
            else if (inputEvent == B1H || inputEvent == IRU) {
              nfcTag.assignedTrack = min(nfcTag.assignedTrack + 10, 255);
              Serial.println(nfcTag.assignedTrack);
              mp3.playMp3FolderTrack(nfcTag.assignedTrack);
              waitPlaybackToFinish();
              mp3.playFolderTrack(nfcTag.assignedFolder, nfcTag.assignedTrack);
            }
            // button 2 (left) hold for 2 sec or ir remote down: jump 10 tracks backwards
            else if (inputEvent == B2H || inputEvent == IRD) {
              nfcTag.assignedTrack = max(nfcTag.assignedTrack - 10, 1);
              Serial.println(nfcTag.assignedTrack);
              mp3.playMp3FolderTrack(nfcTag.assignedTrack);
              waitPlaybackToFinish();
              mp3.playFolderTrack(nfcTag.assignedFolder, nfcTag.assignedTrack);
            }
#if defined(STATUSLED)
            statusLedBlink();
#endif
            mp3.loop();
          }
          delay(500);
        }
        switchButtonConfiguration(PAUSE);
        shutdownTimer(START);
        uint8_t bytesToWrite[] = {0x13, 0x37, 0xb3, 0x47,            // 0x1337 0xb347 magic cookie to identify our nfc tags
                                  0x01,                              // version 1
                                  nfcTag.assignedFolder,             // the folder selected by the user
                                  nfcTag.playbackMode,               // the playback mode selected by the user
                                  nfcTag.assignedTrack,              // the track selected by the user
                                  0x00, 0x00, 0x00, 0x00,            // reserved for future use
                                  0x00, 0x00, 0x00, 0x00             // reserved for future use
                                 };
        uint8_t writeNfcTagStatus = writeNfcTagData(bytesToWrite, sizeof(bytesToWrite));
        if (writeNfcTagStatus == 1) mp3.playMp3FolderTrack(msgSetupNewTagConfirm);
        else mp3.playMp3FolderTrack(msgSetupNewTagError);
        inputEvent = NOACTION;
      }
      // # end - nfc tag does not have our magic cookie 0x1337 0xb347 on it (0)
      // ######################################################################
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
    if (!isLocked) {
      Serial.println(F("lock"));
      isLocked = true;
#if defined(STATUSLED)
      statusLedBurst();
#endif
    }
    else {
      Serial.println(F("unlock"));
      isLocked = false;
#if defined(STATUSLED)
      statusLedBurst();
#endif
    }
  }
  // button 0 (middle) press or ir remote play+pause: toggle playback
  else if ((inputEvent == B0P && !isLocked) || inputEvent == IRP) {
    if (isPlaying) {
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
  else if (((inputEvent == B1P && !isLocked) || inputEvent == IRU) && isPlaying) {
    if (mp3CurrentVolume < mp3MaxVolume) {
      mp3.setVolume(++mp3CurrentVolume);
      Serial.print(F("volume "));
      Serial.println(mp3CurrentVolume);
    }
  }
  // button 2 (left) press or ir remote down while playing: decrease volume
  else if (((inputEvent == B2P && !isLocked) || inputEvent == IRD) && isPlaying) {
    if (mp3CurrentVolume > 0) {
      mp3.setVolume(--mp3CurrentVolume);
      Serial.print(F("volume "));
      Serial.println(mp3CurrentVolume);
    }
  }
  // button 1 (right) hold for 2 sec or ir remote right, only during album, party and story book mode while playing: next track
  else if (((inputEvent == B1H && !isLocked) || inputEvent == IRR) && (nfcTag.playbackMode == ALBUM || nfcTag.playbackMode == PARTY || nfcTag.playbackMode == STORYBOOK) && isPlaying) {
    Serial.println(F("next"));
    playNextTrack(random(65536), true, true);
  }
  // button 2 (left) hold for 2 sec or ir remote left, only during album, party and story book mode while playing: previous track
  else if (((inputEvent == B2H && !isLocked) || inputEvent == IRL) && (nfcTag.playbackMode == ALBUM || nfcTag.playbackMode == PARTY || nfcTag.playbackMode == STORYBOOK) && isPlaying) {
    Serial.println(F("previous"));
    playNextTrack(random(65536), false, true);
  }
  // button 0 (middle) hold for 5 sec or ir remote menu, only during story book mode while playing: reset progress
  else if (((inputEvent == B0H && !isLocked) || inputEvent == IRM) && nfcTag.playbackMode == STORYBOOK && isPlaying) {
    playback.playTrack = 1;
    Serial.print(F("reset "));
    printModeFolderTrack(playback.playTrack, true);
    EEPROM.update(nfcTag.assignedFolder, 0);
    mp3.playFolderTrack(nfcTag.assignedFolder, playback.playTrack);
  }
  // button 0 (middle) hold for 5 sec or ir remote menu while not playing: erase nfc tag
  else if (((inputEvent == B0H && !isLocked) || inputEvent == IRM) && !isPlaying) {
    playback.playListMode = false;
    switchButtonConfiguration(CONFIG);
    shutdownTimer(STOP);
    Serial.println(F(">tag erase"));
    mp3.playMp3FolderTrack(msgEraseTag);
    // loop until tag is erased
    uint8_t writeNfcTagStatus = 0;
    while (!writeNfcTagStatus) {
      checkForInput();
      // button 0 (middle) hold for 2 sec or ir remote menu: cancel erase nfc tag
      if (inputEvent == B0H || inputEvent == IRM) {
        switchButtonConfiguration(PAUSE);
        shutdownTimer(START);
        Serial.println(F("cancel"));
        mp3.playMp3FolderTrack(msgEraseTagCancel);
        return;
      }
      // wait for nfc tag, erase once detected
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        switchButtonConfiguration(PAUSE);
        shutdownTimer(START);
        uint8_t bytesToWrite[16];
        for (uint8_t i = 0; i < 16; i++) bytesToWrite[i] = 0x00;
        writeNfcTagStatus = writeNfcTagData(bytesToWrite, sizeof(bytesToWrite));
        if (writeNfcTagStatus == 1) mp3.playMp3FolderTrack(msgEraseTagConfirm);
        else mp3.playMp3FolderTrack(msgEraseTagError);
      }
#if defined(STATUSLED)
      statusLedBlink();
#endif
      mp3.loop();
    }
  }
  // # end - handle button or ir remote events during playback or while waiting for nfc tags
  // #######################################################################################

  mp3.loop();
}
