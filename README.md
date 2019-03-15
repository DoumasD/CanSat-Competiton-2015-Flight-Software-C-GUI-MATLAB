# Project Name: CanSat Competition 2015
// Demetrios Doumas 11/12/18
// Flight Software for the CanSat Competition 2015


# Description:

The 2015 CanSat Competition mission is to develop a can-sized satellite that has to do several objectives. The cansat has to separate into two parts at a certain altitude and send telemetry data every second to the ground station. The telemetry data includes: Team id, mission time, altitude sensor, outside temperature, inside temperature, voltage, flight state, X acceleration, Y acceleration, and Z acceleration. The flight states are: PreFlightTest(0), LaunchWait(1), Ascent(2), RocketDeployment(3),Stabilization(4), Separation(5), Descent(6), and Landed(7). Record video of the descent of the cansat. The two parts of the cansat have to fall at a certain rate. The competition is meant to give teams, the experience of the design cycle in the Aerospace Industry. There are two presentations, which are the preliminary design review (PDR), and the critical design review (CDR). I uploaded the CDR slides. Another experience that was taught is building the cansat product.

The Mechanical group was tasked to develop the cansat itself, the separating mechanism, and the deploy system. The electrical group developed the PCB.

My Role:

As the leader of flight Controls, I developed the flight software of the cansat satellite. The software collects data from sensors and determines its flight state based on the data. The cansat perform flight operations such as separation at a specific altitude, turning on a camera, and send telemetry data back to the ground station at approximately 1 Hz.  

As the leader of ground station, I was tasked to read a telemetry string from an Arduino microcontroller in the cansat called Arduino Pro Mini 5v. The data is parsed, plotted, and saved in real time. The microcontroller is located in the payload. I was also in charge of communications. I used xBee radio modules Pro 3B 900 MHz series and two omnidirectional antennas. The antenna stand was made by Fady Mekhael, its height is approximately 3.5 meters.

Microcontroller used: Arduino Pro mini 5v    
Language used: C/C++ and MATLAB


Sensors:
	BMP180,  Altitude sensor                        - Read Altitude data
	TMP36,   Temperature sensor			            - Read outside the cansat temperature
	LSM303,  Accelerometer + Magnetometer sensor    - Read accelerometer data and use to stabilizes camera during descent  
	TSL2591, Luminosity sensor		                - Measure the light that the cansat sees, used to determine flight state that tells us when the cansat is released from the rocket itself.

Camera:
	Spycrushers Keychain Camera

External Clock:
	ChronoDot     - used to keep track of time, just in case the microcontroller time has been reset. 


# Installation:

I don't have access to the PCB design yet in order to replicate it. This repository is meant to display the Arduino code and the ground station software as well.

Flight Software:
1.) Download the folder "CanSat2015ElectronicsCode."
2.) Open the Arduino IDE, and open the folder content.
3.) Attach the Arduino Pro mini 5 volts to your computer by USB.
4.) Connect to the comport where the microcontroller is located.
5.) Go to the menu and select "Arduino Pro" under Tools and select Board.
6.) Press compile (check mark symbol). Before compiling please install the libraries first. 
7.) Press upload (arrow from left to right symbol)
8.) Check the serial monitor to see the telemetry data being printed to the console.

Ground Station Software:

This software is intended for windows OS.
1.) Download the folder "2015 Payload GUI."
2.) Open MATLAB and the "cansat2015guipayloaddraft1.m" file
3.) Click run.
Note: Make sure all files are in the same directory.

# Usage:

Ground Station Software:

1.) Select serial comport.
2.) Click start. 
3.) Make sure you close the serial port by clicking the stop button; if you exit the application without pressing this button, you won't be able to access the serial port again, unless you restart your computer.


# Credits:

Please look at the publish power point presentation called "Cansat 2015 Critical Design Review CCNY", on slide two.

Mechanical Group:
Ruben Perez -- Cansat Leader
Patrick Guillaume
Majlinda Malellari
Sakif Chowdhury

Electrical Group:
Maria F. Caceres
Joel Annenberg (Biomedical Engineer)

Software Group:
Demetrios Doumas (Computer Engineer)

