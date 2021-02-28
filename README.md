      __ __| |           |  /_) |     ___|             |           |
           |   __ \   _ \  ' /  | |  / |      _ \ __ \   |      _` | __ \   __|
           |   | | |  __/  . \  |   <  |   |  __/ |   |  |     (   | |   |\__ \
          _|  _| |_|\___| _|\_\_|_|\_\\____|\___|_|  _| _____|\__,_|_.__/ ____/

# KIKPAD
KiKPad : the Midiplus SmartPad reinvented !  

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=thekikgen@gmail.com&lc=FR&item_name=Donation+to+TheKikGen+projects&no_note=0&cn=&currency_code=EUR&bn=PP-DonationsBF:btn_donateCC_LG.gif:NonHosted)

<img alt="SmartPad from Midiplus" style="border-width:0" src="https://img.static-thomann.de/thumb/orig/pics/bdb/456726/13950016_800.webp" /></a>

We did it again ! 

The KikGen Labs proposes again a firmware entirely rewritten for a commercial product.  This time it is the **Smartpad** from Midiplus, which technical characteristics being very close to the Launchpad from Novation. That firmware allows now 64 pads, 64 RGB colors (instead 3  !), 8 encoders, all buttons operationals, and obviously usb midin in/out.  It was written with the stm32duino platform, for the STM32F1 uC family. 

The perspectives offered by this new open firmware are vast: fully custom0ized Ableton Live controller, chord generator, autonomous sequencer, master keyboard , etc....For example, I have developped an Akai Surface controller emulation for the MPC Live, allowing my Kikpad to be used as it was the native hardware.

[![tkglctrl video](https://img.youtube.com/vi/l4OzAfEUoIQ/0.jpg)](https://www.youtube.com/watch?v=l4OzAfEUoIQ)

[![tkglctrl video](https://img.youtube.com/vi/PQ-h3_DM6EI/0.jpg)](https://www.youtube.com/watch?v=PQ-h3_DM6EI)

[![tkglctrl video](https://img.youtube.com/vi/fVG7otydEA0/0.jpg)](https://www.youtube.com/watch?v=fVG7otydEA0)

For a quick start, you can check the "demo" module, transforming the Kikpad in a small midi keyboard and controller. 

I want to thank here DerFetzer for his similar project (developed in Rust) and the inspiration for the launch of this project realized under "lockdown " !  have a look here : https://github.com/DerFetzer/open-cleverpad

Solenoid made also a great job by collecting a lot of information on the Midiplus Smartpad : https://github.com/s0len0id/smartpad-tester

<br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=thekikgen@gmail.com&lc=FR&item_name=Donation+to+TheKikGen+projects&no_note=0&cn=&currency_code=EUR&bn=PP-DonationsBF:btn_donateCC_LG.gif:NonHosted)

# How to flash

First, you must upload the bootloader at address 0x08000000, with STM-LINK UTILITY. You need an ST-LINK V2 stick to do that.   
It is necessary to open the Smartpad (remove panel from the back), and connect the stick to the CN2 .   

Pinout is :

* 1.GND
* 2.SWDIO
* 3.SWDCLK
* 4.RESET

You must also connect the USB cable for powering the Smartpad

Download the bootloader firmware here :
https://github.com/TheKikGen/stm32-tkg-hid-bootloader/releases

Open the ST-LINK utility, and flash the bootloader bin file at 0x08000000.

Unplug the STLINK, and the usb cable, and plug again the USB cable to the computer. You should see a new HID device in the device manager (or an lsusb command if under Linux).  Use then the tkg-flash utility provided with the bootloader to upload the Kikpad firmware.




