#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <APRS-IS.h>

#include "LoRa_APRS.h"

#include "settings.h"
#include "display.h"
#include "power_management.h"
#include "web_config.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 60*60);
#if defined(ARDUINO_T_Beam) && !defined(ARDUINO_T_Beam_V0_7)
PowerManagement powerManagement;
#endif
APRS_IS * aprs_is = NULL;
LoRa_APRS lora_aprs;

int next_update = -1;

void setup_lora();
void setup_ntp();

String BeaconMsg;

// cppcheck-suppress unusedFunction
void setup()
{
	Serial.begin(115200);

#if defined(ARDUINO_T_Beam) && !defined(ARDUINO_T_Beam_V0_7)
	Wire.begin(SDA, SCL);
	if (!powerManagement.begin(Wire))
	{
		Serial.println("LoRa-APRS / Init / AXP192 Begin PASS");
	} else {
		Serial.println("LoRa-APRS / Init / AXP192 Begin FAIL");
	}
	powerManagement.activateLoRa();
	powerManagement.activateOLED();
	powerManagement.deactivateGPS();
#endif

	setup_display();
	
	delay(1000);
	Serial.println("[INFO] LoRa APRS iGate by OE5BPA (Peter Buchegger)");
	show_display("OE5BPA", "LoRa APRS iGate", "by Peter Buchegger", 2000);

	web_config::setup();
}

bool need_reconfigure = true;

// cppcheck-suppress unusedFunction
void loop()
{
	web_config::doLoop();
	if(web_config::connectionState() != IOTWEBCONF_STATE_ONLINE)
	{
		need_reconfigure = true;
		return;
	}
	if(need_reconfigure)
	{
		setup_lora();
		setup_ntp();
		if(aprs_is)
		{
			delete aprs_is;
			aprs_is = NULL;
		}
		aprs_is = new APRS_IS(web_config::getIsUser(), web_config::getIsPassword(), TOOL, VERS);

		APRSMessage msg;
		msg.setSource(web_config::getIsUser());
		msg.setDestination("APLG0");
		msg.getAPRSBody()->setData(String("=") + web_config::getBeaconPosLat() + "I" + web_config::getBeaconPosLong() + "&" + web_config::getBeaconMsg());
		BeaconMsg = msg.encode();
		
		need_reconfigure = false;
	}
	timeClient.update();
	/*if(!aprs_is.connected())
	{
		Serial.print("[INFO] connecting to server: ");
		Serial.print(SERVER);
		Serial.print(" on port: ");
		Serial.println(PORT);
		show_display("INFO", "Connecting to server");
#ifdef FILTER
		if(!aprs_is.connect(SERVER, PORT, FILTER))
#else
		if(!aprs_is.connect(SERVER, PORT))
#endif
		{
			Serial.println("[ERROR] Connection failed.");
			Serial.println("[INFO] Waiting 5 seconds before retrying...");
			show_display("ERROR", "Server connection failed!", "waiting 5 sec");
			delay(5000);
			return;
		}
		Serial.println("[INFO] Connected to server!");
	}
	if(next_update == timeClient.getMinutes() || next_update == -1)
	{
		show_display(USER, "Beacon to Server...");
		Serial.print("[" + timeClient.getFormattedTime() + "] ");
		aprs_is.sendMessage(BeaconMsg);
		next_update = (timeClient.getMinutes() + BEACON_TIMEOUT) % 60;
	}
	if(aprs_is.available() > 0)
	{
		String str = aprs_is.getMessage();
		Serial.print("[" + timeClient.getFormattedTime() + "] ");
		Serial.println(str);
#ifdef FILTER
		show_display(USER, timeClient.getFormattedTime() + "    IS-Server", str, 0);
#endif
#ifdef SEND_MESSAGES_FROM_IS_TO_LORA
		std::shared_ptr<APRSMessage> msg = std::shared_ptr<APRSMessage>(new APRSMessage());
		msg->decode(str);
		lora_aprs.sendMessage(msg);
#endif
	}
	if(lora_aprs.hasMessage())
	{
		std::shared_ptr<APRSMessage> msg = lora_aprs.getMessage();

		show_display(USER, timeClient.getFormattedTime() + "         LoRa", "RSSI: " + String(lora_aprs.getMessageRssi()) + ", SNR: " + String(lora_aprs.getMessageSnr()), msg->toString(), 0);
		Serial.print("[" + timeClient.getFormattedTime() + "] ");
		Serial.print(" Received packet '");
		Serial.print(msg->toString());
		Serial.print("' with RSSI ");
		Serial.print(lora_aprs.getMessageRssi());
		Serial.print(" and SNR ");
		Serial.println(lora_aprs.getMessageSnr());

		aprs_is.sendMessage(msg->encode());
	}*/
}

void setup_lora()
{
	Serial.print("[INFO] frequency: ");
	Serial.println(LORA_RX_FREQUENCY);
	
	if (!lora_aprs.begin())
	{
		Serial.println("[ERROR] Starting LoRa failed!");
		show_display("ERROR", "Starting LoRa failed!");
		while (1);
	}
	Serial.println("[INFO] LoRa init done!");
	show_display("INFO", "LoRa init done!", 2000);
}

void setup_ntp()
{
	timeClient.begin();
	if(!timeClient.forceUpdate())
	{
		Serial.println("[WARN] NTP Client force update issue!");
		show_display("WARN", "NTP Client force update issue!", 2000);
	}
	Serial.println("[INFO] NTP Client init done!");
	show_display("INFO", "NTP Client init done!", 2000);
}
