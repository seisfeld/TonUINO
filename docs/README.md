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
- Konfigurationsdialoge (NFC Tags/Karten anlernen/löschen, Elternmenü etc.) können abgebrochen werden.
- NFC Tags/Karten können wieder komplett gelöscht werden.
- Elternmenü um NFC Tags/Karten zu löschen und um Einstellungen wie Startlautstärke, Maximallautstärke, Equalizer und Abschalttimer (benötigt eine externe Schaltung) vorzunehmen. Dort kann die Abschaltung auch manuell vorgenommen werden.
- Einstellungen werden im EEPROM gespeichert.
- Optional: Umstellbar auf 5 Tasten Bedienung.
- Optional: Fernbedienbar über eine Infrarotfernbedienung (diese muss mindestens 7 Tasten haben), welche über das Elternmenü angelernt werden kann. Über die Fernbedienung ist es dann auch möglich die Tasten von TonUINO zu sperren.
- Optional: Unterstützung einer Status LED.
- Optional: Unterspannungsabschaltung für z.B. die [CubieKid Platine](https://www.thingiverse.com/thing:3148200). Desweiteren ist es dann auch möglich sich den aktuellen Batteriefüllstand im Elternmenü ansagen zu lassen.

## Tastenbelegung

![Tastenbelegung](usage_cheat_sheet_de.png)

## Ordnerstruktur auf der SD Karte

Die Ordner auf der SD Karte, in denen eure Audio Dateien abgelegt werden, müssen **01 bis 99** heißen - also **zweistellig** sein. Die Dateien in den Ordnern müssen mit einer **dreistelligen** Nummer beginnen - **001 bis 255** - können aber in der Regel [1] weitere Zeichen enthalten. Erlaubt wäre demnach `001.mp3` oder auch `001Lieblingslied.mp3`.

Es hat sich bewährt, die gesammte Ordnerstruktur auf dem Comupter vorzubereiten und dann in einem Rutsch auf die SD Karte zu kopieren. So wird sichergestellt, daß alle Dateien auch in der richtigen Reihenfolge sind.

[1] Nicht alle DFPlayer Mini Module akzeptieren im Dateinamen weitere Zeichen hinter der dreistelligen Nummer. Hier geht dann leider nur `001.mp3`, `002.mp3` usw.

## Audio Meldungen

TonUINO funktioniert nur korrekt, wenn ein **zur Firmware passendes** Set and Audio Meldungen auf der SD Karte vorhanden ist (die Ordner **mp3** und **advert**).

Das passende Set an Audio Meldungen lässt sich mit dem beigelegten Script `create_audio_messages.py` erzeugen. Das Script kann sowohl deutsche als auch englische Audio Meldungen erzeugen. Die Beschreibung weiter unten beschreibt *deutsch*.

Dabei gibt es zwei Möglichkeiten:

  - Unter macOS mit den Tools `say` (ist Teil von macOS) und `ffmpeg`. Sofern man einen Mac hat ist dieser Weg schnell und einfach.
  - Mit Hilfe des Google-Service Text-to-speech. Hierzu muss man erst einen kostenfreien API Key besorgen. Dafür klingt das Ergebnis besser.

Zum Download kann ich die Dateien derzeit leider nicht anbieten!

## Audio Meldungen mit `say` erzeugen

Neben dem Tool `say` (ist Teil von macOS) wird hier noch `ffmpeg` benötigt. Letzteres lässt sich z.B. ganz einfach über Homebrew installieren.

Zur Installation von Homebrew öffnet Ihr einfach `Terminal.app` und folgt danach den Anweisungen auf der [Homebrew](https://brew.sh) Webseite. Nachdem das erledigt ist, führt ihr im Terminal folgende Schritte aus:

### Installation von ffmpeg

1. `brew update`
2. `brew install ffmpeg`

Wenn dies erfolgreich erledigt ist, können die Audio Meldungen erzeugt werden. Dazu im Terminal:

### Audio Meldungen erzeugen

1. In den Ordner wechseln wo ihr die `.zip` Datei von GitHub entpackt, bzw. das Repository gecloned habt.
2. `python create_audio_messages.py de` ausführen.
3. Kopiert den Inhalt des Ordners **sd-card** auf die SD Karte.

## Audio Meldungen mit Google text-to-speech erzeugen

1. Auf der [Google text-to-speech Seite](https://cloud.google.com/text-to-speech/) einen Account anlegen und einen API-Key erzeugen.
2. In den Ordner wechseln wo ihr die `.zip` Datei von GitHub entpackt, bzw. das Repository gecloned habt.
3. Die Datei `create_audio_messages.py` öffnen und darin bei `googleApiKey=` den API-Key eintragen und das Kommentarzeichen `#` am Anfang der Zeile entfernen.
4. `python create_audio_messages.py de` ausführen.
5. Kopiert den Inhalt des Ordners **sd-card** auf die SD Karte.

## Lizenz

GPL v3. Siehe [LICENSE](../LICENSE.md).