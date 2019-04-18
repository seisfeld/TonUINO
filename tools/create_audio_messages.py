#!/usr/bin/python

# By default the text-to-speech engine of MacOS is used (command `say`)
# If you want to use the Google text-to-speech engine instead, go to https://cloud.google.com/text-to-speech/
# and create an account and an API key.
# Then uncomment the following line and put in your API key.
#googleApiKey="ABCDEFGH12345678"


### End of config ###


import base64, json, os, re, shutil, subprocess, sys


if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + " [en | de]")
    exit(1)

lang = sys.argv[1]
if lang == 'de':
    sayVoice = 'Anna'
    googleVoice = { 'languageCode': 'de-DE', 'name': 'de-DE-Wavenet-C' }
elif lang == 'en':
    sayVoice = 'Samantha'
    googleVoice = { 'languageCode': 'en-US', 'name': 'en-US-Wavenet-D' }
else:
    print("Unsupported language: " + lang)
    exit(1)

targetDir = 'sd-card'
if os.path.isdir(targetDir):
    print("Directory `" + targetDir + "` already exists.")
    exit(1)
else:
    os.mkdir(targetDir)
    os.mkdir(targetDir + '/advert')
    os.mkdir(targetDir + '/mp3')


useGoogleTextToSpeech = True
try:
    googleApiKey
except NameError:
    useGoogleTextToSpeech = False


def postJson(url, postBody):
    response = subprocess.check_output(['curl', '--header', 'Content-Type: application/json; charset=utf-8', '--data', json.dumps(postBody).encode('utf-8'), url])
    return json.loads(response)


def textToSpeech(text, targetFile):
    print('\nGenerating: ' + targetFile + ' - ' + text)
    if useGoogleTextToSpeech:
        responseJson = postJson(
            'https://texttospeech.googleapis.com/v1beta1/text:synthesize?key=' + googleApiKey,
            {
                'audioConfig': {
                    'audioEncoding': 'MP3',
                    'speakingRate': 1.0,
                    'pitch': 2.0,  # Default is 0.0
                    'sampleRateHertz': 44100,
                    'effectsProfileId': [ 'small-bluetooth-speaker-class-device' ]
                },
                'voice': googleVoice,
                'input': { 'text': text }
            }
        )

        mp3Data = base64.b64decode(responseJson['audioContent'])

        with open(targetFile, 'wb') as f:
            f.write(mp3Data)
    else:
        subprocess.call([ 'say', '-v', sayVoice, '-o', 'temp.aiff', text ])
        subprocess.call([ 'ffmpeg', '-y', '-i', 'temp.aiff', '-acodec', 'libmp3lame', '-ab', '128k', '-ac', '1', targetFile ])
        os.remove('temp.aiff')


for i in range(1,256):
    targetFile1 = '{}/mp3/{:0>4}.mp3'.format(targetDir, i)
    targetFile2 = '{}/advert/{:0>4}.mp3'.format(targetDir, i)
    textToSpeech('{}'.format(i), targetFile1)
    shutil.copy(targetFile1, targetFile2)

with open('audio_messages_' + lang + '.txt') as f:
    lineRe = re.compile('^([^|]+)\\|(.*)$')
    for line in f:
        match = lineRe.match(line.strip())
        if match:
            fileName = match.group(1)
            text = match.group(2)
            textToSpeech(text, targetDir + '/mp3/' + fileName)
