EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:voltage regulators-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LM7810ACT U1
U 1 1 55BCD1B4
P 2250 1400
F 0 "U1" H 2050 1600 40  0000 C CNN
F 1 "7810" H 2250 1600 40  0000 L CNN
F 2 "TO-220" H 2250 1500 30  0000 C CIN
F 3 "" H 2250 1400 60  0000 C CNN
	1    2250 1400
	1    0    0    -1  
$EndComp
$Comp
L LM7810ACT U2
U 1 1 55BCD1EB
P 3600 1400
F 0 "U2" H 3400 1600 40  0000 C CNN
F 1 "7810" H 3600 1600 40  0000 L CNN
F 2 "TO-220" H 3600 1500 30  0000 C CIN
F 3 "" H 3600 1400 60  0000 C CNN
	1    3600 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 1350 4000 1850
Wire Wire Line
	4000 1850 2250 1850
Wire Wire Line
	2250 1850 2250 1650
Wire Wire Line
	3200 1050 3200 1450
Wire Wire Line
	3200 1050 1850 1050
Wire Wire Line
	1850 1050 1850 1350
Wire Wire Line
	4000 1350 4300 1350
Wire Wire Line
	3600 1650 3600 2000
Wire Wire Line
	3600 2000 5250 2000
Wire Wire Line
	4700 2000 4700 1650
$Comp
L GND #PWR4
U 1 1 55BCD31D
P 4250 2150
F 0 "#PWR4" H 4250 1900 50  0001 C CNN
F 1 "GND" H 4250 2000 50  0000 C CNN
F 2 "" H 4250 2150 60  0000 C CNN
F 3 "" H 4250 2150 60  0000 C CNN
	1    4250 2150
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR5
U 1 1 55BCD337
P 5250 1300
F 0 "#PWR5" H 5250 1150 50  0001 C CNN
F 1 "+5V" H 5250 1440 50  0000 C CNN
F 2 "" H 5250 1300 60  0000 C CNN
F 3 "" H 5250 1300 60  0000 C CNN
	1    5250 1300
	1    0    0    -1  
$EndComp
$Comp
L +24V #PWR1
U 1 1 55BCD3E4
P 2150 1000
F 0 "#PWR1" H 2150 850 50  0001 C CNN
F 1 "+24V" H 2150 1140 50  0000 C CNN
F 2 "" H 2150 1000 60  0000 C CNN
F 3 "" H 2150 1000 60  0000 C CNN
	1    2150 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 1350 2650 1350
$Comp
L C C1
U 1 1 55BCD52B
P 2800 1600
F 0 "C1" H 2825 1700 50  0000 L CNN
F 1 "C" H 2825 1500 50  0000 L CNN
F 2 "" H 2838 1450 30  0000 C CNN
F 3 "" H 2800 1600 60  0000 C CNN
	1    2800 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 1450 2800 1350
Connection ~ 2800 1350
Wire Wire Line
	2800 1750 2800 1850
Connection ~ 2800 1850
$Comp
L C C2
U 1 1 55BCD756
P 3200 1600
F 0 "C2" H 3225 1700 50  0000 L CNN
F 1 "C" H 3225 1500 50  0000 L CNN
F 2 "" H 3238 1450 30  0000 C CNN
F 3 "" H 3200 1600 60  0000 C CNN
	1    3200 1600
	1    0    0    -1  
$EndComp
Connection ~ 3200 1350
Wire Wire Line
	3200 1750 3600 1750
Connection ~ 3600 1750
$Comp
L C C3
U 1 1 55BCD85A
P 4250 1600
F 0 "C3" H 4275 1700 50  0000 L CNN
F 1 "C" H 4275 1500 50  0000 L CNN
F 2 "" H 4288 1450 30  0000 C CNN
F 3 "" H 4250 1600 60  0000 C CNN
	1    4250 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 1250 4250 1450
Connection ~ 4250 1350
Wire Wire Line
	4250 1750 4250 2150
Connection ~ 4250 2000
Wire Wire Line
	5250 1350 5100 1350
$Comp
L C C4
U 1 1 55BCD8FE
P 5250 1700
F 0 "C4" H 5275 1800 50  0000 L CNN
F 1 "C" H 5275 1600 50  0000 L CNN
F 2 "" H 5288 1550 30  0000 C CNN
F 3 "" H 5250 1700 60  0000 C CNN
	1    5250 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1300 5250 1550
Wire Wire Line
	5250 2000 5250 1850
Connection ~ 4700 2000
Connection ~ 5250 1350
Wire Wire Line
	2150 1000 2150 1050
Connection ~ 2150 1050
$Comp
L +10V #PWR3
U 1 1 55BD4D92
P 4250 1250
F 0 "#PWR3" H 4250 1350 50  0001 C CNN
F 1 "+10V" H 4250 1400 50  0000 C CNN
F 2 "" H 4250 1250 60  0000 C CNN
F 3 "" H 4250 1250 60  0000 C CNN
	1    4250 1250
	1    0    0    -1  
$EndComp
$Comp
L +20V #PWR2
U 1 1 55BD4E59
P 2950 1350
F 0 "#PWR2" H 2950 1450 50  0001 C CNN
F 1 "+20V" H 2950 1500 50  0000 C CNN
F 2 "" H 2950 1350 60  0000 C CNN
F 3 "" H 2950 1350 60  0000 C CNN
	1    2950 1350
	1    0    0    -1  
$EndComp
$Comp
L LM7805CT U3
U 1 1 55BFFABC
P 4700 1400
F 0 "U3" H 4500 1600 40  0000 C CNN
F 1 "7805" H 4700 1600 40  0000 L CNN
F 2 "TO-220" H 4700 1500 30  0000 C CIN
F 3 "" H 4700 1400 60  0000 C CNN
	1    4700 1400
	1    0    0    -1  
$EndComp
$EndSCHEMATC