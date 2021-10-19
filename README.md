# OTA-Programmer
> IOT Website used to Upload Binary files, Tranmit it to the MCU using WIFI and Flash the Binary into MCU using simple UART Bootloader

### How to use

> note: No external libraries are used in This Project, So there's No dependencies except **ArmGCC** Toolchain downloaded in your computer, **edit the path of the Toolchain in the makefile before build** 

* burn the bootloader hex file into your **stm32f103xx** Microcontroller
* when the Mcu is Up, it will connect to WIFI with your **predefined username and password**
  * just edit  ```wifi_access.h``` file in ```Bootloader Code\includes``` directory with your **username** and **password**, then reBuild the project by typing ```make``` in your **cmd window** or your **Terminal** opened in ```/Bootloader Code/``` directory
* Go to the website http://www.bfota.freevar.com/ and select some hex file for the same Microcontroller, then press **Flash** and your file will be uploaded to the server, your Microcontroller shall be notified with the file Now
  * Note that only txt format is accepted in the website so make sure to convert the type ```.hex``` to ```.txt``` for your hex file then upload it
* wait approx. One Minute and Your Uploaded code will be Up and Running in the Microcontroller 

