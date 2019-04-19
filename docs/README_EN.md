Alternative TonUINO Firmware
============================

This is my alternative firmware for the wonderful [TonUINO](https://www.voss.earth/tonuino/) project. The goal of this firmware is not to implement 100% the same features as the original firmware. I more or less just added what fits my use case and i also removed (for now?) some stuff i currently don't really need. Overall it's just fun to play around with TonUINO, which is also the reason why i am very active in the [TonUINO Community](https://discourse.voss.earth/). Make sure you drop by, there is lots of stuff to discover.

**The firmware is provided "as-is". I'm happy if all this is useful for anyone else, but can't offer support.**

## Features

- Playback modes: Story, album, party, single, story book.
- Saves playback modes etc. directly to the NFC tags/cards.
- Next/previous track in album, party and story mode.
- Supports MIFARE Classic (Mini, 1K & 4K) tags/cards.
- Supports MIFARE Ultralight / Ultralight C tags/cards.
- Supports NTAG213/215/216 tags/cards.
- Debug output to the serial console.
- Preferences are stored in EEPROM.
- Setup dialogues (setup/erase NFC tags/cards, parents menu etc.) can be aborted.
- NFC tags/cards can be erased.
- Parents menu to erase NFC tags/cards and to change preferences like startup volume, maximum volume, menu volume, equalizer and shutdown timer (requires an external circuit or compatible power bank). You can also manually trigger the shutdown there and reset the story book progress and preferences.
- Optional: PIN to protect parental functions.
- Optional: 5 Buttons.
- Optional: IR remote control (incl. box lock). The remote (which needs at least 7 keys) can be learned in using the parents menu.
- Optional: Status LED.
- Optional: Low voltage shutdown i.e. for the [CubieKid PCB](https://www.thingiverse.com/thing:3148200).

## Button Cheat Sheet

![Tastenbelegung](usage_cheat_sheet_en.png)

## PIN Code

The (optional) PIN Code to secure the parental functions is by default

* `play/pause, vol-, vol+, play/pause`

and can be changed in the sketch before compile time.

## SD Card Folder Structure

The folders on the SD card, that will hold your media files, need to be named **01 bis 99** - that is **two digits**. The media files in these folders need to start with a **three digit zero padded number** like **001 to 255**, but may [1] contain more characters afterwards. Allowed would be `001.mp3` or `001MyTune.mp3`.

It has been proven benefitial to prepare the whole folder structure on the computer and then copy everything to the SD card in one go. That way it's made sure the order of the files is correct.

[1] Not all derivates of the DFPlayer Mini module accept characters after the three digit number. In this case only `001.mp3`, `002.mp3` etc. would be allowed.

## Audio Messages

TonUINO only functions correctly, when there is the correct (**as in matches the firmware**) set of audio messages on the SD card. These are stored in the folders **mp3** and **advert**.

You can create the matching set of audio messages, using the `create_audio_messages.py` python script supplied in this repo. The script can create *english* and *german* audio messages. The description down below describes *english*.

The script offers two options to do this:

  1. Using the tools `say` (requires macOS) and `ffmpeg`. If you have a Mac this way is pretty simple.
  2. Using Googles text-to-speech service. For this option you need an API key (which is free for the amount of messages you need), but the result sounds better than option one.

The audio message files are not available for download at this point in time.

### How to create audio messages using `say` (option one)

Besides the tool `say` (part of macOS) you also need `ffmpeg`. The later can quickly be installed via Homebrew.

To install Homebrew, open `Terminal.app` and follow the instructions on the [Homebrew](https://brew.sh) website. After that, please follow these steps in Terminal:

#### Install ffmpeg

1. `brew update`
2. `brew install ffmpeg`

When that completed successfully, you can create the audio messages. In Terminal:

#### Create Audio Messages

1. Change into the folder where you unzipped the `.zip` from GitHub or where you cloned the repo to.
2. Run `python create_audio_messages.py en`.
3. Copy the contents of the folder **sd-card** to the SD Card.

### How to create audio messages using Googles text-to-speech service (option two)

1. Go to [Googles text-to-speech website](https://cloud.google.com/text-to-speech/) and create an account and an API key.
2. Change into the folder where you unzipped the `.zip` from GitHub or where you cloned the repo to.
3. Open the file `create_audio_messages.py` and enter your API key at the line with `googleApiKey=`. Then remove the comment character `#` at the begining of the line.
4. Run `python create_audio_messages.py en`.
5. Copy the contents of the folder **sd-card** to the SD Card.

## Tool for lead-in messages

In story mode there is the problem that when playing the card one does not know which episode is played. If you play for example Benjamin Blossom, then you'll always hear the title song first, which sounds the same in all episodes.

The script `add_lead_in_messages.py` adds a lead-in message to the mp3 file, such as "Benjamin Blossom on vacation". If you want to hear another episode, then you can just lay on the Benjamin Blossom card again.

**Functionality:**

Suppose you have a folder with the following content:

```
+- 04_Benjamin Blossom/
   +- Benjamin Blossom has his birthday.mp3
   +- Benjamin Blossom on vacation.mp3
   +- Benjamin Blossom as a pilot.mp3
```

Then you can use the following call to generate mp3 files with lead-in messages (example):

    python add_lead_in_messages.py -i '04_Benjamin Blossom' -o /Volumes/TonUINO/04 --google-key=ABCD --add-numbering

What happens:

  - New mp3 files are generated including the lead-in messages. You can also write directly to the SD card (as in the example).
  - The original files are not changed.
  - The mp3 data is not re-encoded (so no quality loss).
  - Optionally, the mp3 files are numbered compatible with DFPlayer Mini. For example `001_Benjamin Blossom has his birthday.mp3` (parameter` --add-numbering`)
  - The script generates the lead-in messages either with Google Text-to-speech (if the parameter `--google-key` was specified) or with the Mac tool `say`.

The result looks like this:

```
+- /Volumes/TonUINO/04/
   +- 001_Benjamin Blossom has his birthday.mp3
   +- 002_Benjamin Blossom on vacation.mp3
   +- 003_Benjamin Blossom as a pilot.mp3
```

## License

GPL v3. See [LICENSE](../LICENSE.md).