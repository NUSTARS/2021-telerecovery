# Recovery Systems Telemetry Module

## Basic Requirements:
- Under $200 total 
- 2 mile range to be safe
  - Assuming typical NSL flight profile, apogee is typically between 4000-6000ft 
- Small form factor
  - Do not want to take up too much space in the rocket 
- Detect separation of launch vehicle
- Detect connection status of ejection charges (maybe not, NASA would not like)
- Protected from black powder residue
- Remember to consider the fact that carbon fiber is RF opaque

## Further Objectives:
- Add more sensors to create a more complete sensor/telemetry suite
- Create a desktop/web-app dashboard for the telemetry data. Maybe someting like [this](https://ambasat.com/wp-content/uploads/2019/05/Dashboard-with-dropshadow2.jpg).

## Google Drive
Here is a [Google Drive link](https://drive.google.com/drive/u/1/folders/1rWdLhBEQf0NXYTq_rxv_L2-M3DT5xfJa) for anything else we may not want to put on GitHub like pictures, videos, test results, etc.

## Useage
*Note: almost all this code is hardware dependent so unless you have the exact same hardware, you can't use it. The groundstation app is not entirely hardware dependent/the incoming data could be simulated without too much trouble*

The easiest way to use the code in this repo is to download PlatformIO and use the provided platformio.ini initialization file when you create a new project. PlatformIO functions well with many modernIDE's and text editors like Visual Studio Code or Atom. 

