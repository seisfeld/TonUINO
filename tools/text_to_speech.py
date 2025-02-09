#!/usr/bin/env python3

# Converts text into spoken language saved to an mp3 file.


import argparse, base64, json, os, subprocess, sys
import requests  # For ElevenLabs API
try:
    import urllib.request
except ImportError:
    print("WARNING: It looks like you are using an old version of Python. Please use Python 3 if you intend to use Google Text to Speech.")

class PatchedArgumentParser(argparse.ArgumentParser):
    def error(self, message):
        sys.stderr.write('ERROR: %s\n\n' % message)
        self.print_help()
        sys.exit(2)


sayVoiceByLang = {
    'de': 'Anna',
    'en': 'Samantha',
    'fr': 'Thomas',
    'nl': 'Xander',
    'es': 'Monica',
    'cz': 'Zuzana',
    'it': 'Alice'
}
googleVoiceByLang = {
    'de': { 'languageCode': 'de-DE', 'name': 'de-DE-Wavenet-C' },
    'en': { 'languageCode': 'en-US', 'name': 'en-US-Wavenet-C' },
    'fr': { 'languageCode': 'fr-FR', 'name': 'fr-FR-Wavenet-C' },
    'nl': { 'languageCode': 'nl-NL', 'name': 'nl-NL-Wavenet-A' },
    'es': { 'languageCode': 'es-ES', 'name': '' },
    'cz': { 'languageCode': 'cs-CZ', 'name': 'cs-CZ-Wavenet-A' },
    'it': { 'languageCode': 'it-IT', 'name': 'it-IT-Standard-B' }
}
amazonVoiceByLang = {
    # See: https://docs.aws.amazon.com/de_de/polly/latest/dg/voicelist.html
    'de': 'Vicki',
    'en': 'Joanna',
    'fr': 'Celine',
    'nl': 'Lotte',
    'es': 'Lucia',
    'it': 'Carla'
}
elevenlabsVoiceByLang = {
    # See: https://elevenlabs.io/app/voice-library
    'de': 'Ewvy14akxdhONg4fmNry', # Finnegan Fairytale
    'en': 'NOpBlnGInO9m6vDvFkFC', # Grandpa Spuds Oxley
    'fr': 'FvmvwvObRqIHojkEGh5N', # Adina
    'nl': 'YUdpWWny7k5yb4QCeweX', # Ruth
    'es': 'KHCvMklQZZo0O30ERnVn', # Sara Martin - 1
    'it': '3DPhHWXDY263XJ1d2EPN' # Linda Fiore
}


textToSpeechDescription = """
The following text-to-speech engines are supported:
- With `--use-say` the text-to-speech engine of MacOS is used (command `say`).
- With `--use-amazon` Amazon Polly is used. Requires the AWS CLI to be installed and configured. See: https://aws.amazon.com/cli/
- With `--use-google-key=YOUR_API_KEY` Google text-to-speech is used. See: https://cloud.google.com/text-to-speech/
- With `--use-elevenlabs-key=YOUR_API_KEY`, ElevenLabs text-to-speech is used. See: https://elevenlabs.io/docs

ElevenLabs sounds best, Amazon Polly is second, Google text-to-speech is third, MacOS `say` sounds worst.'
""".strip()

def addArgumentsToArgparser(argparser):
    argparser.add_argument('--lang', choices=['de', 'en', 'fr', 'nl', 'es', 'cz', 'it'], default='de', help='The language (default: de)')
    argparser.add_argument('--use-say', action='store_true', default=None, help="If set, the MacOS tool `say` will be used.")
    argparser.add_argument('--use-amazon', action='store_true', default=None, help="If set, Amazon Polly is used. If missing the MacOS tool `say` will be used.")
    argparser.add_argument('--use-google-key', type=str, default=None, help="The API key of the Google text-to-speech account to use.")
    argparser.add_argument('--use-elevenlabs-key', type=str, default=None, help="The API key of the ElevenLabs account to use.")


def checkArgs(argparser, args):
    if not args.use_say and not args.use_amazon and args.use_google_key is None and args.use_elevenlabs_key is None:
        print('ERROR: You have to provide one of the arguments `--use-say`, `--use-amazon`, `--use-google-key`, or `--use-elevenlabs-key`\n')
        argparser.print_help()
        sys.exit(2)
    if args.use_say:
        checkLanguage(sayVoiceByLang, args.lang, argparser)
    if args.use_google_key:
        checkLanguage(googleVoiceByLang, args.lang, argparser)
    if args.use_amazon:
        checkLanguage(amazonVoiceByLang, args.lang, argparser)
    if args.use_elevenlabs_key:
        checkLanguage(elevenlabsVoiceByLang, args.lang, argparser)

def checkLanguage(dictionary, lang, argparser):
    if lang not in dictionary:
        print('ERROR: Language is not supported by selected text-to-speech engine\n')
        argparser.print_help()
        sys.exit(2)


def textToSpeechUsingArgs(text, targetFile, args):
    textToSpeech(text, targetFile, lang=args.lang, useAmazon=args.use_amazon, useGoogleKey=args.use_google_key, useElevenLabsKey=args.use_elevenlabs_key)


def textToSpeech(text, targetFile, lang='de', useAmazon=False, useGoogleKey=None, useElevenLabsKey=None):
    print('\nGenerating: ' + targetFile + ' - ' + text)
    if useAmazon:
        response = subprocess.check_output(['aws', 'polly', 'synthesize-speech', '--output-format', 'mp3',
            '--voice-id', amazonVoiceByLang[lang], '--text-type', 'ssml',
            '--text', '<speak><amazon:effect name="drc"><prosody rate=\"+10%\">' + text + '</prosody></amazon:effect></speak>',
            targetFile])
    elif useGoogleKey:
        responseJson = postJson(
            'https://texttospeech.googleapis.com/v1/text:synthesize?key=' + useGoogleKey,
            {
                'audioConfig': {
                    'audioEncoding': 'MP3',
                    'speakingRate': 1.0,
                    'pitch': 2.0,  # Default is 0.0
                    'sampleRateHertz': 44100,
                    'effectsProfileId': [ 'small-bluetooth-speaker-class-device' ]
                },
                'voice': googleVoiceByLang[lang],
                'input': { 'text': text }
            }
        )

        mp3Data = base64.b64decode(responseJson['audioContent'])

        with open(targetFile, 'wb') as f:
            f.write(mp3Data)
    elif useElevenLabsKey:
        # Use ElevenLabs for TTS
        elevenlabs_url = "https://api.elevenlabs.io/v1/text-to-speech/uvysWDLbKpA4XvpD3GI6"
        payload = {
            "text": text,
            "voice_settings": {"stability": 0.5, "similarity_boost": 0.7},
            "model_id": "eleven_multilingual_v2"
        }
        headers = {
            "Accept": "application/json",
            "Content-Type": "application/json",
            "xi-api-key": useElevenLabsKey
        }
        response = requests.post(elevenlabs_url, json=payload, headers=headers)
        if response.status_code != 200:
            print(f"ERROR: ElevenLabs API request failed with status {response.status_code}: {response.text}")
            sys.exit(1)
        with open(targetFile, "wb") as f:
            f.write(response.content)
    else:
        subprocess.call([ 'say', '-v', sayVoiceByLang[lang], '-o', 'temp.aiff', text ])
        subprocess.call([ 'ffmpeg', '-y', '-i', 'temp.aiff', '-acodec', 'libmp3lame', '-ab', '128k', '-ac', '1', targetFile ])
        os.remove('temp.aiff')


def postJson(postUrl, postBody, headers = {}):
    headers['Content-Type'] = 'application/json; charset=utf-8'
    postData = json.dumps(postBody).encode('utf-8')
    try:
        postRequest = urllib.request.Request(postUrl, postData, headers)
        with urllib.request.urlopen(postRequest) as req:
            postResponseData=req.read()
        return json.loads(postResponseData.decode())
    except Exception as e:
        print(e)


if __name__ == '__main__':
    argFormatter = lambda prog: argparse.RawDescriptionHelpFormatter(prog, max_help_position=30, width=100)
    argparser = PatchedArgumentParser(
        description=
            'Converts text into spoken language saved to an mp3 file.\n\n' +
            textToSpeechDescription,
        usage='%(prog)s -t "This is my text" -o my-output.mp3 [optional arguments...]',
        formatter_class=argFormatter)
    argparser.add_argument('-t', '--text', type=str, required=True, help='The text to convert into spoken language.')
    argparser.add_argument('-o', '--output', type=str, required=True, help='The output mp3 file to create')
    addArgumentsToArgparser(argparser)
    args = argparser.parse_args()


    checkArgs(argparser, args)

    if os.path.exists(args.output):
        print('ERROR: Output file already exists: ' + os.path.abspath(args.output))
        sys.exit(1)


    textToSpeechUsingArgs(text=args.text, targetFile=args.output, args=args)
