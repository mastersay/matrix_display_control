//
// Created by japru on 12/03/2023.
//

#include "wifi_server.hpp"
#include <ESP32Time.h>
#include <WiFi.h>
#include <WebServer.h>
#include <display.hpp>

#define WIFI_SSID "ESP_PRIME"
#define WIFI_PASSWORD "01234567"
//IPAddress local_ip(192, 168, 2, 1);
//IPAddress gateway(192, 168, 2, 1);
//IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

String no_networks_found = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Network setup</title>
</head>
<body>
<p>No networks found, if you have hidden ssid, please enter bellow or refresh the page to search again:</p>
<form action="/login" method="POST">
    <label for="ssid">Hidden ssid</label>
    <input type="text" name="ssid" id="ssid">
    <label for="password">Password</label>
    <input type="text" name="password" id="password">
    <input type="submit" value="Connect">
</form>
<form action="/datetime" method="get" id="datetime-form">
    <label for="ntp">Auto date and time:</label>
    <input type="checkbox" onclick="manual_date_time(hide=this.checked)" id="ntp" name="ntp" checked>
    <div id="manual-date-time" style="display: none">
        <label for="datetime">Setup date and time:</label>
        <input type="datetime-local" id="datetime"
               name="datetime" value="2023-06-15T07:00">
    </div>
    <input type="submit" value="Set">
</form>
<form action="/alarm" method="get" id="alarm-form">
    <label for="alarm_on">Alarm enabled:</label>
    <input type="checkbox" onclick="alarm_time_show(turned_on=this.checked)" id="alarm_on" name="alarm_on">
    <div id="alarm-time" style="display: none">
        <label for="alarm_time">Setup date and time:</label>
        <input type="time" id="alarm_time"
               name="alarm_time" value="00:00">
    </div>
    <input type="submit" value="Set">
</form>
</body>
<script>
    function manual_date_time(hide) {
        if (hide) {
            document.getElementById("manual-date-time").style.display = "none"
        } else {
            document.getElementById("manual-date-time").style.display = "block"
        }
    }
    function alarm_time_show(turned_on) {
        if (turned_on) {
            document.getElementById("alarm-time").style.display = "block"
        } else {
            document.getElementById("alarm-time").style.display = "none"
        }
    }
</script>
</html>
)rawliteral";

void handle_on_root() {
    // Network search
    Serial.println("Handling on_root '/'");
    int network_count = WiFi.scanNetworks();
    Serial.print("Network scan complete: ");
    Serial.println(network_count);
    if (network_count == 0) {
        Serial.println("Before sending no_net_found");
        server.send(200, "text/html", no_networks_found);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    } else if (network_count > 0) {
        Serial.println("Creating response");
        // Html part for showing networks
        String entries = "Networks <br>";
        for (int i = 0; i < network_count; ++i) {
            entries += R"rawliteral(<form action="/login" method="POST">)rawliteral";
            entries += "SSID: ";
            entries += R"rawliteral( <input type="text" name="ssid" id="ssid" value=")rawliteral" + WiFi.SSID(i) +
                       R"rawliteral(" readonly> )rawliteral";
            entries += " RSSI: ";
            entries += WiFi.RSSI(i);
            if (WiFi.encryptionType(i) != 0) {
                entries += R"rawliteral( <input type="text" name="password" id="password" required> )rawliteral";
            }
            entries += R"rawliteral( <input type="submit" value="Connect"> )rawliteral";
            entries += "</form> <br>";
        }
        entries += R"rawliteral(
            <form action="/login" method="POST">
            <label for="ssid">Hidden ssid</label>
            <input type="text" name="ssid" id="ssid">
            <label for="password">Password</label>
            <input type="text" name="password" id="password">
            <input type="submit" value="Connect">
            </form>)rawliteral";
        Serial.println("Sending response");
        entries += R"rawliteral(
            <form action="/datetime" method="get" id="datetime-form">
            <label for="ntp">Auto date and time:</label>
            <input type="checkbox" onclick="manual_date_time(hide=this.checked)" id="ntp" name="ntp" checked>
            <div id="manual-date-time" style="display: none">
                <label for="datetime">Setup date and time:</label>
                <input type="datetime-local" id="datetime"
                       name="datetime" value="2023-06-15T07:00">
            </div>
            <input type="submit" value="Set">
            </form>
            <form action="/alarm" method="get" id="alarm-form">
                <label for="alarm_on">Alarm enabled:</label>
                <input type="checkbox" onclick="alarm_time_show(turned_on=this.checked)" id="alarm_on" name="alarm_on">
                <div id="alarm-time" style="display: none">
                    <label for="alarm_time">Setup date and time:</label>
                    <input type="time" id="alarm_time"
                           name="alarm_time" value="00:00">
                </div>
                <input type="submit" value="Set">
            </form>
            <script>
                function manual_date_time(hide) {
                    if (hide) {
                        document.getElementById("manual-date-time").style.display = "none"
                    } else {
                        document.getElementById("manual-date-time").style.display = "block"
                    }
                }
                function alarm_time_show(turned_on) {
                    if (turned_on) {
                        document.getElementById("alarm-time").style.display = "block"
                    } else {
                        document.getElementById("alarm-time").style.display = "none"
                    }
                }
            </script>)rawliteral";
        server.send(200, "text/html", entries);
        Serial.println("Response sent");
    } else {
        server.send(200, "text/html", "Error 500");
    }
}

void handle_login() {

    char ssid_char[server.arg("ssid").length() + 1];
    char password_char[server.arg("password").length() + 1];
    server.arg("ssid").toCharArray(ssid_char, server.arg("ssid").length() + 1);
    server.arg("password").toCharArray(password_char, server.arg("password").length() + 1);
    if (WiFiClass::status() == WL_IDLE_STATUS || WiFiClass::status() == WL_DISCONNECTED) {
        if (server.arg("password").isEmpty()) {
            WiFi.begin(ssid_char);
        } else {
            WiFi.begin(ssid_char, password_char);
        }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    String connection_response = "";
    if (WiFiClass::status() == WL_CONNECTED) {
        connection_response += "Connected with ip: <br>";
        connection_response += WiFi.localIP().toString();
    } else {
        connection_response += "Connection unsuccessful. Try again!";
        WiFi.reconnect();
    }
    server.send(200, "text/html", connection_response);
}

void handle_datetime() {
    Serial.print("NTP state: ");
    Serial.println(server.arg("ntp"));
    String connection_response = "";
    if (server.arg("ntp") == "on") {
        Serial.println("Auto time set");
        connection_response = "Auto time set";
        vTaskResume(update_time_handle);
    } else {
        vTaskSuspend(update_time_handle);
        Serial.println("Auto time turned off");
        connection_response += "Auto time turned off\n";
        connection_response += server.arg("datetime");
        Serial.println(server.arg("datetime"));
//        settimeofday(&time_info, nullptr);
        String datetime = server.arg("datetime");
        ESP32Time rtc;
        time_info.tm_year = datetime.substring(0, 4).toInt() - 1900;
        time_info.tm_mon = datetime.substring(5, 7).toInt() - 1;
        time_info.tm_mday = datetime.substring(8, 10).toInt();
        time_info.tm_hour = datetime.substring(11, 13).toInt();
        time_info.tm_min = datetime.substring(14, 16).toInt();
        rtc.setTimeStruct(time_info);
    }
    Serial.println(time_info.tm_year);
    Serial.println(time_info.tm_mon);
    Serial.println(time_info.tm_mday);
    Serial.println(time_info.tm_hour);
    Serial.println(time_info.tm_min);

    server.send(200, "text/html", connection_response);
}

void handle_alarm() {
    Serial.print("Alarm state: ");
    Serial.println(server.arg("alarm_on"));
    String connection_response = "";
    if (server.arg("alarm_on") == "on") {
        Serial.println("Alarm set");
        connection_response = "Alarm set";
        connection_response += server.arg("alarm_time");
        Serial.println(server.arg("alarm_time"));
//        settimeofday(&time_info, nullptr);
        String alarm_time = server.arg("alarm_time");
        alarm_info.hour = alarm_time.substring(0, 2).toInt();
        alarm_info.minute = alarm_time.substring(3, 5).toInt();
        alarm_enabled = true;

    } else {
        Serial.println("Alarm off");
        connection_response += "Alarm off\n";
        alarm_enabled = false;
    }
    server.send(200, "text/html", connection_response);
}

void wifi_init_soft_ap() {
    WiFiClass::mode(WIFI_AP_STA);
//    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    server.on("/", handle_on_root);
//    server.on("/post",  handle_post);
    server.on("/login", HTTP_POST, handle_login);
    server.on("/datetime", HTTP_GET, handle_datetime);
    server.on("/alarm", HTTP_GET, handle_alarm);
    server.begin();
    Serial.println("Server initialized");
    Serial.println(server.uri());
}

void client_loop() {
    server.handleClient();
}
