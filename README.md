
```markdown
# ESP32/ESP8266 Contact Book Web Server

This project creates a simple contact book web server using an ESP32 or ESP8266 microcontroller. The server allows users to save contact information (name, phone number, and priority) through a web interface and stores the data using the Non-Volatile Storage (NVS) system. The web server also displays logs of actions performed.

## Features
- **Web Interface for Contact Management**: Allows users to add contacts with name, phone number, and priority fields.
- **NVS Storage**: Uses Non-Volatile Storage to save contact details on the microcontroller.
- **Log Viewer**: Displays logs of contact actions (e.g., save, search, remove) in real-time.
- **Responsive Web Design**: The web interface is mobile-friendly.

## Prerequisites
- ESP32 or ESP8266 board.
- [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/).
- Required libraries:
  - `ESPAsyncWebServer`
  - `AsyncTCP` (ESP32) / `ESPAsyncTCP` (ESP8266)
  - `SPIFFS`
  - `Preferences` (ESP32 only)

## Installation

1. **Install Arduino IDE or PlatformIO:**
   - [Arduino IDE Installation](https://www.arduino.cc/en/software)
   - [PlatformIO Installation](https://platformio.org/install)

2. **Install the required libraries:**
   - In the Arduino IDE, go to **Sketch** > **Include Library** > **Manage Libraries**, and install the following libraries:
     - `ESPAsyncWebServer`
     - `AsyncTCP` (for ESP32) or `ESPAsyncTCP` (for ESP8266)
     - `SPIFFS`

3. **Clone the Repository:**
   ```bash
   git clone https://github.com/yourusername/esp-contact-book.git
   ```

4. **Upload the Code to the Board:**
   - Open the `ContactBook.ino` file in Arduino IDE.
   - Select your board under **Tools** > **Board** (choose ESP32 or ESP8266).
   - Set the correct **Port** under **Tools** > **Port**.
   - Click **Upload** to flash the code onto your board.

## Usage

1. **Connect to the Wi-Fi Network:**
   - Modify the `ssid` and `password` variables in the code to match your Wi-Fi credentials.
   ```cpp
   const char* ssid = "your_SSID";
   const char* password = "your_PASSWORD";
   ```
   
2. **Access the Web Interface:**
   - Once the ESP32/ESP8266 connects to the Wi-Fi, it will print the IP address in the Serial Monitor.
   - Open a web browser and navigate to the displayed IP address to access the contact book interface.

3. **Add a Contact:**
   - Fill in the contact's name, phone number, and priority, and click the **Save** button.
   - The contact information will be saved to NVS storage and a log entry will be created.

4. **View Logs:**
   - The **Logs** section on the web interface displays real-time log entries, showing the actions performed.

## File Structure

- `ContactBook.ino`: Main source code for the project.
- `index.html`: Embedded within the source code for the web interface (stored in program memory).
- `/data/log.txt`: Log file stored in SPIFFS, used for storing action logs.

## Code Overview

- **Setup Wi-Fi Connection**: Connects the ESP32/ESP8266 to a Wi-Fi network.
- **Web Server Setup**: Handles requests and serves the web interface.
- **NVS Storage**: Uses the `Preferences` library to store and retrieve contact details.
- **Logging**: Uses SPIFFS to append actions to a log file.

## Dependencies

The following libraries are needed for this project:

- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP) (for ESP32) / [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) (for ESP8266)
- [SPIFFS](https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS) (for file system support)
- [Preferences](https://github.com/espressif/arduino-esp32/tree/master/libraries/Preferences) (for NVS storage)

## Troubleshooting

- **Wi-Fi Connection Issues**: Make sure the Wi-Fi credentials are correct and the board is within range of the router.
- **Library Compatibility**: Ensure the correct library versions are installed. Use the Library Manager in Arduino IDE to install/update libraries.
- **Serial Monitor**: Check the Serial Monitor for debugging information. It can help diagnose issues with Wi-Fi connection, server setup, or storage.

## Future Improvements

- Add user authentication for web interface access
- Implement contact search and removal functionality
- Use AJAX for dynamic updates without page reloads
- Add data export/import feature for backup
- Display contact list on the web interface


Contributions are welcome! Please create a pull request with your changes or suggest new features via issues.
