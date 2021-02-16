/* Sachin Iyer */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include <EEPROM.h>
#define DATA_PIN     2
#define NUM_LEDS    484
CRGB leds[NUM_LEDS];
extern "C" {
#include "libb64/cdecode.h"
}
#define FASTLED_ESP8266_RAW_PIN_ORDER

const char* ssid = "";
const char* password = "";

const char* awsEndpoint = ".iot.region.amazonaws.com";

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = "";
// xxxxxxxxxx-private.pem.key
const String privatePemKey = "";
// AWS cert
const String caPemCrt =													\
	//-----BEGIN CERTIFICATE-----
	"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF" \
	"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6" \
	"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL" \
	"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv" \
	"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj" \
	"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM" \
	"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw" \
	"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6" \
	"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L" \
	"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm" \
	"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC" \
	"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA" \
	"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI" \
	"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs" \
	"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv" \
	"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU" \
	"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy" \
	"rqXRfboQnoZsG4q5WTP468SQvvG5";
//-----END CERTIFICATE-----

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient);

int name[] = {0,0,255};
int back[] = {255, 0, 0};
int nameleds[] = {};
int backleds[] = {};
char message[50];
String hexname;
String hexback;
int full[6];
int newfull[6];
int ind = 0;

void setup() {
	Serial.begin(115200);
	Serial.println();
	delay(4000);
	FastLED.addLeds<WS2812, DATA_PIN>(leds, NUM_LEDS);
	FastLED.setMaxPowerInVoltsAndMilliamps(5,1000);
	EEPROM.begin(512);
	readvals();
	for (int i = 0; i < 3; i++) {
		name[i] = full[i];
	}
	for (int i = 3; i < 6; i++) {
		back[i-3] = full[i];
	}
	updateleds();

	Serial.print("Connecting to "); Serial.print(ssid);
	WiFi.begin(ssid, password);
	WiFi.waitForConnectResult();
	Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

	setCurrentTime();

	uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
	int len = b64decode(certificatePemCrt, binaryCert);
	wiFiClient.setCertificate(binaryCert, len);

	uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
	len = b64decode(privatePemKey, binaryPrivate);
	wiFiClient.setPrivateKey(binaryPrivate, len);

	uint8_t binaryCA[caPemCrt.length() * 3 / 4];
	len = b64decode(caPemCrt, binaryCA);
	wiFiClient.setCACert(binaryCA, len);
}

void loop() {
	pubSubCheckConnect();
}

void writearray(int address, int numbers[], int arraySize) {
	int addressIndex = address;
	for (int i = 0; i < arraySize; i++) {
		EEPROM.write(addressIndex, numbers[i] >> 8);
		EEPROM.write(addressIndex + 1, numbers[i] & 0xFF);
		addressIndex += 2;
	}
}
void readarray(int address, int numbers[], int arraySize) {
	int addressIndex = address;
	for (int i = 0; i < arraySize; i++) {
			numbers[i] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
			addressIndex += 2;
		}
	}

	void convert(String num, bool which) {
		int finalnum[] = {0,0,0};
		String nums[] = {num.substring(0,2), num.substring(2,4), num.substring(4,6)};
		char numschar[] = {' ', ' '};
		for (int i = 0; i < 3; i++) {
			numschar[0] = nums[i].charAt(0);
			numschar[1] = nums[i].charAt(1);
			finalnum[i] = (int) strtoul(numschar, 0, 16);
		}
		if (which) {
			for (int i = 0; i < 3; i++) {
				name[i] = finalnum[i];
			}
		}
		else {
			for (int i = 0; i < 3; i++) {
				back[i] = finalnum[i];
			}
		}
	}

	void updateleds() {
		for (int i = 0; i < 379; i++) {
			leds[backleds[i]].setRGB(back[1], back[0], back[2]);
		}
		for (int i = 0; i < 116; i++) {
			leds[nameleds[i]].setRGB(name[1], name[0], name[2]);
		}
		FastLED.show();
	}

	void writenewvals(int full[]) {
		EEPROM.begin(512);
		writearray(0,full,6);
		EEPROM.commit();
		Serial.println("THIS IS WRITE VALS");
		for (int i = 0; i < 6; i++) {
			Serial.print(full[i]);
			Serial.print(" ");
		}
		Serial.println();
	}

	void readvals() {
		int newarray[] = {0,0,0,0,0,0};
		readarray(0, newarray, 6);
		Serial.println("THIS IS READ VALS");
		for (int i = 0; i < 6; i++) {
			Serial.print(newarray[i]);
			Serial.print(" ");
		}
		for (int i = 0; i < 6; i++) {
			full[i] = newarray[i];
		}
		Serial.println();
	}

	void msgReceived(char* topic, byte* payload, unsigned int length) {
		for (int i = 0; i < length; i++) {
			message[i] = (char) payload[i];
		}
		hexname = "";
		hexback = "";
		for (int i = 14; i < 20; i++) {
			hexback += message[i];
		}
		for (int i = 35; i < 41; i++) {
			hexname += message[i];
		}
		convert(hexname, true);
		convert(hexback, false);
		Serial.println("THIS IS ORGINAL VALS");
		for (int i = 0; i < 3; i++) {
			full[i] = name[i];
			Serial.print(name[i]);
			Serial.print(" ");
		}
		for (int i = 0; i < 3; i++) {
			full[i+3] = back[i];
			Serial.print(back[i]);
			Serial.print(" ");
		}
		Serial.println();
		writenewvals(full);
		readvals();
		updateleds();
		delay(5);
	}

	void pubSubCheckConnect() {
		if ( ! pubSubClient.connected()) {
			Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
			while ( ! pubSubClient.connected()) {
				Serial.print(".");
				pubSubClient.connect("ESPthing");
			}
			Serial.println(" connected");
			pubSubClient.subscribe("inTopic");
		}
		pubSubClient.loop();
	}

	int b64decode(String b64Text, uint8_t* output) {
		base64_decodestate s;
		base64_init_decodestate(&s);
		int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
		return cnt;
	}

	void setCurrentTime() {
		configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

		Serial.print("Waiting for NTP time sync: ");
		time_t now = time(nullptr);
		while (now < 8 * 3600 * 2) {
			delay(500);
			Serial.print(".");
			now = time(nullptr);
		}
		Serial.println("");
		struct tm timeinfo;
		gmtime_r(&now, &timeinfo);
		Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
	}
