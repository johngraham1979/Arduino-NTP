# Arduino NTP Client using the duinotech ESP13 with ESP8266 WiFi chip.
## Arduino project code for conducting NTP communication and decoding the time.

**Included in this repo:**
- Arduino *NTPClient.ino* project file that, when uploaded to the ESP13, connects to the internet over WiFi and requests, recieves and prints the current time. 
- New Arduino libraries that the .ino project requires. These libraries hide unnecessary complexity from the Arduino programmer of the NTP communication, calculating the hours, minutes, seconds, creating a time-string.

**17 February 2018:**  
1. This has been written-for and tested-with duinotech's ESP13 board (containing an ESP8266 WiFi chip).

**Guide:**
1. Install the ESP8266 boards for the Arduino IDE/framework by following this guide: https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/
2. Clone or otherwise download this repo.
3. Copy the library folders from this repo (*TimeConverter*, *NTP*, and *ESP8266WiFi*) to your *../Arduino/libraries* location. On the Windows operating system, this is defaulted to the user's *Documents* folder.
4. Copy the Arduino project folder (*NTPClient*) to wherever you store your regular Arduino projects.
5. Open the *NTPClient.ino* Arduino project file with the Arduino IDE.
6. Upload this code to your ESP13 WiFi board (see the **Programming the ESP13 board** section).
7. Select "Serial Monitor" from the Arduino IDE's Tools menu to periodically see the latest time.

**Notes:**
1. Originally programmed and tested on the duinotech's ESP13 WiFi board (with ESP8266 WiFi chip), the code has been written with the goal of ultimately working with Arduino compatible RJ45 (non WiFi) boards as well. Testing and polishing this additional use is pending, however.
2. Note in the line of code in the NTPClient.ino project file: `const char* timeString = TimeConverter::getInstance( UTC_OFFSET )->getTimeCString( ntp.decodeSeconds() );` ...

  `ntp.decodeSeconds()` extracts the POSIX time (in total seconds) from the NTP response, and the rest of the line of code above provides a time string in the format HH:MM:SS.
  
  Alternatively, if you are wanting to do something specific with either the hours, minutes of seconds, then these can be obtained by:
  
```
    unsigned long totalSeconds = ntp.decodeSeconds();
    
    unsigned short hours = TimeConverter::getInstance( UTC_OFFSET )->getHours( totalSeconds );
    
    // Note: The UTC_OFFSET only needs to be passed once.
    
    unsigned short minutes = TimeConverter::getInstance()->getMinutes( totalSeconds );
    
    unsigned short seconds = TimeConverter::getInstance()->getSeconds( totalSeconds ); 
```
 
**Programming the ESP13 board:**
See the image below.
1. Use a jumper cable to join *D0* to *G*. Note:
  - There is a 2 x 9 grid of pins with the two rows labelled *G*, *3V*, *D*. Each column is numbered. This is where to find *D0*.
  - There is a 3 x 4 grid of pins with the three rows labelled *A*, *5V*, *G*. Connecting *D0* to any of the pins on the row labelled *G* is fine.
2. Connect USB wires to the board to provide power and enable writing. Note:
  - There is a 2 x 4 grid of pins with the two rows labelled *UART*, *OTH*. The columns are labelled *3V3*, *G*, *RXO*, *TXO*.
  - The red and black wires from the USB plug connect to the *3V3* and *G* pins, on the *UART* row, respectively.
  - The green and white wires from the USB plug connect to the *RXO* and *TXO* pins, on the same *UART* row, respectively.
3. Select *Generic ESP8266 Module* as the board.
4. The port may not have a descriptive label other than *COM#*.
5. Code can now be uploaded to the ESP13 board.
6. the *RST* button on the ESP13 board may need to be pressed before each upload.

**Image(s)**

![alt text](https://i.imgur.com/Ky0mRgn.jpg)
Fig 1. USB plug wiring as per the instructions described in the **Programming the ESP13 board** section.

Email any questions or comments to me, John, at john.graham@outlook.co.nz. Please begin your email title with "NTPClient Repo".
I look forward to answering your questions or reading your comments.
