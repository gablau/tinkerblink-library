/**
 * @file       BlynkSimpleEsp8266_SSL.h
 * @author     Volodymyr Shymanskyy, Gabriele Lauricella
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 *             Copyright (c) 2026 Gabriele Lauricella
 * @date       Jan 2016
 * @brief
 *
 */

#ifndef BlynkSimpleEsp8266_SSL_h
#define BlynkSimpleEsp8266_SSL_h

#ifndef ESP8266
#error This code is intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

// How the server is verified: see Adapters/BlynkArduinoClientBearSSL.h

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <ESP8266WiFi.h>
// BearSSL client: ESP8266 Arduino core 2.5.0 or newer
#include <WiFiClientSecureBearSSL.h>
#include <Adapters/BlynkArduinoClientBearSSL.h>

template <typename Transport>
class BlynkWifi
    : public BlynkProtocol<Transport>
{
    typedef BlynkProtocol<Transport> Base;
public:
    BlynkWifi(Transport& transp)
        : Base(transp)
    {}

    // Call these before config() or begin()
    void setRootCA(const char* pem)     { this->conn.setRootCA(pem); }
    void setFingerprint(const char* fp) { this->conn.setFingerprint(fp); }
    void setInsecure()                  { this->conn.setInsecure(); }

    void connectWiFi(const char* ssid, const char* pass)
    {
        BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
        WiFi.mode(WIFI_STA);
        if (WiFi.status() != WL_CONNECTED) {
            if (pass && strlen(pass)) {
                WiFi.begin(ssid, pass);
            } else {
                WiFi.begin(ssid);
            }
        }
        while (WiFi.status() != WL_CONNECTED) {
            BlynkDelay(500);
        }
        BLYNK_LOG1(BLYNK_F("Connected to WiFi"));

        IPAddress myip = WiFi.localIP();
        BLYNK_LOG_IP("IP: ", myip);
    }

    // fingerprint: if given, pins the server certificate (SHA-1)
    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
                const char* fingerprint = NULL)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
        if (fingerprint) {
            this->conn.setFingerprint(fingerprint);
        }
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT_SSL,
                const char* fingerprint = NULL)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
        if (fingerprint) {
            this->conn.setFingerprint(fingerprint);
        }
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* fingerprint = NULL)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port, fingerprint);
        while(this->connect() != true) {}
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* fingerprint = NULL)
    {
        connectWiFi(ssid, pass);
        config(auth, ip, port, fingerprint);
        while(this->connect() != true) {}
    }

};

static BearSSL::WiFiClientSecure _blynkWifiClient;
static BlynkArduinoClientSecure<BearSSL::WiFiClientSecure> _blynkTransport(_blynkWifiClient);
BlynkWifi<BlynkArduinoClientSecure<BearSSL::WiFiClientSecure> > Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
