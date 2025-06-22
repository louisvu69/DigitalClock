// // #include <ESP8266WiFi.h>
// // #include <ESP8266WebServer.h>
// // #include <DHT.h>
// // #include "RTC_LCD.h"      // RTC library
// // #include "LCD_Control.h"  // LCD library
// // #include "MQ135.h"        // MQ135 library

// // // WiFi credentials
// // const char* ssid = "202";
// // const char* password = "Nguyenthuyhanh1998";

// // // DHT11 sensor setup
// // #define DHTPIN D7
// // #define DHTTYPE DHT11
// // DHT dht(DHTPIN, DHTTYPE);
// // byte degree[8] = {
// //   0B01110,
// //   0B01010,
// //   0B01110,
// //   0B00000,
// //   0B00000,
// //   0B00000,
// //   0B00000,
// //   0B00000
// // };

// // // MQ135 sensor setup
// // #define PIN_MQ135 A0
// // MQ135 mq135_sensor = MQ135(PIN_MQ135);

// // // Web server setup
// // ESP8266WebServer server(80);

// // // Array to store the last 10 temperature and PPM readings and timestamps
// // #define DATA_POINTS 10
// // float temperatureData[DATA_POINTS];
// // float ppmData[DATA_POINTS];
// // unsigned long timestamps[DATA_POINTS];
// // unsigned long lastUpdateTime = 0;
// // const long updateInterval = 1000;  // Update every 1 second for real-time

// // // GPIO D5 pin to control the buzzer
// // const int buzzer_pin = D5;

// // // Structure to store alarm information
// // struct Alarm {
// //   int hour;                     // Alarm hour
// //   int minute;                   // Alarm minute
// //   bool triggered;               // Whether the alarm has been triggered
// //   unsigned long buzzStartTime;  // Time when buzzer starts (for millis)
// // };
// // Alarm alarms[10];                   // Array to store up to 10 alarms
// // int alarmCount = 0;                 // Current number of alarms
// // const int buzzerDuration = 300000;  // Fixed buzzer duration: 5 minutes (300,000 ms)
// // bool buzzerActive = false;          // Flag to track if buzzer is currently active

// // void setup() {
// //   Serial.begin(115200);

// //   // Initialize RTC and LCD
// //   initRTC();
// //   initLCD();
// //   lcd.createChar(1, degree);

// //   // Connect to WiFi
// //   WiFi.begin(ssid, password);
// //   while (WiFi.status() != WL_CONNECTED) {
// //     delay(500);
// //     Serial.print(".");
// //   }
// //   Serial.println("\nConnected to WiFi");
// //   Serial.println(WiFi.localIP());

// //   // Initialize DHT sensor
// //   dht.begin();

// //   // Initialize data arrays
// //   for (int i = 0; i < DATA_POINTS; i++) {
// //     temperatureData[i] = 0.0;
// //     ppmData[i] = 0.0;
// //     timestamps[i] = 0;
// //   }

// //   // Configure buzzer pin
// //   pinMode(buzzer_pin, OUTPUT);
// //   digitalWrite(buzzer_pin, LOW);

// //   // Configure web server routes
// //   server.on("/", handleRoot);
// //   server.on("/graph", drawGraph);
// //   server.on("/ppmGraph", drawPPMGraph);
// //   server.on("/data", handleData);
// //   server.on("/setAlarm", handleAlarm);
// //   server.on("/deleteAlarm", handleDeleteAlarm);
// //   server.on("/editAlarm", handleEditAlarm);
// //   server.on("/stopAlarm", handleStopAlarm);
// //   server.on("/buzzerState", handleBuzzerState);
// //   server.on("/getAlarms", handleGetAlarms);
// //   server.begin();
// //   Serial.println("Web server started");
// // }

// // void loop() {
// //   server.handleClient();  // Handle client requests

// //   unsigned long currentMillis = millis();
// //   if (currentMillis - lastUpdateTime >= updateInterval) {
// //     lastUpdateTime = currentMillis;
// //     DateTime now = rtc.now();

// //     // Read temperature and humidity from DHT11 with retry
// //     float temp = readTemperatureWithRetry();
// //     float hum = dht.readHumidity();
// //     if (isnan(hum)) {
// //       hum = 0.0;  // Default humidity if reading fails
// //       Serial.println("Failed to read humidity from DHT sensor!");
// //     }

// //     // Update temperature and PPM data
// //     if (!isnan(temp)) {
// //       // Shift temperature array and add new value
// //       for (int i = 0; i < DATA_POINTS - 1; i++) {
// //         temperatureData[i] = temperatureData[i + 1];
// //         timestamps[i] = timestamps[i + 1];
// //       }
// //       temperatureData[DATA_POINTS - 1] = temp;
// //       timestamps[DATA_POINTS - 1] = now.unixtime();

// //       // Read corrected PPM from MQ135
// //       float correctedPPM = mq135_sensor.getCorrectedPPM(temp, hum);

// //       // Shift PPM array and add new value
// //       for (int i = 0; i < DATA_POINTS - 1; i++) {
// //         ppmData[i] = ppmData[i + 1];
// //       }
// //       ppmData[DATA_POINTS - 1] = correctedPPM;

// //       Serial.print("Temperature: ");
// //       Serial.print(temp);
// //       Serial.print("°C, Humidity: ");
// //       Serial.print(hum);
// //       Serial.print("%, Corrected PPM: ");
// //       Serial.print(correctedPPM);
// //       Serial.println("ppm");
// //     } else {
// //       Serial.println("Failed to read temperature from DHT sensor after retries!");
// //       // Shift data with default value if reading fails
// //       for (int i = 0; i < DATA_POINTS - 1; i++) {
// //         temperatureData[i] = temperatureData[i + 1];
// //         timestamps[i] = timestamps[i + 1];
// //       }
// //       temperatureData[DATA_POINTS - 1] = 0.0;  // Default value
// //       timestamps[DATA_POINTS - 1] = now.unixtime();
// //     }

// //     // Display date and time on LCD
// //     displayDate(getDate());
// //     displayTime(getTime());
// //     lcd.setCursor(9, 1);  // Set cursor to line 1, column 9
// //     lcd.print(temp, 1);   // Display temperature with 1 decimal place
// //     lcd.write(1);
// //     lcd.print("C");       // Add °C unit
// //     checkAlarms();        // Check and trigger alarms
// //   }
// // }

// // // Function to read temperature with retry
// // float readTemperatureWithRetry() {
// //   float temp = dht.readTemperature();
// //   if (isnan(temp)) {
// //     delay(100);  // Wait before retry
// //     temp = dht.readTemperature();
// //     if (isnan(temp)) {
// //       delay(100);  // Second retry
// //       temp = dht.readTemperature();
// //     }
// //   }
// //   return temp;
// // }

// // // Serve the main web page with real-time updates
// // void handleRoot() {
// //   String html = R"=====(
// //     <!DOCTYPE html>
// //     <html>
// //     <head>
// //       <meta charset="utf-8">
// //       <title>ESP8266 DHT11 & MQ135 Monitoring</title>
// //       <script>
// //         function updateData() {
// //           var xhr = new XMLHttpRequest();
// //           xhr.open('GET', '/data', true);
// //           xhr.onload = function () {
// //             if (xhr.status === 200) {
// //               var data = JSON.parse(xhr.responseText);
// //               document.getElementById('currentDate').innerText = data.date;
// //               document.getElementById('currentTime').innerText = data.time;
// //               document.getElementById('currentTemperature').innerText = data.temperature + "°C";
// //               document.getElementById('currentPPM').innerText = data.ppm + "ppm";
// //             }
// //           };
// //           xhr.send();
// //         }
// //         setInterval(updateData, 1000);
// //         updateData();

// //         function checkBuzzerState() {
// //           var xhr = new XMLHttpRequest();
// //           xhr.open('GET', '/buzzerState', true);
// //           xhr.onload = function () {
// //             if (xhr.status === 200) {
// //               var stopButton = document.getElementById('stopButton');
// //               if (xhr.responseText === 'active') {
// //                 stopButton.style.display = 'block';
// //               } else {
// //                 stopButton.style.display = 'none';
// //               }
// //             }
// //           };
// //           xhr.send();
// //         }
// //         setInterval(checkBuzzerState, 1000);

// //         function setAlarm() {
// //           var alarmTime = document.getElementById('alarmTime').value;
// //           if (!alarmTime) {
// //             alert('Please select a time!');
// //             return;
// //           }
// //           var hours = alarmTime.split(':')[0];
// //           var minutes = alarmTime.split(':')[1];
// //           var xhr = new XMLHttpRequest();
// //           xhr.open('POST', '/setAlarm?hour=' + hours + '&minute=' + minutes, true);
// //           xhr.onload = function () {
// //             if (xhr.status === 200) {
// //               console.log(xhr.responseText);
// //               updateAlarmList();
// //             }
// //           };
// //           xhr.send();
// //           document.getElementById('alarmTime').value = '';
// //         }

// //         function deleteAlarm(hour, minute) {
// //           var xhr = new XMLHttpRequest();
// //           xhr.open('POST', '/deleteAlarm?hour=' + hour + '&minute=' + minute, true);
// //           xhr.onload = function () {
// //             if (xhr.status === 200) {
// //               console.log('Delete response: ' + xhr.responseText);
// //               updateAlarmList();
// //             } else {
// //               console.error('Delete failed with status: ' + xhr.status);
// //             }
// //           };
// //           xhr.send();
// //         }

// //         function editAlarm(oldHour, oldMinute) {
// //           var newTime = prompt('Enter new time (HH:MM):', oldHour + ':' + oldMinute);
// //           if (newTime) {
// //             var newHour = newTime.split(':')[0];
// //             var newMinute = newTime.split(':')[1];
// //             if (newHour && newMinute) {
// //               var xhr = new XMLHttpRequest();
// //               xhr.open('POST', '/editAlarm?oldHour=' + oldHour + '&oldMinute=' + oldMinute + '&newHour=' + newHour + '&newMinute=' + newMinute, true);
// //               xhr.onload = function () {
// //                 if (xhr.status === 200) {
// //                   console.log(xhr.responseText);
// //                   updateAlarmList();
// //                 }
// //               };
// //               xhr.send();
// //             } else {
// //               alert('Invalid time format!');
// //             }
// //           }
// //         }

// //         function stopAlarm() {
// //           var xhr = new XMLHttpRequest();
// //           xhr.open('POST', '/stopAlarm', true);
// //           xhr.onload = function () {
// //             if (xhr.status === 200) {
// //               console.log(xhr.responseText);
// //               document.getElementById('stopButton').style.display = 'none';
// //             }
// //           };
// //           xhr.send();
// //         }

// //         function updateAlarmList() {
// //           var alarmList = document.getElementById('alarmList');
// //           alarmList.innerHTML = '';
// //           var xhr = new XMLHttpRequest();
// //           xhr.open('GET', '/getAlarms', true);
// //           xhr.onload = function () {
// //             if (xhr.status === 200) {
// //               var alarms = JSON.parse(xhr.responseText);
// //               for (var i = 0; i < alarms.length; i++) {
// //                 var div = document.createElement('div');
// //                 div.style = 'margin:10px 0; padding:10px; background:#f9f9f9; border-radius:5px; display:flex; justify-content:space-between; align-items:center;';
// //                 div.textContent = 'Alarm: ' + alarms[i].hour + ':' + (alarms[i].minute < 10 ? '0' : '') + alarms[i].minute;
// //                 var editBtn = document.createElement('button');
// //                 editBtn.textContent = 'Edit';
// //                 editBtn.onclick = (function(hour, minute) {
// //                   return function() { editAlarm(hour, minute); };
// //                 })(alarms[i].hour, alarms[i].minute);
// //                 var deleteBtn = document.createElement('button');
// //                 deleteBtn.textContent = 'Delete';
// //                 deleteBtn.onclick = (function(hour, minute) {
// //                   return function() { deleteAlarm(hour, minute); };
// //                 })(alarms[i].hour, alarms[i].minute);
// //                 div.appendChild(editBtn);
// //                 div.appendChild(deleteBtn);
// //                 alarmList.appendChild(div);
// //               }
// //             } else {
// //               console.error('Failed to load alarms: ' + xhr.status);
// //             }
// //           };
// //           xhr.send();
// //         }
// //         window.onload = updateAlarmList;
// //       </script>
// //       <style>
// //         body {
// //           font-family: 'Segoe UI', Arial, sans-serif;
// //           background: linear-gradient(135deg, #e0e7ff, #f3e8ff);
// //           margin: 0;
// //           padding: 20px;
// //           color: #333;
// //         }

// //         .container {
// //           max-width: 1200px;
// //           margin: 0 auto;
// //           background: white;
// //           padding: 30px;
// //           border-radius: 15px;
// //           box-shadow: 0 4px 20px rgba(0, 0, 0, 0.1);
// //           display: flex;
// //           gap: 20px;
// //         }

// //         h1 {
// //           font-size: 2.2em;
// //           color: #1e3a8a;
// //           text-align: center;
// //           margin-bottom: 20px;
// //           text-transform: uppercase;
// //           letter-spacing: 1px;
// //         }

// //         .left-column {
// //           flex: 1;
// //           display: flex;
// //           flex-direction: column;
// //           gap: 20px;
// //         }

// //         .middle-column {
// //           flex: 1;
// //           display: flex;
// //           flex-direction: column;
// //           gap: 20px;
// //         }

// //         .middle-row {
// //           flex: 1;
// //           padding: 20px;
// //           background: #f9fafb;
// //           border-radius: 8px;
// //           border: 1px solid #e5e7eb;
// //         }

// //         .right-column {
// //           flex: 1;
// //           display: flex;
// //           flex-direction: column;
// //           gap: 20px;
// //         }

// //         p {
// //           font-size: 1.2em;
// //           margin: 10px 0;
// //           color: #4b5563;
// //         }

// //         #currentDate, #currentTime, #currentTemperature, #currentPPM {
// //           font-weight: bold;
// //           color: #1f2937;
// //         }

// //         #graph, #ppmGraph {
// //           display: block;
// //           margin: 0;
// //           border: 1px solid #e5e7eb;
// //           border-radius: 8px;
// //           box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
// //           width: 100%;
// //         }

// //         h3 {
// //           font-size: 1.5em;
// //           color: #1e3a8a;
// //           margin: 20px 0 10px;
// //           text-align: center;
// //         }

// //         input[type="time"] {
// //           padding: 8px;
// //           font-size: 1em;
// //           border: 1px solid #d1d5db;
// //           border-radius: 5px;
// //           margin-right: 10px;
// //           width: 150px;
// //         }

// //         button {
// //           padding: 10px 20px;
// //           font-size: 1em;
// //           border: none;
// //           border-radius: 5px;
// //           background: #3b82f6;
// //           color: white;
// //           cursor: pointer;
// //           transition: background 0.3s, transform 0.2s;
// //         }

// //         button:hover {
// //           background: #2563eb;
// //           transform: translateY(-2px);
// //         }

// //         #stopButton {
// //           display: none;
// //           background: #ef4444;
// //           margin: 20px auto;
// //           padding: 12px 24px;
// //         }

// //         #stopButton:hover {
// //           background: #dc2626;
// //         }

// //         #alarmList {
// //           margin: 20px 0;
// //         }

// //         .alarm-item {
// //           display: flex;
// //           justify-content: space-between;
// //           align-items: center;
// //           background: #f9fafb;
// //           padding: 12px;
// //           margin: 10px 0;
// //           border-radius: 8px;
// //           border: 1px solid #e5e7eb;
// //           transition: box-shadow 0.3s;
// //         }

// //         .alarm-item:hover {
// //           box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
// //         }

// //         .alarm-item button {
// //           padding: 8px 16px;
// //           font-size: 0.9em;
// //         }

// //         .alarm-item button:first-of-type {
// //           background: #10b981;
// //         }

// //         .alarm-item button:first-of-type:hover {
// //           background: #059669;
// //         }

// //         .alarm-item button:last-of-type {
// //           background: #ef4444;
// //         }

// //         .alarm-item button:last-of-type:hover {
// //           background: #dc2626;
// //         }

// //         /* Responsive design */
// //         @media (max-width: 900px) {
// //           .container {
// //             flex-direction: column;
// //             padding: 20px;
// //           }

// //           .left-column, .middle-column, .right-column {
// //             flex: none;
// //             width: 100%;
// //           }

// //           .middle-column {
// //             margin: 20px 0;
// //           }
// //         }
// //       </style>
// //     </head>
// //     <body>
// //       <div class="container">
// //         <div class="left-column">
// //           <h1>ESP8266 DHT11 & MQ135 Monitoring</h1>
// //           <img src="/graph" alt="Temperature Graph">
// //           <img src="/ppmGraph" alt="CO2 PPM Graph">
// //         </div>
// //         <div class="middle-column">
// //           <div class="middle-row">
// //             <p>Current Date: <span id="currentDate"></span></p>
// //             <p>Current Time: <span id="currentTime"></span> +07 (Vietnam Time)</p>
// //           </div>
// //           <div class="middle-row">
// //             <p>Current Temperature: <span id="currentTemperature"></span></p>
// //             <p>Current CO2: <span id="currentPPM"></span></p>
// //           </div>
// //         </div>
// //         <div class="right-column">
// //           <h3>Set Alarm</h3>
// //           <input type="time" id="alarmTime">
// //           <button onclick="setAlarm()">Set Alarm</button>
// //           <button id="stopButton" onclick="stopAlarm()">Stop Alarm</button>
// //           <h3>Alarm List</h3>
// //           <div id="alarmList"></div>
// //         </div>
// //       </div>
// //     </body>
// //     </html>
// //   )=====";
// //   server.send(200, "text/html", html);
// // }

// // // Serve data as JSON for real-time updates
// // void handleData() {
// //   String json = "{";
// //   json += "\"date\":\"" + getDate() + "\",";
// //   json += "\"time\":\"" + getTime() + "\",";
// //   json += "\"temperature\":" + String(temperatureData[DATA_POINTS - 1]) + ",";
// //   json += "\"ppm\":" + String(ppmData[DATA_POINTS - 1]);
// //   json += "}";
// //   server.send(200, "application/json", json);
// // }

// // // Draw the temperature graph (SVG) with HH:MM on X-axis
// // void drawGraph() {
// //   String out;
// //   out.reserve(2600);
// //   out += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"400\" height=\"200\">\n";
// //   out += "<rect width=\"400\" height=\"200\" fill=\"white\"/>\n";
// //   out += "<text x=\"200\" y=\"15\" font-size=\"14\" text-anchor=\"middle\">Temperature Values</text>\n";

// //   // Y-axis (temperature from 0-50°C)
// //   out += "<line x1=\"50\" y1=\"20\" x2=\"50\" y2=\"170\" stroke=\"black\"/>\n";
// //   for (int y = 0; y <= 50; y += 10) {
// //     int y_pos = 170 - (y * 3);
// //     out += "<line x1=\"50\" y1=\"" + String(y_pos) + "\" x2=\"55\" y2=\"" + String(y_pos) + "\" stroke=\"black\"/>\n";
// //     out += "<text x=\"20\" y=\"" + String(y_pos + 5) + "\" font-size=\"10\">" + String(y) + "°C</text>\n";
// //   }

// //   // X-axis (time with hours and minutes)
// //   out += "<line x1=\"50\" y1=\"170\" x2=\"380\" y2=\"170\" stroke=\"black\"/>\n";
// //   for (int i = 0; i < DATA_POINTS; i++) {
// //     if (timestamps[i] > 0) {
// //       int x_pos = 50 + (i * 33);
// //       DateTime timeLabel(timestamps[i]);
// //       String label = String(timeLabel.hour()) + ":" + (timeLabel.minute() < 10 ? "0" : "") + String(timeLabel.minute());
// //       out += "<text x=\"" + String(x_pos - 15) + "\" y=\"185\" font-size=\"10\">" + label + "</text>\n";
// //       out += "<line x1=\"" + String(x_pos) + "\" y1=\"170\" x2=\"" + String(x_pos) + "\" y2=\"175\" stroke=\"black\"/>\n";
// //     }
// //   }

// //   // Plot temperature data
// //   out += "<polyline points=\"";
// //   for (int i = 0; i < DATA_POINTS; i++) {
// //     if (temperatureData[i] != 0.0) {
// //       int x = 50 + (i * 33);
// //       int y = 170 - (temperatureData[i] * 3);
// //       out += String(x) + "," + String(y) + " ";
// //     }
// //   }
// //   out += "\" style=\"fill:none;stroke:blue;stroke-width:2\"/>\n";
// //   out += "</svg>\n";
// //   server.send(200, "image/svg+xml", out);
// // }

// // // Draw the CO2 PPM graph (SVG) with HH:MM on X-axis
// // void drawPPMGraph() {
// //   String out;
// //   out.reserve(2600);
// //   out += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"400\" height=\"200\">\n";
// //   out += "<rect width=\"400\" height=\"200\" fill=\"white\"/>\n";
// //   out += "<text x=\"200\" y=\"15\" font-size=\"14\" text-anchor=\"middle\">CO2 PPM Values</text>\n";

// //   // Y-axis (PPM from 0-300 ppm)
// //   out += "<line x1=\"50\" y1=\"20\" x2=\"50\" y2=\"170\" stroke=\"black\"/>\n";
// //   for (int y = 0; y <= 300; y += 50) {
// //     int y_pos = 170 - (y * 0.5);  // Scale: 300 ppm over 150px height
// //     out += "<line x1=\"50\" y1=\"" + String(y_pos) + "\" x2=\"55\" y2=\"" + String(y_pos) + "\" stroke=\"black\"/>\n";
// //     out += "<text x=\"20\" y=\"" + String(y_pos + 5) + "\" font-size=\"10\">" + String(y) + "ppm</text>\n";
// //   }

// //   // X-axis (time with hours and minutes)
// //   out += "<line x1=\"50\" y1=\"170\" x2=\"380\" y2=\"170\" stroke=\"black\"/>\n";
// //   for (int i = 0; i < DATA_POINTS; i++) {
// //     if (timestamps[i] > 0) {
// //       int x_pos = 50 + (i * 33);
// //       DateTime timeLabel(timestamps[i]);
// //       String label = String(timeLabel.hour()) + ":" + (timeLabel.minute() < 10 ? "0" : "") + String(timeLabel.minute());
// //       out += "<text x=\"" + String(x_pos - 15) + "\" y=\"185\" font-size=\"10\">" + label + "</text>\n";
// //       out += "<line x1=\"" + String(x_pos) + "\" y1=\"170\" x2=\"" + String(x_pos) + "\" y2=\"175\" stroke=\"black\"/>\n";
// //     }
// //   }

// //   // Plot PPM data
// //   out += "<polyline points=\"";
// //   for (int i = 0; i < DATA_POINTS; i++) {
// //     if (ppmData[i] != 0.0) {
// //       int x = 50 + (i * 33);
// //       int y = 170 - (ppmData[i] * 0.5);  // Scale: 300 ppm over 150px
// //       out += String(x) + "," + String(y) + " ";
// //     }
// //   }
// //   out += "\" style=\"fill:none;stroke:green;stroke-width:2\"/>\n";
// //   out += "</svg>\n";
// //   server.send(200, "image/svg+xml", out);
// // }

// // // Handler for setting an alarm
// // void handleAlarm() {
// //   if (server.hasArg("hour") && server.hasArg("minute")) {
// //     int hour = server.arg("hour").toInt();
// //     int minute = server.arg("minute").toInt();

// //     if (alarmCount < 10) {
// //       alarms[alarmCount] = { hour, minute, false, 0 };
// //       alarmCount++;
// //       server.send(200, "text/plain", "Alarm set: " + String(hour) + ":" + String(minute));
// //       Serial.println("Alarm set: " + String(hour) + ":" + String(minute));
// //     } else {
// //       server.send(400, "text/plain", "Alarm list is full!");
// //     }
// //   } else {
// //     server.send(400, "text/plain", "Missing hour or minute parameter");
// //   }
// // }

// // // Handler for deleting an alarm
// // void handleDeleteAlarm() {
// //   if (server.hasArg("hour") && server.hasArg("minute")) {
// //     int hour = server.arg("hour").toInt();
// //     int minute = server.arg("minute").toInt();
// //     Serial.print("Attempting to delete alarm: ");
// //     Serial.print(hour);
// //     Serial.print(":");
// //     Serial.println(minute);

// //     for (int i = 0; i < alarmCount; i++) {
// //       if (alarms[i].hour == hour && alarms[i].minute == minute) {
// //         Serial.print("Found alarm at index ");
// //         Serial.println(i);
// //         for (int j = i; j < alarmCount - 1; j++) {
// //           alarms[j] = alarms[j + 1];
// //         }
// //         alarmCount--;
// //         server.send(200, "text/plain", "Alarm deleted: " + String(hour) + ":" + String(minute));
// //         Serial.println("Alarm deleted: " + String(hour) + ":" + String(minute));
// //         return;
// //       }
// //     }
// //     server.send(404, "text/plain", "Alarm not found: " + String(hour) + ":" + String(minute));
// //     Serial.println("Alarm not found");
// //   } else {
// //     server.send(400, "text/plain", "Missing hour or minute parameter");
// //     Serial.println("Missing hour or minute parameter");
// //   }
// // }

// // // Handler for editing an alarm
// // void handleEditAlarm() {
// //   if (server.hasArg("oldHour") && server.hasArg("oldMinute") && server.hasArg("newHour") && server.hasArg("newMinute")) {
// //     int oldHour = server.arg("oldHour").toInt();
// //     int oldMinute = server.arg("oldMinute").toInt();
// //     int newHour = server.arg("newHour").toInt();
// //     int newMinute = server.arg("newMinute").toInt();

// //     for (int i = 0; i < alarmCount; i++) {
// //       if (alarms[i].hour == oldHour && alarms[i].minute == oldMinute) {
// //         alarms[i].hour = newHour;
// //         alarms[i].minute = newMinute;
// //         alarms[i].triggered = false;
// //         alarms[i].buzzStartTime = 0;
// //         server.send(200, "text/plain", "Alarm edited: " + String(oldHour) + ":" + String(oldMinute) + " to " + String(newHour) + ":" + String(newMinute));
// //         Serial.println("Alarm edited: " + String(oldHour) + ":" + String(oldMinute) + " to " + String(newHour) + ":" + String(newMinute));
// //         return;
// //       }
// //     }
// //     server.send(404, "text/plain", "Alarm not found: " + String(oldHour) + ":" + String(oldMinute));
// //   } else {
// //     server.send(400, "text/plain", "Missing oldHour, oldMinute, newHour, or newMinute parameter");
// //   }
// // }

// // // Handler to stop the buzzer
// // void handleStopAlarm() {
// //   if (buzzerActive) {
// //     digitalWrite(buzzer_pin, LOW);
// //     buzzerActive = false;
// //     for (int i = 0; i < alarmCount; i++) {
// //       if (alarms[i].buzzStartTime > 0) {
// //         alarms[i].buzzStartTime = 0;
// //       }
// //     }
// //     server.send(200, "text/plain", "Alarm stopped");
// //     Serial.println("Alarm stopped manually");
// //   } else {
// //     server.send(200, "text/plain", "No active alarm");
// //   }
// // }

// // // Handler to check buzzer state
// // void handleBuzzerState() {
// //   if (buzzerActive) {
// //     server.send(200, "text/plain", "active");
// //   } else {
// //     server.send(200, "text/plain", "inactive");
// //   }
// // }

// // // Handler to get alarm list
// // void handleGetAlarms() {
// //   String json = "[";
// //   for (int i = 0; i < alarmCount; i++) {
// //     json += "{\"hour\":" + String(alarms[i].hour) + ",\"minute\":" + String(alarms[i].minute) + "}";
// //     if (i < alarmCount - 1) json += ",";
// //   }
// //   json += "]";
// //   server.send(200, "application/json", json);
// // }

// // // Function to check and trigger alarms
// // void checkAlarms() {
// //   DateTime now = rtc.now();
// //   int currentHour = now.hour();
// //   int currentMinute = now.minute();
// //   int currentSecond = now.second();
// //   unsigned long currentMillis = millis();

// //   for (int i = 0; i < alarmCount; i++) {
// //     if (!alarms[i].triggered && alarms[i].hour == currentHour && alarms[i].minute == currentMinute && currentSecond < 5) {
// //       Serial.println("Alarm triggered: " + String(alarms[i].hour) + ":" + String(alarms[i].minute));
// //       digitalWrite(buzzer_pin, HIGH);
// //       buzzerActive = true;
// //       alarms[i].buzzStartTime = currentMillis;
// //       alarms[i].triggered = true;
// //     }

// //     // Check if buzzer should stop based on duration
// //     if (buzzerActive && alarms[i].buzzStartTime > 0 && (currentMillis - alarms[i].buzzStartTime >= (unsigned long)buzzerDuration)) {
// //       digitalWrite(buzzer_pin, LOW);
// //       buzzerActive = false;
// //       alarms[i].buzzStartTime = 0;
// //     }
// //   }
// // }
// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <DHT.h>
// #include "RTC_LCD.h"      // RTC library
// #include "LCD_Control.h"  // LCD library
// #include "MQ135.h"        // MQ135 library

// // WiFi credentials
// const char* ssid = "202";
// const char* password = "Nguyenthuyhanh1998";

// // DHT11 sensor setup
// #define DHTPIN D7
// #define DHTTYPE DHT11
// DHT dht(DHTPIN, DHTTYPE);
// byte degree[8] = {
//   0B01110,
//   0B01010,
//   0B01110,
//   0B00000,
//   0B00000,
//   0B00000,
//   0B00000,
//   0B00000
// };

// // MQ135 sensor setup
// #define PIN_MQ135 D6
// MQ135 mq135_sensor = MQ135(PIN_MQ135);

// // Web server setup
// ESP8266WebServer server(80);

// // Array to store the last 10 temperature and PPM readings and timestamps
// #define DATA_POINTS 10
// float temperatureData[DATA_POINTS];
// float ppmData[DATA_POINTS];
// unsigned long timestamps[DATA_POINTS];
// unsigned long lastUpdateTime = 0;
// const long updateInterval = 1000;  // Update every 1 second for real-time

// // GPIO D5 pin to control the buzzer
// const int buzzer_pin = D5;

// // Structure to store alarm information
// struct Alarm {
//   int hour;                     // Alarm hour
//   int minute;                   // Alarm minute
//   bool triggered;               // Whether the alarm has been triggered
//   unsigned long buzzStartTime;  // Time when buzzer starts (for millis)
// };
// Alarm alarms[10];                   // Array to store up to 10 alarms
// int alarmCount = 0;                 // Current number of alarms
// const int buzzerDuration = 300000;  // Fixed buzzer duration: 5 minutes (300,000 ms)
// bool buzzerActive = false;          // Flag to track if buzzer is currently active

// void setup() {
//   Serial.begin(115200);

//   // Initialize RTC and LCD
//   initRTC();
//   initLCD();
//   lcd.createChar(1, degree);

//   // Connect to WiFi
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nConnected to WiFi");
//   Serial.println(WiFi.localIP());

//   // Initialize DHT sensor
//   dht.begin();

//   // Initialize data arrays
//   for (int i = 0; i < DATA_POINTS; i++) {
//     temperatureData[i] = 0.0;
//     ppmData[i] = 0.0;
//     timestamps[i] = 0;
//   }

//   // Configure buzzer pin
//   pinMode(buzzer_pin, OUTPUT);
//   digitalWrite(buzzer_pin, LOW);

//   // Configure web server routes
//   server.on("/", handleRoot);
//   server.on("/graph", drawGraph);
//   server.on("/ppmGraph", drawPPMGraph);
//   server.on("/data", handleData);
//   server.on("/setAlarm", handleAlarm);
//   server.on("/deleteAlarm", handleDeleteAlarm);
//   server.on("/editAlarm", handleEditAlarm);
//   server.on("/stopAlarm", handleStopAlarm);
//   server.on("/buzzerState", handleBuzzerState);
//   server.on("/getAlarms", handleGetAlarms);
//   server.begin();
//   Serial.println("Web server started");
// }

// void loop() {
//   server.handleClient();  // Handle client requests

//   unsigned long currentMillis = millis();
//   if (currentMillis - lastUpdateTime >= updateInterval) {
//     lastUpdateTime = currentMillis;
//     DateTime now = rtc.now();

//     // Read temperature and humidity from DHT11 with retry
//     float temp = readTemperatureWithRetry();
//     float hum = dht.readHumidity();
//     if (isnan(hum)) {
//       hum = 0.0;  // Default humidity if reading fails
//       Serial.println("Failed to read humidity from DHT sensor!");
//     }

//     // Update temperature and PPM data
//     if (!isnan(temp)) {
//       // Shift temperature array and add new value
//       for (int i = 0; i < DATA_POINTS - 1; i++) {
//         temperatureData[i] = temperatureData[i + 1];
//         timestamps[i] = timestamps[i + 1];
//       }
//       temperatureData[DATA_POINTS - 1] = temp;
//       timestamps[DATA_POINTS - 1] = now.unixtime();

//       // Read corrected PPM from MQ135
//       float correctedPPM = mq135_sensor.getCorrectedPPM(temp, hum);

//       // Shift PPM array and add new value
//       for (int i = 0; i < DATA_POINTS - 1; i++) {
//         ppmData[i] = ppmData[i + 1];
//       }
//       ppmData[DATA_POINTS - 1] = correctedPPM;

//       Serial.print("Temperature: ");
//       Serial.print(temp);
//       Serial.print("°C, Humidity: ");
//       Serial.print(hum);
//       Serial.print("%, Corrected PPM: ");
//       Serial.print(correctedPPM);
//       Serial.println("ppm");
//     } else {
//       Serial.println("Failed to read temperature from DHT sensor after retries!");
//       // Shift data with default value if reading fails
//       for (int i = 0; i < DATA_POINTS - 1; i++) {
//         temperatureData[i] = temperatureData[i + 1];
//         timestamps[i] = timestamps[i + 1];
//       }
//       temperatureData[DATA_POINTS - 1] = 0.0;  // Default value
//       timestamps[DATA_POINTS - 1] = now.unixtime();
//     }

//     // Display date and time on LCD
//     displayDate(getDate());
//     displayTime(getTime());
//     lcd.setCursor(9, 1);  // Set cursor to line 1, column 9
//     lcd.print(temp, 1);   // Display temperature with 1 decimal place
//     lcd.write(1);
//     lcd.print("C");       // Add °C unit
//     checkAlarms();        // Check and trigger alarms
//   }
// }

// // Function to read temperature with retry
// float readTemperatureWithRetry() {
//   float temp = dht.readTemperature();
//   if (isnan(temp)) {
//     delay(100);  // Wait before retry
//     temp = dht.readTemperature();
//     if (isnan(temp)) {
//       delay(100);  // Second retry
//       temp = dht.readTemperature();
//     }
//   }
//   return temp;
// }

// // Serve the main web page with real-time updates
// void handleRoot() {
//   String html = R"=====(
//     <!DOCTYPE html>
//     <html>
//     <head>
//       <meta charset="utf-8">
//       <title>ESP8266 DHT11 & MQ135 Monitoring</title>
//       <script>
//         function updateData() {
//           var xhr = new XMLHttpRequest();
//           xhr.open('GET', '/data', true);
//           xhr.onload = function () {
//             if (xhr.status === 200) {
//               var data = JSON.parse(xhr.responseText);
//               document.getElementById('currentDate').innerText = data.date;
//               document.getElementById('currentTime').innerText = data.time;
//               document.getElementById('currentTemperature').innerText = data.temperature + "°C";
//               document.getElementById('currentPPM').innerText = data.ppm + "ppm";
//             }
//           };
//           xhr.send();
//         }
//         setInterval(updateData, 1000);
//         updateData();

//         function checkBuzzerState() {
//           var xhr = new XMLHttpRequest();
//           xhr.open('GET', '/buzzerState', true);
//           xhr.onload = function () {
//             if (xhr.status === 200) {
//               var stopButton = document.getElementById('stopButton');
//               if (xhr.responseText === 'active') {
//                 stopButton.style.display = 'block';
//               } else {
//                 stopButton.style.display = 'none';
//               }
//             }
//           };
//           xhr.send();
//         }
//         setInterval(checkBuzzerState, 1000);

//         function setAlarm() {
//           var alarmTime = document.getElementById('alarmTime').value;
//           if (!alarmTime) {
//             alert('Please select a time!');
//             return;
//           }
//           var hours = alarmTime.split(':')[0];
//           var minutes = alarmTime.split(':')[1];
//           var xhr = new XMLHttpRequest();
//           xhr.open('POST', '/setAlarm?hour=' + hours + '&minute=' + minutes, true);
//           xhr.onload = function () {
//             if (xhr.status === 200) {
//               console.log(xhr.responseText);
//               updateAlarmList();
//             }
//           };
//           xhr.send();
//           document.getElementById('alarmTime').value = '';
//         }

//         function deleteAlarm(hour, minute) {
//           var xhr = new XMLHttpRequest();
//           xhr.open('POST', '/deleteAlarm?hour=' + hour + '&minute=' + minute, true);
//           xhr.onload = function () {
//             if (xhr.status === 200) {
//               console.log('Delete response: ' + xhr.responseText);
//               updateAlarmList();
//             } else {
//               console.error('Delete failed with status: ' + xhr.status);
//             }
//           };
//           xhr.send();
//         }

//         function editAlarm(oldHour, oldMinute) {
//           var newTime = prompt('Enter new time (HH:MM):', oldHour + ':' + oldMinute);
//           if (newTime) {
//             var newHour = newTime.split(':')[0];
//             var newMinute = newTime.split(':')[1];
//             if (newHour && newMinute) {
//               var xhr = new XMLHttpRequest();
//               xhr.open('POST', '/editAlarm?oldHour=' + oldHour + '&oldMinute=' + oldMinute + '&newHour=' + newHour + '&newMinute=' + newMinute, true);
//               xhr.onload = function () {
//                 if (xhr.status === 200) {
//                   console.log(xhr.responseText);
//                   updateAlarmList();
//                 }
//               };
//               xhr.send();
//             } else {
//               alert('Invalid time format!');
//             }
//           }
//         }

//         function stopAlarm() {
//           var xhr = new XMLHttpRequest();
//           xhr.open('POST', '/stopAlarm', true);
//           xhr.onload = function () {
//             if (xhr.status === 200) {
//               console.log(xhr.responseText);
//               document.getElementById('stopButton').style.display = 'none';
//             }
//           };
//           xhr.send();
//         }

//         function updateAlarmList() {
//           var alarmList = document.getElementById('alarmList');
//           alarmList.innerHTML = '';
//           var xhr = new XMLHttpRequest();
//           xhr.open('GET', '/getAlarms', true);
//           xhr.onload = function () {
//             if (xhr.status === 200) {
//               var alarms = JSON.parse(xhr.responseText);
//               for (var i = 0; i < alarms.length; i++) {
//                 var div = document.createElement('div');
//                 div.style = 'margin:10px 0; padding:10px; background:#f9f9f9; border-radius:5px; display:flex; justify-content:space-between; align-items:center;';
//                 div.textContent = 'Alarm: ' + alarms[i].hour + ':' + (alarms[i].minute < 10 ? '0' : '') + alarms[i].minute;
//                 var editBtn = document.createElement('button');
//                 editBtn.textContent = 'Edit';
//                 editBtn.onclick = (function(hour, minute) {
//                   return function() { editAlarm(hour, minute); };
//                 })(alarms[i].hour, alarms[i].minute);
//                 var deleteBtn = document.createElement('button');
//                 deleteBtn.textContent = 'Delete';
//                 deleteBtn.onclick = (function(hour, minute) {
//                   return function() { deleteAlarm(hour, minute); };
//                 })(alarms[i].hour, alarms[i].minute);
//                 div.appendChild(editBtn);
//                 div.appendChild(deleteBtn);
//                 alarmList.appendChild(div);
//               }
//             } else {
//               console.error('Failed to load alarms: ' + xhr.status);
//             }
//           };
//           xhr.send();
//         }
//         window.onload = updateAlarmList;
//       </script>
//       <style>
//         body {
//           font-family: 'Segoe UI', Arial, sans-serif;
//           background: linear-gradient(135deg, #e0e7ff, #f3e8ff);
//           margin: 0;
//           padding: 20px;
//           color: #333;
//         }

//         .container {
//           max-width: 1200px;
//           margin: 0 auto;
//           background: white;
//           padding: 30px;
//           border-radius: 15px;
//           box-shadow: 0 4px 20px rgba(0, 0, 0, 0.1);
//           display: flex;
//           gap: 20px;
//         }

//         h1 {
//           font-size: 2.2em;
//           color: #1e3a8a;
//           text-align: center;
//           margin-bottom: 20px;
//           text-transform: uppercase;
//           letter-spacing: 1px;
//         }

//         .left-column {
//           flex: 1;
//           display: flex;
//           flex-direction: column;
//           gap: 20px;
//         }

//         .middle-column {
//           flex: 1;
//           display: flex;
//           flex-direction: column;
//           gap: 20px;
//         }

//         .middle-row {
//           flex: 1;
//           padding: 20px;
//           background: #f9fafb;
//           border-radius: 8px;
//           border: 1px solid #e5e7eb;
//         }

//         .right-column {
//           flex: 1;
//           display: flex;
//           flex-direction: column;
//           gap: 20px;
//         }

//         p {
//           font-size: 1.2em;
//           margin: 10px 0;
//           color: #4b5563;
//         }

//         #currentDate, #currentTime, #currentTemperature, #currentPPM {
//           font-weight: bold;
//           color: #1f2937;
//         }

//         #graph, #ppmGraph {
//           display: block;
//           margin: 0;
//           border: 1px solid #e5e7eb;
//           border-radius: 8px;
//           box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
//           width: 100%;
//         }

//         h3 {
//           font-size: 1.5em;
//           color: #1e3a8a;
//           margin: 20px 0 10px;
//           text-align: center;
//         }

//         input[type="time"] {
//           padding: 10px 20px;
//           font-size: 1em;
//           border: 1px solid #d1d5db;
//           border-radius: 5px;
//           margin-right: 10px;
//           width: auto;
//         }

//         button {
//           padding: 10px 20px;
//           font-size: 1em;
//           border: none;
//           border-radius: 5px;
//           background: #3b82f6;
//           color: white;
//           cursor: pointer;
//           transition: background 0.3s, transform 0.2s;
//         }

//         button:hover {
//           background: #2563eb;
//           transform: translateY(-2px);
//         }

//         #stopButton {
//           display: none;
//           background: #ef4444;
//           margin: 20px auto;
//           padding: 12px 24px;
//         }

//         #stopButton:hover {
//           background: #dc2626;
//         }

//         #alarmList {
//           margin: 20px 0;
//         }

//         .alarm-item {
//           display: flex;
//           justify-content: space-between;
//           align-items: center;
//           background: #f9fafb;
//           padding: 12px;
//           margin: 10px 0;
//           border-radius: 8px;
//           border: 1px solid #e5e7eb;
//           transition: box-shadow 0.3s;
//         }

//         .alarm-item:hover {
//           box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
//         }

//         .alarm-item button {
//           padding: 8px 16px;
//           font-size: 0.9em;
//         }

//         .alarm-item button:first-of-type {
//           background: #10b981;
//         }

//         .alarm-item button:first-of-type:hover {
//           background: #059669;
//         }

//         .alarm-item button:last-of-type {
//           background: #ef4444;
//         }

//         .alarm-item button:last-of-type:hover {
//           background: #dc2626;
//         }

//         /* Responsive design */
//         @media (max-width: 900px) {
//           .container {
//             flex-direction: column;
//             padding: 20px;
//           }

//           .left-column, .middle-column, .right-column {
//             flex: none;
//             width: 100%;
//           }

//           .middle-column {
//             margin: 20px 0;
//           }
//         }
//       </style>
//     </head>
//     <body>
//       <div class="container">
//         <div class="left-column">
//           <h1>ESP8266 DHT11 & MQ135 Monitoring</h1>
//           <img src="/graph" alt="Temperature Graph">
//           <img src="/ppmGraph" alt="CO2 PPM Graph">
//         </div>
//         <div class="middle-column">
//           <div class="middle-row">
//             <p>Current Date: <span id="currentDate"></span></p>
//             <p>Current Time: <span id="currentTime"></span> +07 (Vietnam Time)</p>
//           </div>
//           <div class="middle-row">
//             <p>Current Temperature: <span id="currentTemperature"></span></p>
//             <p>Current CO2: <span id="currentPPM"></span></p>
//           </div>
//         </div>
//         <div class="right-column">
//           <h3>Set Alarm</h3>
//           <input type="time" id="alarmTime">
//           <button onclick="setAlarm()">Set Alarm</button>
//           <button id="stopButton" onclick="stopAlarm()">Stop Alarm</button>
//           <h3>Alarm List</h3>
//           <div id="alarmList"></div>
//         </div>
//       </div>
//     </body>
//     </html>
//   )=====";
//   server.send(200, "text/html", html);
// }

// // Serve data as JSON for real-time updates
// void handleData() {
//   String json = "{";
//   json += "\"date\":\"" + getDate() + "\",";
//   json += "\"time\":\"" + getTime() + "\",";
//   json += "\"temperature\":" + String(temperatureData[DATA_POINTS - 1]) + ",";
//   json += "\"ppm\":" + String(ppmData[DATA_POINTS - 1]);
//   json += "}";
//   server.send(200, "application/json", json);
// }

// // Draw the temperature graph (SVG) with HH:MM on X-axis
// void drawGraph() {
//   String out;
//   out.reserve(2600);
//   out += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"400\" height=\"200\">\n";
//   out += "<rect width=\"400\" height=\"200\" fill=\"white\"/>\n";
//   out += "<text x=\"200\" y=\"15\" font-size=\"14\" text-anchor=\"middle\">Temperature Values</text>\n";

//   // Y-axis (temperature from 0-50°C)
//   out += "<line x1=\"50\" y1=\"20\" x2=\"50\" y2=\"170\" stroke=\"black\"/>\n";
//   for (int y = 0; y <= 50; y += 10) {
//     int y_pos = 170 - (y * 3);
//     out += "<line x1=\"50\" y1=\"" + String(y_pos) + "\" x2=\"55\" y2=\"" + String(y_pos) + "\" stroke=\"black\"/>\n";
//     out += "<text x=\"20\" y=\"" + String(y_pos + 5) + "\" font-size=\"10\">" + String(y) + "°C</text>\n";
//   }

//   // X-axis (time with hours and minutes)
//   out += "<line x1=\"50\" y1=\"170\" x2=\"380\" y2=\"170\" stroke=\"black\"/>\n";
//   for (int i = 0; i < DATA_POINTS; i++) {
//     if (timestamps[i] > 0) {
//       int x_pos = 50 + (i * 33);
//       DateTime timeLabel(timestamps[i]);
//       String label = String(timeLabel.hour()) + ":" + (timeLabel.minute() < 10 ? "0" : "") + String(timeLabel.minute());
//       out += "<text x=\"" + String(x_pos - 15) + "\" y=\"185\" font-size=\"10\">" + label + "</text>\n";
//       out += "<line x1=\"" + String(x_pos) + "\" y1=\"170\" x2=\"" + String(x_pos) + "\" y2=\"175\" stroke=\"black\"/>\n";
//     }
//   }

//   // Plot temperature data
//   out += "<polyline points=\"";
//   for (int i = 0; i < DATA_POINTS; i++) {
//     if (temperatureData[i] != 0.0) {
//       int x = 50 + (i * 33);
//       int y = 170 - (temperatureData[i] * 3);
//       out += String(x) + "," + String(y) + " ";
//     }
//   }
//   out += "\" style=\"fill:none;stroke:blue;stroke-width:2\"/>\n";
//   out += "</svg>\n";
//   server.send(200, "image/svg+xml", out);
// }

// // Draw the CO2 PPM graph (SVG) with HH:MM on X-axis
// void drawPPMGraph() {
//   String out;
//   out.reserve(2600);
//   out += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"400\" height=\"200\">\n";
//   out += "<rect width=\"400\" height=\"200\" fill=\"white\"/>\n";
//   out += "<text x=\"200\" y=\"15\" font-size=\"14\" text-anchor=\"middle\">CO2 PPM Values</text>\n";

//   // Y-axis (PPM from 0-300 ppm)
//   out += "<line x1=\"50\" y1=\"20\" x2=\"50\" y2=\"170\" stroke=\"black\"/>\n";
//   for (int y = 0; y <= 300; y += 50) {
//     int y_pos = 170 - (y * 0.5);  // Scale: 300 ppm over 150px height
//     out += "<line x1=\"50\" y1=\"" + String(y_pos) + "\" x2=\"55\" y2=\"" + String(y_pos) + "\" stroke=\"black\"/>\n";
//     out += "<text x=\"20\" y=\"" + String(y_pos + 5) + "\" font-size=\"10\">" + String(y) + "ppm</text>\n";
//   }

//   // X-axis (time with hours and minutes)
//   out += "<line x1=\"50\" y1=\"170\" x2=\"380\" y2=\"170\" stroke=\"black\"/>\n";
//   for (int i = 0; i < DATA_POINTS; i++) {
//     if (timestamps[i] > 0) {
//       int x_pos = 50 + (i * 33);
//       DateTime timeLabel(timestamps[i]);
//       String label = String(timeLabel.hour()) + ":" + (timeLabel.minute() < 10 ? "0" : "") + String(timeLabel.minute());
//       out += "<text x=\"" + String(x_pos - 15) + "\" y=\"185\" font-size=\"10\">" + label + "</text>\n";
//       out += "<line x1=\"" + String(x_pos) + "\" y1=\"170\" x2=\"" + String(x_pos) + "\" y2=\"175\" stroke=\"black\"/>\n";
//     }
//   }

//   // Plot PPM data
//   out += "<polyline points=\"";
//   for (int i = 0; i < DATA_POINTS; i++) {
//     if (ppmData[i] != 0.0) {
//       int x = 50 + (i * 33);
//       int y = 170 - (ppmData[i] * 0.5);  // Scale: 300 ppm over 150px
//       out += String(x) + "," + String(y) + " ";
//     }
//   }
//   out += "\" style=\"fill:none;stroke:green;stroke-width:2\"/>\n";
//   out += "</svg>\n";
//   server.send(200, "image/svg+xml", out);
// }

// // Handler for setting an alarm
// void handleAlarm() {
//   if (server.hasArg("hour") && server.hasArg("minute")) {
//     int hour = server.arg("hour").toInt();
//     int minute = server.arg("minute").toInt();

//     if (alarmCount < 10) {
//       alarms[alarmCount] = { hour, minute, false, 0 };
//       alarmCount++;
//       server.send(200, "text/plain", "Alarm set: " + String(hour) + ":" + String(minute));
//       Serial.println("Alarm set: " + String(hour) + ":" + String(minute));
//     } else {
//       server.send(400, "text/plain", "Alarm list is full!");
//     }
//   } else {
//     server.send(400, "text/plain", "Missing hour or minute parameter");
//   }
// }

// // Handler for deleting an alarm
// void handleDeleteAlarm() {
//   if (server.hasArg("hour") && server.hasArg("minute")) {
//     int hour = server.arg("hour").toInt();
//     int minute = server.arg("minute").toInt();
//     Serial.print("Attempting to delete alarm: ");
//     Serial.print(hour);
//     Serial.print(":");
//     Serial.println(minute);

//     for (int i = 0; i < alarmCount; i++) {
//       if (alarms[i].hour == hour && alarms[i].minute == minute) {
//         Serial.print("Found alarm at index ");
//         Serial.println(i);
//         for (int j = i; j < alarmCount - 1; j++) {
//           alarms[j] = alarms[j + 1];
//         }
//         alarmCount--;
//         server.send(200, "text/plain", "Alarm deleted: " + String(hour) + ":" + String(minute));
//         Serial.println("Alarm deleted: " + String(hour) + ":" + String(minute));
//         return;
//       }
//     }
//     server.send(404, "text/plain", "Alarm not found: " + String(hour) + ":" + String(minute));
//     Serial.println("Alarm not found");
//   } else {
//     server.send(400, "text/plain", "Missing hour or minute parameter");
//     Serial.println("Missing hour or minute parameter");
//   }
// }

// // Handler for editing an alarm
// void handleEditAlarm() {
//   if (server.hasArg("oldHour") && server.hasArg("oldMinute") && server.hasArg("newHour") && server.hasArg("newMinute")) {
//     int oldHour = server.arg("oldHour").toInt();
//     int oldMinute = server.arg("oldMinute").toInt();
//     int newHour = server.arg("newHour").toInt();
//     int newMinute = server.arg("newMinute").toInt();

//     for (int i = 0; i < alarmCount; i++) {
//       if (alarms[i].hour == oldHour && alarms[i].minute == oldMinute) {
//         alarms[i].hour = newHour;
//         alarms[i].minute = newMinute;
//         alarms[i].triggered = false;
//         alarms[i].buzzStartTime = 0;
//         server.send(200, "text/plain", "Alarm edited: " + String(oldHour) + ":" + String(oldMinute) + " to " + String(newHour) + ":" + String(newMinute));
//         Serial.println("Alarm edited: " + String(oldHour) + ":" + String(oldMinute) + " to " + String(newHour) + ":" + String(newMinute));
//         return;
//       }
//     }
//     server.send(404, "text/plain", "Alarm not found: " + String(oldHour) + ":" + String(oldMinute));
//   } else {
//     server.send(400, "text/plain", "Missing oldHour, oldMinute, newHour, or newMinute parameter");
//   }
// }

// // Handler to stop the buzzer
// void handleStopAlarm() {
//   if (buzzerActive) {
//     digitalWrite(buzzer_pin, LOW);
//     buzzerActive = false;
//     for (int i = 0; i < alarmCount; i++) {
//       if (alarms[i].buzzStartTime > 0) {
//         alarms[i].buzzStartTime = 0;
//       }
//     }
//     server.send(200, "text/plain", "Alarm stopped");
//     Serial.println("Alarm stopped manually");
//   } else {
//     server.send(200, "text/plain", "No active alarm");
//   }
// }

// // Handler to check buzzer state
// void handleBuzzerState() {
//   if (buzzerActive) {
//     server.send(200, "text/plain", "active");
//   } else {
//     server.send(200, "text/plain", "inactive");
//   }
// }

// // Handler to get alarm list
// void handleGetAlarms() {
//   String json = "[";
//   for (int i = 0; i < alarmCount; i++) {
//     json += "{\"hour\":" + String(alarms[i].hour) + ",\"minute\":" + String(alarms[i].minute) + "}";
//     if (i < alarmCount - 1) json += ",";
//   }
//   json += "]";
//   server.send(200, "application/json", json);
// }

// // Function to check and trigger alarms
// void checkAlarms() {
//   DateTime now = rtc.now();
//   int currentHour = now.hour();
//   int currentMinute = now.minute();
//   int currentSecond = now.second();
//   unsigned long currentMillis = millis();

//   for (int i = 0; i < alarmCount; i++) {
//     if (!alarms[i].triggered && alarms[i].hour == currentHour && alarms[i].minute == currentMinute && currentSecond < 5) {
//       Serial.println("Alarm triggered: " + String(alarms[i].hour) + ":" + String(alarms[i].minute));
//       digitalWrite(buzzer_pin, HIGH);
//       buzzerActive = true;
//       alarms[i].buzzStartTime = currentMillis;
//       alarms[i].triggered = true;
//     }

//     // Check if buzzer should stop based on duration
//     if (buzzerActive && alarms[i].buzzStartTime > 0 && (currentMillis - alarms[i].buzzStartTime >= (unsigned long)buzzerDuration)) {
//       digitalWrite(buzzer_pin, LOW);
//       buzzerActive = false;
//       alarms[i].buzzStartTime = 0;
//     }
//   }
// }

#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include "RTC_LCD.h"      // RTC library
#include "LCD_Control.h"  // LCD library
#include "MQ135.h"        // MQ135 library

// WiFi credentials
const char* ssid = "202";
const char* password = "Nguyenthuyhanh1998";

// DHT11 sensor setup
#define DHTPIN D7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

// MQ135 sensor setup
#define PIN_MQ135 A0
MQ135 mq135_sensor = MQ135(PIN_MQ135);

// Web server setup
ESP8266WebServer server(80);

// Array to store the last 10 temperature and PPM readings and timestamps
#define DATA_POINTS 10
float temperatureData[DATA_POINTS];
float ppmData[DATA_POINTS];
unsigned long timestamps[DATA_POINTS];
unsigned long lastUpdateTime = 0;
const long updateInterval = 1000;  // Update every 1 second for real-time

// GPIO D5 pin to control the buzzer
const int buzzer_pin = D5;

// Structure to store alarm information
struct Alarm {
  int hour;                     // Alarm hour
  int minute;                   // Alarm minute
  bool triggered;               // Whether the alarm has been triggered
  unsigned long buzzStartTime;  // Time when buzzer starts (for millis)
};
Alarm alarms[10];                   // Array to store up to 10 alarms
int alarmCount = 0;                 // Current number of alarms
const int buzzerDuration = 300000;  // Fixed buzzer duration: 5 minutes (300,000 ms)
bool buzzerActive = false;          // Flag to track if buzzer is currently active

void setup() {
  Serial.begin(115200);
  // Initialize WiFiManager to connect to WiFi
  WiFiManager wm;
  bool res = wm.autoConnect("DigitalClock", "12345678");  // Create AP with name and password
  // Initialize RTC and LCD
  initRTC();
  initLCD();
  
  lcd.createChar(1, degree);
  lcd.backlight();      // Turn on LCD backlight
  lcd.setCursor(1, 0);  // Set cursor to start of line 1
  lcd.print("DIGITAL CLOCK");
  delay(5000);

  if (!res) {  // If WiFi connection fails
    lcd.print("Failed to connect");
    Serial.println("Failed to connect");
    while (1) delay(1000);  // Halt program
  } else {                  // If connection succeeds
    lcd.clear();
    lcd.print("Connected, IP:");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP().toString());  // Display IP on LCD
    Serial.println("connected...yeey :)");
    Serial.println("IP: " + WiFi.localIP().toString());
    delay(5000);
  }

  // Initialize DHT sensor
  dht.begin();

  // Initialize data arrays
  for (int i = 0; i < DATA_POINTS; i++) {
    temperatureData[i] = 0.0;
    ppmData[i] = 0.0;
    timestamps[i] = 0;
  }

  // Configure buzzer pin
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, LOW);

  // Configure web server routes
  server.on("/", handleRoot);
  server.on("/graph", drawGraph);
  server.on("/ppmGraph", drawPPMGraph);
  server.on("/data", handleData);
  server.on("/setAlarm", handleAlarm);
  server.on("/deleteAlarm", handleDeleteAlarm);
  server.on("/editAlarm", handleEditAlarm);
  server.on("/stopAlarm", handleStopAlarm);
  server.on("/buzzerState", handleBuzzerState);
  server.on("/getAlarms", handleGetAlarms);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();  // Handle client requests

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;
    DateTime now = rtc.now();

    // Read temperature and humidity from DHT11 with retry
    float temp = readTemperatureWithRetry();
    float hum = dht.readHumidity();
    if (isnan(hum)) {
      hum = 0.0;  // Default humidity if reading fails
      Serial.println("Failed to read humidity from DHT sensor!");
    }

    // Update temperature and PPM data
    if (!isnan(temp)) {
      // Shift temperature array and add new value
      for (int i = 0; i < DATA_POINTS - 1; i++) {
        temperatureData[i] = temperatureData[i + 1];
        timestamps[i] = timestamps[i + 1];
      }
      temperatureData[DATA_POINTS - 1] = temp;
      timestamps[DATA_POINTS - 1] = now.unixtime();

      // Read corrected PPM from MQ135
      float correctedPPM = mq135_sensor.getCorrectedPPM(temp, hum);

      // Shift PPM array and add new value
      for (int i = 0; i < DATA_POINTS - 1; i++) {
        ppmData[i] = ppmData[i + 1];
      }
      ppmData[DATA_POINTS - 1] = correctedPPM;

      Serial.print("Temperature: ");
      Serial.print(temp);
      Serial.print("°C, Humidity: ");
      Serial.print(hum);
      Serial.print("%, Corrected PPM: ");
      Serial.print(correctedPPM);
      Serial.println("ppm");
    } else {
      Serial.println("Failed to read temperature from DHT sensor after retries!");
      // Shift data with default value if reading fails
      for (int i = 0; i < DATA_POINTS - 1; i++) {
        temperatureData[i] = temperatureData[i + 1];
        timestamps[i] = timestamps[i + 1];
      }
      temperatureData[DATA_POINTS - 1] = 0.0;  // Default value
      timestamps[DATA_POINTS - 1] = now.unixtime();
    }

    // Display date and time on LCD
    displayDate(getDate());
    displayTime(getTime());
    lcd.setCursor(9, 1);  // Set cursor to line 1, column 9
    lcd.print(temp, 1);   // Display temperature with 1 decimal place
    lcd.write(1);
    lcd.print("C");  // Add °C unit
    checkAlarms();   // Check and trigger alarms
  }
}

// Function to read temperature with retry
float readTemperatureWithRetry() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    delay(100);  // Wait before retry
    temp = dht.readTemperature();
    if (isnan(temp)) {
      delay(100);  // Second retry
      temp = dht.readTemperature();
    }
  }
  return temp;
}

// Serve the main web page with real-time updates
void handleRoot() {
  String html = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="utf-8">
      <title>ESP8266 DHT11 & MQ135 Monitoring</title>
      <script>
        function updateData() {
          var xhr = new XMLHttpRequest();
          xhr.open('GET', '/data', true);
          xhr.onload = function () {
            if (xhr.status === 200) {
              var data = JSON.parse(xhr.responseText);
              document.getElementById('currentDate').innerText = data.date;
              document.getElementById('currentTime').innerText = data.time;
              document.getElementById('currentTemperature').innerText = data.temperature + "°C";
              document.getElementById('currentPPM').innerText = data.ppm + "ppm";
            }
          };
          xhr.send();
        }
        setInterval(updateData, 1000);
        updateData();

        function checkBuzzerState() {
          var xhr = new XMLHttpRequest();
          xhr.open('GET', '/buzzerState', true);
          xhr.onload = function () {
            if (xhr.status === 200) {
              var stopButton = document.getElementById('stopButton');
              if (xhr.responseText === 'active') {
                stopButton.style.display = 'block';
              } else {
                stopButton.style.display = 'none';
              }
            }
          };
          xhr.send();
        }
        setInterval(checkBuzzerState, 1000);

        function setAlarm() {
          var alarmTime = document.getElementById('alarmTime').value;
          if (!alarmTime) {
            alert('Please select a time!');
            return;
          }
          var hours = alarmTime.split(':')[0];
          var minutes = alarmTime.split(':')[1];
          var xhr = new XMLHttpRequest();
          xhr.open('POST', '/setAlarm?hour=' + hours + '&minute=' + minutes, true);
          xhr.onload = function () {
            if (xhr.status === 200) {
              console.log(xhr.responseText);
              updateAlarmList();
            }
          };
          xhr.send();
          document.getElementById('alarmTime').value = '';
        }

        function deleteAlarm(hour, minute) {
          var xhr = new XMLHttpRequest();
          xhr.open('POST', '/deleteAlarm?hour=' + hour + '&minute=' + minute, true);
          xhr.onload = function () {
            if (xhr.status === 200) {
              console.log('Delete response: ' + xhr.responseText);
              updateAlarmList();
            } else {
              console.error('Delete failed with status: ' + xhr.status);
            }
          };
          xhr.send();
        }

        function editAlarm(oldHour, oldMinute) {
          var newTime = prompt('Enter new time (HH:MM):', oldHour + ':' + oldMinute);
          if (newTime) {
            var newHour = newTime.split(':')[0];
            var newMinute = newTime.split(':')[1];
            if (newHour && newMinute) {
              var xhr = new XMLHttpRequest();
              xhr.open('POST', '/editAlarm?oldHour=' + oldHour + '&oldMinute=' + oldMinute + '&newHour=' + newHour + '&newMinute=' + newMinute, true);
              xhr.onload = function () {
                if (xhr.status === 200) {
                  console.log(xhr.responseText);
                  updateAlarmList();
                }
              };
              xhr.send();
            } else {
              alert('Invalid time format!');
            }
          }
        }

        function stopAlarm() {
          var xhr = new XMLHttpRequest();
          xhr.open('POST', '/stopAlarm', true);
          xhr.onload = function () {
            if (xhr.status === 200) {
              console.log(xhr.responseText);
              document.getElementById('stopButton').style.display = 'none';
            }
          };
          xhr.send();
        }

        function updateAlarmList() {
          var alarmList = document.getElementById('alarmList');
          alarmList.innerHTML = '';
          var xhr = new XMLHttpRequest();
          xhr.open('GET', '/getAlarms', true);
          xhr.onload = function () {
            if (xhr.status === 200) {
              var alarms = JSON.parse(xhr.responseText);
              for (var i = 0; i < alarms.length; i++) {
                var div = document.createElement('div');
                div.style = 'margin:10px 0; padding:10px; background:#f9f9f9; border-radius:5px; display:flex; justify-content:space-between; align-items:center;';
                div.textContent = 'Alarm: ' + alarms[i].hour + ':' + (alarms[i].minute < 10 ? '0' : '') + alarms[i].minute;
                var editBtn = document.createElement('button');
                editBtn.textContent = 'Edit';
                editBtn.onclick = (function(hour, minute) {
                  return function() { editAlarm(hour, minute); };
                })(alarms[i].hour, alarms[i].minute);
                var deleteBtn = document.createElement('button');
                deleteBtn.textContent = 'Delete';
                deleteBtn.onclick = (function(hour, minute) {
                  return function() { deleteAlarm(hour, minute); };
                })(alarms[i].hour, alarms[i].minute);
                div.appendChild(editBtn);
                div.appendChild(deleteBtn);
                alarmList.appendChild(div);
              }
            } else {
              console.error('Failed to load alarms: ' + xhr.status);
            }
          };
          xhr.send();
        }
        window.onload = updateAlarmList;
      </script>
      <style>
        body {
          font-family: 'Segoe UI', Arial, sans-serif;
          background: linear-gradient(135deg, #e0e7ff, #f3e8ff);
          margin: 0;
          padding: 20px;
          color: #333;
        }

        .container {
          max-width: 1200px;
          margin: 0 auto;
          background: white;
          padding: 30px;
          border-radius: 15px;
          box-shadow: 0 4px 20px rgba(0, 0, 0, 0.1);
          display: flex;
          gap: 20px;
        }

        h1 {
          font-size: 2.2em;
          color: #1e3a8a;
          text-align: center;
          margin-bottom: 20px;
          text-transform: uppercase;
          letter-spacing: 1px;
        }

        .left-column {
          flex: 1;
          display: flex;
          flex-direction: column;
          gap: 20px;
        }

        .middle-column {
          flex: 1;
          display: flex;
          flex-direction: column;
          gap: 20px;
        }

        .middle-row {
          flex: 1;
          padding: 20px;
          background: #f9fafb;
          border-radius: 8px;
          border: 1px solid #e5e7eb;
        }

        .right-column {
          flex: 1;
          display: flex;
          flex-direction: column;
          gap: 20px;
        }

        p {
          font-size: 1.2em;
          margin: 10px 0;
          color: #4b5563;
        }

        #currentDate, #currentTime, #currentTemperature, #currentPPM {
          font-weight: bold;
          color: #1f2937;
        }

        #graph, #ppmGraph {
          display: block;
          margin: 0;
          border: 1px solid #e5e7eb;
          border-radius: 8px;
          box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
          width: 100%;
        }

        h3 {
          font-size: 1.5em;
          color: #1e3a8a;
          margin: 20px 0 10px;
          text-align: center;
        }

        input[type="time"] {
          padding: 10px 20px;
          font-size: 1em;
          border: 1px solid #d1d5db;
          border-radius: 5px;
          margin-right: 10px;
          width: auto;
        }

        button {
          padding: 10px 20px;
          font-size: 1em;
          border: none;
          border-radius: 5px;
          background: #3b82f6;
          color: white;
          cursor: pointer;
          transition: background 0.3s, transform 0.2s;
        }

        button:hover {
          background: #2563eb;
          transform: translateY(-2px);
        }

        #stopButton {
          display: none;
          background: #ef4444;
          margin: 20px auto;
          padding: 12px 24px;
        }

        #stopButton:hover {
          background: #dc2626;
        }

        #alarmList {
          margin: 20px 0;
        }

        .alarm-item {
          display: flex;
          justify-content: space-between;
          align-items: center;
          background: #f9fafb;
          padding: 12px;
          margin: 10px 0;
          border-radius: 8px;
          border: 1px solid #e5e7eb;
          transition: box-shadow 0.3s;
        }

        .alarm-item:hover {
          box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
        }

        .alarm-item button {
          padding: 8px 16px;
          font-size: 0.9em;
        }

        .alarm-item button:first-of-type {
          background: #10b981;
        }

        .alarm-item button:first-of-type:hover {
          background: #059669;
        }

        .alarm-item button:last-of-type {
          background: #ef4444;
        }

        .alarm-item button:last-of-type:hover {
          background: #dc2626;
        }

        /* Responsive design */
        @media (max-width: 900px) {
          .container {
            flex-direction: column;
            padding: 20px;
          }

          .left-column, .middle-column, .right-column {
            flex: none;
            width: 100%;
          }

          .middle-column {
            margin: 20px 0;
          }
        }
      </style>
    </head>
    <body>
      <div class="container">
        <div class="left-column">
          <h1>ESP8266 DHT11 & MQ135 Monitoring</h1>
          <img src="/graph" alt="Temperature Graph">
          <img src="/ppmGraph" alt="CO2 PPM Graph">
        </div>
        <div class="middle-column">
          <div class="middle-row">
            <p>Current Date: <span id="currentDate"></span></p>
            <p>Current Time: <span id="currentTime"></span> +07 (Vietnam Time)</p>
          </div>
          <div class="middle-row">
            <p>Current Temperature: <span id="currentTemperature"></span></p>
            <p>Current CO2: <span id="currentPPM"></span></p>
          </div>
        </div>
        <div class="right-column">
          <h3>Set Alarm</h3>
          <input type="time" id="alarmTime">
          <button onclick="setAlarm()">Set Alarm</button>
          <button id="stopButton" onclick="stopAlarm()">Stop Alarm</button>
          <h3>Alarm List</h3>
          <div id="alarmList"></div>
        </div>
      </div>
    </body>
    </html>
  )=====";
  server.send(200, "text/html", html);
}

// Serve data as JSON for real-time updates
void handleData() {
  String json = "{";
  json += "\"date\":\"" + getDate() + "\",";
  json += "\"time\":\"" + getTime() + "\",";
  json += "\"temperature\":" + String(temperatureData[DATA_POINTS - 1]) + ",";
  json += "\"ppm\":" + String(ppmData[DATA_POINTS - 1]);
  json += "}";
  server.send(200, "application/json", json);
}

// Draw the temperature graph (SVG) with auto-scaled Y-axis
void drawGraph() {
  String out;
  out.reserve(2600);
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"400\" height=\"200\">\n";
  out += "<rect width=\"400\" height=\"200\" fill=\"white\"/>\n";
  out += "<text x=\"200\" y=\"15\" font-size=\"14\" text-anchor=\"middle\">Temperature Values</text>\n";

  // Find min and max temperature values
  float minTemp = 100.0, maxTemp = -100.0;
  for (int i = 0; i < DATA_POINTS; i++) {
    if (temperatureData[i] != 0.0 && !isnan(temperatureData[i])) {
      minTemp = min(minTemp, temperatureData[i]);
      maxTemp = max(maxTemp, temperatureData[i]);
    }
  }
  if (minTemp == 100.0 || maxTemp == -100.0) {
    minTemp = 0.0;
    maxTemp = 50.0;  // Default range if no valid data
  } else if (maxTemp == minTemp) {
    maxTemp += 1.0;  // Avoid division by zero
  }

  // Y-axis scaling
  const int yHeight = 150;  // Height of the graph area (170 - 20)
  out += "<line x1=\"50\" y1=\"20\" x2=\"50\" y2=\"170\" stroke=\"black\"/>\n";
  for (float y = minTemp; y <= maxTemp; y += (maxTemp - minTemp) / 5) {
    int y_pos = 170 - (y - minTemp) * yHeight / (maxTemp - minTemp);
    out += "<line x1=\"50\" y1=\"" + String(y_pos) + "\" x2=\"55\" y2=\"" + String(y_pos) + "\" stroke=\"black\"/>\n";
    out += "<text x=\"20\" y=\"" + String(y_pos + 5) + "\" font-size=\"10\">" + String(y, 1) + "°C</text>\n";
  }

  // X-axis (time with hours and minutes)
  out += "<line x1=\"50\" y1=\"170\" x2=\"380\" y2=\"170\" stroke=\"black\"/>\n";
  for (int i = 0; i < DATA_POINTS; i++) {
    if (timestamps[i] > 0) {
      int x_pos = 50 + (i * 33);
      DateTime timeLabel(timestamps[i]);
      String label = String(timeLabel.hour()) + ":" + (timeLabel.minute() < 10 ? "0" : "") + String(timeLabel.minute());
      out += "<text x=\"" + String(x_pos - 15) + "\" y=\"185\" font-size=\"10\">" + label + "</text>\n";
      out += "<line x1=\"" + String(x_pos) + "\" y1=\"170\" x2=\"" + String(x_pos) + "\" y2=\"175\" stroke=\"black\"/>\n";
    }
  }

  // Plot temperature data
  out += "<polyline points=\"";
  for (int i = 0; i < DATA_POINTS; i++) {
    if (temperatureData[i] != 0.0 && !isnan(temperatureData[i])) {
      int x = 50 + (i * 33);
      int y = 170 - (temperatureData[i] - minTemp) * yHeight / (maxTemp - minTemp);
      out += String(x) + "," + String(y) + " ";
    }
  }
  out += "\" style=\"fill:none;stroke:blue;stroke-width:2\"/>\n";
  out += "</svg>\n";
  server.send(200, "image/svg+xml", out);
}

// Draw the CO2 PPM graph (SVG) with auto-scaled Y-axis
void drawPPMGraph() {
  String out;
  out.reserve(2600);
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"400\" height=\"200\">\n";
  out += "<rect width=\"400\" height=\"200\" fill=\"white\"/>\n";
  out += "<text x=\"200\" y=\"15\" font-size=\"14\" text-anchor=\"middle\">CO2 PPM Values</text>\n";

  // Find min and max PPM values
  float minPPM = 1000.0, maxPPM = -1000.0;
  for (int i = 0; i < DATA_POINTS; i++) {
    if (ppmData[i] != 0.0 && !isnan(ppmData[i])) {
      minPPM = min(minPPM, ppmData[i]);
      maxPPM = max(maxPPM, ppmData[i]);
    }
  }
  if (minPPM == 1000.0 || maxPPM == -1000.0) {
    minPPM = 0.0;
    maxPPM = 300.0;  // Default range if no valid data
  } else if (maxPPM == minPPM) {
    maxPPM += 10.0;  // Avoid division by zero
  }

  // Y-axis scaling
  const int yHeight = 150;  // Height of the graph area (170 - 20)
  out += "<line x1=\"50\" y1=\"20\" x2=\"50\" y2=\"170\" stroke=\"black\"/>\n";
  for (float y = minPPM; y <= maxPPM; y += (maxPPM - minPPM) / 5) {
    int y_pos = 170 - (y - minPPM) * yHeight / (maxPPM - minPPM);
    out += "<line x1=\"50\" y1=\"" + String(y_pos) + "\" x2=\"55\" y2=\"" + String(y_pos) + "\" stroke=\"black\"/>\n";
    out += "<text x=\"20\" y=\"" + String(y_pos + 5) + "\" font-size=\"10\">" + String(y, 1) + "ppm</text>\n";
  }

  // X-axis (time with hours and minutes)
  out += "<line x1=\"50\" y1=\"170\" x2=\"380\" y2=\"170\" stroke=\"black\"/>\n";
  for (int i = 0; i < DATA_POINTS; i++) {
    if (timestamps[i] > 0) {
      int x_pos = 50 + (i * 33);
      DateTime timeLabel(timestamps[i]);
      String label = String(timeLabel.hour()) + ":" + (timeLabel.minute() < 10 ? "0" : "") + String(timeLabel.minute());
      out += "<text x=\"" + String(x_pos - 15) + "\" y=\"185\" font-size=\"10\">" + label + "</text>\n";
      out += "<line x1=\"" + String(x_pos) + "\" y1=\"170\" x2=\"" + String(x_pos) + "\" y2=\"175\" stroke=\"black\"/>\n";
    }
  }

  // Plot PPM data
  out += "<polyline points=\"";
  for (int i = 0; i < DATA_POINTS; i++) {
    if (ppmData[i] != 0.0 && !isnan(ppmData[i])) {
      int x = 50 + (i * 33);
      int y = 170 - (ppmData[i] - minPPM) * yHeight / (maxPPM - minPPM);
      out += String(x) + "," + String(y) + " ";
    }
  }
  out += "\" style=\"fill:none;stroke:green;stroke-width:2\"/>\n";
  out += "</svg>\n";
  server.send(200, "image/svg+xml", out);
}

// Handler for setting an alarm
void handleAlarm() {
  if (server.hasArg("hour") && server.hasArg("minute")) {
    int hour = server.arg("hour").toInt();
    int minute = server.arg("minute").toInt();

    if (alarmCount < 10) {
      alarms[alarmCount] = { hour, minute, false, 0 };
      alarmCount++;
      server.send(200, "text/plain", "Alarm set: " + String(hour) + ":" + String(minute));
      Serial.println("Alarm set: " + String(hour) + ":" + String(minute));
    } else {
      server.send(400, "text/plain", "Alarm list is full!");
    }
  } else {
    server.send(400, "text/plain", "Missing hour or minute parameter");
  }
}

// Handler for deleting an alarm
void handleDeleteAlarm() {
  if (server.hasArg("hour") && server.hasArg("minute")) {
    int hour = server.arg("hour").toInt();
    int minute = server.arg("minute").toInt();
    Serial.print("Attempting to delete alarm: ");
    Serial.print(hour);
    Serial.print(":");
    Serial.println(minute);

    for (int i = 0; i < alarmCount; i++) {
      if (alarms[i].hour == hour && alarms[i].minute == minute) {
        Serial.print("Found alarm at index ");
        Serial.println(i);
        for (int j = i; j < alarmCount - 1; j++) {
          alarms[j] = alarms[j + 1];
        }
        alarmCount--;
        server.send(200, "text/plain", "Alarm deleted: " + String(hour) + ":" + String(minute));
        Serial.println("Alarm deleted: " + String(hour) + ":" + String(minute));
        return;
      }
    }
    server.send(404, "text/plain", "Alarm not found: " + String(hour) + ":" + String(minute));
    Serial.println("Alarm not found");
  } else {
    server.send(400, "text/plain", "Missing hour or minute parameter");
    Serial.println("Missing hour or minute parameter");
  }
}

// Handler for editing an alarm
void handleEditAlarm() {
  if (server.hasArg("oldHour") && server.hasArg("oldMinute") && server.hasArg("newHour") && server.hasArg("newMinute")) {
    int oldHour = server.arg("oldHour").toInt();
    int oldMinute = server.arg("oldMinute").toInt();
    int newHour = server.arg("newHour").toInt();
    int newMinute = server.arg("newMinute").toInt();

    for (int i = 0; i < alarmCount; i++) {
      if (alarms[i].hour == oldHour && alarms[i].minute == oldMinute) {
        alarms[i].hour = newHour;
        alarms[i].minute = newMinute;
        alarms[i].triggered = false;
        alarms[i].buzzStartTime = 0;
        server.send(200, "text/plain", "Alarm edited: " + String(oldHour) + ":" + String(oldMinute) + " to " + String(newHour) + ":" + String(newMinute));
        Serial.println("Alarm edited: " + String(oldHour) + ":" + String(oldMinute) + " to " + String(newHour) + ":" + String(newMinute));
        return;
      }
    }
    server.send(404, "text/plain", "Alarm not found: " + String(oldHour) + ":" + String(oldMinute));
  } else {
    server.send(400, "text/plain", "Missing oldHour, oldMinute, newHour, or newMinute parameter");
  }
}

// Handler to stop the buzzer
void handleStopAlarm() {
  if (buzzerActive) {
    digitalWrite(buzzer_pin, LOW);
    buzzerActive = false;
    for (int i = 0; i < alarmCount; i++) {
      if (alarms[i].buzzStartTime > 0) {
        alarms[i].buzzStartTime = 0;
      }
    }
    server.send(200, "text/plain", "Alarm stopped");
    Serial.println("Alarm stopped manually");
  } else {
    server.send(200, "text/plain", "No active alarm");
  }
}

// Handler to check buzzer state
void handleBuzzerState() {
  if (buzzerActive) {
    server.send(200, "text/plain", "active");
  } else {
    server.send(200, "text/plain", "inactive");
  }
}

// Handler to get alarm list
void handleGetAlarms() {
  String json = "[";
  for (int i = 0; i < alarmCount; i++) {
    json += "{\"hour\":" + String(alarms[i].hour) + ",\"minute\":" + String(alarms[i].minute) + "}";
    if (i < alarmCount - 1) json += ",";
  }
  json += "]";
  server.send(200, "application/json", json);
}

// Function to check and trigger alarms
void checkAlarms() {
  DateTime now = rtc.now();
  int currentHour = now.hour();
  int currentMinute = now.minute();
  int currentSecond = now.second();
  unsigned long currentMillis = millis();

  for (int i = 0; i < alarmCount; i++) {
    if (!alarms[i].triggered && alarms[i].hour == currentHour && alarms[i].minute == currentMinute && currentSecond < 5) {
      Serial.println("Alarm triggered: " + String(alarms[i].hour) + ":" + String(alarms[i].minute));
      digitalWrite(buzzer_pin, HIGH);
      buzzerActive = true;
      alarms[i].buzzStartTime = currentMillis;
      alarms[i].triggered = true;
    }

    // Check if buzzer should stop based on duration
    if (buzzerActive && alarms[i].buzzStartTime > 0 && (currentMillis - alarms[i].buzzStartTime >= (unsigned long)buzzerDuration)) {
      digitalWrite(buzzer_pin, LOW);
      buzzerActive = false;
      alarms[i].buzzStartTime = 0;
    }
  }
}