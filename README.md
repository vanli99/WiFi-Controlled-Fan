# WiFi-Controlled-Fan

This project uses the Arduino IDE and the NodeMCU to program a fan that turns on and changes speed with a web page from any WiFi capable device.
The NodeMCU uses the ESP8266 WiFi module to enable the WiFi cababilities. It also acts as a microcontroller. 
There are three speeds: low, medium, high.
Each speed also has an associated LED that lights up when that speed is on. 
To control the speed, there is a webpage that can be accessed after going to the IP address that is provided in the code (192.168.1.1). 
On the webpage, there is a button that can be pressed mutiple times depending on what speed is desired. 
