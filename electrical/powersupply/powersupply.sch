EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "24V to 5V DC Power Supply"
Date "2022-02-18"
Rev "1"
Comp "Oregon State University"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Capstone:TPS54232 U1
U 1 1 618FEBE4
P 5650 3800
F 0 "U1" H 5650 4267 50  0000 C CNN
F 1 "TPS54232" H 5650 4176 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 5650 3400 50  0001 C CNN
F 3 "" H 5650 3450 50  0001 C CNN
F 4 "TPS54232D" H 5650 3800 50  0001 C CNN "MFN"
	1    5650 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R1
U 1 1 619051F0
P 4550 3900
F 0 "R1" H 4618 3946 50  0000 L CNN
F 1 "169k" H 4618 3855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 4590 3890 50  0001 C CNN
F 3 "~" H 4550 3900 50  0001 C CNN
F 4 "RMCF0805FT169K" H 4550 3900 50  0001 C CNN "MFN"
	1    4550 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R2
U 1 1 61906D26
P 4550 4450
F 0 "R2" H 4618 4496 50  0000 L CNN
F 1 "16.9k" H 4618 4405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 4590 4440 50  0001 C CNN
F 3 "~" H 4550 4450 50  0001 C CNN
F 4 "RMCF0805FT16K9" H 4550 4450 50  0001 C CNN "MFN"
	1    4550 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R3
U 1 1 6190A6A4
P 6350 4050
F 0 "R3" H 6418 4096 50  0000 L CNN
F 1 "30.1k" H 6418 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 6390 4040 50  0001 C CNN
F 3 "~" H 6350 4050 50  0001 C CNN
F 4 "RMCF0805FT30K1" H 6350 4050 50  0001 C CNN "MFN"
	1    6350 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R4
U 1 1 6190B0D6
P 7450 3400
F 0 "R4" H 7518 3446 50  0000 L CNN
F 1 "10.2k" H 7518 3355 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 7490 3390 50  0001 C CNN
F 3 "~" H 7450 3400 50  0001 C CNN
F 4 "RMCF0805FT10K2" H 7450 3400 50  0001 C CNN "MFN"
	1    7450 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R5
U 1 1 6190B824
P 7450 4000
F 0 "R5" H 7518 4046 50  0000 L CNN
F 1 "1.96k" H 7518 3955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 7490 3990 50  0001 C CNN
F 3 "~" H 7450 4000 50  0001 C CNN
F 4 "RMCF0805FT1K96" H 7450 4000 50  0001 C CNN "MFN"
	1    7450 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 6190C13C
P 3700 3900
F 0 "C2" H 3815 3946 50  0000 L CNN
F 1 "10uF" H 3815 3855 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric_Pad1.33x2.70mm_HandSolder" H 3738 3750 50  0001 C CNN
F 3 "~" H 3700 3900 50  0001 C CNN
F 4 "CL32B106KBJZW6E" H 3700 3900 50  0001 C CNN "MFN"
	1    3700 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 6190E1F8
P 5000 3450
F 0 "C4" H 5115 3496 50  0000 L CNN
F 1 "0.1uF" H 5115 3405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5038 3300 50  0001 C CNN
F 3 "~" H 5000 3450 50  0001 C CNN
F 4 "CL10B104KO8NFNC" H 5000 3450 50  0001 C CNN "MFN"
	1    5000 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 61911434
P 5100 4200
F 0 "C5" H 5215 4246 50  0000 L CNN
F 1 "8200pF" H 5215 4155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 5138 4050 50  0001 C CNN
F 3 "~" H 5100 4200 50  0001 C CNN
F 4 "CL21B822KBANNNC" H 5100 4200 50  0001 C CNN "MFN"
	1    5100 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 619120BB
P 6350 4350
F 0 "C6" H 6465 4396 50  0000 L CNN
F 1 "1800pF" H 6465 4305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 6388 4200 50  0001 C CNN
F 3 "~" H 6350 4350 50  0001 C CNN
F 4 "CL10C182JB8NNNC" H 6350 4350 50  0001 C CNN "MFN"
	1    6350 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 619128BF
P 6750 4250
F 0 "C8" H 6865 4296 50  0000 L CNN
F 1 "6.8pF" H 6865 4205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 6788 4100 50  0001 C CNN
F 3 "~" H 6750 4250 50  0001 C CNN
F 4 "CL10C6R8CB8NNNC" H 6750 4250 50  0001 C CNN "MFN"
	1    6750 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 61912EAC
P 6650 3400
F 0 "C7" H 6765 3446 50  0000 L CNN
F 1 "10uF" H 6765 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 6688 3250 50  0001 C CNN
F 3 "~" H 6650 3400 50  0001 C CNN
F 4 "CL21A106KAYNNNG" H 6650 3400 50  0001 C CNN "MFN"
	1    6650 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:L L1
U 1 1 61913708
P 6500 3250
F 0 "L1" V 6690 3250 50  0000 C CNN
F 1 "15uH" V 6599 3250 50  0000 C CNN
F 2 "Inductor_SMD:L_7.3x7.3_H4.5" H 6500 3250 50  0001 C CNN
F 3 "~" H 6500 3250 50  0001 C CNN
F 4 "CDRH74NP-150MC-B" H 6500 3250 50  0001 C CNN "MFN"
	1    6500 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4550 4050 4550 4300
Wire Wire Line
	4550 4050 4900 4050
Wire Wire Line
	4900 4050 4900 3900
Wire Wire Line
	4900 3900 5250 3900
Connection ~ 4550 4050
Wire Wire Line
	5100 4050 5250 4050
$Comp
L power:GND #PWR06
U 1 1 6192A487
P 5100 4350
F 0 "#PWR06" H 5100 4100 50  0001 C CNN
F 1 "GND" H 5105 4177 50  0000 C CNN
F 2 "" H 5100 4350 50  0001 C CNN
F 3 "" H 5100 4350 50  0001 C CNN
	1    5100 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 6192B28F
P 4550 4600
F 0 "#PWR05" H 4550 4350 50  0001 C CNN
F 1 "GND" H 4555 4427 50  0000 C CNN
F 2 "" H 4550 4600 50  0001 C CNN
F 3 "" H 4550 4600 50  0001 C CNN
	1    4550 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 6192B77C
P 3700 4050
F 0 "#PWR03" H 3700 3800 50  0001 C CNN
F 1 "GND" H 3705 3877 50  0000 C CNN
F 2 "" H 3700 4050 50  0001 C CNN
F 3 "" H 3700 4050 50  0001 C CNN
	1    3700 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3600 5250 3600
Wire Wire Line
	5000 3300 5000 3250
Wire Wire Line
	5000 3250 5650 3250
Wire Wire Line
	6050 3600 6100 3600
Wire Wire Line
	6100 3600 6100 3250
Connection ~ 6100 3250
Wire Wire Line
	6050 3750 6200 3750
Wire Wire Line
	6200 3750 6200 3550
Wire Wire Line
	6350 4650 6550 4650
Wire Wire Line
	6750 3900 6750 4100
Wire Wire Line
	6550 4650 6550 4750
Connection ~ 6550 4650
Wire Wire Line
	6550 4650 6750 4650
$Comp
L power:GND #PWR07
U 1 1 61933A5B
P 6550 4750
F 0 "#PWR07" H 6550 4500 50  0001 C CNN
F 1 "GND" H 6555 4577 50  0000 C CNN
F 2 "" H 6550 4750 50  0001 C CNN
F 3 "" H 6550 4750 50  0001 C CNN
	1    6550 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 4050 6250 4050
$Comp
L power:GND #PWR09
U 1 1 61937A01
P 7450 4150
F 0 "#PWR09" H 7450 3900 50  0001 C CNN
F 1 "GND" H 7455 3977 50  0000 C CNN
F 2 "" H 7450 4150 50  0001 C CNN
F 3 "" H 7450 4150 50  0001 C CNN
	1    7450 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 61943844
P 7850 4000
F 0 "D2" V 7889 3882 50  0000 R CNN
F 1 "LED" V 7798 3882 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7850 4000 50  0001 C CNN
F 3 "~" H 7850 4000 50  0001 C CNN
F 4 "‎SML-D12P8WT86‎" H 7850 4000 50  0001 C CNN "MFN"
	1    7850 4000
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R6
U 1 1 61946A94
P 7850 3400
F 0 "R6" H 7918 3446 50  0000 L CNN
F 1 "140" H 7918 3355 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 7890 3390 50  0001 C CNN
F 3 "~" H 7850 3400 50  0001 C CNN
F 4 "RMCF0805FT140R" H 7850 3400 50  0001 C CNN "MFN"
	1    7850 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 619485C0
P 7850 4150
F 0 "#PWR010" H 7850 3900 50  0001 C CNN
F 1 "GND" H 7855 3977 50  0000 C CNN
F 2 "" H 7850 4150 50  0001 C CNN
F 3 "" H 7850 4150 50  0001 C CNN
	1    7850 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3850 7850 3550
Wire Wire Line
	7850 3250 7450 3250
Connection ~ 7450 3250
Connection ~ 7850 3250
$Comp
L power:GND #PWR011
U 1 1 6194A27D
P 8200 3350
F 0 "#PWR011" H 8200 3100 50  0001 C CNN
F 1 "GND" H 8205 3177 50  0000 C CNN
F 2 "" H 8200 3350 50  0001 C CNN
F 3 "" H 8200 3350 50  0001 C CNN
	1    8200 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female J2
U 1 1 61941B99
P 8400 3250
F 0 "J2" H 8428 3226 50  0000 L CNN
F 1 "Vout" H 8428 3135 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8400 3250 50  0001 C CNN
F 3 "~" H 8400 3250 50  0001 C CNN
	1    8400 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3250 8200 3250
$Comp
L Device:D_Schottky D1
U 1 1 621081C2
P 6200 3400
F 0 "D1" V 6154 3480 50  0000 L CNN
F 1 "B340LB" V 6245 3480 50  0000 L CNN
F 2 "Diode_SMD:D_SMB_Handsoldering" H 6200 3400 50  0001 C CNN
F 3 "~" H 6200 3400 50  0001 C CNN
F 4 "B340LB-13-F" H 6200 3400 50  0001 C CNN "MFN"
	1    6200 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	6100 3250 6200 3250
Connection ~ 6200 3250
Wire Wire Line
	6200 3250 6350 3250
Connection ~ 6200 3550
$Comp
L power:GND #PWR01
U 1 1 62112669
P 3000 3850
F 0 "#PWR01" H 3000 3600 50  0001 C CNN
F 1 "GND" H 3005 3677 50  0000 C CNN
F 2 "" H 3000 3850 50  0001 C CNN
F 3 "" H 3000 3850 50  0001 C CNN
	1    3000 3850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female J1
U 1 1 6211266F
P 2800 3750
F 0 "J1" H 2828 3726 50  0000 L CNN
F 1 "Vin" H 2828 3635 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2800 3750 50  0001 C CNN
F 3 "~" H 2800 3750 50  0001 C CNN
	1    2800 3750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3000 3750 3250 3750
$Comp
L Device:CP1 C1
U 1 1 6211C11C
P 3250 3900
F 0 "C1" H 3365 3946 50  0000 L CNN
F 1 "100uF" H 3365 3855 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 3250 3900 50  0001 C CNN
F 3 "~" H 3250 3900 50  0001 C CNN
F 4 "35ZLH100MEFC6.3X11" H 3250 3900 50  0001 C CNN "MFN"
	1    3250 3900
	1    0    0    -1  
$EndComp
Connection ~ 3250 3750
Connection ~ 3700 3750
Wire Wire Line
	3700 3750 4100 3750
$Comp
L power:GND #PWR02
U 1 1 6211EE7A
P 3250 4050
F 0 "#PWR02" H 3250 3800 50  0001 C CNN
F 1 "GND" H 3255 3877 50  0000 C CNN
F 2 "" H 3250 4050 50  0001 C CNN
F 3 "" H 3250 4050 50  0001 C CNN
	1    3250 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3750 3700 3750
$Comp
L power:GND #PWR08
U 1 1 62120A16
P 7050 3550
F 0 "#PWR08" H 7050 3300 50  0001 C CNN
F 1 "GND" H 7150 3450 50  0000 C CNN
F 2 "" H 7050 3550 50  0001 C CNN
F 3 "" H 7050 3550 50  0001 C CNN
	1    7050 3550
	1    0    0    -1  
$EndComp
Connection ~ 7450 3700
Wire Wire Line
	7450 3550 7450 3700
Wire Wire Line
	7450 3700 7450 3850
Wire Wire Line
	6250 4050 6250 3700
Connection ~ 6650 3550
Wire Wire Line
	6650 3550 6200 3550
$Comp
L Device:C C9
U 1 1 621244C5
P 7050 3400
F 0 "C9" H 7165 3446 50  0000 L CNN
F 1 "10uF" H 7165 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 7088 3250 50  0001 C CNN
F 3 "~" H 7050 3400 50  0001 C CNN
F 4 "CL21A106KAYNNNG" H 7050 3400 50  0001 C CNN "MFN"
	1    7050 3400
	1    0    0    -1  
$EndComp
Connection ~ 7050 3250
Wire Wire Line
	7050 3250 7450 3250
Wire Wire Line
	6650 3250 7050 3250
Connection ~ 6650 3250
Connection ~ 7050 3550
Wire Wire Line
	6650 3550 7050 3550
Wire Wire Line
	6250 3700 6950 3700
$Comp
L Device:C C3
U 1 1 6212F65A
P 4100 3900
F 0 "C3" H 4215 3946 50  0000 L CNN
F 1 "10uF" H 4215 3855 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric_Pad1.33x2.70mm_HandSolder" H 4138 3750 50  0001 C CNN
F 3 "~" H 4100 3900 50  0001 C CNN
F 4 "CL32B106KBJZW6E" H 4100 3900 50  0001 C CNN "MFN"
	1    4100 3900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 6212F660
P 4100 4050
F 0 "#PWR04" H 4100 3800 50  0001 C CNN
F 1 "GND" H 4105 3877 50  0000 C CNN
F 2 "" H 4100 4050 50  0001 C CNN
F 3 "" H 4100 4050 50  0001 C CNN
	1    4100 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3750 4550 3750
Connection ~ 4100 3750
Connection ~ 4550 3750
Wire Wire Line
	6050 3900 6350 3900
Connection ~ 6350 3900
Wire Wire Line
	6350 3900 6750 3900
Wire Wire Line
	6350 4500 6350 4650
Wire Wire Line
	6750 4650 6750 4400
$Comp
L Connector:TestPoint TP2
U 1 1 6213571E
P 4550 3750
F 0 "TP2" H 4608 3868 50  0000 L CNN
F 1 "Vin" H 4608 3777 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 4750 3750 50  0001 C CNN
F 3 "~" H 4750 3750 50  0001 C CNN
	1    4550 3750
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP3
U 1 1 6213E431
P 5650 3250
F 0 "TP3" H 5708 3368 50  0000 L CNN
F 1 "PH" H 5708 3277 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 5850 3250 50  0001 C CNN
F 3 "~" H 5850 3250 50  0001 C CNN
	1    5650 3250
	1    0    0    -1  
$EndComp
Connection ~ 5650 3250
Wire Wire Line
	5650 3250 6100 3250
$Comp
L Connector:TestPoint TP5
U 1 1 6213F822
P 7850 3250
F 0 "TP5" H 7908 3368 50  0000 L CNN
F 1 "Vout" H 7908 3277 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 8050 3250 50  0001 C CNN
F 3 "~" H 8050 3250 50  0001 C CNN
	1    7850 3250
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP1
U 1 1 62140EF9
P 4250 3350
F 0 "TP1" H 4308 3468 50  0000 L CNN
F 1 "GND" H 4308 3377 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 4450 3350 50  0001 C CNN
F 3 "~" H 4450 3350 50  0001 C CNN
	1    4250 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 3750 5250 3750
$Comp
L Connector:TestPoint TP4
U 1 1 62144165
P 6950 3700
F 0 "TP4" H 6892 3726 50  0000 R CNN
F 1 "VSENSE" H 6892 3817 50  0000 R CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 7150 3700 50  0001 C CNN
F 3 "~" H 7150 3700 50  0001 C CNN
	1    6950 3700
	-1   0    0    1   
$EndComp
Connection ~ 6950 3700
Wire Wire Line
	6950 3700 7450 3700
$Comp
L power:GND #PWR012
U 1 1 62146A47
P 4250 3350
F 0 "#PWR012" H 4250 3100 50  0001 C CNN
F 1 "GND" H 4255 3177 50  0000 C CNN
F 2 "" H 4250 3350 50  0001 C CNN
F 3 "" H 4250 3350 50  0001 C CNN
	1    4250 3350
	1    0    0    -1  
$EndComp
$EndSCHEMATC