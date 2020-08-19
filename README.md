# Embedded CoAP Server

Author: Agustin Bassi - 2020

## 
## Table of Contents

* [Intro](#intro)
* [Install dependencies](#install-dependencies)
* [Run CoAP server](#run-coap-server)
* [Test CoAP server with CoAP client (optional)](#test-coap-server-with-coap-client-(optional))
* [Want to help?](#want-to-help-?)
* [License](#license)

## Intro

This project consists in a embedded CoAP server based in Arduino framework using PlatformIO tool. 

The original board target is ESP32 but it can be easily modified to work with ESP8266 or similar, configuring the `platformio.ini` file in the root folder of the project.

## Install dependencies

The project need the next dependencies:

* [Visual Studio Code](https://code.visualstudio.com/), a good editor that can be used to program many languages. 
* [PlatformIO](https://platformio.org/) extension in order to compile the project and upload the code into the board. In [this link](https://iot-es.herokuapp.com/post/details/17) there is a guide to install PlatformIO for Visual Studio Code, compile and run an example project. 
* Docker & Docker-Compose (optional to test the server). In this links are the official installation steps for [Docker](https://docs.docker.com/get-docker/) and [Docker Compose](https://docs.docker.com/compose/install/).

_PlatformIO is an application that can works standolone with code edited in any editor, but the recommended an easiest way is using it via VS Code plugin._

## Run CoAP server

Begin by downloading this project with the command below.

```sh
git clone https://github.com/ce-iot/embedded-coap-server.git
cd embedded-coap-server/
```

Configure WiFi settings accordingly modifying the lines 48-49 of file `src/main.cpp`.

```c
const char* WIFI_SSID = "WIFI_SSID";
const char* WIFI_PASS = "WIFI_PASS";
```

Compile the project with the next command.

```sh
pio run
```

Download the code to the board and open the serial terminal all in the same operation with the command below.

```sh
pio run -t upload && pio device monitor
```

When program starts an output like this must be shown.

```sh
Welcome to CoAP Server Device!
WiFi connected - IP address: 192.168.0.46
[Coap_InitServer] - Setup Callback Light
[Coap_InitServer] - Setup Callback Button
[Coap_InitServer] - Setup Response Callback
```

## Test CoAP server with CoAP client (optional)

To test the CoAP server any CoAP client can be used. In this case a Docker container with libcoap tool installed inside is proposed. Start by downloading the image with the next command.

```sh
docker pull obgm/libcoap:develop 
```
Obtain the `light` resource state with the next command.

```sh
docker run --rm --net=host -it obgm/libcoap:develop \
coap-client -m get -p 5683 coap://IP_SERVER/light
```

Set `light` resource ON with next command.

```sh
docker run --rm --net=host -it obgm/libcoap:develop \
coap-client -m put -e "{'light': true}" -p 5683 coap://IP_SERVER/light
```

Set `light` resource OFF with next command.

```sh
docker run --rm --net=host -it obgm/libcoap:develop \
coap-client -m put -e "{'light': false}" -p 5683 coap://IP_SERVER/light
```

Obtain the `button` resource state with the next command.

```sh
docker run --rm --net=host -it obgm/libcoap:develop \
coap-client -m get -p 5683 coap://IP_SERVER/button
```

## 
## Want to help?

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

If you find it useful please helpme following my Github user and give to this project a Star. This will animate me to continue contribuiting with the great open source community.

## 
## License

This project is licensed under the GPLV3 License.
