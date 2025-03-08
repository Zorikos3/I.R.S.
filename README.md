# I.R.S. - Infrared Remote System

I.R.S. is a powerful firmware for the M5Stick-C Plus that turns it into a universal IR remote control. It can power off TVs, projectors, LED remotes, and more with a single button press. Designed for maximum battery efficiency, it includes features like deep sleep mode, auto-dim, and a battery indicator.

## Features
- Power off **TVs**, **projectors**, and **LED remotes**.
- Deep sleep mode for maximum battery saving.
- Auto-dim mode to reduce screen brightness when idle.
- Battery level indicator with a visual icon.
- Toggle between built-in IR and external IR module.

## Supported Devices
- **TVs**: LG, Samsung, Panasonic, Sony, Philips, Sharp, Toshiba, Hisense, TCL, Vizio, Grundig, Loewe, Telefunken, Vestel.
- **Projectors**: Epson, BenQ, Optoma.
- **LED Remotes**: Common LED remote codes.

## Setup
1. Clone this repository.
2. Open the project in PlatformIO.
3. Build and upload the firmware to your M5Stick-C Plus.
4. Press the **M5 button** to start sending IR codes.

## Usage
- **Main Menu**: Press the **M5 button** to start sending IR codes. Press the **Next button** to toggle the IR output source.
- **Running**: The device sends IR codes and displays "Running..." on the screen.
- **Complete**: After sending IR codes, the device displays "Complete!" and waits for the next command.
- **Auto-Dim**: If no buttons are pressed for 10 seconds, the screen brightness drops to 1%. Press any button to restore brightness.
- **Deep Sleep**: If no buttons are pressed for 30 seconds, the device enters deep sleep mode. Press the **M5 button** to wake up.

## Credits
- Developed by **Orpheas / Zorikos3**.
- Uses the [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) library.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Flashing the Firmware
1. Download the `IRS.bin` file from the main page.
2. Use **M5Burner** or **esptool.py** to flash the `.bin` file to your M5Stick-C Plus.
   - For **M5Burner**:
     1. Open M5Burner and select your M5Stick-C Plus model.
     2. Click **Burn** and select the `firmware.bin` file.
     3. Follow the on-screen instructions.
   - For **esptool.py**:
     ```bash
     esptool.py --port <PORT> --baud 115200 write_flash 0x10000 firmware.bin
     ```
     Replace `<PORT>` with your device's serial port.
