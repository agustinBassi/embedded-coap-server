# Embedded CoAP Server

Author: Agustin Bassi - 2020

## 
## Table of Contents

* [Intro](#intro)
* [Install dependencies](#install-dependencies)
* [Run CoAP server](#run-coap-server)
* [Test CoAP server (optional)](#test-coap-server-(optional))
* [Want to help?](#want-to-help-?)
* [License](#license)

## Intro

This project consists in a embedded CoAP server based in Arduino framework using PlatformIO tool. 

The original board target is ESP32 but it can be easily modified to work with ESP8266 or similar, configuring the `platformio.ini` file in the root folder of the project.

## Install dependencies

The project need the next dependencies:

* [PlatformIO](https://platformio.org/): a full CLI tool to manage libraries, platforms, frameworks and projects for embedded systems. It handles all task needed in background with a easy to use command line interface. Even, it can be installed as an extension for many IDEs, for example Visual Studio Code.
* [Visual Studio Code](https://code.visualstudio.com/) (optional but nice to hace): a popular IDE that can be used to program many languages. It has an extension for PlatformIO. So, if Visual Studio code is installed, the convenient way to install PlatformIO is by adding it as an extension instead of install them separately. 
* Docker & Docker-Compose (optional to test CoAP server). In this links are the official installation steps for [Docker](https://docs.docker.com/get-docker/) and [Docker Compose](https://docs.docker.com/compose/install/).


## Run CoAP server

Begin by downloading this project with the command below.

```sh
git clone https://github.com/agustinBassi/embedded-coap-server.git
cd embedded-coap-server/
```

Configure WiFi settings accordingly modifying the lines 48-49 of file `src/main.cpp`.

```c
const char* WIFI_SSID = "WIFI_SSID";
const char* WIFI_PASS = "WIFI_PASS";
```

Compile the firmware, download it to the board and open the serial monitor, all in one, with the next command.

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

## Test CoAP server (optional)

To test the CoAP server any CoAP client can be used. In this case a Docker container with libcoap tool installed inside is proposed. Start by downloading the image with the next command.

```sh
docker pull abassi/libcoap:latest
```

* Get the `.well-known/core` resource (available CoAP server resources) with the next command.

```sh
docker run --rm --net=host -it abassi/libcoap \
coap-client -m get -p 5683 coap://IP_SERVER/.well-known/core
```

* Get the `light` resource state with the next command.

```sh
docker run --rm --net=host -it abassi/libcoap \
coap-client -m get -p 5683 coap://IP_SERVER/light
```

* Update `light` resource ON with next command.

```sh
docker run --rm --net=host -it abassi/libcoap \
coap-client -m put -e '{"light":true}' -p 5683 coap://IP_SERVER/light
```

* Update `light` resource OFF with next command.

```sh
docker run --rm --net=host -it abassi/libcoap \
coap-client -m put -e '{"light":true}' -p 5683 coap://IP_SERVER/light
```

* Get the `button` resource state with the next command.

```sh
docker run --rm --net=host -it abassi/libcoap \
coap-client -m get -p 5683 coap://IP_SERVER/button
```

* Test not found resource response with the next command.

```sh
docker run --rm --net=host -it abassi/libcoap \
coap-client -m get -p 5683 coap://IP_SERVER/not/found/resource
```

* Test some not allowed method resource response with the next command.

```sh
docker run --rm --net=host -it abassi/libcoap \
coap-client -m post -e '{"light":true}' -p 5683 coap://IP_SERVER/light
```

* Test bad request resource response with the next command.

```sh
docker run --rm --net=host -it abassi/libcoap \
coap-client -m post -e '{"light":invalid_state}' -p 5683 coap://IP_SERVER/light
```

## 
## Want to help?

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

If you find it useful please helpme following my Github user and give to this project a Star. This will animate me to continue contribuiting with the great open source community.

## 
## License

This project is licensed under the GPLV3 License.
