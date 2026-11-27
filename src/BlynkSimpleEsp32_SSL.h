/**
 * @file       BlynkSimpleEsp32_SSL.h
 * @author     Volodymyr Shymanskyy, Gabriele Lauricella
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 *             Copyright (c) 2026 Gabriele Lauricella
 * @date       Dec 2017
 * @brief
 *
 */

#ifndef BlynkSimpleEsp32_SSL_h
#define BlynkSimpleEsp32_SSL_h

#ifndef ESP32
#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

/*
 * How the server is verified (see BlynkConfig.h):
 * - default: the server certificate must chain to ISRG Root X1 (Let's Encrypt)
 * - BLYNK_SSL_ROOT_CA / Blynk.setRootCA(pem): own CA or self-signed server
 *   certificate, PEM
 * - BLYNK_SSL_FINGERPRINT / Blynk.setFingerprint(fp): SHA-256 fingerprint of
 *   the server certificate, hex, optionally separated by spaces or colons
 * - BLYNK_SSL_INSECURE / Blynk.setInsecure(): no verification (testing only)
 *
 * With ESP32 Arduino core 1.0.x, once setInsecure() was used the CA is not
 * checked again until reboot: pick the mode once, before begin().
 */

static const char BLYNK_DEFAULT_ROOT_CA[] =
#include <certs/isrgroot_pem.h>

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <WiFiClientSecure.h>

template <typename Client>
class BlynkArduinoClientSecure
    : public BlynkArduinoClientGen<Client>
{
public:
    enum VerifyMode {
        VERIFY_ROOT_CA,
        VERIFY_FINGERPRINT,
        VERIFY_NONE
    };

    BlynkArduinoClientSecure(Client& client)
        : BlynkArduinoClientGen<Client>(client)
        , mode(VERIFY_ROOT_CA)
        , caCert(NULL)
        , fingerprint(NULL)
    {
#if defined(BLYNK_SSL_INSECURE)
        mode = VERIFY_NONE;
#elif defined(BLYNK_SSL_FINGERPRINT)
        setFingerprint(BLYNK_SSL_FINGERPRINT);
#elif defined(BLYNK_SSL_ROOT_CA)
        setRootCA(BLYNK_SSL_ROOT_CA);
#endif
    }

    void setRootCA(const char* pem) {
        caCert = pem;
        mode = VERIFY_ROOT_CA;
    }

    void setFingerprint(const char* fp) {
        fingerprint = fp;
        mode = VERIFY_FINGERPRINT;
    }

    void setInsecure() {
        mode = VERIFY_NONE;
    }

    bool connect() {
        switch (mode) {
        case VERIFY_NONE:
            BLYNK_LOG1(BLYNK_F("WARNING: server certificate NOT verified"));
            this->client->setInsecure();
            break;
        case VERIFY_FINGERPRINT:
            if (!fingerprint) {
                BLYNK_LOG1(BLYNK_F("Invalid fingerprint"));
                return false;
            }
            // The chain is not checked: the certificate is pinned after
            // the handshake
            this->client->setInsecure();
            break;
        default:
            this->client->setCACert(caCert ? caCert : BLYNK_DEFAULT_ROOT_CA);
            break;
        }

        if (!BlynkArduinoClientGen<Client>::connect()) {
            BLYNK_LOG1(BLYNK_F("Secure connection failed"));
            return false;
        }

        switch (mode) {
        case VERIFY_FINGERPRINT:
            if (!this->client->verify(fingerprint, NULL)) {
                BLYNK_LOG1(BLYNK_F("Fingerprint does not match"));
                this->disconnect();
                return false;
            }
            BLYNK_LOG1(BLYNK_F("Fingerprint OK"));
            break;
        case VERIFY_ROOT_CA:
            BLYNK_LOG1(BLYNK_F("Certificate OK"));
            break;
        default:
            break;
        }
        return true;
    }

private:
    VerifyMode  mode;
    const char* caCert;
    const char* fingerprint;
};

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
        if (pass && strlen(pass)) {
            WiFi.begin(ssid, pass);
        } else {
            WiFi.begin(ssid);
        }
        while (WiFi.status() != WL_CONNECTED) {
            BlynkDelay(500);
        }
        BLYNK_LOG1(BLYNK_F("Connected to WiFi"));

        IPAddress myip = WiFi.localIP();
        BLYNK_LOG_IP("IP: ", myip);
    }

    // root_ca: if given, replaces the CA set before (PEM)
    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
                const char* root_ca = NULL)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
        if (root_ca) {
            this->conn.setRootCA(root_ca);
        }
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT_SSL,
                const char* root_ca = NULL)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
        if (root_ca) {
            this->conn.setRootCA(root_ca);
        }
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* root_ca = NULL)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port, root_ca);
        while(this->connect() != true) {}
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* root_ca = NULL)
    {
        connectWiFi(ssid, pass);
        config(auth, ip, port, root_ca);
        while(this->connect() != true) {}
    }

};

static WiFiClientSecure _blynkWifiClient;
static BlynkArduinoClientSecure<WiFiClientSecure> _blynkTransport(_blynkWifiClient);
BlynkWifi<BlynkArduinoClientSecure<WiFiClientSecure> > Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
