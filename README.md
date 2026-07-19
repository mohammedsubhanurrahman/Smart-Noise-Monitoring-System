# Smart-Noise-Monitoring-System

**A custom-built noise detection and alert system designed for real-time monitoring and automated warnings.**

#### 🚀 Project Overview
The Smart Noise Monitor is an embedded system project developed to detect ambient sound levels and trigger a visual/audible alert based on customizable logic. This project utilizes an Arduino Uno and analog sound sensors to provide an efficient, low-latency monitoring solution.

#### 🛠 Tech Stack
* **Hardware:** Arduino Uno, Analog Sound Sensor, LED, Buzzer.
* **Software:** Arduino IDE, C++.
* **Logic:** Custom alert thresholding and state management.

#### 💡 Key Features
* **Real-time Detection:** Continuously monitors ambient noise levels via analog input.
* **Custom Alert Logic:** Implements a specific 3-second blinking warning system when noise thresholds are exceeded.
* **Robust Alerting:** Immediate feedback mechanism to notify users of high-noise environments.

#### ⚙️ System Architecture
![Smart Noise Monitor Setup](WhatsApp_Image_2026-07-19_at_3.45.52_PM.jpeg)

#### 📝 How to Run
1. Connect your Arduino Uno to your PC.
2. Open the code in the Arduino IDE.
3. Select the correct Board and Port from the Tools menu.
4. Upload the code to your Arduino.
5. Observe the LED/Buzzer behavior when noise levels exceed your set threshold.

#### 📈 Future Scope
* **Cloud Integration:** Log noise data to a database for long-term pattern analysis.
* **Wireless Alerts:** Integrate an ESP8266 or Bluetooth module to send notifications to a mobile app.
* **Data Visualization:** Build a dashboard to visualize noise fluctuations over time.
