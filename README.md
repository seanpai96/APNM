# APNM

This is a implementation of AUTOSAR's Adaptive Platform Network Management(NM) Service Module. This implementation uses a WebSocket connection to update node status and recieve `NetworkRequestedState` update.

This NM(not including the demo) complies with AUTOSAR's R22-11 Standard.

# Requriments
In order to build this demo with WebSocket support, you need to have IXWebSocket library built and installed in your device using [this guide](https://machinezone.github.io/IXWebSocket/build/).

# Licenses

The demo is using following third-party libraries: 

* [machinezone/IXWebSocket](https://github.com/machinezone/IXWebSocket) licensed under the [BSD-3-Clause license](https://github.com/machinezone/IXWebSocket/blob/master/LICENSE.txt)
* [nlohmann/json](https://github.com/nlohmann/json) licensed under the [MIT license](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT)
