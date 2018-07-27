# esp8266-AccessPoint-Logger-OfflineVisualisation
This repo contains the project code for an esp8266. The esp8266 shall work as access point and log temperature data. As the position of the logger will be far away from WiFi, the Data Visualisation shall via a Browser but without internet connection.

I created this repo hoping to get help and suggestions to the code. I am very new to this and am glad about every input you guys can provide. 

Sources:
https://techtutorialsx.com/2017/04/25/esp8266-setting-an-access-point/
https://tttapa.github.io/ESP8266/Chap16%20-%20Data%20Logging.html
https://42project.net/esp8266-flash-dateisystem-spiffs-beispielhaft-benutzen/
http://bienonline.magix.net/public/esp8266-webserver-klasse.html

Cheers!

Story: I want to brew beer. To do so, I need to find yeast that brews at a specific temperature. That specific temperature is the temperature of my basement. Unfortunately, I do not know the temperature in my basement. Therefore, I want to log the temperature down there to find the right yeast, with which I can work. Since there is no possibility to connect to the esp8266 to my router, I want to use it as access point, which I then can access and browse to a neat visualisation of the temperature profile over the last few days.

![Alt text](Situation.png?raw=true "This is what is up")
