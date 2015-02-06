# Aircondition
A Project using TX21IT+ wireless 868Mhz sensors with RFM12 and PIC24F to control air ventilators based on absolute humidity.
You will find the whole C source and Eagle project files inside.
The Circuit and Software uses the following technologies:
* OW protocoll managing config data for DS2430A 1 wire 256 EEPROM
* Microchip MCU PIC24F P24FJ32GB002
* Hope RFM12B IRQ driven communication with TX21IT+ LaCrosse IT+ Weather sensors
* IC2 communication with Newhaven Display NHD-C0216CiZ-FN-FBW-3V (which was tricky)
* Rotary encoder using digital filter algorithm for debouncing for configuring the system  

The setup is very simple:  
0. The system checks for valid config and CRC in the EEPROM and if OK goes to 3 or step 1  
1. You have to register the 2 TX21IT+ sensors each time you change their battery the get a new ID.  
2. Then you setup in sensor, out sensor, absolute humidity threshold, pause time, run time for ventitalors which is stored in the OW EEPROM  
3. Then the mainloop starts  
3.1. In the Mainlopp the MCU will listen with the RFM12B for 30 TX21IT+ records  
3.2. The ID of the records is used to match in and out sensors configured in step 1 and 2, if no sensor matches found it starts @1   
3.3 The temperature and relative humidity will be used to calculate absolute humidity for the in and out sensor  
3.5 If abs-hum-in > (threshold + abs-hum-out) the vens will run as long the configured runtime in step 2  
3.6. The system waits until the pause time is over and starte @3  
At any time a long press on the rotary knob will start reconfiguring mode step 1  

The system runs rockstable since 6 months in my garage with Marley Silencio Ventilators.
Bottom ventilator blows air from outside, top ventilator blows air to outside.
No moldy smell and always uses the right time for air condition to prevent mold creation during all seasons.

The project uses MPLABX and has also a .project Eclipse file which I used to utilize Git Eclipse integration to manage versioning.


