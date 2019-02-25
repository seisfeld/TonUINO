#!/bin/sh
if [ -z "$1" ];then
  echo "Usage: $0 [en | de]"
  exit 1
elif [ "$1" = "de" ];then
  VOICE="Anna"
  INPUT="audio_messages_de.txt"
elif [ "$1" = "en" ];then
  VOICE="Samantha"
  INPUT="audio_messages_en.txt"
fi
for dir in advert mp3
do
  if [ -d $dir ];then
    echo "Directory $dir already exists."
    exit 1
  else
    mkdir $dir
  fi
done
for i in {1..255};
do
  j=$(printf "%04d" $i)
  say -v $VOICE -o $j.aiff $i
  ffmpeg -y -i $j.aiff -acodec libmp3lame -ab 128k -ac 1 advert/$j.mp3 -acodec libmp3lame -ab 128k -ac 1 mp3/$j.mp3
  rm $j.aiff
done
awk -F'|' '{system("say -v '$VOICE' -o outfile.aiff " $2 " && ffmpeg -y -i outfile.aiff -acodec libmp3lame -ab 128k -ac 1 mp3/" $1 " && rm outfile.aiff")}' < $INPUT
