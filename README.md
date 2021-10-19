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

### About your Code

* head to the ```mem.ld``` or ```stm32_flash.ld``` in your Project and edit the following section
```C
  /* Specify the memory areas */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 20K
  MEMORY_B1 (rx)  : ORIGIN = 0x60000000, LENGTH = 0K
}
```
  whatever the number in the ```LENGTH``` beside ```FLASH (rx)```, you need to decrease it by **8**, and the ```ORIGIN``` need to be ```ORIGIN = 0x08002000``` which is the start address of your Code in Flash memory
  
so it should be look like this
```C
  /* Specify the memory areas */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08002000, LENGTH = 56K
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 20K
  MEMORY_B1 (rx)  : ORIGIN = 0x60000000, LENGTH = 0K
}
```
the reason is that the Bootloader is taking the first **8** pages of your Flash space **i.e. 8 KB**, so the Application shall be flashed in the Remaining space, taking only 56 KB of the 64 KB and starting from ```0x08002000``` instead of ```0x0800000``` , **Leave the Rest of code as it is**

