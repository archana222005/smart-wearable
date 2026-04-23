# Smart Wearable with Geofencing

An IoT-based wearable device that tracks user location using GPS,
detects falls using MPU6050 accelerometer, and sends SMS alerts
via GSM when safety boundaries are crossed.

## Features
- GPS-based geofencing with safe zone boundary alerts
- Fall detection using MPU6050 accelerometer
- Inactivity monitoring — alert if no movement for 10 seconds
- SMS alerts sent to registered phone number
- Green LED = Safe, Red LED = Alert
- Buzzer with different beep patterns for different alerts

## Components Used
| Component | Purpose |
|---|---|
| Arduino Uno | Main microcontroller |
| MPU6050 | Accelerometer for fall detection |
| NEO-6M GPS | Location tracking |
| SIM800L GSM | SMS alerts |
| Buzzer | Local audio alert |
| Red + Green LED | Visual status indicator |

## Circuit Connections
| Component | Arduino Pin |
|---|---|
| Buzzer | Pin 8 |
| Green LED | Pin 12 |
| Red LED | Pin 13 |
| MPU6050 SDA | A4 |
| MPU6050 SCL | A5 |
| GSM TX | Pin 2 |
| GSM RX | Pin 3 |

## How It Works
1. GPS continuously reads current coordinates
2. System checks if coordinates are within safe zone radius
3. If outside — Red LED glows, buzzer beeps, SMS sent
4. MPU6050 monitors for sudden acceleration changes
5. Free fall followed by impact → Emergency SMS triggered
6. No movement for 10 seconds → Inactivity alert sent

## Simulation
Simulated using [Wokwi](https://wokwi.com) online simulator.
GSM SMS alerts are displayed via Serial Monitor.

## How to Run
1. Open `smart_wearables.ino` in Arduino IDE
2. Install MPU6050 library from Library Manager
3. Change `safe_lat` and `safe_lon` to your location coordinates
4. Change `phoneNumber` to your phone number
5. Upload code to Arduino Uno

## Tech Stack
- Embedded C / Arduino IDE
- Wokwi Simulator
