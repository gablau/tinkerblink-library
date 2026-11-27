/**
 * @file       BlynkWiFiStation.h
 * @author     Gabriele Lauricella
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2026 Gabriele Lauricella
 * @brief      Blynk over a WiFi library with the usual Arduino API
 *             (WiFi.begin/status/localIP): Raspberry Pi Pico W
 *             (arduino-pico), Arduino mbed boards (GIGA R1 WiFi)
 *
 */

#ifndef BlynkWiFiStation_h
#define BlynkWiFiStation_h

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>

#ifndef BLYNK_WIFI_RETRY_MS
#define BLYNK_WIFI_RETRY_MS 10000UL
#endif

template <typename Transport, uint16_t DefaultPort>
class BlynkWiFiStation
    : public BlynkProtocol<Transport>
{
    typedef BlynkProtocol<Transport> Base;
public:
    BlynkWiFiStation(Transport& transp)
        : Base(transp)
    {}

    // TLS only (secure transports); call these before config() or begin()
    void setRootCA(const char* pem)     { this->conn.setRootCA(pem); }
    void setFingerprint(const char* fp) { this->conn.setFingerprint(fp); }
    void setInsecure()                  { this->conn.setInsecure(); }

    void connectWiFi(const char* ssid, const char* pass)
    {
        while (WiFi.status() != WL_CONNECTED) {
            BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
            if (pass && strlen(pass)) {
                WiFi.begin(ssid, pass);
            } else {
                WiFi.begin(ssid);
            }
            const millis_time_t started = BlynkMillis();
            while (WiFi.status() != WL_CONNECTED &&
                   BlynkMillis() - started < BLYNK_WIFI_RETRY_MS)
            {
                BlynkDelay(500);
            }
        }
        BLYNK_LOG1(BLYNK_F("Connected to WiFi"));

        IPAddress myip = WiFi.localIP();
        BLYNK_LOG_IP("IP: ", myip);
    }

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = DefaultPort)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = DefaultPort)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = DefaultPort)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port);
        while(this->connect() != true) {}
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port   = DefaultPort)
    {
        connectWiFi(ssid, pass);
        config(auth, ip, port);
        while(this->connect() != true) {}
    }
};

#endif
