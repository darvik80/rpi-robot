Robot based on RPI 3
![](https://raw.githubusercontent.com/darvik80/rpi-robot/master/images/robot.jpeg)

## Components

* [HID Joystick](https://en.wikipedia.org/wiki/Human_interface_device)
* [Servo Driver HAT](https://www.waveshare.com/wiki/Servo_Driver_HAT)
* [RPi Motor Driver Board](https://www.waveshare.com/wiki/RPi_Motor_Driver_Board)
* SRF05

## Libraries

* [Boost.Lib >= 1.74](https://www.boost.org/)
* [fmt >= 8](https://github.com/fmtlib/fmt)
* [nlohmann-json](https://github.com/nlohmann/json)
* [gpiod](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git)

## Troubleshoots

* Build with conan
1. Install dependencies
```bash
  conan install . --output-folder=cmake-build-debug --build=missing
```
2. Add parameter for CMake
```bash
  -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
```
![](https://raw.githubusercontent.com/darvik80/rpi-robot/master/images/cmake-config.png)

* GDB Server does not work for Docker
  add run arg for docker config: --security-opt seccomp=unconfined
* Remote GDB Server
  ![](https://raw.githubusercontent.com/darvik80/rpi-robot/master/images/remote-debug.png)

## IoT Yandex Core

### Device

| topic                         | alias               | description               | 
|-------------------------------|---------------------|---------------------------|
| $devices/${deviceId}/events   | $me/device/events   | telemetry                 |
| $devices/${deviceId}/state    | $me/device/state    | telemetry                 |
| $devices/${deviceId}/commands | $me/device/commands | commands                  |
| $devices/${deviceId}/config   | $me/device/config   | configuration             |
| $monitoring/${deviceId}/json  | $me/device/config   | monitoring in json format |

## RegistryDo

| topic                             | alias                   | description               | 
|-----------------------------------|-------------------------|---------------------------|
| registries/${registryId}/events   | $me/registryId/events   | telemetry                 |
| registries/${registryId}/state    | $me/registryId/state    | telemetry                 |
| registries/${registryId}/commands | $me/registryId/commands | commands                  |
| registries/${registryId}/config   | $me/registryId/config   | configuration             |
| registries/${registryId}/json     | $me/registryId/config   | monitoring in json format |

## IoT ThingsBoard

### Device

| topic                                  | description                                                                       | 
|----------------------------------------|-----------------------------------------------------------------------------------|
| v1/devices/me/telemetry                | [telemetry](https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api) |
| v1/devices/me/rpc/request/$request_id  | [command request](https://thingsboard.io/docs/reference/mqtt-api/#rpc-api)        |
| v1/devices/me/rpc/request/$response_id | [command response](https://thingsboard.io/docs/reference/mqtt-api/#rpc-api)       |
| v1/devices/me/attributes               | [attributes](https://thingsboard.io/docs/reference/mqtt-api/#attributes-api)      |

