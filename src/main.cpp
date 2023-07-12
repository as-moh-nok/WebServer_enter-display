/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-input-data-html-form/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
//#include <SPIFFS.h>
#include <Preferences.h>

AsyncWebServer server(80);
Preferences preferences;//save Contact in nvs

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "hwa";
const char* password = "wifi1373";

const char* PARAM_NAME = "name";
const char* PARAM_PHONE = "phone";
const char* PARAM_PROPRITY= "priority";

const char* SAVE_ACTION= "save";
const char* REMOVE_ACTION="remove";
const char* SEARCH_ACTION="search";
// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      /* Add responsive styles for mobile and desktop */
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
      <h1>Form</h1>
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

        <input class="form-button" type="submit" name="action" value="save" onclick="submitContact()">Save</input>
      </form>
      <iframe style="display:none" name="hidden-form"></iframe>
      <div class="logs">
        <iframe name="log-form" src="/log" height="200" width="300" title="Logs"></iframe>
      </div>
    </div>
  </body>
  <script>
    function submitContact() {
      alert("Saved Contact to ESP nvs");
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

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void writeNVS(Preferences& preferences , const char * nameToSave, const char * phoneToSave){
  Serial.printf("Save contact: name:%s with %s number and no priority\r\n", nameToSave, phoneToSave);
  preferences.begin(nameToSave, false);
//  preferences.putString("name", stringToSave);
  preferences.putString("phone", phoneToSave);
  Serial.println("Contect save to file!");
  preferences.end();
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputName;
    String inputPhone;
    String buttonSubmit;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam("action")) {
      String action = request->getParam("action")->value();
      //inputPhone = request->getParam("save")->value();
      if(action == "save"){
      Serial.println("save");
      inputName = request->getParam("save")->value();
      inputPhone = request->getParam("save")->value();
      writeNVS(preferences, inputName.c_str(),  inputPhone.c_str());
      }
      else if (action == "remove"){
      Serial.println("remove");

      }
      else if (action == "search")
    {
      Serial.println("search");
    }
    else {
      //inputName = "No message sent";
      //inputPhone = "none";
      Serial.println("No message sent");

    }
    //Serial.println(No message sent);
    request->send(200, "text/html", " ");
    }
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  
  
}
