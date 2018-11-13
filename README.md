# CanSat-Competiton-2015-Flight-Software
// Demetrios Doumas 11/12/18
// Flight Software for the CanSat Competition 2015


Purpose: 

The 2015 CanSat Competition mission is to develop a can-sized satellite that has to do several objectives. Separate into two parts at a certain altitude. 
Send telemetry data every second to the ground station. The telemetry data includes: Team id, mission time, alt sensor, outside temperature, inside temperature, voltage, flight state, X, Y, and Z. The flight states are: PreFlightTest(0), LaunchWait(1), Ascent(2), RocketDeployment(3), 
Stabilization(4), Separation(5), Descent(6), and Landed(7). Record video of the descent of the cansat. 

Microcontroller used: Arduino Pro mini 5v    
Language used: C/C++


Sensors:
	BMP180,  Altitude sensor                        - Read Altitude data
	TMP36,   Temperature sensor			                - Read outside the cansat temperature
	LSM303,  Accelerometer + Magnetometer sensor    - Read accelerometer data and use to stabilizes camera during descent  
	TSL2591, Luminosity sensor		                	- Measure the light that the cansat sees, used to determine flight state that tells us when the cansat is released from the rocket itself.

Camera:
	Spycrushers Keychain Camera

External Clock:
	ChronoDot     - used to keep track of time, just in case the microcontroller has reset. 

EEPROMs: used to save telemetry data if transmission fails.
