#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char *sta_ssid = "SPWH_L11_F7561E";
const char *sta_pass = "123456789";

const char *ap_ssid = "tahiesp32";
const char *ap_pass = "123456789";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

uint8_t monitoringMAC1[6] = {0xb4, 0x7c, 0x9c, 0x1f, 0x3d, 0x8a};
uint8_t monitoringMAC2[6] = {0x68, 0x37, 0xe9, 0x1d, 0xb6, 0xae};

boolean detectingMAC1 = false;
int counter1 = 0;
boolean detectingMAC2 = false;
int counter2 = 0;

String getStrMAC(uint8_t mac[6])
{
    String res = String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[2], HEX) + ":" +
                 String(mac[3], HEX) + ":" + String(mac[4], HEX) + ":" + String(mac[5], HEX);
    return res;
}

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
    uint8_t *mac = info.wifi_ap_probereqrecved.mac;

    // Serial.println("catch : " + getStrMAC(mac));

    if (memcmp(mac, monitoringMAC1, 6) == 0)
    {
        detectingMAC1 = true;
    }
    if (memcmp(mac, monitoringMAC2, 6) == 0)
    {
        detectingMAC2 = true;
    }
}

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.onEvent(WiFiEvent);
    WiFi.softAP(ap_ssid, ap_pass);
    esp_wifi_set_event_mask(0);
    Serial.println(WiFi.softAPIP());
    WiFi.begin(sta_ssid, sta_pass);
}

void loop()
{
    if (detectingMAC1)
    {
        counter1++;
        Serial.println("Button1 pushed!");
        if (counter1 >= 10)
        {
            counter1 == 0;
            detectingMAC1 = false;
        }
    }
    if (detectingMAC2)
    {
        counter2++;
        Serial.println("Button2 pushed!");
        if (counter2 >= 10)
        {
            counter2 == 0;
            detectingMAC2 = false;
        }
    }
}