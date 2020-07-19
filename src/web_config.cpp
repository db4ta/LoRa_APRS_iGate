#include "web_config.h"
#include "LoRa_APRS.h"


namespace web_config
{
    void handleRoot();
    void configSaved();
    bool formValidator();

    char _user[STRING_LEN];
    char _password[STRING_LEN];
    char _is_server[STRING_LEN];
    char _port[NUMBER_LEN];
    char _timeout[NUMBER_LEN];
    char _latPos[STRING_LEN];
    char _longPos[STRING_LEN];
    char _msg[STRING_LEN];
    char _rx_frequency[NUMBER_LEN];

    DNSServer _dnsServer;
    WebServer _server(80);
    IotWebConf _iotWebConf(WIFI_INITIAL_SSID, &_dnsServer, &_server, WIFI_INITIAL_KEY, CONFIG_VERSION);

    IotWebConfSeparator _separator0 = IotWebConfSeparator();
    IotWebConfParameter _userParam = IotWebConfParameter("APRS-IS User", "user", _user, STRING_LEN, "text", DEFAULT_IS_CALL, DEFAULT_IS_CALL);
    IotWebConfParameter _passwordParam = IotWebConfParameter("APRS-IS Password", "password", _password, STRING_LEN, "text", DEFAULT_IS_PASSWORD, DEFAULT_IS_PASSWORD);
    IotWebConfParameter _serverParam = IotWebConfParameter("APRS-IS Server", "server", _is_server, STRING_LEN, "text", DEFAULT_SERVER, DEFAULT_SERVER);
    IotWebConfParameter _portParam = IotWebConfParameter("APRS-IS Port", "port", _port, NUMBER_LEN, "number", DEFAULT_PORT, DEFAULT_PORT);
    IotWebConfSeparator _separator1 = IotWebConfSeparator();
    IotWebConfParameter _timeoutParam = IotWebConfParameter("Timeout", "timeout", _timeout, NUMBER_LEN, "number", DEFAULT_BEACON_TIMEOUT, DEFAULT_BEACON_TIMEOUT);
    IotWebConfParameter _latPosParam = IotWebConfParameter("Lat Position", "lat", _latPos, STRING_LEN, "text", DEFAULT_BEACON_LAT_POS, DEFAULT_BEACON_LAT_POS);
    IotWebConfParameter _longPosParam = IotWebConfParameter("Long Position", "long", _longPos, STRING_LEN, "text", DEFAULT_BEACON_LONG_POS, DEFAULT_BEACON_LONG_POS);
    IotWebConfParameter _msgParam = IotWebConfParameter("Message", "msg", _msg, STRING_LEN, "text", DEFAULT_BEACON_MESSAGE, DEFAULT_BEACON_MESSAGE);
    IotWebConfSeparator _separator2 = IotWebConfSeparator();
    IotWebConfParameter _rxFrequencyParam = IotWebConfParameter("Timeout", "timeout", _rx_frequency, NUMBER_LEN, "number", "LORA_RX_FREQUENCY", "LORA_RX_FREQUENCY");


    void setup()
    {
        _iotWebConf.addParameter(&_separator0);
        _iotWebConf.addParameter(&_userParam);
        _iotWebConf.addParameter(&_passwordParam);
        _iotWebConf.addParameter(&_serverParam);
        _iotWebConf.addParameter(&_portParam);
        _iotWebConf.addParameter(&_separator1);
        _iotWebConf.addParameter(&_timeoutParam);
        _iotWebConf.addParameter(&_latPosParam);
        _iotWebConf.addParameter(&_longPosParam);
        _iotWebConf.addParameter(&_msgParam);
        _iotWebConf.addParameter(&_separator1);
        _iotWebConf.addParameter(&_rxFrequencyParam);

        _iotWebConf.setConfigSavedCallback(&configSaved);
        _iotWebConf.setFormValidator(&formValidator);
        _iotWebConf.getApTimeoutParameter()->visible = true;

        _iotWebConf.init();

        _server.on("/", handleRoot);
        _server.on("/config", []{ _iotWebConf.handleConfig(); });
        _server.onNotFound([](){ _iotWebConf.handleNotFound(); });
    }

    void doLoop()
    {
        _iotWebConf.doLoop();
    }

    String getIsUser()
    {
        return _user;
    }

    String getIsPassword()
    {
        return _password;
    }

    String getIsServer()
    {
        return _is_server;
    }

    int getIsPort()
    {
        return String(_port).toInt();
    }

    int getBeaconTimeout()
    {
        return String(_timeout).toInt();
    }

    String getBeaconPosLat()
    {
        return _latPos;
    }

    String getBeaconPosLong()
    {
        return _longPos;
    }

    String getBeaconMsg()
    {
        return _msg;
    }

    long getRxFrequency()
    {
        return String(_rx_frequency).toInt();
    }

    byte connectionState()
    {
        return _iotWebConf.getState();
    }

    void handleRoot()
    {
        // -- Let IotWebConf test and handle captive portal requests.
        if (_iotWebConf.handleCaptivePortal())
        {
            // -- Captive portal request were already served.
            return;
        }

        String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
        s += "<title>IotWebConf 03 Custom Parameters</title></head><body>Hello world!";
        s += "<ul>";
        s += "<li>APRS-IS-User: ";
        s += _user;
        s += "<li>APRS-IS-Password: ";
        s += _password;
        s += "<li>APRS-IS-Server: ";
        s += _is_server;
        s += "<li>APRS-IS-Port: ";
        s += _port;
        s += "</ul>";
        s += "Go to <a href='config'>configure page</a> to change values.";
        s += "</body></html>\n";

        _server.send(200, "text/html", s);
    }

    void configSaved()
    {
        Serial.println("Configuration was updated.");
    }

    bool formValidator()
    {
        Serial.println("Validating form.");
        bool valid = true;
        return valid;
    }

}
