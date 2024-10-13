# Carbon Footprint and Air Quality Monitor

## Project Overview
The **Carbon Footprint and Air Quality Monitor** is designed to monitor air quality by tracking various harmful gases, including **Carbon Dioxide (CO2)**, **Methane (CH4)**, **Carbon Monoxide (CO)**, and **Liquefied Petroleum Gas (LPG)**, along with **temperature** and **humidity** levels. Using sensors connected to the **ESP32** microcontroller, this project collects data and displays real-time readings through a web server hosted on the ESP32.

By providing real-time environmental data, this project aims to raise awareness about air pollution and help users understand how these gases contribute to climate change.

## Basic Working
- The **ESP32** microcontroller is connected to gas sensors (MQ135, MQ4, MQ7, MQ9) and the DHT11 sensor (temperature and humidity).
- Sensor readings are processed and displayed on a web interface hosted by the ESP32, accessible via a Wi-Fi connection.
- Users can view real-time levels of CO2, Methane, CO, LPG, temperature, and humidity.
- The project also features customizable alert thresholds for harmful gas levels, allowing users to be notified when dangerous concentrations are detected.

## Programming Languages and Tools
The project makes use of a variety of programming languages and technologies:
- **C++**: The core logic of the project is written in C++ using the **Arduino IDE**, which interacts with the hardware components and processes sensor data.
- **JavaScript (JS)**: Used for the web server interface to display real-time sensor readings on the web.
- **JSX**: For structuring the frontend design and interface elements of the web server.
- **HTML/CSS**: For the basic structure and styling of the web server interface.

## Improvements
Some future improvements include:
- Adding **eco-friendly batteries** for power instead of USB.
- Integrating an **LCD screen** for local monitoring.
- Implementing **data storage** for long-term analysis of air quality trends.

## Getting Started

### Requirements
- **ESP32** microcontroller
- **MQ135** (CO2 sensor), **MQ4** (Methane sensor), **MQ7** (CO sensor), **MQ9** (LPG sensor), **DHT11** (Temperature and Humidity sensor)
- **Arduino IDE** to upload the code
- **Wi-Fi connection** to access the web server interface

### Installation
1. Clone the repository:
    ```bash
    git clone https://github.com/BilalSubhani/CarbonFootprint-AirQualityMonitor-ESP32.git
    ```
2. Open the code in the **Arduino IDE** and upload it to the **ESP32**.
3. Access the web server through the IP address provided by the **ESP32** once it's connected to the Wi-Fi.

## Contact Info
- **Project Lead**: Bilal Subhani  
  - Email: [bilalsubhani.025@gmail.com](mailto:bilalsubhani.025@gmail.com)
  - GitHub: [Bilal Subhani](https://github.com/BilalSubhani)

## Additional Information
- Contributions are welcome! If you'd like to contribute to the project, feel free to fork the repository and submit a pull request.
- This project is a part of an ongoing effort to promote environmental awareness and create accessible technology for monitoring pollution and climate change.

---

Thank you for your interest in the **Carbon Footprint and Air Quality Monitor** project! Let's work together to protect the environment and raise awareness about the dangers of pollution.
