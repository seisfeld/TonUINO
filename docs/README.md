[For english, please click here!](README_EN.md)

Alternative TonUINO Firmware
============================

Dies ist meine alternative Firmware für das wundervolle [TonUINO](https://www.voss.earth/tonuino/) Projekt. Ziel ist hier nicht unbedingt 100%ige Funktionsgleichheit mit der original Firmware. Es wurden vielmehr für meinen Zweck interessante Funktionen hinzugefügt und desweiteren auch einige Punkte (erstmal?) ausgelassen. Im großen und ganzen macht es einfach Spaß, sich mit der TonUINO Plattform auszutoben, ich bin deswegen auch viel in der [TonUINO Community](https://discourse.voss.earth/) aktiv. Schaut doch einfach mal vorbei - ihr findet dort viele Bauvorschläge, Hacks und Informationen rund um den TonUINO.

**Die Firmware wird "as-is" zur Verfügung gestellt. Wenn jemand die Firmware in seinem TonUINO einsetzt, freue ich mich natürlich darüber. Ich kann allerdings keinen Support bieten.**

## Funktionsübersicht

- Abspielmodi: Hörspiel, Album, Party, Lieblingsfolge und Hörbuch.
- Speichert die Ordnerverknüpfungen, Abspielmodi etc. auf den NFC Tags/Karten.
- Nächster/Vorheriger Titel in den Abspielmodi Album, Party und Hörbuch.
- Unterstützung für MIFARE Classic (Mini, 1K & 4K) Tags/Karten.
- Unterstützung für MIFARE Ultralight / Ultralight C Tags/Karten.
- Unterstützung für NTAG213/215/216 Tags/Karten.
- Debugausgabe auf der seriellen Konsole.
- Einstellungen werden im EEPROM gespeichert.
- Konfigurationsdialoge (NFC Tags/Karten anlernen/löschen, Elternmenü etc.) können abgebrochen werden.
- NFC Tags/Karten können wieder komplett gelöscht werden.
- Elternmenü um NFC Tags/Karten zu löschen und um Einstellungen wie Startlautstärke, Maximallautstärke, Menülautstärke, Equalizer und Abschalttimer (benötigt eine externe Schaltung oder eine passende Powerbank) vorzunehmen. Dort kann TonUINO auch von Hand abgeschaltet werden und es lassen sich der Hörbuchfortschritt und die Einstellugen zurücksetzen.
- Optional: PIN Code um Elternfunktionen zu schützen.
- Optional: Umstellbar auf 5 Tasten Bedienung.
- Optional: Fernbedienbar über eine Infrarotfernbedienung (diese muss mindestens 7 Tasten haben), welche über das Elternmenü angelernt werden kann. Über die Fernbedienung ist es dann auch möglich die Tasten von TonUINO zu sperren.
- Optional: Unterstützung einer Status LED.
- Optional: Unterspannungsabschaltung für z.B. die [CubieKid Platine](https://www.thingiverse.com/thing:3148200).

## Tastenbelegung

![Tastenbelegung](usage_cheat_sheet_de.png)

## PIN Code

Der (optional einschaltbare) PIN Code um die Elternfunktionen abzusichern lautet standard mässig

* `play/pause, vol-, vol+, play/pause`

und kann im Sketch vor dem kompilieren geändert werden.

## Ordnerstruktur auf der SD Karte

Die Ordner auf der SD Karte, in denen eure Audio Dateien abgelegt werden, müssen **01 bis 99** heißen - also **zweistellig** sein. Die Dateien in den Ordnern müssen mit einer **dreistelligen** Nummer beginnen - **001 bis 255** - können aber in der Regel [1] weitere Zeichen enthalten. Erlaubt wäre demnach `001.mp3` oder auch `001Lieblingslied.mp3`.

Es hat sich bewährt, die gesammte Ordnerstruktur auf dem Comupter vorzubereiten und dann in einem Rutsch auf die SD Karte zu kopieren. So wird sichergestellt, daß alle Dateien auch in der richtigen Reihenfolge sind.

[1] Nicht alle DFPlayer Mini Module akzeptieren im Dateinamen weitere Zeichen hinter der dreistelligen Nummer. Hier geht dann leider nur `001.mp3`, `002.mp3` usw.

## Audio Meldungen

TonUINO funktioniert nur korrekt, wenn ein **zur Firmware passendes** Set an Audio Meldungen auf der SD Karte vorhanden ist (die Ordner **mp3** und **advert**).

Das passende Set an Audio Meldungen lässt sich mit dem beigelegten Python Script `create_audio_messages.py` erzeugen. Das Script kann sowohl *deutsche* als auch *englische* Audio Meldungen erzeugen. Die Beschreibung weiter unten beschreibt *deutsch*.

Das Skript bietet dabei zwei Möglichkeiten:

  1. Mit den Tools `say` (benötigt macOS) und `ffmpeg`. Sofern man einen Mac hat ist dieser Weg schnell und einfach.
  2. Mit Hilfe des text-to-speech Service von Google. Hierzu muss man erst einen (für die Anzahl der benötigten Meldungen) kostenfreien API Key besorgen, dafür klingt das Ergebnis besser als Möglichkeit Eins.

Zum Download kann ich die Dateien derzeit leider nicht anbieten!

### Audio Meldungen mit `say` erzeugen (Möglichkeit Eins)

Neben dem Tool `say` (ist Teil von macOS) wird hier noch `ffmpeg` benötigt. Letzteres lässt sich z.B. ganz einfach über Homebrew installieren.

Zur Installation von Homebrew öffnet Ihr einfach `Terminal.app` und folgt danach den Anweisungen auf der [Homebrew](https://brew.sh) Webseite. Nachdem das erledigt ist, führt ihr im Terminal folgende Schritte aus:

#### Installation von ffmpeg

1. `brew update`
2. `brew install ffmpeg`

Wenn dies erfolgreich erledigt ist, können die Audio Meldungen erzeugt werden. Dazu im Terminal:

#### Audio Meldungen erzeugen

1. In den Ordner wechseln wo ihr die `.zip` Datei von GitHub entpackt, bzw. das Repository gecloned habt.
2. `python create_audio_messages.py de` ausführen.
3. Kopiert den Inhalt des Ordners **sd-card** auf die SD Karte.

### Audio Meldungen mit dem text-to-speech Service von Google erzeugen (Möglichkeit Zwei)

1. Auf [Googles text-to-speech Seite](https://cloud.google.com/text-to-speech/) einen Account anlegen und einen API-Key erzeugen.
2. In den Ordner wechseln wo ihr die `.zip` Datei von GitHub entpackt, bzw. das Repository gecloned habt.
3. Die Datei `create_audio_messages.py` öffnen und darin bei `googleApiKey=` den API-Key eintragen und das Kommentarzeichen `#` am Anfang der Zeile entfernen.
4. `python create_audio_messages.py de` ausführen.
5. Kopiert den Inhalt des Ordners **sd-card** auf die SD Karte.

## Tool für Ansagen

Im Hörspielmodus gibt es das Problem, dass man beim Auflegen der Karte nicht weiß, welche Folge abgespielt wird. Spielt man z.B. Benjamin Blümchen ab, dann kommt immer zuerst der Titelsong, der sich bei allen Folgen gleich anhört.

Das Skript `add_lead_in_messages.py` fügt der mp3-Datei eine Ansage wie z.B. "Benjamin Blümchen im Urlaub" hinzu. Wenn man eine andere Folge hören will, kann man dann einfach nochmal die Benjamin-Blümchen-Karte auflegen.

**Funktionsweise:**

Angenommen man hat einen Ordner mit folgendem Inhalt:

```
+- 04_Benjamin Blümchen/
   +- Benjamin Blümchen hat Geburtstag.mp3
   +- Benjamin Blümchen im Urlaub.mp3
   +- Benjamin Blümchen als Pilot.mp3
```

Dann kann man mit folgendem Aufruf mp3-Dateien mit Ansagen generieren (Beispiel):

    python add_lead_in_messages.py -i '04_Benjamin Blümchen' -o /Volumes/TonUINO/04 --google-key=ABCD --add-numbering

Was dann passiert:
  - Es werden neue mp3-Dateien mit den Ansagen erzeugt. Man kann dabei auch direkt auf die SD-Karte schreiben (wie im Beispiel).
  - Die Original-Dateien werden dabei nicht geändert.
  - Die mp3-Daten werden nicht neu kodiert (also kein Qualitätsverlust).
  - Auf Wunsch werden die mp3-Dateien kompatibel zu DFPlayer Mini numeriert. Also z.B. `001_Benjamin Blümchen hat Geburtstag.mp3` (Parameter `--add-numbering`)
  - Das Skript erzeugt die Ansagen entweder mit Google Text-to-speech (wenn der Parameter `--google-key` angegeben wurde) oder mit dem Mac-Tool `say`.

Das Ergebnis sieht dann so aus:

```
+- /Volumes/TonUINO/04/
   +- 001_Benjamin Blümchen hat Geburtstag.mp3
   +- 002_Benjamin Blümchen im Urlaub.mp3
   +- 003_Benjamin Blümchen als Pilot.mp3
```

## Lizenz

GPL v3. Siehe [LICENSE](../LICENSE.md).