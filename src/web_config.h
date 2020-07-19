#ifndef WEB_CONFIG_H_
#define WEB_CONFIG_H_

#include "settings.h"
#include <IotWebConf.h>

namespace web_config
{
    #define STRING_LEN 128
    #define NUMBER_LEN 32

    void setup();
    void doLoop();

    String getIsUser();
    String getIsPassword();
    String getIsServer();
    int getIsPort();

    int getBeaconTimeout();
    String getBeaconPosLat();
    String getBeaconPosLong();
    String getBeaconMsg();

    long getRxFrequency();

    byte connectionState();
}

#endif
