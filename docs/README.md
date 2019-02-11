###[For english, please click here!](README_EN.md)

Alternative TonUINO Firmware
============================

Dies ist meine alternative Firmware für das wundervolle [TonUINO](https://www.voss.earth/tonuino/) Projekt. Ziel ist hier nicht unbedingt 100%ige Funktionsgleichheit mit der original Firmware. Es wurden vielmehr für meinen Zweck interessante Funktionen hinzugefügt und desweiteren auch einige Punkte (erstmal?) ausgelassen. Im großen und ganzen macht es einfach Spaß, sich mit der TonUINO Plattform auszutoben, ich bin deswegen auch viel in der [TonUINO Community](https://discourse.voss.earth/) aktiv. Schaut doch einfach mal vorbei - ihr findet dort viele Bauvorschläge, Hacks und Informationen rund um den TonUINO.

**Die Firmware wird "as-is" zur Verfügung gestellt. Wenn jemand die Firmware in seinem TonUINO einsetzt, freue ich mich natürlich darüber. Ich kann allerdings keinen Support bieten.**

## Funktionsübersicht

- Abspielmodi: Hörspiel, Album, Party, Lieblingsfolge und Hörbuch.
- Speichert die Ordnerverknüpfungen, Abspielmodi etc. auf den NFC Tags/Karten.
- Unterstützung für MIFARE Classic (Mini, 1k & 4K) Tags/Karten.
- Unterstützung für MIFARE Ultralight / Ultralight C Tags/Karten.
- Unterstützung für NTAG213/215/216 Tags/Karten.
- Debugausgabe auf der seriellen Konsole.
- Maximale Lautstärke kann eingestellt werden.
- Konfigurationsdialoge (NFC Tags/Karten anlernen/löschen etc.) können abgebrochen werden.
- NFC Tags/Karten können komplett wieder gelöscht werden.
- Nächster/Vorheriger Titel in den Abspielmodi Album, Party und Hörbuch.
- Timer zum automatischen abschalten.
- Optional: Fernbedienbar über Infrarotfernbedienung (inkl. Tastensperre).
- Optional: Unterstützung einer Status LED.
- Optional: Unterspannungsabschaltung für z.B. die [CubieKid Platine](https://www.thingiverse.com/thing:3148200).

## Tastenbelegung

![Tastenbelegung](usage_cheat_sheet_de.png)

## Ordnerstruktur auf der SD Karte

Die Ordner auf der SD Karte, in denen eure Audio Dateien abgelegt werden, müssen **01 bis 99** heißen - also **zweistellig** sein. Die Dateien in den Ordnern müssen mit einer **dreistelligen** Nummer beginnen - **001 bis 255** - können aber in der Regel [1] weitere Zeichen enthalten. Erlaubt wäre demnach `001.mp3` oder auch `001Lieblingslied.mp3`.

Es hat sich bewährt, die gesammte Ordnerstruktur auf dem Comupter vorzubereiten und dann in einem Rutsch auf die SD Karte zu kopieren. So wird sichergestellt, daß alle Dateien auch in der richtigen Reihenfolge sind.

[1] Nicht alle DFPlayer Mini Module akzeptieren im Dateinamen weitere Zeichen hinter der dreistelligen Nummer. Hier geht dann leider nur `001.mp3`, `002.mp3` usw.

## Audio Meldungen

TonUINO funktioniert nur korrekt, wenn ein **zur Firmware passendes** Set and Audio Meldungen auf der SD Karte vorhanden ist (die Ordner **mp3** und **advert**).

Unter macOS lässt sich das passende Set an Audio Meldungen mit dem beigelegten Shellscript `create_audio_messages.sh` schnell und einfach erzeugen. Benötigt werden dazu zusätzlich noch die Tools `say` (ist Teil von macOS) und `ffmpeg`. Letzteres lässt sich z.B. ganz einfach über Homebrew installieren.

Zur installation von Homebrew folgt ihr einfach den Anweisungen auf der [Homebrew](https://brew.sh) Webseite. Nachdem das erledigt ist, führt ihr folgende Schritte aus:

### Installation von ffmpeg

1. `brew update`
2. `brew install ffmpeg`

Wenn dies erfolgreich erledigt ist, können die Audio Meldungen erzeugt werden:

### Audio Meldungen erzeugen

1. In den Ordner wechseln wo ihr die `.zip` Datei von GitHub entpackt, bzw. das Repository gecloned habt.
2. `sh create_audio_messages.sh` ausführen.
3. Kopiert die Ordner **mp3** und **advert** auf die SD Karte.

Zum Download kann ich die Dateien derzeit leider nicht anbieten!

## Lizenz

GPL v3. Siehe [LICENSE](../LICENSE.md).
