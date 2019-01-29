#!/usr/bin/env bash

# replace google api key with your key
GOOGLE_API_KEY=qwertz

[ -d mp3 ] || mkdir mp3
[ -d advert ] || mkdir advert

rm -f mp3/*mp3 advert/*mp3

for i in {1..255}
do
	echo -n "mp3/$(printf "%04d" $i).mp3 ... "
  curl -s -H "Content-Type: application/json; charset=utf-8" -d "{ 'audioConfig': {'audioEncoding': 'MP3','speakingRate': 1.1,'sampleRateHertz': 44100,'pitch': 0}, 'voice': {'languageCode': 'de-DE','name': 'de-DE-Wavenet-C'}, 'input': {'text': '${i}'} }" "https://texttospeech.googleapis.com/v1/text:synthesize?fields=audioContent&key=${GOOGLE_API_KEY}" | php -r "print(json_decode(file_get_contents('php://stdin'),true)['audioContent']);" | base64 --decode > mp3/$(printf "%04d" $i).mp3
	echo "DONE"
  sleep 0.5
done

cp mp3/*.mp3 advert/

while IFS= read -d $'\n' -r line; do
	echo -n "${line%|*} ... "
  curl -s -H "Content-Type: application/json; charset=utf-8" -d "{ 'audioConfig': {'audioEncoding': 'MP3','speakingRate': 1.1,'sampleRateHertz': 44100,'pitch': 0}, 'voice': {'languageCode': 'de-DE','name': 'de-DE-Wavenet-C'}, 'input': {'text': '${line#*|}'} }" "https://texttospeech.googleapis.com/v1/text:synthesize?fields=audioContent&key=${GOOGLE_API_KEY}" | php -r "print(json_decode(file_get_contents('php://stdin'),true)['audioContent']);" | base64 --decode > ${line%|*}
  echo "DONE"
  sleep 0.5
done < audio_messages.txt
