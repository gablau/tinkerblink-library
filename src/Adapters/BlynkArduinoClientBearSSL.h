/**
 * @file       BlynkArduinoClientBearSSL.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @brief      TLS transport on the BearSSL client of the ESP8266 and
 *             Raspberry Pi Pico W (arduino-pico) cores
 *
 * How the server is verified (see BlynkConfig.h):
 * - default: the server certificate must chain to ISRG Root X1 (Let's Encrypt)
 * - BLYNK_SSL_ROOT_CA / Blynk.setRootCA(pem): own CA or self-signed server
 *   certificate, PEM, in RAM or PROGMEM
 * - BLYNK_SSL_FINGERPRINT / Blynk.setFingerprint(fp): SHA-1 fingerprint of
 *   the server certificate, hex, optionally separated by spaces or colons
 * - BLYNK_SSL_INSECURE / Blynk.setInsecure(): no verification (testing only)
 *
 * CA verification checks the certificate dates, so the time is set
 * via NTP first (BLYNK_SSL_NTP_SERVER1/2, BLYNK_SSL_NTP_TIMEOUT_MS).
 */

#ifndef BlynkArduinoClientBearSSL_h
#define BlynkArduinoClientBearSSL_h

#include <BlynkApiArduino.h>
#include <Adapters/BlynkArduinoClient.h>
#include <time.h>

#ifndef PROGMEM
#define PROGMEM
#endif

static const uint8_t BLYNK_DEFAULT_ROOT_CA_DER[] PROGMEM =
#include <certs/isrgroot_der.h>

#ifndef BLYNK_SSL_NTP_SERVER1
#define BLYNK_SSL_NTP_SERVER1 "pool.ntp.org"
#endif
#ifndef BLYNK_SSL_NTP_SERVER2
#define BLYNK_SSL_NTP_SERVER2 "time.nist.gov"
#endif
#ifndef BLYNK_SSL_NTP_TIMEOUT_MS
#define BLYNK_SSL_NTP_TIMEOUT_MS 20000UL
#endif

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
        , rootCA(NULL)
        , fingerprint(NULL)
        , trustAnchors(NULL)
        , ntpStarted(false)
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
        rootCA = pem;
        mode = VERIFY_ROOT_CA;
        if (trustAnchors) {
            delete trustAnchors;
            trustAnchors = NULL;
        }
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
            if (!fingerprint || !this->client->setFingerprint(fingerprint)) {
                BLYNK_LOG1(BLYNK_F("Invalid fingerprint"));
                return false;
            }
            break;
        default:
            if (!loadTrustAnchors()) {
                return false;
            }
            syncTime();
            this->client->setX509Time(time(nullptr));
            this->client->setTrustAnchors(trustAnchors);
            break;
        }

#if defined(BLYNK_SSL_RX_BUF_SIZE) && defined(BLYNK_SSL_TX_BUF_SIZE)
        this->client->setBufferSizes(BLYNK_SSL_RX_BUF_SIZE, BLYNK_SSL_TX_BUF_SIZE);
#endif

        if (BlynkArduinoClientGen<Client>::connect()) {
            switch (mode) {
            case VERIFY_FINGERPRINT: BLYNK_LOG1(BLYNK_F("Fingerprint OK"));  break;
            case VERIFY_ROOT_CA:     BLYNK_LOG1(BLYNK_F("Certificate OK"));  break;
            default: break;
            }
            return true;
        }

#ifdef BLYNK_PRINT
        char err[64] = { 0 };
        const int code = this->client->getLastSSLError(err, sizeof(err));
        if (code) {
            BLYNK_LOG4(BLYNK_F("TLS error "), code, BLYNK_F(": "), err);
        } else {
            BLYNK_LOG1(BLYNK_F("Secure connection failed"));
        }
#endif
        return false;
    }

private:
    bool loadTrustAnchors() {
        if (trustAnchors) {
            return true;
        }
        if (rootCA) {
            trustAnchors = new BearSSL::X509List(rootCA);
        } else {
            trustAnchors = new BearSSL::X509List(BLYNK_DEFAULT_ROOT_CA_DER,
                                                 sizeof(BLYNK_DEFAULT_ROOT_CA_DER));
        }
        if (!trustAnchors || !trustAnchors->getCount()) {
            BLYNK_LOG1(BLYNK_F("Failed to load root CA certificate!"));
            delete trustAnchors;
            trustAnchors = NULL;
            return false;
        }
        return true;
    }

    void syncTime() {
        if (!ntpStarted) {
#if defined(ARDUINO_ARCH_RP2040)
            NTP.begin(BLYNK_SSL_NTP_SERVER1, BLYNK_SSL_NTP_SERVER2);
#else
            configTime(0, 0, BLYNK_SSL_NTP_SERVER1, BLYNK_SSL_NTP_SERVER2);
#endif
            ntpStarted = true;
        }
        // Any time after 2020 means the clock was set
        const millis_time_t started = BlynkMillis();
        while (time(nullptr) < 1577836800L &&
               BlynkMillis() - started < BLYNK_SSL_NTP_TIMEOUT_MS)
        {
            BlynkDelay(500);
        }
        time_t now = time(nullptr);
        if (now < 1577836800L) {
            BLYNK_LOG1(BLYNK_F("NTP time not set: certificate dates cannot be checked"));
            return;
        }
#ifdef BLYNK_PRINT
        struct tm timeinfo;
        gmtime_r(&now, &timeinfo);
        String ntpTime = asctime(&timeinfo);
        ntpTime.trim();
        BLYNK_LOG2(BLYNK_F("NTP time: "), ntpTime);
#endif
    }

    VerifyMode          mode;
    const char*         rootCA;
    const char*         fingerprint;
    BearSSL::X509List*  trustAnchors;
    bool                ntpStarted;
};

#endif
