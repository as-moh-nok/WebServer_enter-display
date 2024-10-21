/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-input-data-html-form/
  
  Modifications and additional features by Asma Mohammadi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <iostream>

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Preferences.h>

// Create a web server instance on port 80
AsyncWebServer server(80);

// Preferences object to handle NVS storage
Preferences preferences;

// Function declaration for appending logs
void appendLog(const char* message);

// WiFi credentials
const char* ssid = "your ssid";
const char* password = "your password";

// Form parameter names
const char* PARAM_NAME = "name";
const char* PARAM_PHONE = "phone";
const char* PARAM_PROPRITY= "priority";

// Action names for different operations
const char* SAVE_ACTION= "save";
const char* REMOVE_ACTION="remove";
const char* SEARCH_ACTION="search";

// HTML web page template stored in program memory
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      /* CSS styles for responsiveness and form appearance */
      * {
        box-sizing: border-box;
      }
      body {
        font-family: Arial, sans-serif;
      }
      .container {
        padding: 20px;
        max-width: 600px;
        margin: 0 auto;
      }
      .form-input {
        display: block;
        margin-bottom: 10px;
        width: 100%;
        padding: 10px;
        font-size: 16px;
        border: 1px solid #ccc;
        border-radius: 4px;
      }
      .form-input:focus {
        border-color: #007bff;
        outline: none;
      }
      .form-label {
        display: block;
        margin-bottom: 5px;
        font-size: 14px;
        font-weight: bold;
      }
      .form-button {
        display: inline-block;
        padding: 10px 20px;
        font-size: 16px;
        font-weight: bold;
        text-align: center;
        text-decoration: none;
        background-color: #007bff;
        color: #fff;
        border: none;
        border-radius: 4px;
        cursor: pointer;
      }
      .form-button:hover {
        background-color: #0069d9;
      }
      .logs {
        margin-top: 20px;
        padding: 10px;
        border: 1px solid #ccc;
        border-radius: 4px;
        height: 200px;
        overflow-y: auto;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>Contact Book</h1>
      <form action="/get" target="hidden-form">
        <label class="form-label" for="name-input">Name:</label>
        <input class="form-input" type="text" id="name-input"  name="name" required>

        <label class="form-label" for="phone-input">Phone Number:</label>
        <input class="form-input" type="tel" id="phone-input"  name="phone" required>

        <label class="form-label" for="priority-input">Priority:</label>
        <select class="form-input" id="priority-input"  name="priority" required>
          <option value="">-- Select Priority --</option>
          <option value="high">High</option>
          <option value="medium">Medium</option>
          <option value="low">Low</option>
        </select>

        <input class="form-button" type="submit" name="action" value="save" onclick="submitContact()"></input>
      </form>
      <iframe style="display:none" name="hidden-form"></iframe>
      <div class="logs">
        <iframe name="log-form" src="/log" height="200" width="300" title="Logs"></iframe>
      </div>
    </div>
  </body>
  <script>
    // JavaScript to handle form submission and log refreshing
    function submitContact() {
      alert("Saved Contact to ESP NVS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
    function refreshLogs() {
      var logFrame = document.getElementsByName("log-form")[0];
      logFrame.contentDocument.location.reload(true);
    }
    setInterval(refreshLogs, 1000);
  </script>
</html>
)rawliteral";

// Structure to store contact information
struct Contact{
  String name;
  String phone;
  String priority;
};

// Handler for 404 errors (not found)
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Function to initialize logging
void startLog(){
  SPIFFS.begin();
  SPIFFS.open("/log.txt", FILE_WRITE);
  File logFile = SPIFFS.open("/log.txt", "a");
  if (!logFile) {
    Serial.println("No log file");
    return;
  }

  // Append welcome message to the log file
  logFile.println("Welcome to ContactBook Webserver:\n");
  logFile.println("---------------------------------\n");
  logFile.close();
}

// Function to save contact information in NVS
void writeNVS(Preferences& preferences , const char * nameToSave, const char *phoneToSave, const char *priorityToSave){
  Serial.printf("Save contact: name:%s with %s number and %s priority\r\n", nameToSave, phoneToSave, priorityToSave);
  preferences.begin(nameToSave, false);
  preferences.putString("phone", phoneToSave);
  preferences.putString("priority", priorityToSave);

  // Log the save action
  char buffer[100];
  std::sprintf(buffer, "Save contact: name:%s with %s number and %s priority\r\n", nameToSave, phoneToSave, priorityToSave);
  appendLog(buffer);

  Serial.println("Contact saved to file!");
  preferences.end();
}

// Function to append messages to the log file
void appendLog(const char* message){
  SPIFFS.begin();
  File logFile = SPIFFS.open("/log.txt", "a");
  if (!logFile) {
    Serial.println("Failed to open log file.");
    return;
  }

  logFile.println(message);
  logFile.close();
}

// Function to send log content to the client
void sendLogs(AsyncWebServerRequest *request){
  File logFile = SPIFFS.open("/log.txt", "r");
  if (!logFile) {
    request->send(200, "text/plain", "No logs available.");
    return;
  }

  String logString = "";
  while (logFile.available()) {
    logString += (char)logFile.read();
  }
  logFile.close();
  request->send(200, "text/plain", logString);
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  startLog(); 
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up server routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Handle form submissions
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputName, inputPhone, inputPriority;
    if (request->hasParam("action")) {
      String action = request->getParam("action")->value();
      if (action == "save") {
        inputName = request->getParam("name")->value();
        inputPhone = request->getParam("phone")->value();
        inputPriority = request->getParam("priority")->value();
        writeNVS(preferences, inputName.c_str(), inputPhone.c_str(), inputPriority.c_str());
      } else {
        Serial.println("No valid action received");
      }
    }
    request->send(200, "text/html", " ");
  });

  // Route to send logs
  server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request){
    sendLogs(request);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // Empty loop; most logic is handled in callbacks
}
