Title:   RFID Reader
Summary: A documentation of the RFID Reader and how to use it.
Authors: Andrew Bargen
Date:    October 20, 2025

# About the RFID
The rfid reader used is the MFRC522. The [datasheet](https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf) describes the properties of the chip. It also describes how to use the chip. Thankfully, someone has already done the heavy lifting and has implemented a library with a few simple function calls so that we don't have to think about implementation details. The chip uses the SPI (Serial Periphrial Interface) to communcate with the microcontroller.

## SPI
SPI was invented in the 1980s by the Motorola Corporation. It uses four pins to communicate:

| Pin  | Name                | Function |
| ---- | ------------------- | ---- |
| MOSI | Master-Out-Slave-In | Transfers data from the microcontroller to the RFID reader. |
| MISO | Master-In-Slave-Out | Transfers data from the RFID reader to the microcontroller. |
| SCK  | Serial Clock        | Synchronizes the transmission of data |
| SS   | Slave Select        | Pulled HIGH by the microcontroller to select which slave device to communicate with |

Most of the communication that happens is done under the hood of the library we're using.

## Wiring up the RFID module

The RFID Mondule is connected according to the following table:

| Signal | MFRC522 Reader Pin | Arduino Pin |
| ------ | ------------------ | ----------- |
| RST/Reset  | RST         | D9             |
| SPI SS     | SDA(SS)     | D10            |
| SPI MOSI   | MOSI        | D11            |
| SPI MISO   | MISO        | D12            |
| SPI SCK    | SCK         | D13            |

## Programming the RFID module
The RFID reader can be tested by uploading an example program to the Arduino. Once the RFID Module is wired up, we can easily test its functionality and read data from RFID cards.

1. In the Ardino IDE, go to Tools > Manage Libraries, then search "mfrc522" and install the latest version (author should be listed as by "miguelbalboa" or "GitHub Community"), available at [GitHub](https://github.com/miguelbalboa/rfid)

2. Upload the [example sketch]({{ EXAMPLE_SKETCH_HERE }}) to the Arduino

3. Open the Serial Monitor (Tools > Serial Monitor or ctrl+shift+M) and check for errors. If the reader is connected sucessfully, try placing a card in the vicinity of the reader. If a number appears on the Serial Monitor, then the RFID is correctly connected and configured.


