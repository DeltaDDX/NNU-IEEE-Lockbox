# TODOs

## Power saving
### Sleep functions
implement sleep on nano every wake using IRQ pin on RFID general power management: turn off devices when not in use (screen, LEDs...)

#### Current progress
We implemented a funtionality in features/sleep where the controller sleeps whenever the IRQ pin is HIGH. Alternative method that might save even more power:

#### Improvement to current design
Both the controller and the rfid reader draw a current on the order of 10 mA. Put them both to sleep and save more power. This could be accomplished by sending the RFID reader to sleep, then sending the arduino to sleep for a set period of time, then waking it with the watchdog timer, then checking for a card...

The host controller needs to awake on its own; it cannot rely on the IRQ pin when the rfid reader is sleeping.

The sleep command to the RFID reader is done through the library (I think?). But the library we use (miguelbalboa/GitHub Community) says it isn't supported 🤔. https://github.com/miguelbalboa/rfid?tab=readme-ov-file#what-works-and-not

Potential Solutions (technical):

https://github.com/miguelbalboa/rfid/issues/269#issuecomment-287571721

https://forum.arduino.cc/t/rc522-rfid-real-sleep-mode/653664


### processing power
When the processor is not in sleep state: change the code from continuous polling in the `void loop ()` to something interrupt-driven.

## Persistent memory for the card UID
eeprom

## RFID compatability
RFID needs fix: new tag, so new library. The current library only reads MIFARE classic tags. we have NTAG213 (see "NFC ReWriter" in GitHub to write/read from the tags. Simply put: thes tags have different data fields mapped to different pages of memory. Probably something we'll have to save for a future iteration the design.

UPDATE: We CAN actually read these tags, it's just not outputting the correct number. See info.md .

## Low Voltage protection
Read the battery voltage and shut it down to prevent the arduino from dying due to low power
