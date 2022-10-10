# Recovery Systems Telemetry Module (SkyNet)
A Telemetry Software-Hardware Package for High Powered Rockets.

# TODO:
( ) Implement storing data in onboard flash memory in transmitter board
[] Use sensor readings to determine apogee
[] Improvements on groundstation


## Useage
*Note: almost all this code is hardware dependent so unless you have the exact same hardware, you can't use it. The groundstation app is not entirely hardware dependent/the incoming data could be simulated without too much trouble*

The easiest way to use the code in this repo is to download PlatformIO and use the provided platformio.ini initialization file when you create a new project. PlatformIO works great with VS code. 

Checkout the [Wiki](https://github.com/NUSTARS/2021-telerecovery/wiki/Hardware) page for more information. 

### Google Drive
Here is a [Google Drive link](https://drive.google.com/drive/u/1/folders/1rWdLhBEQf0NXYTq_rxv_L2-M3DT5xfJa) for anything else we may not want to put on GitHub like pictures, videos, test results, etc.

## Initial Objectives:
- Under $200 total 
- 2 mile range to be safe
  - Assuming typical NSL flight profile, apogee is typically between 4000-6000ft 
- Small form factor
  - Do not want to take up too much space in the rocket 
- Detect separation of launch vehicle
- Collect as much data as possible; acceleration, gyro, pressure, temperature
- Protected from black powder residue
- Remember to consider the fact that carbon fiber is RF opaque
- Cross-platform desktop GUI application for displaying sensor data in a nice way

