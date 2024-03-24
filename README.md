# PQC-SoloKey

## Über dieses Repository

Dieses Repository enthält die Implementierungen und Anpassungen von Post-Quanten-Kryptografie-Algorithmen (PQC) auf SoloKeys, entwickelt im Rahmen meiner Bachelorarbeit. Ziel der Arbeit ist die Evaluation der Performanz ausgewählter PQC-Algorithmen in einer eingebetteten Umgebung, mit einem speziellen Fokus auf die Sicherheitshardware SoloKeys.

## Enthaltene Algorithmen

Die Auswahl der Algorithmen basiert auf dem Standardisierungsprozess des National Institute of Standards and Technology (NIST) und umfasst:

- Crystals-Kyber
- SPHINCS+
- Falcon
- Crystals-Dilithium

Jeder dieser Algorithmen wurde unter Berücksichtigung der begrenzten Ressourcen und spezifischen Anforderungen der SoloKeys-Hardware implementiert und getestet.

## Repository Struktur

Das Repository ist wie folgt strukturiert:

- `/src`: Quellcode der implementierten Algorithmen und Anpassungen.
- `/firmware`: Angepasste Firmware für SoloKeys, die die PQC-Algorithmen unterstützt.
- `/docs`: Dokumentation der Implementierung, Testergebnisse und Performance-Analyse.

## Voraussetzungen

Um den Code in diesem Repository zu verwenden und auf einem SoloKey zu testen, sind folgende Voraussetzungen nötig:

- SoloKey Hardware
- ARM Toolchain für die Kompilierung
- Solo CLI für das Deployment der Firmware

## Installation und Verwendung

Eine detaillierte Anleitung zur Installation der erforderlichen Software, zur Kompilierung der Firmware und zum Deployment auf einem SoloKey finden Sie im `/docs`-Verzeichnis.

## Beitrag

Feedback und Beiträge zu diesem Projekt sind willkommen. Falls Sie Verbesserungen vorschlagen möchten oder Fehler gefunden haben, erstellen Sie bitte ein Issue oder einen Pull Request.

## Lizenz

Dieses Projekt ist unter der MIT Lizenz lizenziert. Siehe die [LICENSE](LICENSE) Datei für weitere Details.

## Kontakt

Für weitere Fragen oder Anregungen kontaktieren Sie mich bitte über die in meinem GitHub-Profil hinterlegten Kontaktdaten.
