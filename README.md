# College Bus Tracker — Initial Version (WiFi Hotspot)

## Overview

The **College Bus Tracker** is an IoT-based bus monitoring system designed to help students, parents, and the college transport office monitor the status of a college bus.

The initial version uses an **ESP32**, the driver's **mobile hotspot**, and **college Wi-Fi** to communicate with a Flask server. The system records trip status, heartbeat updates, departure and arrival times, and displays the information through a web-based dashboard.

## Project idea

### College Bus Tracker — Initial Version (WiFi Hotspot)

The ESP32 is installed on the college bus and is pre-configured with the driver's mobile hotspot and college Wi-Fi credentials.

When the bus starts its trip:

1. The driver turns on the mobile hotspot.
2. The ESP32 connects to the hotspot.
3. The ESP32 sends a heartbeat to the college server every **30 seconds**.
4. The driver uses a physical button to update the trip status.
5. When the bus returns to campus, the ESP32 detects the college Wi-Fi.
6. The ESP32 automatically switches to the college network.
7. The system updates the status to **Arrived at Campus**.

The dashboard displays the current bus status, last ping time, elapsed time, and the complete trip log for the day.

## Team Members

| No. | Name           |
| --- | -------------- |
| 1   | Felizya Shenil |
| 2   | Aric Antony    |
| 3   | Aldrin Antony  |
| 4   | Donald P Paul  |

## Objectives

* To develop an **ESP32-based college bus tracking system**.
* To monitor the **current trip status** of the bus.
* To transmit bus status using **Wi-Fi and mobile hotspot connectivity**.
* To automatically switch between **driver hotspot and college Wi-Fi**.
* To provide regular **heartbeat updates** from the bus.
* To record **departure and arrival times**.
* To provide a web-based dashboard for monitoring.
* To maintain a **daily trip log** for the transport office.
* To provide a foundation for **future GPS-based live tracking**.

## To Do List

*  Define system requirements and architecture.
*  Configure ESP32 with multiple Wi-Fi networks.
*  Implement the driver button-based trip status system.
*  Implement HTTP heartbeat communication.
*  Develop the Flask server and SQLite database.
*  Implement automatic Wi-Fi switching.
*  Develop the college dashboard.
*  Display live bus status and last ping time.
*  Record daily departure and arrival details.
*  Test and demonstrate the complete system.
*  Add NEO-6M GPS module.
*  Implement live GPS location tracking.
*  Display the bus location on a map.

## Features

## Key Features of the Project

1. **Real-Time Bus Status**
   Displays the current status of the bus, such as Departed, En Route, and Arrived.

2. **Mobile Hotspot Connectivity**
   The ESP32 connects to the driver's mobile hotspot when the bus is away from campus.

3. **College Wi-Fi Detection**
   The ESP32 detects the college Wi-Fi when the bus returns to campus.

4. **Automatic Wi-Fi Switching**
   The system automatically switches between the driver's hotspot and college Wi-Fi.

5. **Trip Status Button**
   A physical button allows the driver to cycle through the available trip statuses.

6. **LED Connection Indicator**

   * 🔵 **Blue** — Connected to college Wi-Fi
   * 🟡 **Yellow** — Connected to mobile hotspot
   * 🔴 **Red** — No network connection

7. **Heartbeat Updates**
   The ESP32 sends a heartbeat update to the server every **30 seconds**.

8. **Live Dashboard**
   Displays the bus name, current status, last ping time, and elapsed time.

9. **Trip Duration**
   Records and displays the total duration of the bus trip.

10. **Daily Trip Log**
    Stores departure time, arrival time, and total trip duration.

11. **SQLite Database**
    Stores trip and status information locally on the Flask server.

12. **GPS Upgrade Support**
    The system can be extended with a **NEO-6M GPS module** for live location tracking and map visualization.

## Technologies Used

* **Hardware:** ESP32 Dev Module, Push Button, LEDs
* **Software:** Arduino IDE, Flask
* **Programming Languages:** C++, Python
* **Database:** SQLite
* **Communication:** Wi-Fi, HTTP
* **Backend:** Python Flask
* **Future Hardware Upgrade:** NEO-6M GPS Module

## System Architecture

```text
        ┌─────────────────────┐
        │     College Bus     │
        │                     │
        │       ESP32         │
        │    ┌───────────┐    │
        │    │   Button  │    │
        │    └───────────┘    │
        │    ┌───────────┐    │
        │    │    LEDs   │    │
        │    └───────────┘    │
        └──────────┬──────────┘
                   │
          Wi-Fi / Mobile Hotspot
                   │
                   ▼
        ┌─────────────────────┐
        │    Flask Server     │
        │                     │
        │    HTTP API         │
        │        │            │
        │        ▼            │
        │     SQLite          │
        └──────────┬──────────┘
                   │
                   ▼
        ┌─────────────────────┐
        │   Web Dashboard      │
        │                     │
        │ Bus Status           │
        │ Last Ping            │
        │ Elapsed Time         │
        │ Trip Log             │
        └─────────────────────┘
```

## Trip Status Flow

```text
        ┌───────────┐
        │  Departed │
        └─────┬─────┘
              │
              ▼
        ┌───────────┐
        │  En Route │
        └─────┬─────┘
              │
              ▼
        ┌───────────┐
        │  Arrived  │
        └─────┬─────┘
              │
              ▼
    ┌─────────────────────┐
    │ Arrived at Campus   │
    └─────────────────────┘
```

## Future Improvements

* Integrate a **NEO-6M GPS module**.
* Track the bus's **live geographical location**.
* Display the bus location on an interactive map.
* Add estimated arrival time (**ETA**).
* Support multiple buses and routes.
* Add notifications for students and parents.
* Add historical route tracking.

## Project Deliverables

* `bus_tracker.ino` — ESP32 firmware with multi-SSID Wi-Fi, button state machine, LED indicators, and HTTP heartbeat.
* **Flask Server** — Backend API, SQLite trip logging, and dashboard.
* **TV Dashboard** — Live bus status and daily trip log.
* **Screenshots** — Dashboard screenshots showing En Route, Arrived, and trip log states.
* **One-Page Report** — Connection logic, trip state machine, Wi-Fi switchover, and GPS upgrade plan.

## Future GPS Upgrade

The next version of the project will integrate a **NEO-6M GPS module** with the ESP32.

The GPS module will provide:

```text
Latitude
Longitude
   ↓
ESP32
   ↓
Flask Server
   ↓
Web Dashboard
   ↓
Live Bus Location on Map
```

This upgrade will transform the current **bus status tracking system** into a complete **real-time GPS bus tracking system**.

---

### Project Status

**Current Version:** ✅ Initial Wi-Fi Hotspot Version
**GPS Tracking:** 🔜 Future Upgrade
