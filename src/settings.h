
#ifndef SETTINGS_H_
#define SETTINGS_H_

#define WIFI_INITIAL_SSID "LORA32-APRS-IS"
#define WIFI_INITIAL_KEY ""
#define CONFIG_VERSION "LORA1"

#define DEFAULT_IS_CALL "NOCALL-10"
#define DEFAULT_IS_PASSWORD ""
#define TOOL "ESP32-APRS-IS"
#define VERS "0.2"
// if FILTER is NOT active: it will use no filter on the IS server: will display just LoRa received messages as there are no messages coming from the server.
// if FILTER is active: it will be used on login on IS server and will display received messages from server. More Info: http://www.aprs-is.net/javAPRSFilter.aspx
//#define FILTER "r/48.29/14.29/25"

#define DEFAULT_SERVER "austria.aprs2.net"
//#define DEFAULT_SERVER "euro.aprs2.net"
#define DEFAULT_PORT "14580"

#define DEFAULT_BEACON_TIMEOUT "15"
#define DEFAULT_BEACON_LAT_POS "4819.82N"
#define DEFAULT_BEACON_LONG_POS "01418.68E"
#define DEFAULT_BEACON_MESSAGE "LoRa IGATE (RX only), Info: github.com/peterus/LoRa_APRS_iGate"

//#define SEND_MESSAGES_FROM_IS_TO_LORA

#endif
