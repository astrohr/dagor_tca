DR. JOHANNES HEIDENHAIN GmbH
Datei: Readme_de.txt f�r EIB 700 driver CD
Datum: 11.02.2016



EIB 700 Software
================

1. Inhalt
---------

/EIB_74x
   /doc
      /de                Dokumentation und Lizenzbedingungen in Deutsch
      /en                Dokumentation und Lizenzbedingungen in Englisch

   /labview
      /examples          Beispiele
      /subvi             allgemeine VIs
      /typedefs          Typ-Definitionen f�r die VIs
      /VIs               LabView VIs f�r die EIB 700
      eib7_top.mnu       Funktionspalette aller VIs f�r die EIB 700

   /linux
      /examples          Beispiele
         /c_cpp          Beispiele f�r Programmiersprache C und C++
      /include           Headerdatei f�r die Funktionen der EIB 700
      /lib               Funktions-Bibliothek f�r die EIB 700 (Linux x32)
      /lib64             Funktions-Bibliothek f�r die EIB 700 (Linux x64)
      /tools             Hilfsprogramme f�r die EIB 700

   /windows
      /app               Beispielapplikation
      /bin               DLL f�r die EIB 700 (Windows x32)
      /bin64             DLL f�r die EIB 700 (Windows x64)
      /examples          Beispiele
         /c_cpp          Beispiele f�r Programmiersprache C und C++
      /include           Headerdatei f�r die Funktionen der EIB 700
      /lib               Bibliothek f�r die EIB 700 (Windows x32)
      /lib64             Bibliothek f�r die EIB 700 (Windows x64)
      /tools             Hilfsprogramme f�r die EIB 700



2. Anforderungen
----------------

LabView:
	Version 8.0 oder h�her

Linux:
	Bibliotheken: pthread
	Die SO-Bibliothek wurde erstellt mit
	  - Kernel 3.1.0
	  - gcc 4.6.2
	  - libc 2.14.1

Windows:
	Betriebssystem: Windows 2000, Windows XP, Windows Vista, Windows 7, 
                        Windows 8.1, Windows 10
	Bibliotheken: user32.lib, ws2_32.lib, kernel32.lib



3. Erstinbetriebnahme
---------------------

Software installieren
---------------------
Die Treiberdateien (eib7.dll/eib7_64.dll bzw. libeib7.so/libeib7_64.so) m�ssen,
wie im Handbuch beschrieben, von der CD in das Systemverzeichnis kopiert 
werden. Anschlie�end muss der Inhalt des Ordners "EIB_741_742" von der CD in ein
beliebiges Verzeichnis auf einer lokalen Festplatte kopiert werden. Danach sind
die Beispiele und Tools verwendbar.

Netzwerkverbindung zur EIB 700 herstellen
-----------------------------------------
Bei der ersten Inbetriebnahme muss die EIB 700 mittels einer Punkt-zu-Punkt-
Netzwerkverbindung mit dem Rechner verbunden und konfiguriert werden. Dazu wird 
vor dem Anschlie�en des Netzwerkkabels auf dem Rechner folgende 
TCP/IP-Konfiguration f�r die verwendete Netzwerkkarte eingestellt:

- Feste IP-Adresse verwenden (nicht automatisch beziehen)
- IP-Adresse:   192.168.1.1
- Subnetzmaske: 255.255.255.0

Wurden diese Einstellungen vorgenommen, muss das Netzwerkkabel am Rechner und 
an der EIB 700 eingesteckt und die EIB 700 eingeschaltet werden. Sobald 
die STATUS-LED leuchtet, ist die EIB 700 betriebsbereit.

Testen der Verbindung
---------------------
Es wird empfohlen, als erstes mit dem ping-Befehl (DOS-Konsole bzw.Linux-Shell) 
zu pr�fen, ob die EIB 700 per Netzwerk ansprechbar ist. Dies sollte der Fall 
sein, wenn alle beschriebenen Schritte ordnungsgem�� durchgef�hrt wurden.

Befehl:
ping 192.168.1.2

Auf der Konsole wird gemeldet, ob die EIB 700 ansprechbar ist oder nicht. 
Im zuerst genannten Fall k�nnen die Tools sowie die Beispielprogramme verwendet 
werden. Bei einigen dieser Programme muss die IP-Adresse der EIB 700 
eingegeben werden. In Auslieferungszustand der EIB 700 hat sie den 
Wert 192.168.1.2.

Falls die Netzwerkparameter der EIB 700 bereits ver�ndert wurden, ist die 
EIB 700 nicht unter der Standard-IP-Adresse 192.168.1.2 ansprechbar. In diesem 
Fall wird ein Bootvorgang mit Standard-Netzwerkeinstellungen empfohlen.
(Siehe Installationsanleitung, Abschnitt "Reset-Modi")

Konfiguration der Netzwerkparameter der EIB 700
-----------------------------------------------
Dies erfolgt mit dem Tool "networksettings" (siehe unten).
Die EIB 700 kann damit f�r eine individuelle Netzwerkumgebung konfiguriert 
werden.

Ausf�hren der Beispiele und Tools
---------------------------------
Die mitgelieferten Programme f�r Windows und Linux werden direkt von der 
Konsole (Shell) ausgef�hrt. Die LabView-Programme liegen als VI's vor.



4. Beispiele
------------

C/C++ Beispiele f�r Window und Linux
------------------------------------

AuxAxis:
--------
Die Positionswerte des Messger�ts an Achse 1 k�nnen durch zwei verschiedenen
Triggerquellen gespeichert werden. Dazu wird die Hilfsachse verwendet.
An der Achse 1 muss ein Messger�t mit Inkrementalsignalen angeschlossen
sein. Die Positionswerte werden periodisch �ber den internen Timer Trigger
gespeichert. Parallel dazu kann die Position mit einem zweiten asynchronen
Triggersignal gespeichert werden.

easy:
-----
Die Achse 1 der EIB 700 wird f�r Messger�te mit Inkrementalsignalen
initialisiert. Von dieser Achse wird der Positionswert und das Statuswort
abgefragt und angezeigt. Die EIB 700 muss die IP-Adresse 192.168.1.2
besitzen.

eib7info:
---------
Die Ger�tedaten der EIB 700 werden ausgelesen. Hierzu z�hlen die
Seriennummer und MAC-Adresse sowie die Netzwerk-Parameter. Alle Daten
werden auf der Konsole ausgegeben. Die IP-Adresse der EIB 700 wird 
nach dem Programmstart abgefragt.

IntervalCounter:
----------------
Das Messger�t an Achse 1 generiert Triggerimpulse abh�ngig von der
aktuellen Position. Die Position der Achse 2 wird durch diese Triggerquelle
gespeichert und angezeigt.

MatchIncrEnDat:
---------------
Die absolute und die inkrementale Position eines EnDat01 Messger�ts wird
zeitgleich ausgelesen und angezeigt.

polling:
--------
Der Positionswert und das Statuswort einer Achse der EIB 700 wird ausgelesen
und angezeigt. Dazu wird die EIB 700 im Polling Modus betrieben. Die Achse,
von der die Positionswerte gelesen werden, ist w�hlbar. Ebenso wird der Typ
der Messger�teschnittstelle vom Programm abgefragt. Der Anwender kann
zwischen Inkremental, EnDat 2.1 und EnDat 2.2 w�hlen. Die Positionswerte
werden in einer Schleife fortlaufend ausgelesen. Die IP-Adresse der EIB 700
muss nach dem Programmstart eingegeben werden.

PulsCounter:
------------
Die EIB 700 wird im Recording Modus betrieben und eine w�hlbare Achse
f�r inkrementale Messger�te konfiguriert. Die Triggerung erfolgt �ber den
internen Timer Trigger. Die Triggerimpulse werden per Software Trigger
gestartet und durch den PulsCounter auf eine bestimmte Anzahl begrenzt.

softrealtime:
-------------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden f�r eine Achse der EIB 700 ausgelesen und angezeigt. Die EIB 700 
wird dazu im Soft Realtime Modus betrieben. Die Daten werden f�r vier Achsen
�bertragen. Der Anwender kann w�hlen, welche der vier Achsen angezeigt wird.
Weiterhin kann der Anwender zwischen externer und interner Triggerung w�hlen.
Die Positionswerte werden in einer Schleife fortlaufend ausgelesen. Die
IP-Adresse der EIB 700 wird nach dem Programmstart abgefragt.

SoftrealtimeEnDat:
------------------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden f�r eine Achse der EIB 700 ausgelesen und angezeigt. Die EIB 700 
wird dazu im Soft Realtime Modus betrieben. Die Achse wird f�r EnDat 2.2 Messger�te
konfiguriert. Der Anwender kann w�hlen, welche der vier Achsen angezeigt wird.
Weiterhin kann der Anwender zwischen externer und interner Triggerung w�hlen.
Die Positionswerte werden in einer Schleife fortlaufend ausgelesen. Die
IP-Adresse der EIB 700 wird nach dem Programmstart abgefragt.

streaming:
----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden f�r eine Achse der EIB 700 ausgelesen und angezeigt. Die EIB 700 
wird dazu im Streaming Modus betrieben. Die Daten werden f�r vier Achsen
�bertragen. Der Anwender kann w�hlen, welche der vier Achsen angezeigt wird.
Weiterhin kann der Anwender zwischen externer und interner Triggerung w�hlen.
Die Positionswerte werden in einer Schleife fortlaufend ausgelesen. Die
IP-Adresse der EIB 700 wird nach dem Programmstart abgefragt.

recording:
----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden f�r eine w�hlbare Achse in der EIB 700 aufgezeichnet. Die EIB 700 
wird dazu im Recording Modus betrieben. Das Datenpaket wird so konfiguriert, dass
nur die Daten f�r die entsprechende Achse aufgezeichnet werden. Nach der
Aufzeichnungsphase werden die Daten zum PC �bertragen und angezeigt. Die
IP-Adresse der EIB 700 wird nach dem Programmstart abgefragt. Der Anwender
kann zwischen externer und interner Triggerung w�hlen.

TriggerIO:
-----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden f�r eine Achse der EIB 700 ausgelesen und angezeigt. Die EIB 700 
wird dazu im Soft Realtime Modus betrieben. Alle Achsen werden f�r inkrementale
Messger�te konfiguriert. Der Anwender kann w�hlen, welche der vier Achsen
angezeigt wird. Die Triggerung erfolgt �ber den internen Timer Trigger.
Dar�ber hinaus wird das Triggersignal am Triggerausgang 1 ausgegeben. Die
IP-Adresse der EIB 700 wird nach dem Programmstart abgefragt.

TriggerRI:
----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden f�r eine w�hlbare Achse der EIB 700 ausgelesen und angezeigt. Die
EIB 700 wird dazu im Soft Realtime Modus betrieben. Die Triggerung erfolgt
�ber die Referenzmarke des Messger�ts. Die IP-Adresse der EIB 700 wird nach
dem Programmstart abgefragt.


LabView Beispiele
-----------------

/EIB7Info:
----------

AuxAxis:
--------
Die Positionswerte des Messger�ts an Achse 1 k�nnen durch zwei verschiedenen
Triggerquellen gespeichert werden. Dazu wird die Hilfsachse verwendet.
An der Achse 1 muss ein Messger�t mit Inkrementalsignalen angeschlossen
sein. Die Positionswerte werden periodisch �ber den internen Timer Trigger
gespeichert. Parallel dazu kann die Position mit einem zweiten asynchronen
Triggersignal, dem Software Trigger 1, gespeichert werden. Beide
Positionswerte werden angezeigt.

/EIB7Info:
----------

GetConnInfo:
------------
Die Daten f�r alle derzeit ge�ffneten Verbindungen zur EIB 700 werden
ausgegeben. Hierzu z�hlen die IP-Adresse und Port-Nummer f�r die
TCP-Verbindungen jeweils auf der Seite der EIB 700 und des Hosts.

GetNetworkSettings:
-------------------
Die Einstellungen f�r die Netzwerkschnittstelle der EIB 700 werden 
ausgelesen und angezeigt.

SetNetworkSettings:
-------------------
Die Einstellungen f�r die Netzwerkschnittstelle der EIB 700 k�nnen ver�ndert
werden. Bei der Ausf�hrung dieses Programms werden die Einstellungen auf die
angegebenen Werte in den Eingabefeldern ge�ndert. Diese sind nach dem
n�chsten Reset wirksam.

/Polling:
---------

PollEncoderData:
----------------
F�r eine Achse der EIB 700 wird der Trigger Counter, der Positionswert,
das Statuswort und der Timestamp ausgegeben. Die Achse wird f�r inkrementale
Messger�te konfiguriert. Pro Programmdurchlauf wird ein Postionswert
abgefragt. Die EIB 700 wird im Polling Modus betrieben.

PollIncrEnDatPostition:
-----------------------
Eine Achse der EIB 700 wird f�r EnDat 01 konfiguriert. Die EnDat- und die
Inkrementalposition werden gleichzeitig ausgelesen.

PollPosition:
-------------
Die Positionswerte und das Statuswort der Achsen 1-4 werden zyklisch
abgerufen. Die Achsen k�nnen individuell f�r inkrementale Messger�te,
oder f�r EnDat 2.1 bzw. EnDat 2.2 konfiguriert werden. Die EIB 700 wird
im Polling Modus betrieben.

/SoftRealtime:
--------------

SoftRealtime:
-------------
Der Positionswert, das Statuswort, der Timestamp und die Referenzpositionen
werden f�r die Achsen 1 und 2 der EIB 700 ausgelesen und angezeigt. Die
EIB 700 wird dazu im Soft Realtime Modus betrieben. Es kann zwischen
externer und interner Triggerung gew�hlt werden. Bei interner Triggerung
ist das Intervall des Timer Triggers einstellbar. Die Achsen werden f�r
inkrementale Messger�te konfiguriert und die Positionswerte in einer Schleife
fortlaufend ausgelesen. Zus�tzlich wird zu Beginn die Speicherung der
Referenzmarke aktiviert.

SoftRealtime_EnDat:
-------------------
Die Achse 1 wird f�r ein inkrementales Messger�t und die Achse 2 f�r ein
EnDat 2.2 Messger�t konfiguriert. F�r das inkrementale Messger�t werden
zus�tzlich zwei Referenzmarken gespeichert und deren Werte �bertragen.
Bei dem EnDat Messger�t werden zyklisch verschiedene EnDat Zusatzinformationen
abgefragt. Die EIB 700 wird im Soft Realtime Modus betrieben. Es kann 
zwischen externer und interner Triggerung gew�hlt werden. Bei interner 
Triggerung ist das Intervall des Timer Triggers einstellbar.

/Streaming:
-----------

Streaming:
----------
Der Positionswert und das Statuswort werden f�r die Achsen 1 und 2 der
EIB 700 ausgelesen und angezeigt. Die EIB 700 wird dazu im Streaming 
Modus betrieben. Es kann zwischen externer und interner Triggerung gew�hlt 
werden. Bei interner Triggerung ist das Intervall des Timer Triggers 
einstellbar. Die Achsen werden f�r inkrementale Messger�te konfiguriert und die
Positionswerte in einer Schleife fortlaufend ausgelesen. Parallel dazu wird
der FIFO f�r die Zwischenspeicherung der Daten in der EIB �berwacht und
dessen Status angezeigt.

/Recording:
-----------

Recording:
----------
Der Positionswert, das Statuswort und der Timestamp werden f�r die Achsen
1 und 2 in der EIB 700 gespeichert. Die EIB 700 wird dazu im Recording 
Modus betrieben. Es kann zwischen externer und interner Triggerung gew�hlt 
werden. Bei interner Triggerung ist das Intervall des Timer Triggers einstellbar.
Parallel dazu wird der Speicher in der EIB 700 �berwacht und angezeigt, wie
viele Samples bereits gespeichert wurden. Die Aufzeichnungsphase kann vom
Anwender beendet werden. Anschlie�end werden die Daten zum PC �bertragen
und der Verlauf der Positionswerte in einem Diagramm graphisch dargestellt.

/Trigger:
---------

ExtTriggerOutput:
-----------------
Der Positionswert, das Statuswort und der Timestamp werden f�r Achse 1 und 2
der EIB 700 ausgelesen und angezeigt. Die EIB 700 wird dazu im 
Soft Realtime Modus betrieben. Alle Achsen werden f�r inkrementale Messger�te 
konfiguriert. Die Triggerung erfolgt �ber den internen Timer Trigger. Dar�ber 
hinaus wird das Triggersignal des internen Timers am Triggerausgang 1 ausgegeben.

IntervalCounter:
----------------
Das Messger�t an Achse 1 generiert Triggerimpulse abh�ngig von der
aktuellen Position. Die Position der Achse 2 wird durch diese Triggerquelle
gespeichert und angezeigt. Der Abstand der Triggerimpulse ist einstellbar.

PulsCounter:
------------
Die EIB 700 wird im Recording Modus betrieben und die Achsen 1 und 2
f�r inkrementale Messger�te konfiguriert. Die Triggerung erfolgt �ber den
internen Timer Trigger. Der erste Triggerimpuls wird durch den
Software Trigger freigegeben und die Anzahl der Triggerimpulse durch den
PulsCounter auf eine bestimmte Anzahl begrenzt. Die aufgezeichneten
Positionswerte werden nach der Messung in einem Diagramm dargestellt.

RI_Trigger:
-----------
Der Positionswert, das Statuswort, der Timestamp und der Trigger Counter
werden f�r eine Achse der EIB 700 ausgelesen und angezeigt. Die 
EIB 700 wird dazu im Soft Realtime Modus betrieben. Die Triggerung 
erfolgt �ber die Referenzmarke des Messger�ts.



5. Tools
--------

networksettings:
----------------
Die Einstellungen f�r die Netzwerkschnittstelle der EIB 700 k�nnen
ver�ndert werden. Hierzu z�hlen die IP-Adresse, die Netzwerkmaske,
das Standard Gateway, die Einstellungen f�r den DHCP Client und
der Hostname.



6. Kontakt
----------

HEIDENHAIN
------------------------------------------
DR. JOHANNES HEIDENHAIN GmbH
Dr.-Johannes-Heidenhain-Stra�e 5
83301 Traunreut, Germany

Tel:    +49 8669 31-0
E-Mail: info@heidenhain.de
------------------------------------------
Messsysteme (EIB700 Support)
Tel:    +49 8669 31-3104
E-Mail: service.ms-support@heidenhain.de
------------------------------------------
www.heidenhain.de

