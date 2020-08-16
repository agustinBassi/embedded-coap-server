/*==================[ file header ]============================================
 * File:    main.cpp
 * Author:  Agustin Bassi (https://github.com/agustinBassi)
 * Licence: GPLV3+
 * Version: 1.0.0
 * Date:    July 2020
====================[inclusions]=============================================*/

#include <WiFi.h>

#include <WiFiUdp.h>
#include <coap-simple.h>

/*==================[macros and definitions]=================================*/

// APPLICATION SETTINGS
#define LED_ONBOARD                 2
#define INIT_DELAY                  3000
#define SERIAL_BAURDATE             115200
#define WIFI_CONNECTION_DELAY       500

/*==================[internal data declaration]==============================*/

// WiFi settings
const char* WIFI_SSID = "Fibertel WiFi152 2.4GHz";
const char* WIFI_PASS = "0043510112";

// const char* ssid     = "your-ssid";
// const char* password = "your-password";

// UDP and CoAP class
WiFiUDP udp;
Coap coap(udp);

// LED STATE
bool LEDSTATE;

/*==================[external data definition]===============================*/

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port);

// CoAP server endpoint url callback
void callback_light(CoapPacket &packet, IPAddress ip, int port);

/*==================[internal functions definition]==========================*/

void Wifi_EstablishConnection() {
    // Print network SSID
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
    // Try to connect
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    // Wait until connection is established
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_CONNECTION_DELAY);
        Serial.print(".");
    }
    // Report IP address
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}


// CoAP server endpoint URL
void callback_light(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Light] ON/OFF");
  
  // send response
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  String message(p);

  if (message.equals("0"))
    LEDSTATE = false;
  else if(message.equals("1"))
    LEDSTATE = true;
      
  if (LEDSTATE) {
    digitalWrite(9, HIGH) ; 
    coap.sendResponse(ip, port, packet.messageid, "1");
  } else { 
    digitalWrite(9, LOW) ; 
    coap.sendResponse(ip, port, packet.messageid, "0");
  }
}

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);
}

void init_coap(void){
    // add server url endpoints.
    // can add multiple endpoint urls.
    // exp) coap.server(callback_switch, "switch");
    //      coap.server(callback_env, "env/temp");
    //      coap.server(callback_env, "env/humidity");
    Serial.println("Setup Callback Light");
    coap.server(callback_light, "light");

    // client response callback.
    // this endpoint is single callback.
    Serial.println("Setup Response Callback");
    coap.response(callback_response);

    // start coap server/client
    coap.start();
}

/*==================[external functions definition]==========================*/

void setup(void){
    // wait a moment before start
    delay(INIT_DELAY);
    // Configure serial port at 115200 baudrios
    Serial.begin(SERIAL_BAURDATE);
    // Configure pins of buttons and leds
    pinMode(LED_ONBOARD, OUTPUT);
    // print to console Init message
    Serial.println("Welcome to CoAP Server Device!");
    // connect to wifi network
    Wifi_EstablishConnection();
    // create the things that will be exposed
    init_coap();
    // put led in a known state
    digitalWrite(LED_ONBOARD, true);
}

void loop(void){
    delay(1000);
    coap.loop();
}

/*==================[end of file]============================================*/

/*
if you change LED, req/res test with coap-client(libcoap), run following.
coap-client -m get coap://(arduino ip addr)/light
coap-client -e "1" -m put coap://(arduino ip addr)/light
coap-client -e "0" -m put coap://(arduino ip addr)/light
*/