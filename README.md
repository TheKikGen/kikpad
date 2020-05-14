      __ __| |           |  /_) |     ___|             |           |
           |   __ \   _ \  ' /  | |  / |      _ \ __ \   |      _` | __ \   __|
           |   | | |  __/  . \  |   <  |   |  __/ |   |  |     (   | |   |\__ \
          _|  _| |_|\___| _|\_\_|_|\_\\____|\___|_|  _| _____|\__,_|_.__/ ____/

# KIKPAD
KiKPad : the Midiplus SmartPad reinvented !  

We did it again ! 

The KikGen Labs proposes again a firmware entirely rewritten for a commercial product.  This time it is the **Smartpad** from Midiplus, which technical characteristics being very close to the Launchpad from Novation.  

At this stage, the firmware is an advanced a prototype, with all pads, encoders, buttons operationals, including midi.   
The perspectives offered by this new firmware are vast: fully customized Ableton Live controller, chord generator, autonomous sequencer, master keyboard , etc....





A multi-port USB MIDI interface for the STM32DUINO platform.

Download the last firmware here : https://github.com/TheKikGen/USBMidiKliK4x4/releases

Check also the wiki here : https://github.com/TheKikGen/USBMidiKliK4x4/wiki

The story of this project starts with a hack of the MIDIPLUS/MIDITECH 4x4 USB to MIDI interface.
Needing more midi jacks, I bought a second Miditech interface, but I discovered it was not possible to use 2 Miditech / Midiplus MIDI USB 4X4 on the same computer to get 8x8, and according to the Miditech support, as that usb midi interface was not updateable at all !
I was stucked....That was motivating me enough to write a totally new and better firmware : the UsbMidiKlik4x4 project was born.

The current version V2 supports full USB midi until 16xIN , 16XOUT plus routing features, enabling configurables standalone mode, merge mode, thru mode, split mode, etc., huge sysex flow, configuration menu from serial USB, and is very fast and stable thanks to the STM32F103.  More of that, you can aggregate until 5 3x3 boards seen as one by activating the "Bus mode".
<img alt="SmartPad from Midiplus" style="border-width:0" src="http://www.midiplus.com/images/xpad-1.jpg" /></a>
  
  <br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=thekikgen@gmail.com&lc=FR&item_name=Donation+to+TheKikGen+projects&no_note=0&cn=&currency_code=EUR&bn=PP-DonationsBF:btn_donateCC_LG.gif:NonHosted)
