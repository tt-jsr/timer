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
LIBS:dk_Embedded-Microcontrollers
LIBS:timer
LIBS:timer-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Kitchen Timer"
Date "2020-01-13"
Rev "v01"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L NanoEvery U2
U 1 1 5E1D4060
P 5250 3500
F 0 "U2" H 5250 4300 60  0000 C CNN
F 1 "NanoEvery" H 5250 2700 60  0000 C CNN
F 2 "" H 5150 4100 60  0001 C CNN
F 3 "" H 5150 4100 60  0001 C CNN
	1    5250 3500
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X14 P1
U 1 1 5E1E7DF1
P 950 3500
F 0 "P1" H 950 4250 50  0000 C CNN
F 1 "Keypad" V 1050 3500 50  0000 C CNN
F 2 "" H 950 3500 50  0000 C CNN
F 3 "" H 950 3500 50  0000 C CNN
	1    950  3500
	-1   0    0    1   
$EndComp
$Comp
L ISD1740 U3
U 1 1 5E1E88C7
P 8450 3600
F 0 "U3" H 8450 4550 47  0000 C CNN
F 1 "ISD1760" H 8450 2950 47  0000 C CNN
F 2 "" H 8550 3600 47  0001 C CNN
F 3 "" H 8550 3600 47  0001 C CNN
	1    8450 3600
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P2
U 1 1 5E1F1C34
P 2750 2350
F 0 "P2" H 2750 2600 50  0000 C CNN
F 1 "Display" V 2850 2350 50  0000 C CNN
F 2 "" H 2750 2350 50  0000 C CNN
F 3 "" H 2750 2350 50  0000 C CNN
	1    2750 2350
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR2
U 1 1 5E1F1F7E
P 3050 2500
F 0 "#PWR2" H 3050 2250 50  0001 C CNN
F 1 "GND" H 3050 2350 50  0000 C CNN
F 2 "" H 3050 2500 50  0000 C CNN
F 3 "" H 3050 2500 50  0000 C CNN
	1    3050 2500
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR1
U 1 1 5E1F2C3E
P 3050 2050
F 0 "#PWR1" H 3050 1900 50  0001 C CNN
F 1 "+5V" H 3050 2190 50  0000 C CNN
F 2 "" H 3050 2050 50  0000 C CNN
F 3 "" H 3050 2050 50  0000 C CNN
	1    3050 2050
	1    0    0    -1  
$EndComp
Text Label 3550 2200 0    47   ~ 0
SDA
Text Label 3500 2300 0    47   ~ 0
SCL
Text Label 4900 2550 0    47   ~ 0
SCLK
Text Label 5750 2800 0    47   ~ 0
MISO
Text Label 5750 2900 0    47   ~ 0
MOSI
Text Label 5750 3000 0    47   ~ 0
SS
Text Label 4300 3600 0    47   ~ 0
SCL
Text Label 4350 3500 0    47   ~ 0
SDA
$Comp
L R R4
U 1 1 5E1FC828
P 9150 3600
F 0 "R4" V 9230 3600 50  0000 C CNN
F 1 "80K" V 9150 3600 50  0000 C CNN
F 2 "" V 9080 3600 50  0000 C CNN
F 3 "" H 9150 3600 50  0000 C CNN
	1    9150 3600
	0    1    1    0   
$EndComp
$Comp
L GND #PWR28
U 1 1 5E1FC8DD
P 9300 3600
F 0 "#PWR28" H 9300 3350 50  0001 C CNN
F 1 "GND" H 9300 3450 50  0000 C CNN
F 2 "" H 9300 3600 50  0000 C CNN
F 3 "" H 9300 3600 50  0000 C CNN
	1    9300 3600
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR25
U 1 1 5E1FC95C
P 7900 2600
F 0 "#PWR25" H 7900 2450 50  0001 C CNN
F 1 "+5V" H 7900 2740 50  0000 C CNN
F 2 "" H 7900 2600 50  0000 C CNN
F 3 "" H 7900 2600 50  0000 C CNN
	1    7900 2600
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR23
U 1 1 5E1FC9C9
P 7500 4050
F 0 "#PWR23" H 7500 3900 50  0001 C CNN
F 1 "+5V" H 7500 4190 50  0000 C CNN
F 2 "" H 7500 4050 50  0000 C CNN
F 3 "" H 7500 4050 50  0000 C CNN
	1    7500 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR26
U 1 1 5E1FCA32
P 7900 4500
F 0 "#PWR26" H 7900 4250 50  0001 C CNN
F 1 "GND" H 7900 4350 50  0000 C CNN
F 2 "" H 7900 4500 50  0000 C CNN
F 3 "" H 7900 4500 50  0000 C CNN
	1    7900 4500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR24
U 1 1 5E1FCA50
P 7700 4400
F 0 "#PWR24" H 7700 4150 50  0001 C CNN
F 1 "GND" H 7700 4250 50  0000 C CNN
F 2 "" H 7700 4400 50  0000 C CNN
F 3 "" H 7700 4400 50  0000 C CNN
	1    7700 4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR30
U 1 1 5E1FCA6E
P 9350 4000
F 0 "#PWR30" H 9350 3750 50  0001 C CNN
F 1 "GND" H 9350 3850 50  0000 C CNN
F 2 "" H 9350 4000 50  0000 C CNN
F 3 "" H 9350 4000 50  0000 C CNN
	1    9350 4000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR29
U 1 1 5E1FCA8C
P 9350 2800
F 0 "#PWR29" H 9350 2550 50  0001 C CNN
F 1 "GND" H 9350 2650 50  0000 C CNN
F 2 "" H 9350 2800 50  0000 C CNN
F 3 "" H 9350 2800 50  0000 C CNN
	1    9350 2800
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5E2069DF
P 7750 2800
F 0 "C4" H 7775 2900 50  0000 L CNN
F 1 ".1uF" H 7775 2700 50  0000 L CNN
F 2 "" H 7788 2650 50  0000 C CNN
F 3 "" H 7750 2800 50  0000 C CNN
	1    7750 2800
	0    1    1    0   
$EndComp
$Comp
L GND #PWR22
U 1 1 5E21BE44
P 7500 2900
F 0 "#PWR22" H 7500 2650 50  0001 C CNN
F 1 "GND" H 7500 2750 50  0000 C CNN
F 2 "" H 7500 2900 50  0000 C CNN
F 3 "" H 7500 2900 50  0000 C CNN
	1    7500 2900
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 5E21BEB9
P 7900 4300
F 0 "C5" H 7925 4400 50  0000 L CNN
F 1 ".1uF" H 7925 4200 50  0000 L CNN
F 2 "" H 7938 4150 50  0000 C CNN
F 3 "" H 7900 4300 50  0000 C CNN
	1    7900 4300
	-1   0    0    1   
$EndComp
$Comp
L C C6
U 1 1 5E21C079
P 9350 3500
F 0 "C6" H 9375 3600 50  0000 L CNN
F 1 ".01uF" H 9375 3400 50  0000 L CNN
F 2 "" H 9388 3350 50  0000 C CNN
F 3 "" H 9350 3500 50  0000 C CNN
	1    9350 3500
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR31
U 1 1 5E21C2BB
P 9600 3500
F 0 "#PWR31" H 9600 3250 50  0001 C CNN
F 1 "GND" H 9600 3350 50  0000 C CNN
F 2 "" H 9600 3500 50  0000 C CNN
F 3 "" H 9600 3500 50  0000 C CNN
	1    9600 3500
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5E21C3BB
P 9600 3850
F 0 "C7" H 9625 3950 50  0000 L CNN
F 1 "4.7uF" H 9625 3750 50  0000 L CNN
F 2 "" H 9638 3700 50  0000 C CNN
F 3 "" H 9600 3850 50  0000 C CNN
	1    9600 3850
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR32
U 1 1 5E21C486
P 9750 3850
F 0 "#PWR32" H 9750 3600 50  0001 C CNN
F 1 "GND" H 9750 3700 50  0000 C CNN
F 2 "" H 9750 3850 50  0000 C CNN
F 3 "" H 9750 3850 50  0000 C CNN
	1    9750 3850
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR27
U 1 1 5E254DF9
P 9150 3450
F 0 "#PWR27" H 9150 3300 50  0001 C CNN
F 1 "+5V" H 9150 3590 50  0000 C CNN
F 2 "" H 9150 3450 50  0000 C CNN
F 3 "" H 9150 3450 50  0000 C CNN
	1    9150 3450
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P3
U 1 1 5E255360
P 6450 4900
F 0 "P3" H 6450 5150 50  0000 C CNN
F 1 "Handset" V 6550 4900 50  0000 C CNN
F 2 "" H 6450 4900 50  0000 C CNN
F 3 "" H 6450 4900 50  0000 C CNN
	1    6450 4900
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR20
U 1 1 5E255EBE
P 7000 3650
F 0 "#PWR20" H 7000 3500 50  0001 C CNN
F 1 "+5V" H 7000 3790 50  0000 C CNN
F 2 "" H 7000 3650 50  0000 C CNN
F 3 "" H 7000 3650 50  0000 C CNN
	1    7000 3650
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5E255EE6
P 7000 3900
F 0 "R1" V 7080 3900 50  0000 C CNN
F 1 "4.7K" V 7000 3900 50  0000 C CNN
F 2 "" V 6930 3900 50  0000 C CNN
F 3 "" H 7000 3900 50  0000 C CNN
	1    7000 3900
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5E255F68
P 7000 4250
F 0 "R2" V 7080 4250 50  0000 C CNN
F 1 "4.7K" V 7000 4250 50  0000 C CNN
F 2 "" V 6930 4250 50  0000 C CNN
F 3 "" H 7000 4250 50  0000 C CNN
	1    7000 4250
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5E255FA8
P 7000 4800
F 0 "R3" V 7080 4800 50  0000 C CNN
F 1 "4.7K" V 7000 4800 50  0000 C CNN
F 2 "" V 6930 4800 50  0000 C CNN
F 3 "" H 7000 4800 50  0000 C CNN
	1    7000 4800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR21
U 1 1 5E255FFF
P 7000 5100
F 0 "#PWR21" H 7000 4850 50  0001 C CNN
F 1 "GND" H 7000 4950 50  0000 C CNN
F 2 "" H 7000 5100 50  0000 C CNN
F 3 "" H 7000 5100 50  0000 C CNN
	1    7000 5100
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5E256026
P 7200 4400
F 0 "C2" H 7225 4500 50  0000 L CNN
F 1 ".1uF" H 7225 4300 50  0000 L CNN
F 2 "" H 7238 4250 50  0000 C CNN
F 3 "" H 7200 4400 50  0000 C CNN
	1    7200 4400
	0    1    1    0   
$EndComp
$Comp
L C C3
U 1 1 5E2560B1
P 7200 4650
F 0 "C3" H 7225 4750 50  0000 L CNN
F 1 ".1uF" H 7225 4550 50  0000 L CNN
F 2 "" H 7238 4500 50  0000 C CNN
F 3 "" H 7200 4650 50  0000 C CNN
	1    7200 4650
	0    1    1    0   
$EndComp
$Comp
L S8050 Q1
U 1 1 5E256E6B
P 9850 4450
F 0 "Q1" H 10050 4525 50  0000 L CNN
F 1 "S8050" H 10050 4450 50  0000 L CNN
F 2 "TO-92" H 10050 4375 50  0000 L CIN
F 3 "" H 9850 4450 50  0000 L CNN
	1    9850 4450
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5E256EBC
P 9400 4700
F 0 "R5" V 9480 4700 50  0000 C CNN
F 1 "390" V 9400 4700 50  0000 C CNN
F 2 "" V 9330 4700 50  0000 C CNN
F 3 "" H 9400 4700 50  0000 C CNN
	1    9400 4700
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 5E256F21
P 9650 4700
F 0 "C8" H 9675 4800 50  0000 L CNN
F 1 ".1uF" H 9675 4600 50  0000 L CNN
F 2 "" H 9688 4550 50  0000 C CNN
F 3 "" H 9650 4700 50  0000 C CNN
	1    9650 4700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR33
U 1 1 5E257195
P 9950 4900
F 0 "#PWR33" H 9950 4650 50  0001 C CNN
F 1 "GND" H 9950 4750 50  0000 C CNN
F 2 "" H 9950 4900 50  0000 C CNN
F 3 "" H 9950 4900 50  0000 C CNN
	1    9950 4900
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P7
U 1 1 5E257545
P 10500 4200
F 0 "P7" H 10500 4350 50  0000 C CNN
F 1 "SPKR" V 10600 4200 50  0000 C CNN
F 2 "" H 10500 4200 50  0000 C CNN
F 3 "" H 10500 4200 50  0000 C CNN
	1    10500 4200
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR34
U 1 1 5E257AF2
P 10300 4150
F 0 "#PWR34" H 10300 4000 50  0001 C CNN
F 1 "+5V" H 10300 4290 50  0000 C CNN
F 2 "" H 10300 4150 50  0000 C CNN
F 3 "" H 10300 4150 50  0000 C CNN
	1    10300 4150
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5E25933B
P 6450 4200
F 0 "C1" H 6475 4300 50  0000 L CNN
F 1 "4.7uF" H 6350 4100 50  0000 L CNN
F 2 "" H 6488 4050 50  0000 C CNN
F 3 "" H 6450 4200 50  0000 C CNN
	1    6450 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR18
U 1 1 5E25949D
P 6450 4450
F 0 "#PWR18" H 6450 4200 50  0001 C CNN
F 1 "GND" H 6450 4300 50  0000 C CNN
F 2 "" H 6450 4450 50  0000 C CNN
F 3 "" H 6450 4450 50  0000 C CNN
	1    6450 4450
	1    0    0    -1  
$EndComp
Text Label 6650 5050 0    51   ~ 0
SPKR+
Text Label 6650 4950 0    51   ~ 0
SPKR-
Text Label 6650 4850 0    51   ~ 0
MIC-
Text Label 6650 4750 0    51   ~ 0
MIC+
$Comp
L CONN_01X02 P5
U 1 1 5E25DBB8
P 5450 1950
F 0 "P5" H 5450 2100 50  0000 C CNN
F 1 "Hook" V 5550 1950 50  0000 C CNN
F 2 "" H 5450 1950 50  0000 C CNN
F 3 "" H 5450 1950 50  0000 C CNN
	1    5450 1950
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X04 P6
U 1 1 5E25E980
P 6250 1950
F 0 "P6" H 6250 2200 50  0000 C CNN
F 1 "Encoder" V 6350 1950 50  0000 C CNN
F 2 "" H 6250 1950 50  0000 C CNN
F 3 "" H 6250 1950 50  0000 C CNN
	1    6250 1950
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR15
U 1 1 5E25EB56
P 5800 2150
F 0 "#PWR15" H 5800 2000 50  0001 C CNN
F 1 "+5V" H 5800 2290 50  0000 C CNN
F 2 "" H 5800 2150 50  0000 C CNN
F 3 "" H 5800 2150 50  0000 C CNN
	1    5800 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR19
U 1 1 5E25EC43
P 6600 2150
F 0 "#PWR19" H 6600 1900 50  0001 C CNN
F 1 "GND" H 6600 2000 50  0000 C CNN
F 2 "" H 6600 2150 50  0000 C CNN
F 3 "" H 6600 2150 50  0000 C CNN
	1    6600 2150
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR11
U 1 1 5E25F4BB
P 4650 4200
F 0 "#PWR11" H 4650 4050 50  0001 C CNN
F 1 "+5V" H 4650 4340 50  0000 C CNN
F 2 "" H 4650 4200 50  0000 C CNN
F 3 "" H 4650 4200 50  0000 C CNN
	1    4650 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR12
U 1 1 5E25F508
P 4700 4400
F 0 "#PWR12" H 4700 4150 50  0001 C CNN
F 1 "GND" H 4700 4250 50  0000 C CNN
F 2 "" H 4700 4400 50  0000 C CNN
F 3 "" H 4700 4400 50  0000 C CNN
	1    4700 4400
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P4
U 1 1 5E25F687
P 4550 1950
F 0 "P4" H 4550 2100 50  0000 C CNN
F 1 "PWR" V 4650 1950 50  0000 C CNN
F 2 "" H 4550 1950 50  0000 C CNN
F 3 "" H 4550 1950 50  0000 C CNN
	1    4550 1950
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR10
U 1 1 5E25F9A8
P 4500 2200
F 0 "#PWR10" H 4500 1950 50  0001 C CNN
F 1 "GND" H 4500 2050 50  0000 C CNN
F 2 "" H 4500 2200 50  0000 C CNN
F 3 "" H 4500 2200 50  0000 C CNN
	1    4500 2200
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR13
U 1 1 5E25F9F8
P 4800 2150
F 0 "#PWR13" H 4800 2000 50  0001 C CNN
F 1 "+5V" H 4800 2290 50  0000 C CNN
F 2 "" H 4800 2150 50  0000 C CNN
F 3 "" H 4800 2150 50  0000 C CNN
	1    4800 2150
	1    0    0    -1  
$EndComp
$Comp
L 74HC595 U1
U 1 1 5E260509
P 2700 3500
F 0 "U1" H 2850 4100 50  0000 C CNN
F 1 "74HC595" H 2700 2900 50  0000 C CNN
F 2 "" H 2700 3500 50  0000 C CNN
F 3 "" H 2700 3500 50  0000 C CNN
	1    2700 3500
	-1   0    0    1   
$EndComp
Wire Wire Line
	7900 3100 6550 3100
Wire Wire Line
	7900 3200 6500 3200
Wire Wire Line
	7900 3300 6600 3300
Wire Wire Line
	6600 3300 6600 2550
Wire Wire Line
	6600 2550 4750 2550
Wire Wire Line
	4750 2550 4750 2800
Wire Wire Line
	7900 3400 6450 3400
Wire Wire Line
	6450 3400 6450 3000
Wire Wire Line
	6450 3000 5750 3000
Wire Wire Line
	2950 2500 3050 2500
Wire Wire Line
	2950 2400 3050 2400
Wire Wire Line
	3050 2400 3050 2050
Wire Wire Line
	4250 3600 4750 3600
Wire Wire Line
	4250 3600 4250 2300
Wire Wire Line
	4250 2300 2950 2300
Wire Wire Line
	4750 3500 4300 3500
Wire Wire Line
	4300 3500 4300 2200
Wire Wire Line
	4300 2200 2950 2200
Wire Wire Line
	6500 3200 6500 2900
Wire Wire Line
	6500 2900 5750 2900
Wire Wire Line
	6550 3100 6550 2800
Wire Wire Line
	6550 2800 5750 2800
Wire Wire Line
	8950 3600 9000 3600
Wire Wire Line
	8950 2800 9350 2800
Wire Wire Line
	7900 3500 7700 3500
Wire Wire Line
	7700 3500 7700 4400
Wire Wire Line
	7600 2800 7500 2800
Wire Wire Line
	7500 2800 7500 2900
Wire Wire Line
	8950 3500 9200 3500
Wire Wire Line
	8950 3800 9100 3800
Wire Wire Line
	9100 3800 9100 3850
Wire Wire Line
	9100 3850 9450 3850
Connection ~ 9150 3500
Wire Wire Line
	9150 3500 9150 3450
Wire Wire Line
	7900 4000 7600 4000
Wire Wire Line
	7600 4000 7600 4950
Wire Wire Line
	7600 4950 6650 4950
Wire Wire Line
	8950 4100 8950 5050
Wire Wire Line
	8950 5050 6650 5050
Wire Wire Line
	7350 3700 7350 4400
Wire Wire Line
	7350 3700 7900 3700
Wire Wire Line
	6450 4050 7000 4050
Wire Wire Line
	7000 4050 7000 4100
Wire Wire Line
	7000 4950 7000 5100
Wire Wire Line
	6650 4400 6650 4850
Wire Wire Line
	6650 4400 7050 4400
Wire Wire Line
	6650 4750 7000 4650
Wire Wire Line
	7000 4650 7050 4650
Connection ~ 7000 4650
Connection ~ 7000 4050
Wire Wire Line
	7900 3800 7400 3800
Wire Wire Line
	7400 3800 7400 4650
Wire Wire Line
	7400 4650 7350 4650
Wire Wire Line
	8950 3900 9050 3900
Wire Wire Line
	9050 3900 9050 4450
Wire Wire Line
	9050 4450 9650 4450
Connection ~ 9400 4450
Connection ~ 9650 4450
Wire Wire Line
	9400 4450 9400 4550
Wire Wire Line
	9650 4450 9650 4550
Wire Wire Line
	9400 4850 9950 4850
Wire Wire Line
	9950 4650 9950 4900
Connection ~ 9950 4850
Connection ~ 9650 4850
Wire Wire Line
	9950 4250 10300 4250
Connection ~ 7000 4400
Wire Wire Line
	5750 3200 6200 3200
Wire Wire Line
	6200 3200 6200 2150
Wire Wire Line
	5750 3300 6300 3300
Wire Wire Line
	6300 3300 6300 2150
$Comp
L GND #PWR5
U 1 1 5E260E32
P 3850 3350
F 0 "#PWR5" H 3850 3100 50  0001 C CNN
F 1 "GND" H 3850 3200 50  0000 C CNN
F 2 "" H 3850 3350 50  0000 C CNN
F 3 "" H 3850 3350 50  0000 C CNN
	1    3850 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3350 3850 3350
Wire Bus Line
	1400 2750 1400 4850
Wire Bus Line
	4550 4850 4550 3200
Entry Wire Line
	1300 2850 1400 2950
Entry Wire Line
	1300 2950 1400 3050
Entry Wire Line
	1300 3050 1400 3150
Entry Wire Line
	1300 3150 1400 3250
Entry Wire Line
	1300 3250 1400 3350
Entry Wire Line
	1300 3350 1400 3450
Entry Wire Line
	1300 3550 1400 3650
Entry Wire Line
	1300 3850 1400 3950
Entry Wire Line
	1300 3950 1400 4050
Entry Wire Line
	1300 4050 1400 4150
Entry Wire Line
	1300 4150 1400 4250
Wire Wire Line
	1150 2850 1300 2850
Wire Wire Line
	1150 2950 1300 2950
Wire Wire Line
	1150 3050 1300 3050
Wire Wire Line
	1150 3150 1300 3150
Wire Wire Line
	1150 3250 1300 3250
Wire Wire Line
	1150 3350 1300 3350
Wire Wire Line
	1150 3550 1300 3550
Wire Wire Line
	1150 3850 1300 3850
Wire Wire Line
	1150 3950 1300 3950
Wire Wire Line
	1150 4050 1300 4050
Wire Wire Line
	1150 4150 1300 4150
Connection ~ 1800 4850
Wire Bus Line
	1800 3250 1800 4850
Entry Wire Line
	1800 3850 1900 3950
Entry Wire Line
	1800 3750 1900 3850
Entry Wire Line
	1800 3650 1900 3750
Entry Wire Line
	1800 3550 1900 3650
Entry Wire Line
	1800 3450 1900 3550
Entry Wire Line
	1800 3350 1900 3450
Entry Wire Line
	1800 3250 1900 3350
Wire Wire Line
	1900 3950 2000 3950
Wire Wire Line
	1900 3850 2000 3850
Wire Wire Line
	1900 3750 2000 3750
Wire Wire Line
	1900 3650 2000 3650
Wire Wire Line
	1900 3550 2000 3550
Wire Wire Line
	1900 3450 2000 3450
Wire Wire Line
	1900 3350 2000 3350
Text Label 1150 2850 0    60   ~ 0
2C3
Text Label 1150 2950 0    60   ~ 0
2C2
Text Label 1150 3050 0    60   ~ 0
R1
Text Label 1150 3150 0    60   ~ 0
R2
Text Label 1150 3250 0    60   ~ 0
2C1
Text Label 1150 3350 0    60   ~ 0
R3
Text Label 1150 3550 0    60   ~ 0
1C4
Text Label 1150 3850 0    60   ~ 0
1C2
Text Label 1150 3950 0    60   ~ 0
R4
Text Label 1150 4050 0    60   ~ 0
1C3
Text Label 1150 4150 0    60   ~ 0
1C1
Text Label 1150 3450 0    60   ~ 0
Vx
Text Label 1150 3650 0    60   ~ 0
VS
Text Label 1150 3750 0    60   ~ 0
LED
Text Label 1900 3950 0    60   ~ 0
1C1
Text Label 1900 3850 0    60   ~ 0
1C2
Text Label 1900 3750 0    60   ~ 0
1C3
Text Label 1900 3650 0    60   ~ 0
1C4
Text Label 1900 3550 0    60   ~ 0
2C1
Text Label 1900 3450 0    60   ~ 0
2C2
Text Label 1900 3350 0    60   ~ 0
2C3
Entry Wire Line
	4550 3300 4650 3400
Entry Wire Line
	4550 3600 4650 3700
Entry Wire Line
	4550 3700 4650 3800
Wire Wire Line
	4050 3300 4750 3300
Wire Wire Line
	4200 3400 4750 3400
Wire Wire Line
	4300 3700 4750 3700
Wire Wire Line
	4450 3800 4750 3800
Text Label 4650 3300 0    60   ~ 0
R1
Text Label 4650 3400 0    60   ~ 0
R2
Text Label 4650 3700 0    60   ~ 0
R3
Text Label 4650 3800 0    60   ~ 0
R4
Text Label 3400 3450 0    60   ~ 0
RCLK
Text Label 3400 3950 0    60   ~ 0
SER
Text Label 3400 3650 0    60   ~ 0
~SRCLR
Text Label 3400 3750 0    60   ~ 0
SRCLK
Text Label 4750 3200 2    60   ~ 0
SER
Text Label 4750 3100 2    60   ~ 0
SRCLK
Text Label 5750 3500 0    60   ~ 0
RCLK
Wire Bus Line
	1400 4850 4550 4850
Entry Wire Line
	4550 3200 4650 3300
Text Label 3400 3350 0    60   ~ 0
~OE
Wire Wire Line
	5750 3100 6100 3100
Wire Wire Line
	6100 3100 6100 2300
Wire Wire Line
	6100 2300 5500 2300
Wire Wire Line
	5500 2300 5500 2150
NoConn ~ 2000 3050
NoConn ~ 2000 3250
NoConn ~ 1150 3450
NoConn ~ 1150 3650
NoConn ~ 1150 3750
NoConn ~ 4750 3900
NoConn ~ 4750 4000
NoConn ~ 5750 4200
NoConn ~ 5750 4100
NoConn ~ 5750 4000
NoConn ~ 5750 3900
NoConn ~ 5750 3600
NoConn ~ 4750 3000
NoConn ~ 8950 2900
NoConn ~ 8950 3000
NoConn ~ 8950 3100
NoConn ~ 8950 3200
NoConn ~ 8950 3300
NoConn ~ 8950 3400
NoConn ~ 8950 3700
NoConn ~ 7900 3600
NoConn ~ 7900 3900
NoConn ~ 7900 3000
NoConn ~ 7900 2900
$Comp
L +5V #PWR3
U 1 1 5E27D47A
P 3200 4200
F 0 "#PWR3" H 3200 4050 50  0001 C CNN
F 1 "+5V" H 3200 4340 50  0000 C CNN
F 2 "" H 3200 4200 50  0000 C CNN
F 3 "" H 3200 4200 50  0000 C CNN
	1    3200 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4200 3000 4200
Wire Wire Line
	3000 4200 3000 4050
$Comp
L GND #PWR4
U 1 1 5E27DD28
P 3550 2900
F 0 "#PWR4" H 3550 2650 50  0001 C CNN
F 1 "GND" H 3550 2750 50  0000 C CNN
F 2 "" H 3550 2900 50  0000 C CNN
F 3 "" H 3550 2900 50  0000 C CNN
	1    3550 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 4000 9350 4000
NoConn ~ 4750 2900
Wire Wire Line
	6450 4350 6450 4450
Wire Wire Line
	7900 4450 7900 4500
Wire Wire Line
	7000 3750 7000 3650
Wire Wire Line
	4700 4400 4700 4100
Wire Wire Line
	4700 4100 4750 4100
Wire Wire Line
	4750 4200 4650 4200
Connection ~ 7900 2800
Wire Wire Line
	7900 2800 7900 2600
Wire Wire Line
	6600 2150 6400 2150
Wire Wire Line
	7900 4100 7500 4100
Wire Wire Line
	7500 4100 7500 4050
Wire Wire Line
	7900 4150 7900 4100
$Comp
L Buzzer BZ1
U 1 1 5E27DCA2
P 5950 5150
F 0 "BZ1" H 6100 5200 50  0000 L CNN
F 1 "Buzzer" H 6100 5100 50  0000 L CNN
F 2 "" V 5925 5250 50  0000 C CNN
F 3 "" V 5925 5250 50  0000 C CNN
	1    5950 5150
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 5E2CB212
P 4200 3650
F 0 "R7" V 4280 3650 50  0000 C CNN
F 1 "10K" V 4200 3650 50  0000 C CNN
F 2 "" V 4130 3650 50  0000 C CNN
F 3 "" H 4200 3650 50  0000 C CNN
	1    4200 3650
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 5E2CB28D
P 4300 4250
F 0 "R8" V 4380 4250 50  0000 C CNN
F 1 "10K" V 4300 4250 50  0000 C CNN
F 2 "" V 4230 4250 50  0000 C CNN
F 3 "" H 4300 4250 50  0000 C CNN
	1    4300 4250
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 5E2CB2FA
P 4450 4250
F 0 "R9" V 4530 4250 50  0000 C CNN
F 1 "10K" V 4450 4250 50  0000 C CNN
F 2 "" V 4380 4250 50  0000 C CNN
F 3 "" H 4450 4250 50  0000 C CNN
	1    4450 4250
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5E2CB42C
P 4050 3450
F 0 "R6" V 4130 3450 50  0000 C CNN
F 1 "10K" V 4050 3450 50  0000 C CNN
F 2 "" V 3980 3450 50  0000 C CNN
F 3 "" H 4050 3450 50  0000 C CNN
	1    4050 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 3800 4450 4100
Wire Wire Line
	4300 3700 4300 4100
Wire Wire Line
	4200 3400 4200 3500
$Comp
L GND #PWR6
U 1 1 5E2CB89E
P 4050 3750
F 0 "#PWR6" H 4050 3500 50  0001 C CNN
F 1 "GND" H 4050 3600 50  0000 C CNN
F 2 "" H 4050 3750 50  0000 C CNN
F 3 "" H 4050 3750 50  0000 C CNN
	1    4050 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR7
U 1 1 5E2CB900
P 4200 3950
F 0 "#PWR7" H 4200 3700 50  0001 C CNN
F 1 "GND" H 4200 3800 50  0000 C CNN
F 2 "" H 4200 3950 50  0000 C CNN
F 3 "" H 4200 3950 50  0000 C CNN
	1    4200 3950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR8
U 1 1 5E2CB962
P 4300 4550
F 0 "#PWR8" H 4300 4300 50  0001 C CNN
F 1 "GND" H 4300 4400 50  0000 C CNN
F 2 "" H 4300 4550 50  0000 C CNN
F 3 "" H 4300 4550 50  0000 C CNN
	1    4300 4550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR9
U 1 1 5E2CB9C4
P 4450 4550
F 0 "#PWR9" H 4450 4300 50  0001 C CNN
F 1 "GND" H 4450 4400 50  0000 C CNN
F 2 "" H 4450 4550 50  0000 C CNN
F 3 "" H 4450 4550 50  0000 C CNN
	1    4450 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3600 4050 3750
Wire Wire Line
	4200 3800 4200 3950
$Comp
L PN2222A Q2
U 1 1 5E30FD12
P 5750 5550
F 0 "Q2" H 5950 5625 50  0000 L CNN
F 1 "PN2222A" H 5950 5550 50  0000 L CNN
F 2 "" H 5950 5475 50  0000 L CIN
F 3 "" H 5750 5550 50  0000 L CNN
	1    5750 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR17
U 1 1 5E31038E
P 5850 5850
F 0 "#PWR17" H 5850 5600 50  0001 C CNN
F 1 "GND" H 5850 5700 50  0000 C CNN
F 2 "" H 5850 5850 50  0000 C CNN
F 3 "" H 5850 5850 50  0000 C CNN
	1    5850 5850
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR16
U 1 1 5E31063C
P 5850 4550
F 0 "#PWR16" H 5850 4400 50  0001 C CNN
F 1 "+5V" H 5850 4690 50  0000 C CNN
F 2 "" H 5850 4550 50  0000 C CNN
F 3 "" H 5850 4550 50  0000 C CNN
	1    5850 4550
	1    0    0    -1  
$EndComp
$Comp
L R R10
U 1 1 5E3106F5
P 5850 4800
F 0 "R10" H 5930 4800 50  0000 C CNN
F 1 "100" V 5850 4800 50  0000 C CNN
F 2 "" V 5780 4800 50  0000 C CNN
F 3 "" H 5850 4800 50  0000 C CNN
	1    5850 4800
	-1   0    0    1   
$EndComp
NoConn ~ 5750 3700
Text Label 5750 3400 0    60   ~ 0
~SRCLR
Wire Wire Line
	4300 4400 4300 4550
Wire Wire Line
	4450 4550 4450 4400
Wire Wire Line
	9500 3500 9600 3500
Wire Wire Line
	5850 4550 5850 4650
Wire Wire Line
	5850 4950 5850 5050
Wire Wire Line
	5850 5250 5850 5350
Wire Wire Line
	5850 5750 5850 5850
Wire Wire Line
	5750 3800 5900 3800
Wire Wire Line
	5900 3800 5900 4350
Wire Wire Line
	5900 4350 5550 4350
Wire Wire Line
	5550 4350 5550 5550
Wire Wire Line
	4600 2150 4800 2150
Wire Wire Line
	3000 2950 3000 2750
Wire Wire Line
	3000 2750 3550 2750
Wire Wire Line
	3550 2750 3550 2900
Wire Wire Line
	4500 2150 4500 2200
Wire Wire Line
	5800 2150 6100 2150
$Comp
L GND #PWR14
U 1 1 5E66D489
P 5400 2150
F 0 "#PWR14" H 5400 1900 50  0001 C CNN
F 1 "GND" H 5400 2000 50  0000 C CNN
F 2 "" H 5400 2150 50  0000 C CNN
F 3 "" H 5400 2150 50  0000 C CNN
	1    5400 2150
	1    0    0    -1  
$EndComp
$Comp
L D_Small D1
U 1 1 5EC37F50
P 5700 5150
F 0 "D1" H 5650 5230 50  0000 L CNN
F 1 "D_Small" H 5550 5070 50  0000 L CNN
F 2 "" V 5700 5150 50  0000 C CNN
F 3 "" V 5700 5150 50  0000 C CNN
	1    5700 5150
	0    1    1    0   
$EndComp
Wire Wire Line
	5850 5050 5700 5050
Wire Wire Line
	5700 5250 5850 5250
$EndSCHEMATC
