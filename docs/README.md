[For english, please click here!](README_EN.md)

Alternative TonUINO Firmware
============================

Dies ist meine alternative Firmware für das wundervolle [TonUINO](https://www.voss.earth/tonuino/) Projekt. Ziel ist hier nicht unbedingt 100%ige Funktionsgleichheit mit der original Firmware. Es wurden vielmehr für meinen Zweck interessante Funktionen hinzugefügt und desweiteren auch einige Punkte (erstmal?) ausgelassen. Im großen und ganzen macht es einfach Spaß, sich mit der TonUINO Plattform auszutoben, ich bin deswegen auch viel in der [TonUINO Community](https://discourse.voss.earth/) aktiv. Schaut doch einfach mal vorbei - ihr findet dort viele Bauvorschläge, Hacks und Informationen rund um den TonUINO.

**Die Firmware wird "as-is" zur Verfügung gestellt. Wenn jemand die Firmware in seinem TonUINO einsetzt, freue ich mich natürlich darüber. Ich kann allerdings keinen Support bieten.**

## Inhalt
- [Funktionsübersicht](https://github.com/seisfeld/TonUINO#funktionsübersicht)
- [Tastenbelegung](https://github.com/seisfeld/TonUINO#tastenbelegung)
- [PIN Code](https://github.com/seisfeld/TonUINO#pin-code)
- [Ordnerstruktur auf der SD Karte](https://github.com/seisfeld/TonUINO#ordnerstruktur-auf-der-sd-karte)
- [Audio Meldungen](https://github.com/seisfeld/TonUINO#audio-meldungen)
  - [Audio Meldungen herunterladen](https://github.com/seisfeld/TonUINO#audio-meldungen-herunterladen)
  - [Audio Meldungen selbst erzeugen](https://github.com/seisfeld/TonUINO#audio-meldungen-selbst-erzeugen)
      - [Audio Meldungen mit say und ffmpeg erzeugen](https://github.com/seisfeld/TonUINO#audio-meldungen-mit-say-und-ffmpeg-erzeugen)
      - [Audio Meldungen mit Amazon Polly erzeugen](https://github.com/seisfeld/TonUINO#audio-meldungen-mit-amazon-polly-erzeugen)
      - [Audio Meldungen mit dem Cloud text-to-speech Service von Google erzeugen](https://github.com/seisfeld/TonUINO#audio-meldungen-mit-dem-cloud-text-to-speech-service-von-google-erzeugen)
      - [Hilfe und weitere Optionen](https://github.com/seisfeld/TonUINO#hilfe-und-weitere-optionen)
- [Titelansagen in MP3-Dateien einfügen](https://github.com/seisfeld/TonUINO#titelansagen-in-mp3-dateien-einfügen)
  - [Funktionsweise](https://github.com/seisfeld/TonUINO#funktionsweise)
  - [Hilfe und weitere Optionen](https://github.com/seisfeld/TonUINO#hilfe-und-weitere-optionen-1)
- [Lizenz](https://github.com/seisfeld/TonUINO#lizenz)

## Funktionsübersicht

- Standard Abspielmodi: Hörspiel, Album, Party, Lieblingsfolge und Hörbuch.
- Erweiterte Abspielmodi: Virtuelle Ordner für die Modi Hörspiel, Album und Party.
- Nächster/Vorheriger Titel sowohl in den Standard Abspielmodi Album, Party und Hörbuch - als auch bei der Nutzung von virtuellen Ordnern in den Modi Album und Party.
- Der aktuell laufende Titel kann dauerhaft wiederholt werden.
- Speichert die Ordnerverknüpfungen, Abspielmodi etc. auf den NFC Tags/Karten.
- Unterstützung für MIFARE Classic (Mini, 1K & 4K) Tags/Karten.
- Unterstützung für MIFARE Ultralight / Ultralight C Tags/Karten.
- Unterstützung für NTAG213/215/216 Tags/Karten.
- Debugausgabe auf der seriellen Konsole.
- Einstellungen werden im EEPROM gespeichert.
- Konfigurationsdialoge (NFC Tags/Karten anlernen/löschen, Elternmenü etc.) können abgebrochen werden.
- NFC Tags/Karten können wieder komplett gelöscht werden.
- Elternmenü um NFC Tags/Karten zu löschen und um Einstellungen wie Startlautstärke, Maximallautstärke, Menülautstärke, Equalizer und Abschalttimer (benötigt eine externe Schaltung oder eine passende Powerbank) vorzunehmen. Dort kann TonUINO auch von Hand abgeschaltet werden und es lassen sich der Hörbuchfortschritt und die Einstellugen zurücksetzen.
- **Optional:** PIN Code um Elternfunktionen zu schützen.
- **Optional:** Umstellbar auf 5 Tasten Bedienung.
- **Optional:** Fernbedienbar über eine Infrarotfernbedienung (diese muss mindestens 7 Tasten haben), welche über das Elternmenü angelernt werden kann. Über die Fernbedienung ist es dann auch möglich die Tasten von TonUINO zu sperren.
- **Optional:** Unterstützung einer Status LED.
- **Optional:** Unterstützung von WS281x LED(s) als Status LED(s).
- **Optional:** Unterspannungsabschaltung für z.B. die [CubieKid Platine](https://www.thingiverse.com/thing:3148200).
- **Optional:** Unterstützung des [Pololu Power Switch (LV)](https://www.pololu.com/product/2808).

## Tastenbelegung

![Tastenbelegung](usage_cheat_sheet_de.png)

## PIN Code

Der (optional einschaltbare) PIN Code um die Elternfunktionen abzusichern lautet standard mässig

- `play/pause, vol-, vol+, play/pause`

und kann im Sketch vor dem kompilieren geändert werden.

## Ordnerstruktur auf der SD Karte

Die Ordner auf der SD Karte, in denen eure MP3-Dateien abgelegt werden, müssen **01 bis 99** heissen - also **zweistellig** sein. Die Dateien in den Ordnern müssen mit einer **dreistelligen** Nummer beginnen - **001 bis 255** - können aber in der Regel [1] weitere Zeichen enthalten. Erlaubt wäre demnach `001.mp3` oder auch `001Lieblingslied.mp3`.

Es hat sich bewährt, die gesammte Ordnerstruktur auf dem Comupter vorzubereiten und dann in einem Rutsch auf die SD Karte zu kopieren. So wird sichergestellt, daß alle Dateien auch in der richtigen Reihenfolge sind.

[1] Nicht alle DFPlayer Mini Module akzeptieren im Dateinamen weitere Zeichen hinter der dreistelligen Nummer. Hier geht dann leider nur `001.mp3`, `002.mp3` usw.

## Audio Meldungen

TonUINO funktioniert nur korrekt, wenn ein **zur Firmware passendes** Set an Audio Meldungen auf der SD Karte vorhanden ist. Dies sind die Ordner **advert** und **mp3**.

### Audio Meldungen herunterladen

Die Audio Meldungen sind mit Amazon Polly generiert worden und können in verschiedenen Sprachen heruntergeladen werden:

- Deutsch: [audio-messages-polly-de.zip](https://seisfeld.github.io/tonuino/audio-messages-polly-de.zip)
- Englisch: [audio-messages-polly-en.zip](https://seisfeld.github.io/tonuino/audio-messages-polly-en.zip)
- Niederländisch: [audio-messages-polly-nl.zip](https://seisfeld.github.io/tonuino/audio-messages-polly-nl.zip)
- Spanisch: [audio-messages-polly-es.zip](https://seisfeld.github.io/tonuino/audio-messages-polly-es.zip)

Die `.zip` Datei entpacken und die Ordner **advert** und **mp3** auf die SD Karte kopieren. Fertig.

### Audio Meldungen selbst erzeugen

Das passende Set an Audio Meldungen lässt sich auch jederzeit mit dem beigelegten Python Skript `create_audio_messages.py` erzeugen. Hier sind dann auch weitere text-to-speech Engines möglich wenn ihr möchtet (siehe unten). Das Skript kann sowohl **deutsche**, **englische**, **niederländische** als auch **spanische** Audio Meldungen erzeugen (unterstützt wird ebenfalls **französisch**, es liegt dafür allerdings momentan keine Quelldatei bei). Es ist unter macOS, Linux und Windows getested, und benötigt **Python 3** und ggf. **ffmpeg**.

Unter Linux kann Python 3 mit dem zur Distribution gehörenden Paketmanager installiert werden. macOS Nutzer benutzen z.B. [Homebrew](https://brew.sh): `brew install python` und für Windows kann es [hier heruntergeladen werden](https://www.python.org/downloads/windows/).

Das Skript unerstützt dabei die Nutzung von drei text-to-speech Engines:

- Lokal - nur unter macOS - mit den Tools `say` und `ffmpeg`. Sofern man einen Mac hat ist dieser Weg schnell, einfach und dauerhaft kostenlos.
- Über das Internet mit Amazon Polly, dem text-to-speech Service von Amazon.
- Über das Internet mit Hilfe des Cloud text-to-speech Service von Google.

Für die Anzahl der benötigten Meldungen ist die Nutzung der Services von Amazon ([Preise](https://aws.amazon.com/de/polly/pricing/)) bzw. Google ([Preise](https://cloud.google.com/text-to-speech/pricing)) erstmal kostenlos. Für beide Services muss man allerdings erst einen Account anlegen, ist dann das Freikontingent irgendwann einmal aufgebraucht fallen Kosten im Bereich von ein paar Cent an.

#### Audio Meldungen mit `say` und `ffmpeg` erzeugen

Neben dem Tool `say` (ist Teil von macOS) wird hier noch `ffmpeg` benötigt.

1. `ffmpeg` installieren, z.B. via [Homebrew](https://brew.sh): `brew install ffmpeg`
2. In den Ordner wechseln wo ihr die `.zip` Datei von GitHub entpackt, bzw. das Repository gecloned habt.
3. `python3 tools/create_audio_messages.py --use-say` ausführen.
4. Kopiert nun den Inhalt des Ordners **sd-card** auf die SD Karte. Fertig.

#### Audio Meldungen mit Amazon Polly erzeugen

1. Auf der [AWS](https://aws.amazon.com/) Webseite einen Account anlegen und Access Keys erzeugen.
2. Das Tool `aws` [installieren](https://docs.aws.amazon.com/de_de/cli/latest/userguide/cli-chap-install.html) (Windows / Linux), macOS z.B. via [Homebrew](https://brew.sh): `brew install awscli`.
3. Das Tool `aws` [konfigurieren](https://docs.aws.amazon.com/de_de/cli/latest/userguide/cli-chap-configure.html).
4. In den Ordner wechseln wo ihr die `.zip` Datei von GitHub entpackt, bzw. das Repository gecloned habt.
5. `python3 tools/create_audio_messages.py --use-amazon` ausführen.
6. Kopiert nun den Inhalt des Ordners **sd-card** auf die SD Karte. Fertig.

#### Audio Meldungen mit dem Cloud text-to-speech Service von Google erzeugen

1. Auf Googles [Cloud text-to-speech](https://cloud.google.com/text-to-speech/) Webseite einen Account anlegen und einen API-Key erzeugen.
2. In den Ordner wechseln wo ihr die `.zip` Datei von GitHub entpackt, bzw. das Repository gecloned habt.
3. `python3 tools/create_audio_messages.py --use-google-key=ABCD` ausführen.
4. Kopiert nun den Inhalt des Ordners **sd-card** auf die SD Karte. Fertig.

#### Hilfe und weitere Optionen

Das Python Skript hat noch einige weitere Funktionen. Eine Übersicht gibt:

- `python3 tools/create_audio_messages.py --help`

## Titelansagen in MP3-Dateien einfügen

Im Hörspielmodus gibt es das Problem, daß man beim Auflegen der Karte nicht weiß, welche Folge abgespielt wird. Spielt man z.B. *Benjamin Blümchen* ab, dann kommt immer zuerst der Titelsong, der sich bei allen Folgen gleich anhört.

Das Python Skript `add_lead_in_messages.py` fügt der MP3-Datei eine Titelansage wie z.B. *Benjamin Blümchen im Urlaub* hinzu. Wenn man eine andere Folge hören möchte, kann man dann einfach nochmal die *Benjamin Blümchen* Karte auflegen. Es ist unter macOS getested, sollte aber mit minimalem Aufwand auch unter Windows / Linux laufen - wenn alle Abhängigkeiten erfüllt werden.

### Funktionsweise

Angenommen man hat einen Ordner mit folgendem Inhalt:

```
+- 04_Benjamin Blümchen
   +- Benjamin Blümchen hat Geburtstag.mp3
   +- Benjamin Blümchen im Urlaub.mp3
   +- Benjamin Blümchen als Pilot.mp3
```

Dann kann man mit folgendem Aufruf MP3-Dateien mit Ansagen generieren (Beispiel):

    python3 tools/add_lead_in_messages.py -i '04_Benjamin Blümchen' -o /Volumes/TonUINO/04 --google-key=ABCD --add-numbering

Was dann passiert:

- Es werden neue MP3-Dateien mit den Ansagen erzeugt. Man kann dabei auch optional direkt auf die SD-Karte schreiben (wie im Beispiel).
- Die Original-Dateien werden dabei nicht geändert.
- Die MP3-Dateien werden nicht neu enkodiert (also kein Qualitätsverlust).
- Auf Wunsch werden die MP3-Dateien kompatibel zum DFPlayer Mini numeriert. Also z.B. `001_Benjamin Blümchen hat Geburtstag.mp3` (Parameter `--add-numbering`)
- Das Skript unerstützt dabei die Nutzung von drei text-to-speech Engines:
  - Lokal - nur unter macOS - mit den Tools `say` und `ffmpeg`. (Parameter `--use-say`)
  - Über das Internet mit Amazon Polly, dem text-to-speech Service von Amazon. (Parameter `--use-amazon`)
  - Über das Internet mit Hilfe des Cloud text-to-speech Service von Google. (Parameter `--google-key=ABCD`)

Das Ergebnis sieht dann so aus:

```
+- /Volumes/TonUINO/04
   +- 001_Benjamin Blümchen hat Geburtstag.mp3
   +- 002_Benjamin Blümchen im Urlaub.mp3
   +- 003_Benjamin Blümchen als Pilot.mp3
```

#### Hilfe und weitere Optionen

Das Python Skript hat noch einige weitere Funktionen. Eine Übersicht gibt:

- `python3 tools/add_lead_in_messages.py --help`

## Lizenz

GPL v3. Siehe [LICENSE](../LICENSE.md).
