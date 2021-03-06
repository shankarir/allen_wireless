**Wireless Neural Recording System**

* [Overview](#Overview)
* [Hardware](#Hardware)
	* [Steps to convert old KiCad design files](#convertKiCad)
	* [Bridge](#Bridge-hw)
	* [Headstages](#headstages)
* [Firmware](#Firmware)
	* [Dependency](#Firmware-dependency)
	* [Bridge](#Bridge-fw)
	* [RHA-headstage](#RHA-fw)
	* [RHD-headstage](#RHD-fw)
		* [Firmware versions](#firmware-versions)
		* [JTAG testing](#JTAG-testing)
		* [Utility Programs](#Utility-programs)
* [Client Software](#gtkclient)
	* [Dependency](#gtkclient-dependency)
	* [Compilation](#gtkclient-compile)
    * [Running gtkclient](#running-gtkclient)
        * [RHA-headstage gtkclient](#RHAgtkclient)
        * [RHD-headstage gtkclient](#RHDgtkclient)
<!---        * [RHD-headstage gtkclient with AGC](#RHDgtkclient_AGC)
        * [RHD-headstage gtkclient with fixed gain](#RHDgtkclient_gain) -->
        * [Spike sorting](#SpikeSorting)
        * [Saving recordings](#SavingRecordings)
        * [Test recording](#TestRecording)
        * [Configuration files](#gtkclientConfigurations)
	* [TODO/possible improvements](#gtkclient-TODO)
* [Quick Start](#quickstart)

## <a name="Overview">Overview</a>
This project is modified from the multi-branch of the wireless neural recording system of the [myopen project](https://github.com/tlh24/myopen/tree/multi) by Tim Hanson.

As summarized in Hanson's [thesis](http://m8ta.com/tim/dissertation.pdf), the original system 

>consists of digitizing headstages, receiver/ethernet bridge, and PC client software. Each headstage has 4 32-channel Intan analog amplifiers, and ADCs to digitize the amplified signals. The amplifiers are connected to an onboard Blackfin DSP BF532 which performs automatic gain control, LMS adaptive noise cancellation, 8 poles of high and lowpass filtering, two templates comparison per channel, data compression, and pipelined radio control. The radio onboard transmits 1.3Mbps full filtered analog trace of four channels and threshold/template match for two units on all 128 channels. This is interleaved with 83kbps reception,which allows the client to edit parameters on the transceiver. The radio protocol allows any memory location to be changed on the headstage (specifically the blackfin), and the headstage streams can stream from any location in memory, with the memory address set by the PC client.

[32-channel Intan analog amplifiers](http://intantech.com/products_RHA2000.html)

[Blackin DSP BF532](http://www.analog.com/en/products/processors-dsp/blackfin/adsp-bf532.html)

The bridge, which is based around [Blackfin DSP BF537](http://www.analog.com/en/products/processors-dsp/blackfin/adsp-bf537.html), collects the wirelessly transmitted data from the headstages and send them to the PC client through ethernet.

<a name="figure_overview">*System overview, from Hanson's [thesis](http://m8ta.com/tim/dissertation.pdf)*</a>
![Hanson_Overview](https://github.com/allenyin/allen_wireless/raw/master/images/overview.png  "Hanson_Overview")

This project modified the wireless headstage by replacing each of the 4 RHA2132 amplifiers, ADCs, and power converter chips with a single [32-channel RHD2132 digitizing amplifier](http://intantech.com/products_RHD2000.html). This is to reduce the power consumption and total area of the final PCB (by about 1/3). In terms of the figure above, this is equivalent of replacing the `G=200 and 32:1 multiplexer`, `ADC` and `LDO` components in the `Digitizing headstage` blocks by one component. 

In the rest of this document, the original headstages will be refered to as **RHA-headstage**, while the new design will be referred to as **RHD-headstage**. The client software will be referred to as **gtkclient**.

This document will describe:

* The overall hardware architecture of the headstages.
* Usage and setup for the bridge, headstage, and the PC client software.
* The different firmware versions used to develop and test the RHD-headstage.
* Debugging tools.

For technical details on the implementation, development, and troubleshooting of the firmware, see Tim's thesis, and the following development notes:

1. [Wireless Headstage: Blackfin BF532 headstage firmware setup](http://allenyin.github.io/reading_list/2016/01/BlackfinSetup)
2. [Wireless Headstage: Blackfin BF532 communication with Intan RHD2132 via SPI simulation via SPORT](http://allenyin.github.io/reading_list/2015/12/Blackfin-Intan-SPORT)
3. [Wireless Headstage: AGC with Blackfin BF532 and Intan RHD2132](http://allenyin.github.io/reading_list/2015/12/WirelessAGC)
4. [Wireless Headstage: Least-Mean Squared (LMS) adaptive noise cancellation](http://allenyin.github.io/reading_list/2016/01/BlackfinLMS)
5. [Wireless Headstage: Implementing direct-form I biquad IIR filters](http://allenyin.github.io/reading_list/2016/01/DirectFormI-IIR-butterworth-filters)
6. [Wireless Headstage: Generate oscillations by setting IIR coefficients](http://allenyin.github.io/reading_list/2016/01/IIRoscillator)
7. [Spike sorting via PCA and spike detection with SAA](http://allenyin.github.io/reading_list/2016/01/BlackfinSpikeSorting)
8. [Wireless Headstage: Radio transmission protocol between headstage and gtkclient](http://allenyin.github.io/reading_list/2016/01/BlackfinRadioTrans)
9. [Wireless Headstage: gtkclient notes](http://allenyin.github.io/reading_list/2016/01/gtkclient)

This repository contains the following folders:

* `bridge_hardware`: Ported from myopen's [stage10](https://github.com/tlh24/myopen/tree/master/emg_dsp/stage10). Components  stored in `bridge_library/`. Footprint archived to `bridge_library.pretty/` to use with newer version of `kicad (Version:0.201506030104+5696~23~ubuntu14.04.1-product,release build)`.

* `headstage_hardware`: Ported from myopen's [stage9](https://github.com/tlh24/myopen/tree/master/emg_dsp/stage9). Components stored in `headstage_library/`. Footprint archived to `headstage_library.pretty/` to use with newer version of kicad.

* `headstage2_hardware`: Modified from myopen's [stage9](https://github.com/tlh24/myopen/tree/master/emg_dsp/stage9). Uses 4xRHD2132 amplifiers rather than 4xRHA2132 amplifiers front-end. Eliminated separate ADCs and analog power-supply components.

* `myopen_multi`: Firmware and gtkclient software, which includes:
    * `bridge_firmware`: Same as myopen's [firmware_stage7](https://github.com/tlh24/myopen/tree/mutli/firmware_stage7).
    * `headstage_firmware`: Firmware for RHA-headstage. Port from myopen's [firmware_stage9_tmpl](https://github.com/tlh24/myopen/tree/multi/firmware_stage9_tmpl). Uses templates matching to transmit spike timings from the headstage.
    * `headstage2_firmware`: Firmware, development and debugging files for RHD-headstage.
    * `gtkclient_multi`: gtkclient source code adapted from myopen. Makefile modified to compile on Ubuntu14.04 and to support working with RHD-headstage's firmware.
    * `flasher`: Code used to write to the flash memories of the bridge and headstage through the parallel port.
    * `common_bfin`: Code common to all firmware and gtkclient.
    * `common_host`: Code common to the bridge firmware and gtkclient.
    * `algs`: Utilities need by gtkclient.
    
---
## <a name="Hardware">Hardware</a>

## <a name="convertKiCad">Steps to convert old KiCad design files</a>
The bridge and RHA-headstage PCB were designed with Tim's [kicadocaml](https://github.com/tlh24/kicadocaml), a variant of the opensource [KiCad](http://kicad-pcb.org/) PCB cad program. The design files were ported to KiCad, however the recent changes in KiCad's EESchema introduces some problems in using projects created with older version out of the box. Those impacts include:

0. Updated device library. The new device library footprints are smaller, this can cause older schematics to be displayed with broken connections. Solution: follow the procedures outlined [here](https://lists.launchpad.net/kicad-developers/msg17639.html) to use the rescue-lib.
0. Saving the old .brd files will convert them to .kicad_pcb. Subsequent openning of the board files will then result in parse errors and no board. Solution: Never save!!! (have not yet found a good solution).

In general, the KiCad files in `bridge_hardware/` and `headstage_hardware/` are for reference only, no save/modification should be needed.

The following steps were used to convert the old project files (example using `headstage_hardware/`):

0. Extract stage9 from `/emg_dsp`.
0. Create component and footprint library folders. `cd headstage_hardware; mkdir headstage_library headstage_library.pretty`
0. Open the schematic file, press OK for the rescue components dialog. Copy the cached component to our newly created library: `cp stage9-cache.lib headstage_library/headstage.lib`
0. Go to *Preferences*->*Component Libraries* and add headstage as an active library.
0. Ctrl-E on any of the component to open library editor. Set active library to be *headstage* and save component.
0. Close schematic and open again. Now editing any of the components in the library editor should show the active library as *headstage.lib*
0.  **Important! When closing the schematic, do not click on save!**
0. Open the .brd file, click ok on the resulting dialog.
0. Ctrl-E on any of the footprint to open the footprint editor. Click on *Create new library and save footprint* from the menu bar, and select the *headstage_library.pretty* folder to store the new library.
0. Go to *Preferences*->*Footprint libraries Wizard*->*Files on my computer* and select the *headstage_library.pretty* folder. This will add that directory to the footpring libraries available in this project. Make sure to select avaliable to project only.
0. *Select active library* in the footprint editor and make it *headstage_library.pretty*.
0. Close footprint editor and go back to the board. Go to *file*->*Archive Footprints*->*Create footprint archive* and select *headstage_library.pretty* as the library. Click yes to replace library. Now *headstage_library.pretty/* contains all the footprint used in the board as .mod files.
0. Make sure when footprint editor is opened, the active library is set correctly to the one we just created.
0. Again remeber to **NOT save the board file!!**
0. Open the schematic file and run Cvpcb to associate the components and the footprints in our own custom libraries. It's possible that a footprint is found in multiple libraries, chose to associate with the ones in *headstage*.

## <a name="Bridge-hw">Bridge</a>
As illustrated by the [Receiver block in the system overview](#figure_overview), the bridge mainly consists of three antenna-2.4GHz radio pairs for wireless communication, Blackfin DSP for processing, Ethernet/MAC chip for communication with gtkclient, and DAC (typo of DAC in Tim's thesis) for outputting received neural signals as audio.

In addition to wireless communication with the headstages, the bridge is also needed to program and debug the headstages.

<a name="bridge_labeled">*Bridge Board* </a>

![labeled_bridge](https://github.com/allenyin/allen_wireless/raw/master/images/bridge_labeled.png  "labeled_bridge")

In the above image:

A. Power switch.

B. USB power cable. 

C. Ethernet cable. Each operating headstage requires a bridge, requiring the PC on which gtkclient operates on to be connected to multiple bridges. Therefore, the ethernet port of the bridges and the client PC should all connect to the same switch or router.

D. Audio jack. When gtkclient is running, connecting the bridge to an audio device allows the user to hear the the 4 selected channels in gtkclient.

E. Blackfin BF537 DSP processor.

F. JTAG header pins for debugging the bridge. The pin missing on the top row ensures correct orientation of the JTAG in-circuit emulator (ICE).

G. Header pins for writing firmware to the bridge's flash memory. See [bridge flashing instructions](#Bridge-fw).

H. Indicator LED lights. After power on, these LEDs would fluctuate. Normal operation is indicated by the LEDs lit up in the image lighting up. Except for the bottom most one, which should be blinking periodically.

I. Header pins for writing firmware to the headstage's flash memory. See [RHA-flashing instructions](#RHA-fw) and [RHD-flashing instructions](#RHD-fw).

J. Omnetics connector for writing firmware to the headstage's flash memory. The signals applied to the column of header pins above it are applied to the headstage through this connection to the corresponding omnetics connector on the headstage.

K. JTAG header pins for debugging the headstage. The pin missing on the left column ensures correct orientation of JTAG ICE. See [RHD firmware testing](#JTAG-testing).

L. Omnetics connector for JTAG debugging the headstage. The signals applied to the JTAG header pins are passed to the headstage through this connection.

M. 2.4GHz Nordic radio and antenna pairs. Having three antenna allow for spatial reception diversity and enable better wireless signal quality.

**Parallel Port Flash Cable**

A 25-pins [IBM PC-comptabile parallel port](https://en.wikipedia.org/wiki/Parallel_port#Pinouts) cable is modified to use for the flashing process, see [firmware section](#Firmware) for instructions.  The unmodified end of the DB25 cable is connected to the parallel port, while the modified end connects to the 7-pin header connectors on the bridge, labeled G and I above.

The flash cable on one end needs to be modified as shown below:

<a name="flash_cable">*Flash Cable Schematic*</a>![flash_cable](https://github.com/allenyin/allen_wireless/raw/master/images/flash_cable_schematic.png "flash_cable")

The left box shows the relevant pins on the unmodified DB25 connector, the right box shows the corresponding header pins on the bridge. There needs to be a `33pF` capacitor connecting `pin23` and `pin4` of the parallel cable. `pin5` of the cable needs to connect to the base of an NPN transistor, whose emmiter is connected to ground via either `pin23` or `pin24`, while the collector goes to `pin7` of the bridge headers.

Specifically, the parallel port to SPI correspondence is:

* `pin2` = `_CS` -- chip select of the SPI flash memory.
* `pin13` = `SO` -- the input of the parallel port is connected to the output (SO) of the flash memory.
* `pin3` = `SI` -- the output of the parallel port is connected to the input (SI) of the flash memory.
* `pin4` = `SCLK` -- serial clock (SCLK) of the flash memory.

`pin5` is not part of the 4-line SPI interface, but it is used to allow the line-driver onboard the bridge to select the flash memory to use the SPI bus. This is necessary since the SPI bus on the Blackfins of both the bridge and headstage is shared between the flash memory and the radio chip.

The modified flash cable will include the capacitor and the NPN transistor. The polarity of the 7-pin connector can be figured out by probing to see which pin is grounded onboard the bridge.

## <a name="headstages">Headstages</a>

In image below, the fully assembled RHA-headstage is on the left. The RHD-headstage is on the right.

<a name="headstages_labeled">*Headstage PCBs*</a>

![labeled_headstages](https://github.com/allenyin/allen_wireless/raw/master/images/headstages.png "labled_headstages")

Labeled in the image are the connectors the headstages can make:

A. RHA-headstage Omnetics connectors to electrodes. Each of the four connectors has two rows of 20 pins each. The leftmost and rightmost pins of each row are guide posts and connect no signals. The second leftmost and the second rightmost pins of each row are the reference electrodes.

B. RHA-headstage power connection to 3.7V battery.

C. RHA-headstage Omnetics connector for flashing memory. The JTAG debugging connector is on the back side of the board at the same location.

D. RHD-headstage Omnetics connectors to electrodes. Exactly the same as the RHA-headstage.

E. RHD-headstage power connection to 3.7V battery. Connection not installed in the pictre.

F. RHD-headstage Omnetics connector for flashing memory. The JTAG debugging connector is on the back side of the board at the same location.

The bottom half of both headstages are the `digitizing headstages` block in the [System overview diagram](#figure_overview), which is where the headstages differ hardware-wise. The top halves are the `transceiver block` and are identical.

While the electrodes for both headstages are identical, the electrode-to-amplifier connections are different. Below are the electrode-to-channel mapping for each headstage, containing only the 32 signal-carrying electrodes of the connector (excluding the guide posts). Note that each amplifier is connected to one group of 32 electrodes.

*RHA-headstage electrode-to-channel mapping, ordered from left-to-right*

top-row|31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|
-------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
bot-row|15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|

*RHD-headstage electrode-to-channel mapping, ordered from left-to-right*

top-row|23|22|21|20|19|18|17|16|15|14|13|12|11|10|9|8|
-------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
bot-row|24|25|26|27|28|29|30|31|0|1|2|3|4|5|6|7|

## <a name="Firmware">Firmware</a>
The firmware is written in a combination of C and Blackfin assembly language. While Analog Devices (ADI) provides its own software development toolchains, it requires purchasing a pretty expensive license (though it comes with a lot more support). For the myopen and this project, the open source gnu gcc blackfin toolchain is used. Further, all development was done on debian based OS. The Makefiles in this repository are tested only in Ubuntu 14.04. Minor adjustments might be needed to work on other debian-based OS.

## <a name="Firmware-dependency">Dependency</a>

Compiling the firmware requires the [Blackfin Toolchain in Linux for Bare Metal](https://blackfin.uclinux.org/doku.php?id=toolchain:installing). For debian-based linux OS:

* Download the latest version, as of June, 2015, it's `blackfin-toolchain-elf-gcc-4.3-2014R1-RC2.x86_64.tar.bz2`
* Extract and move to `/opt`
* Add path to `/opt/uClinux/bfin-elf/bin` to the `PATH` variable through `.bashrc`

Two must-have references to understand Blackfin architecture:

* [ADSP-BF5xx/ADSP-BF60x Blackfin Processor Programming Reference](http://www.analog.com/media/en/dsp-documentation/processor-manuals/Blackfin_pgr_rev2.2.pdf), describes all assembly commands and options.
* [ADSP-BF533 Blackfin Processor Hardware Reference](http://www.analog.com/media/en/dsp-documentation/processor-manuals/ADSP-BF533_hwr_rev3.6.pdf), describes all components and functions of the processor.  BF533 processor is mostly identical with BF532, which are the processors onboard the headstages. For the bridge, see BF537.

After compiling the firmware, the binary needs to be uploaded into the SPI flash memory onboard the bridge or headstage. This is done through a parallel port (for speed and relative ease). There are two ways this could be done.

**Using Libparapin**

In older computers with integrated parallel port (such as our lab's `rabbit` running DebianWheezy), [libparapin](http://parapin.sourceforge.net) can be installed to talk to the paralle port. This was used by Tim to develop and flash the RHA-headstages.

After installation of libparapin (in either user or kernel mode), compile `myopen_multi/flasher/flash.c` with `make clean; make`. 

With proper PC-to-bridge (and bridge-to-headstage) connections, compiled firmware binary can be uploaded with `sudo ./flash myprogram.ldr`.

**Using Pyparallel (recommended)**

Unfortunately Libparapin does not play nicely with parallel port PCI expansion cards and/or newer OS. I'm not sure what the exact culprit is, but the most recent version of libparapin is for kernel version2.6, and there hasn't been any activity on the project in several years.

Fortunately the python library [pyparallel](https://github.com/pyserial/pyparallel), allows us to interact with the parallel conveniently. Note that `pyparallel` can also refer to a Python parallel processing package. See [Lewis Loflin's blog posts](http://www.bristolwatch.com/pport/index.htm) for details on using the library.

After installing this module, firmware can be flashed with `python flash.py myprogram.ldr`. Error might occur indicating parallel port cannot be openened, in which case do `sudo rmmod lp; sudo modprobe ppdev` and try again.

Successful flash message:
```
python ../flasher/flash.py stage.ldr
ldr file: stage.ldr
File size 12202 bytes, 48 pages.

Requesting manufacturer opcode...

Read: 0xbf 

Looks like microchip flash.
Read: 0xbf 

Read: 0x25 

Read: 0x8d 

Read: 0xbf 

Detected: 
SST25VF040
SST25 sanity check ok.
Status register 0x0
Bank protect bits = read-write.
AAI off.
Block protect off.
Device is not write enabled. (default)
Device is ready.
Erasing SST25!
Waiting for the erase to complete..
Status register 0x0
Bank protect bits = read-write.
AAI off.
Block protect off.
Device is not write enabled. (default)
Device is ready.
write_all2_SST25 writing buffer length 12202
Verifying all..
 ok.

All pages verified correctly! :)
```
## <a name="Bridge-fw">Bridge</a>

The bridge firmware was not modified from the original myopen project (except the Makefile, for directory consistency). To flash the bridge, connect the flash cable to the bridge header pins as below, note the direction of cable connection is marked on the red flash cable.

<a name="Bridgeflash">*Flashing the bridge*</a>
![Bridgeflash](https://github.com/allenyin/allen_wireless/raw/master/images/Bridgeflash.jpg)

1. Go to the `myopen_multi/bridge_firmware/` directory, and do `make clean; make` to compile the firmware. 

	Note that each bridge is receives from a different headstage. Therefore, if multiple headstages are used, multiple bridges will need to be flashed and used. 

	If multiple briges are used, make sure the MAC addresses for the bridges are different. Go to `myopen_multi/common_bfin/ethernet.c`, in function `SetupMacAddr()`, change the value of `MACaddr[5]` so that the bridges used have different values.

2. Turn the bridge on, making sure the USB power cable is connected, then do `make flash`. 
 
	`make flash` will use either the libparapin based `flash.c` or pyparallel based `flash.py` to write the firmware to memory, based on what option is enabled in the Makefile under the `flash` rule. The default option will use the Python method.

## <a name="RHA-fw">RHA-headstage</a>

The RHA-headstage firmware was not modified from the original myopen project (except the Makefile, for directory consistency). To flash the heastage, connect the flash cable to the bridge header pins, and the 9-pins Omnetics cable to the bridge as shown below, note the direction of cable connection is marked on the red flash cable.

<a name="bridge_connection">*Bridge connection to flash headstage*</a>![bridge_connection](https://github.com/allenyin/allen_wireless/raw/master/images/bridge_connection.jpg)

1. Connect the other end of the 9-pins cable to the top-side omnetics connector on the headstage as shown below, note that the top wire on the bridge is also the top wire on the headstage.

	<a name="headstage_connection">*RHA-headstage connection*</a>![headstage_connection](https://github.com/allenyin/allen_wireless/raw/master/images/headstage_connection.jpg)

	Note that the RHA-headstage shown in the image is only the transceiver top half.

2. Go to the `myopen_multi/headstage_firmware/` directory, and do `make clean; make` to compile the firmware.

	Note that if multiple headstages are used, they must be configured to operate on different radio frequency. To change the radio channel, go to `myopen_multi/headstage_firmware/main.c` and search for the call to `radio_init()` and change the input argument. An input value of `84`, for example, sets the radio to operate on `2400MHz + 84Hz = 2484Mhz`. Other possible values are 94, 114, and 124. The maximum is 124. These changes must be followed by firmware recompilation.

    Theoretically, the 2.4GHz radio used onboard can operate with 2Msps on 62 different 2MHz bandwidth channels. The radio can operate from 2.4GHz to 2.525Ghz. When operating at 2Msps, each frequency requires a 2MHz bandwidth. Therefore, with no overlap, the input value to `radio_init()` can take values of 0, 2, 4,..., 124. However, the lower frequency bands are prone to attenuation by water absoprtion and interference from WiFi/Bluetooth/ZigBee bands. Therefore, the actual frequencies used should be limited to the higher bands. When multiple headstages are used, they should be set at frequencies as far apart as possible (note the separation of 10-20MHz for the values suggested).

    See [Nordic nRF24L01 manual section6.3](http://www.nordicsemi.com/eng/content/download/2730/34105/file/nRF24L01_Product_Specification_v2_0.pdf).

3. Turn on the bridge, and flash the headstage with `make flash`. 

	`make flash` will use either the libparapin based `flash.c` or pyparallel based `flash.py` to write the firmware to memory, based on what option is enabled in the Makefile under the `flash` rule. The default option will use the Python method.

4. Modify gtkclient:
 	* Make sure in `myopen_multi/gtkclient_multi/Makefile`, the option `HEADSTAGE_TIM` is set to `TRUE`, while setting `RADIO_BASIC`, `RADIO_AGC`, `RADIO_AGC_IIR` and `RADIO_AGC_IIR_SAA` to `false`.
	* Go to `myopen_multi/gtkclient_multi/src/gtkclient.cpp` and make sure the radio channel given to `radio_init()` in the headstage's firmware is present in the array `g_radioChannel`. 
	* If using multiple headstage-bridge pairs for recording, make sure the value of `NSCALE` defined in `myopen_multi/gtkclient_multi/include/gtkclient.h` is set to the number of headstage-bridge pairs used.
   
    After all (if any) gtkclient modifications, recompile gtkclient by navigating into `myopen_multi/gtkclient_multi` and type `make clean; make`. gtkclient is now ready to be used with RHA-headstages.

When powered on, the LED onboard should blink periodically.

The file `memory_map.ods` is a spreadsheet visualizing the memory organization used in the RHA-headstage firmware.

## <a name="RHD-fw">RHD-headstage</a>

The parallel port-to-bridge connections and bridge-to-headstage connections are identical for RHD-headstage as for RHA-headstages. However, there are multiple firmwares available in the `myopen_multi/headstage2_firmware` directory. To program the RHD-headstage to use the final firmware version -- `radio_gain_IIR_SAA.asm`, navigate to that directory and:

1. Uncomment the line `FIRMWARE_VERSION=radio_gain_IIR_SAA` line in `myopen_multi/headstage2_firmware/Makefile`.

2. Type `make clean; make` to compile the firmware.

	Note that if multiple headstages are used, they must be configured to operate on different radio frequency. To change the radio channel, go to `myopen_multi/headstage_firmware/main.c` and search for the call to `radio_init()` and change the input argument. An input value of `84`, for example, sets the radio to operate on `2400MHz + 84Hz = 2484Mhz`. Other possible values are 94, 114, and 124. The maximum is 124. These changes must be followed by firmware recompilation.

    Theoretically, the 2.4GHz radio used onboard can operate with 2Msps on 62 different 2MHz bandwidth channels. The radio can operate from 2.4GHz to 2.525Ghz. When operating at 2Msps, each frequency requires a 2MHz bandwidth. Therefore, with no overlap, the input value to `radio_init()` can take values of 0, 2, 4,..., 124. However, the lower frequency bands are prone to attenuation by water absoprtion and interference from WiFi/Bluetooth/ZigBee bands. Therefore, the actual frequencies used should be limited to the higher bands. When multiple headstages are used, they should be set at frequencies as far apart as possible (note the separation of 10-20MHz for the values suggested).
	
    See [Nordic nRF24L01 manual section6.3](http://www.nordicsemi.com/eng/content/download/2730/34105/file/nRF24L01_Product_Specification_v2_0.pdf).

3.  Turn on the bridge and do `make flash`. 

	There should be a line in the output message that says `FIRMWARE_VERSION is RADIO_GAIN_IIR_SAA`. If not, make sure again in Makefile that all other lines containing `FIMRWARE_VERSION=` is commented out.

4. Modify gtkclient:
	* Go to `myopen_multi/gtkclient_multi/Makefile` and set the option `RADIO_GAIN_IIR_SAA` to true, while setting `HEADSTAGE_TIM`, `RADIO_BASIC`, `RADIO_AGC`, `RADIO_AGC_IIR`, `RADIO_AGC_IIR_SAA` to `false`. This ensures gtkclient can correctly communicate with the RHD-headstage firmware.
	* Go to `myopen_multi/gtkclient_multi/src/gtkclient.cpp` and make sure the radio channel given to `radio_init()` in the headstage's firmware is present in the array `g_radioChannel`.
	* If using multiple headstage-bridge pairs for recording, make sure the value of `NSCALE` defined in `myopen_multi/gtkclient_multi/include/gtkclient.h` is set to the number of headstage-bridge pairs used.

	After all (if any) gtkclient modifications, recompile gtkclient inside `myopen_multi/gtkclient_multi` directory with `make clean; make` -- gtkclient is now ready to be used with RHD-headstage.

When turned on, the onboard LED should blink periodically.

## <a name="firmware-versions">Firmware versions</a> ##
There are other firmware version in this directory, many of them were used in the process and developing
the final firmware.

* `intan_setup_test.asm`:  

    Polls the same channel from all 4 Intan amplifiers, and save periods to memory starting at FP_BASE (0xFF906F00). Plot the result using `plot_memdump.py`, function `within_amp()`. Used to find optimal setup for the RHD amplifiers.

    Compile and flash with `make clean; make; make flash` after uncommenting `FIRMWARE_VERSION=intan_setup_test` and commenting out all other `FIRMWARE_VERSION` comments. Does not use the radio so not supported by gtkclient.

    The blackfin cache memory organization can be visualized by the spreadsheet `Intan_setup.ods`. Memory header is `memory_RHA.h` -- identical to RHA-headstage's `memory.h` file.

* `intan_setup_test2.asm`: 

    Tests the intra-amp performance. Records a set number of periods from all 32 channels of a fixed amp (see comments in file), save to memory in A1 (0xFF904000). Results include Intan-setup responses. Use `plot_memdump.py`, function `within_amp()` to plot the results.

    Compile and flash with `make clean; make; make flash` after uncommenting `FIRMWARE_VERSION=intan_setup_test2` and commenting out all other `FIMRWARE_VERSION` comments. Does not use the radio, not supported by gtkclient.

    The blackfin cache memory organization can be visualized by the spreadsheet `Intan_setup.ods` Memory header is `memory_RHA.h` -- identical to RHA-headstage's `memory.h` file.

* `firmware1.asm`:
   
    Integrated intan_test_setup2 into the firmware structure...after the setup process, repeatedly poll all the SPORT, but saves all channels from just one amp to WF_BUF. This allows for 256 32-bit words, which is equivalent to getting 16 samples from all 32-channels of the designated amplifier. 

    After saving the samples, part of the signal chain (AGC+IIR+SAA) was used to see how much code-path can be fit in before data corruption happens. The data in WFBUF can be dumped after it's full and plotted with `plot_packet_samples.py` (`plot_packet('bla.txt', 16, 32)`). Contains no radio code. Not supported by gtkclient.

    Memory header is `memory_RHA.h` -- identical to RHA-headstage's `memory.h` file.

* `firmware2.asm`:

    Same as `firmware1.asm`, except the extra signal path code is replaced with nops to investiage code-length issues.

* `radio_basic.asm`:

    Built off firmware2.asm, but integrate in radio communication with gtkclient. The radio communication involved is a stripped-down version of the original radio protocol -- writes in the raw samples, but since no spike-sorting is done, the matching bits are simply set to 0. Instructions to add in the echo nibble and packet#-in-frame nibble are present, so gtkclient can correctly read and interpret the packet information. 
                 
    Requires uncommenting the `FIRMWARE_VERSION=radio_basic` in makefile to compile, uses the "memory_radio_basic.h" file, which describes the memory layout. The memory header file is taken care of by the Makefile. The corresponding header will be moved to be "memory.h" when the `FIMRWARE_VERSION` option is set.

    The blackfin cache memory organization can be visualized by the spreadsheet `radio_basic_memoryMap.ods`.

    To use with gtkclient, compile gtkclient with `RADIO_BASIC` set to true in the Makefile. 

    While running gtkclient, since no matches are actually made, in the spike-window, the waveforms may be colored blue and messages in stdin about false-positivies can happen. This is normal and safe to ignore.

* `radio_AGC.asm`:

    Built off radio_basic.asm, but with added AGC functions. Same radio-communication protocol as in radio_basic. Compile with `FIMRWARE_VERSION=radio_AGC`, use "memory_AGC.h". 

    The blackfin cache memory organization can be visualized by the spreadsheet `radio_AGC_memoryMap.ods`.

    To use with gtkclient, compile gtkclient with `RADIO_AGC` set to true in its Makefile. Same spike false-positives will occur.

* `radio_AGC_IIR.asm`:

    Built off radio_AGC.asm, but added two IIR biquads to act as band-pass filter, set to [250, 9000Hz]. Compile with `FIMRWARE_VERSION=radio_AGC_IIR`, use "memory_AGC_IIR.h".

    The blackfin cache memory organization can be visualized by the spreadsheet `radio_AGC_IIR_memoryMap.ods`.

    To use with gtkclient, compile gtkclient with `RADIO_AGC_IIR` set to true in its Makefile. Same spike false-positives will occur.
                   
    <s>Note that unlike the original gtkclient, this version will not have an option to set the gain. This is because previously the samples were 12-bits, with Q16 values, the IIR may adjust its coefficients to apply a gain of up to 2 on its incoming samples, this is the mechanism through which Tim's gtkclient applies the gain. In the RHD version of headstage, the incoming samples are already 16-bits, therefore no gain needs/should be applied.</s> 
    
    The `IIR-gain` field allows an extra gain to be applied via the low-pass biquad. In practice, this should limitied between [-2.5, 2.5]. See [Wireless Headstage: Implementing direct-form I biquad IIR filters](http://allenyin.github.io/reading_list/2016/01/DirectFormI-IIR-butterworth-filters) for details. This is the case for all RHD firmware containing AGC and IIR stages.
                   
    By changing the IIR-biquad coefficients, oscillations of different frequency may be introduced. The `osc` radio button in this version of gtkclient does this. Clicking that radio button sets the IIR coefficients of the selected channels so the final IIR outputs oscilate at 250Hz. Clicking the `250-9000Hz` radio button restores the bandpass filtering behavior on the selected channels. This may serve as a good test for radio operation.

* `IIR_oscillator_test.asm`: 
   
    Used to test IIR-based oscillator. Basically is radio_AGC_IIR.asm without the radio transmission code in the main thread, or the radio_loop code. Sets up initial conditions on the biquads and their coefficients. 
                        
    Save 68 samples each for 32 channels on a single amp (or different, can be changed in code) to memory at 0xff806300. JTAG running and dumping the memory can visualize the resulting waveform. Visualize using plot_memdump.py, `within_amp_32ch()`. Used to confirm the biquad oscillation coefficients.

* `radio_AGC_IIR_SAA.asm`:   

     Adding the spike-sorting using SAA instructions to radio_AGC_IIR.asm. <s>This is the final firmware for RHD headstages</s>. Compile with `FIRMWARE_VERSION=radio_AGC_IIR_SAA`, uses "memory_AGC_IIR_SAA.h". 

     The blackfin cache memory organization can be visualized by the spreadsheet `radio_AGC_IIR_SAA_memoryMap.ods`.

     Edit on 4/5/2016 - Validation has shown that AGC makes spike sort harder when SNR is low. Therefore, this is no longer recommended as the final version for RHD-headstage.

* `radio_gain_IIR_SAA.asm`:

     The IIR and SAA parts of the signal chain are present in this version of the firmware. However, the AGC stage has been replaced by a manually set fixed-gain stage, the gain ranges from -128 to 127.5 in increments of 0.5. In gtkclient, this is the box labeled `preGain`. Compile with `FIRMWARE_VERSION=radio_gain_IIR_SAA`, uses "memory_gain_IIR_SAA.h".

    The blackfin cache memory organization can be visualized by the spreadsheet `radio_gain_IIR_SAA_memoryMap.ods`.

    <s>As of 4/5/2016, this is recommended as the final firmware version to be used on the RHD-headstage.</s>

* `radio_AGC_LMS_IIR_SAA.asm`:

    This version contains AGC, LMS, IIR, and SAA stage (the same signal chain as the RHA version), with the exception that the IIR stage contains only 1 LPF and HPF biquad). The LMS stage is incredibly useful for real recordings to reduce the noise and the IIR-gain can then amplifier the passband even more. LMS can be turned off via the setting in gtkclient. As of 7/26/2017, this is the preferred firmware version to use on the RHD-headstages.

    The blackfin cache memory organization can be visualized by `radio_AGC_LMS_IIR_SAA_memoryMap.ods`.


## <a name="JTAG-testing">JTAG testing</a>
Both RHD and RHA headstages have JTAG connectors that allows for in-circuit debugging. Documented here are the procedures for using the [gnICE debugger](https://docs.blackfin.uclinux.org/doku.php?id=hw:jtag:gnice-plus). 

After flasing the firmware you want to test onto the headstage, connect the 9-pin Omnetics connector labeled L on the [bridge](#bridge_labeled) to the 9-pin Omnetics connector on the back of the headstage.

Connect gnICE from PC USB port to the JTAG header pins labeled J on the [bridge](#bridge_labeled).

Turn on the bridge, in one terminal, type `bfin-gdbproxy -q bfin`, on successful detection of the debugger, the following message would appear:

```
Found USB cable: gnICE
Connected to libftdi driver.
IR length: 5
Chain length: 1
Device Id: 01100010011110100101000011001011 (0x627A50CB)
  Manufacturer: Analog Devices, Inc. (0x0CB)
  Part(0):      BF533 (0x27A5)
  Stepping:     6
  Filename:     /opt/uClinux/bfin-elf/bin/../share/urjtag/analog/bf533/bf533
warning:   bfin: no board selected, BF533 is detected
notice:    bfin: jc: waiting on TCP port 2001
notice:    bfin: jc:  (you must connect GDB before using jtag console)
notice:    bfin-gdbproxy: waiting on TCP port 2000
```
failed message such as:

```
Found USB cable: gnICE
Connected to libftdi driver.
warning: TDO seems to be stuck at 1
error:     bfin: detecting parts failed
Found USB cable: gnICE
error: Couldn't connect to suitable USB device.
error:     bfin: cable initialization failed
Found USB cable: gnICE
error: Couldn't connect to suitable USB device.
error:     bfin: cable initialization failed
```
Usually means the headstage-to-bridge JTAG connection is incorrect, or the bridge is not powered on.

After gnICE is detected, there are two ways to start a gdb debugging session. The first way is from terminal, do `bfin-elf-gdb stage.dxe`. Then in the `bfin-elf-gdb` prompt, type `target remote :2000` to connect to the gnICE. Successful connection would result in the following message in the gnICE terminal:

```
notice:    bfin-gdbproxy: connected
```
Debugging is then identical to using gdb.

The graphical debugger [Insight](https://www.sourceware.org/insight/) can also be used, and is recommended. A good manual for using Insight with blackfin can be found in section5.ch's [IceBear](http://www.section5.ch/dsp/icebear/ICEbear-manual.pdf) manual. IceBear is an alternative in-circuit emulator(ICE)/debugger for the blackfin, but has been discontinued.

To launch Insight, type `bfin-elf-insight stage.dxe`, should be able to connect to bfin-gdbproxy automatically. If not, in `File`->`Target Setting` chose the option `GDBServer/TCP` and for port enter `2000`.

As of version6.6, Insight may pop messages asking `Make breakpoint pending on future shared library load?` -- choose `no` on all of them. Error message saying `error: bad text index "4"` can also be safely ignored by clicking `ok`.

## <a name="Utility-programs">Utility Programs</a>

`myopen_multi/headstage2_firmware` also includes a number of utility programs used in the development and debugging process:

* `plot_memdump.py`, requires matplotlib, numpy and scipy.

    In JTAG debugging, memory values can be dumped when execution is paused. These values can then be processed and plotted. For example, while using `intan_setup_test2.asm`, with the following preprocessor setup:

    ```
    #define TESTFREQ                3200 // Hz
    #define STORAGE                 2480
    #define SAMPLE_PER_CH           150
    #define SAMPLE_PER_CH_MINUS_3   147
    #define TOTAL_CONVERT           4800   // 32*SAMPLE_PER_CH
    #define TOTAL_CONVERT_MINUS_3   4797
    ```

    and the subroutine `save_one_amp` as:
    
    ```
    save_one_amp:
        r1 = w[p0 + (SPORT1_STAT - SPORT0_RX)];
        cc = bittst(r1, 0);
        if !cc jump save_one_amp;

        // save new samples, in order from amp1 to amp4
        r2 = [p0 + (SPORT1_RX - SPORT0_RX)];    // SPORT1 pri - 2nd amp
        r3 = [p0 + (SPORT1_RX - SPORT0_RX)];    // SPORT1 sec - 1st amp
        r2 >>= SHIFT_BITS;
        r3 >>= SHIFT_BITS;
        //w[p1++] = r3;                           // save 1st amp
        //w[p1++] = r2;                           // save 2nd amp

        r2 = [p0];                              // SPORT0 pri - 4th amp
        r3 = [p0];                              // SPORT0 sec - 3rd amp
        r2 >>= SHIFT_BITS;                      
        r3 >>= SHIFT_BITS;
        w[p1++] = r3;                           // save 3rd amp
        //w[p1++] = r2;                           // save 4th amp
        rts;
    ```

    and setting a breakpoint at the line `jump we_finished`, when the execution is paused, we will have recorded 2472 32-bit words starting from memory location `A1` with the address `0xFF904000`. If we were applying a 3200Hz wave to some of the third group of electrodes, that would mean we have recorded 15 periods of that signal.

    In the gdb prompt of Insight debugger (or just the regular gdb prompt), we can dump the recorded signals (in addition to other setup values, see comments) to `gdb.txt` by:

    ```
    set logging redirect on
    set logging on
    x/2480xw 0xFF904000
    set logging off
    ```

    We can then plot the recorded values from all 32 channels of the 3rd group of electrodes in iPython with functions defined in this script:

    ```
    %run plot_memdump.py
    data = within_amp('gdb.txt', 32, 3200, 15)
    ```

    Below on the right is the plot result. Only 4 channels are actually applied the signal, the rest are grounded.

    <a name="JTAG1">*Using plot_memdump.py with intan_setup_test2*</a>
    
    ![JTAG1](https://github.com/allenyin/allen_wireless/raw/master/images/plot_memdump_intan_setup_test2.png)

    Use the function `within_amp_32ch()` for use with `IIR_oscillator_test.asm`.

    The function `saveCh_to_mat()` can convert the dumped gdb values as mat files, which can be imported into Matlab for analysis.

* `plot_packet_samples.py`

    Used with `firmware1.asm` and `firmware2.asm`. Saves samples from all 128 channels to WF_BUF, starting at memory location 0xff804000. Breakpoint should be set at `jump we_finished` in both files. At the end of execution, 256 32-bit words would be saved.

    The dumped values can then be plotted by `plot_packet_allChannels('gdb.txt')`.

* `fixedPointConvert.py`, `AGC_sim.py` contains functions that simulate the fixed-point conversion and arithmetic operations on Blackfin. 

    `AGC_sim.py` also contains simulates the AGC algorithm used in the headstage signal chain (see [AGC article](http://allenyin.github.io/reading_list/2015/12/WirelessAGC/) for details). Sample outputs of `AGC_sim.py` in iPython can be found in `AGC_sim_examples.txt`.

* `calc_memoryAddr.py` is used to confirm that given the different firmware version's memory file, gtkclient will be able to correctly calculate the relevant memory addresses.

    Even though all firmware is compiled with `memory.h`, it is actually a copy of either `memory_RHA.h`, `memory_radio_basic.h`, `memory_radio_AGC.h`, `memory_radio_AGC_IIR.h` or `memory_radio_AGC_IIR_SAA.h`. For example, to change the AGC target of a specific channel, the memory address to write the new value to in headstage running `radio_AGC.asm` is different from that running `radio_AGC_IIR.asm`. These memory address is calculated by gtkclient (specifically, in `myopen_multi/gtkclient_multi/src/headstage.cpp`) from the `memory.h` file generated by `headstage2_firmware/Makefile`.

    To use `calc_memoryAddr.py`, change the variables defined in the beginning of the file to match the corresponding firmware memory header file. Then run `setAGC(15)` for example to calculate what address gtkclient would output for changing the AGC target of channel 15. That address can then be verified against the `radio_AGC_memoryMap.ods`, for example, in the case of testing `radio_AGC.asm`.


## <a name="gtkclient">Client Software</a>

The client software, *gtkclient* is as summarized in Hanson's [thesis](http://m8ta.com/tim/dissertation.pdf)

> The sorting client is written in C/C++ using the GTK2 GUI toolkit with OpenGL and HLSL for graphics, [...] exclusively on Debian GNU/Linux. It consists of around 12600 lines of code, much for managing the GUI, displaying waveforms on the screen, maintaining persistent state, saving data, and communicating with [other] software clients [such as BMI3].

## <a name="gtkclient-dependency">Dependency</a>

Most of the dependency can be installed by going to `myopen_multi/gtkclient_multi` and run `make deps` and following the instructions.

Since gtkclient uses the GTK2 GUI toolkit, which may not be a default repository in newer linux distributions the user may have to edit the sources list or manaully download and install the packages. As of Ubuntu 14.04, this was not needed.

gtkclient also requires `nvidia-cg-toolkit`, it is recommended (if not mandatory) to run gtkclient on Nvidia graphics cards. Also, according to [NVIDIA](https://developer.nvidia.com/cg-toolkit)

>The Cg Toolkit is a legacy NVIDIA toolkit no longer under active development or support. Cg 3.1 is our last release and while we continue to make it avaiable to developers, we do not recommend using it in new development projects because future hardware features may not be supported.

gtkclient is known to work fine on `Nvidia GeForce GTX760/770/550 Ti`. 

On different Linux distros, the file path to `HDF5` and `libmatio` may be different. Compilation errors regarding these libraries can be fixed by giving the installed paths to them in [gtkclient's Makefile](https://github.com/allenyin/allen_wireless/blob/master/myopen_multi/gtkclient_multi/Makefile)(`HDF4INCLUDE`).

## <a name="gtkclient-compile">Compilation</a>

gtkclient can be compiled to work with both RHA-headstage and RHD-headstage, the latter of which may operate on multiple versions of firmware.

The deployment firmware versions is `myopen_multi/headstage_firmware/radio5.asm` for RHA-headstage, and `myopen_multi/headstage2_firmware/radio_gain_IIR_SAA.asm`.

Before compiling gtkclient to work with either RHA-headstage or RHD-headstage with the deployment firmware, one must:

* Match headstage numbers - make sure value of `NSCALE` set in `myopen_multi/gtkclient_multi/include/gtkclient.h` is equal to the number of headstages to be used.
* Match radio channels. 

    * First make sure the headstages to be used operate on different radio channels when flashing their firmware (see [RHA-headstage firmware section](#RHA-fw) and [RHD-headstage firmware section](#RHD-fw)) for instructions.
    * Make sure the radio channels of the headstages are present in the array `g_radioChannel` in `myopen_multi/gtkclient_multi/src/gtkClient.cpp`.

* Make sure if multiple headstages are used, the same number of bridges are used as well and connected to the PC running gtkclient through a switch or router. Also make sure the MAC address for each bridge is different, see [Bridge firmware section](#Bridge-fw) for instructions on this.

Then, if using RHA-headstage, go to `myopen_multi/gtkclient_multi` near the top of the file, set `HEADSTAGE_TIME=true` while setting `RADIO_BASIC=false`, `RADIO_AGC=false`, `RADIO_AGC_IIR=false`, `RADIO_AGC_IIR_SAA=false`, and `RADIO_GAIN_IIR_SAA=false`. Note that the options correspond to the different version of RHD-headstage firmware, with capitalization differences.

If using deployment firmware on RHD-headstage, go to `myopen_multi/gtkclient_multi` near the top of the file, set <s>`RADIO_AGC_IIR_SAA=true`</s> `RADIO_GAIN_IIR_SAA=true` while setting `HEADSTAGE_TIM=false`, `RADIO_BASIC=false`, `RADIO_AGC=false`, `RADIO_AGC_IIR=false`, and `RADIO_AGC_IIR_SAA=false`.

Then `make clean; make` to compile.

To compile gtkclient to work with other [firmware versions on RHD-headstage](#firmware-versions), set the corresponding option among `HEADSTAGE_TIM`, `RADIO_BASIC`, `RADIO_AGC`, `RADIO_AGC_IIR`, `RADIO_AGC_IIR_SAA` and `RADIO_GAIN_IIR_SAA` to `true` while setting the others to `false`.

## <a name="running-gtkclient">Running gtkclient</a>

Communication between gtkclient and the headstages work as follows:

1. When gtkclient is launched, it listens on the network for multicast messages.
2. When the bridges are launched, it sends out multicast packets.
3. gtkclient spawns `NSCALE` threads to listen for multicast packets from bridges. When a bridge packet is received, the sender bridge is mated with gtkclient, and one of these threads then handles all traffic from that bridge. 
4. gtkclient assigns the bridges to operate on the radio frequency given in the `g_radioChannel` array.
5. Each bridge then can exchange traffic with the headstage operating on the same radio frequency. The bridges then relay these traffic to gtkclient.

This protocol makes it necessary for `NSCALE` to match the number of bridges and headstages used, for different bridges to have different MAC addresses, for different headstages to operate on different radio frequencies, and for the headstage radio frequencies to match the ones in `g_radioChannel`.

Since this communication protocol requires using multicast traffic, it is necessary that if the bridges and the client PC are all connected to the same router/switch, the router/switch must be configured to disable multicast/IGMP filtering. Note that switches commonly allow multicast/IGMP traffic by default.

Also, make sure the client PC allows `allmulti` traffic for the ethernet interface by `sudo ifconfig eth0 allmulti` or adding the line to `/etc/network/interfaces` file to enable multicast permanently.

After compilation, gtkclient can be launched from the directory `myopen_multi/gtkclient_multi` by `./gtkclient`. It is recommended to turn on the the bridges after launching gtkclient to ensure mating. In the terminal window, messages such as:

```
Thread 140129180382976: rxed buf= neurobrdg
Thread 140129180382976: a wild bridge appears at 10.145.39.253
Thread 140129180382976: radio channel set to 84. It's super effective!
```
indicates 1) gtkclient has received a multicast packet `neurobrdg`, which 2) indicates the presence of a bridge at 10.145.39.253, and 3) the bridge is configured to operate on radio channel 84. `Thread 140129180382976` is the thread gtkclient spawned that is taking care of this new bridge.

There will be one series of these messages for each bridge turned on. The absence of these messages indicate a problem in multicast traffic communication. Make sure the routers are configured correctly and the computer's ethernet interface allows allmulti.

During operation, the headstages send packets containing waveform and spike matching data wirelessly to the bridge. The bridge then check and timestamps the packets to send to gtkclient. When gtkclient receives the packets from the bridge, it then processes the encapsulated data and renders display accordingly. An iterative estimate of the clock skew between the computer's performance counter and the timestamps received from the bridge is kept by gtkclient for timing.

Actions made in the gtkclient GUI may result in command packets sent from gtkclient to the bridge, then to the headstages, establishing two-way communication.

The gtkclient interfaces for both RHA-headstage and RHD-headstage are largely similar. It's recommended to read this entire section to understand the interfaces and their differences.

## <a name="RHAgtkclient">RHA-headstage gtkclient</a>

Below is a screenshot of the main interface of gtkclient compiled for RHA-headstage (`HEADSTAGE_TIM=true`), when the `rasters` tab is selected.

![RHAclient_main](https://github.com/allenyin/allen_wireless/raw/master/images/RHAclient_main.png)

On the top left corner, the message

```
radio Channel: 84
headecho:84:(SYNC)
```

displays the radio channels from `g_radioChannel`, which the bridge is configured to operate on.

Below that,

* `pkts/sec` displays the rate at which gtkclient is getting packets from the bridges, which is a good estimate of the number of wireless packets the bridge has received from the headstages.

* `dropped` shows how many packets have been lost or dropped since launching gtkclient due to wireless transmission. This number will always increase if the headstages are operating correctly. See [Debugging radio transmission] for more.

* `BER` shows the Bit Error Rate. When bit error happens with received packets, they are rejected by gtkclient.

* `strobe/sec` shows the rate at which gtkclient receives strobe packets, which is how other programs such as BMI3 request spike information from gtkclient.

Below that are four channel selection boxes, and their corresponding gain setting and AGC target setting inputs. Each group of three input boxes is labeled `A`, `B`, `C` or `D`. The signals from the selected channels are plotted correspondingly on the right. The headstages can transmit raw waveforms for only four channels at a time.

When one headstage is used, the range of channels is 0 to 127. When two are used, the range is 0 to 255, and so on.

In the waveform window, teal lines separate each plot, and purple lines represent 0 -- the signal level of seen by the reference electrode.

Below the channel selection, five tabbed panes may be selected - `rasters`, `spikes`, `sort`, `test`, and `file`. Displayed in the previous screenshot is the `raster` tab, which includes the signal-chain and raster-display configurations.

The `gain` and `AGC target` settings configure signal-chain parameters of the firmware onboard the RHA-headstage. The signal-chain consists of a series of stages that processes the samples from the ADC, all preceeding the spike template matching step.

For the RHA-headstage, the signal chain includes, in cascading order (details can be found in Tim's thesis and the links near the start of this document):

1. Automatic Gain Control (AGC) - amplifies the incoming signal so that the resulting signal has a fixed power level. The power level of each channel can be set with the `AGC target` input when that channel is selected. The button `Set all AGCtargets from A` sets the AGC target for all channels from all headstages to that of channel A.

2. Least Mean Squared (LMS) adapative noise cancellations - attenuates the noise from one channel using the signals from the previous 7 channels. This stage can be turned off for all channels by choosing the `off` option in the `LMS` box.

3. Infinite Impulse Response (IIR) digital filters. The RHA-headstage firmware implements an 8-pole butterworth bandpass filter by cascading four direct-form I biquads. The coefficients of these filters can be set in specific ways.

    * The value given to the `gain` setting results in new set of filter coefficients that are written to the headstage. The gain range is between -2 and 2.
    * The `500-6.7k` option in the `filter` box sets the filter on the four channels selected to pass the frequencies between 500Hz and 6.7kHz. This is the default option.
    * The `150-10k` option in the `filter` box sets the filter on the four channels selected to pass the frequencies between 150Hz and 10kHz. 
    * The `osc` option in the `filter` box sets the filter on the four channels such that the filter outputs oscillate at roughly 250Hz, with amplitude depending on the signal power prior to selecting this option. Note that the oscillation is only displayed when the `12 y4(n-1) (hi2 out,final)` option is selected in the `signal chain` box. 
    
        This is an easy way of checking if the wireless linke between the bridge and headstage is working. If there is a noticeable delay between checking this option and the waveforms showing oscillations, then the link quality is poor. Possible solutions include changing locations of the bridge/antenna, and restarting gtkclient and the bridges.

    * The `flat` option in the `filter` box sets the filter to pass all frequencies.

    Note that the default option for the `filter` box is `500-6.7k`. If another option e.g. `osc` is selected the changes will apply only to currently displayed channels (say Channel 7, 31, 8, and 16). When different channels are selected, the radio button for `osc` will still be selected, regardless of the new channel's filter settings.

The first selectable box on the `raster` page allows the user to select what part of the signal chain the headstage will transmit. The default option is `12 y4(n-1) (hi2 out,final)`, which is the output of the last stage of IIR filters.

The available options include:
* `0 mean from integrator`: Intermediate value used in the AGC stage, similar to an estimate of the raw input signal power.

* `1 AGC gain`: channel's AGC gain

* `2 LMS saturated sample`.

* `3 AGC out / LMS save`: Output of the AGC stage.

* `4 x1(n-1) / LMS out`: Output of the LMS stage, and input to the first IIR biquad.

* `5 x1(n-2)`: Input to the first IIR biquad from last time step.

* `6 x2(n-1) / y1(n-1) (lo1 out)`: Output of the first biquad, equivalent to the input to the second biquad. `lo1 out` indicates that the first biquad implements the first lowpass stage of the bandpass filter.

* `7 x2(n-2) / y1(n-2)`: Last time step's output of the first biquad/input to second biquad.

* `8 x3(n-1) / y2(n-1) (hi1 out)`: Output of the second biquad, equivalent to the input to the third biquad. `hi1 out` indicates that the second biquad implements the first highpass stage of the bandpass filter.

* `9 x3(n-2) / y2(n-2)`: Last time step's output of the second biquad/input to third biquad.

* `10 x4(n-1) / y3(n-1) (lo2 out)`: Output of the third biquad, equivalent to the input to the fourth biquad. `lo2 out` indicates the the third biquad implements the second lowpass stage of the bandpass filter.

* `11 x4(n-2) / y3(n-2)`: Last time step's output of the third biquad/input of the fourth biquad.

* `12 y4(n-1) (hi2 out, final)`: Output of the fourth biquad, also the output of the entire signal chain, and the default option selected. `hi2out, final` indicates the fourth biquad implements the second highpass stage of the bandpass filter.

* `13 y4(n-2)`: Last time step's output of the fourth biquad.

The ability to inspect all these intermediate values provide great debugging value.

Below the four waveform displays is the raster display, which shows detected neural spikes as blue or yellow dots. The four horizontal red lines indicate the channels whose waveforms are currently displayed. The vertical red line indicates the current time.

The `zoom` selection box sets the time length of the the displayed data for the waveform display (see `zoomSpinCB()` in `src/gtkclient.cpp`).

The `Raster Window` selection box selects which channels' rasters are viewed. Each raster window displays the rspikes for 128 channels. If only one headstage is used, this option is inactivated. If two headstages are used, a value of `1` will display the rasters for channel 128-255, corresponding to channels from the second headstage.

The `Raster span` selection box sets the time length of the displayed data for the raster display (see `g_rasterSpan` in `src/gtkclient.cpp`).


## <a name="RHDgtkclient">RHD-headstage gtkclient</a>

Below is a screenshot of the main interface of gtkclient compiled for RHD-headstage <s>deployment</s> firmware (`RADIO_AGC_LMS_IIR_SAA=true`), with the `raster` tab selected.

![RHDclient_main_LMS](https://github.com/allenyin/allen_wireless/raw/master/images/RHDclient_main_LMS.png)

It is now mostly identical to that of the RHA-headstage gtkclient. The biggest difference is the options available in the signal-chain selection box.

These differences reflect the differences in the DSP signal-chain implemented. Specifically, we have introduced `Samples from Intan` as the first inspectable signal, and the removal of the `x4(n-1)/y3(n-1)` to `y4(n-2)` items from the signal chain. The removal is due to the RHD-headstage only implement two IIR biquads instead of 4.

We have reduced the available filter options to either inducing osccilation using `osc`, or use the default option of `250-9kHz`. The `osc` option sets the filters to oscillate at 250Hz instead of 919Hz used in RHA-headstage.

The default signal chain, and that immediately preceeding spike sorting is `9 y2(n-1)/final output HPF`.

The available options are:
* `0 Samples from Intan`: They are samples output by the Intan RHD2132 amplifiers, before any DSP from the blackfin.
* `1 Integrated mean`: Intermediate value used in the AGC stage, similar to an estimate of the raw input signal power.
* `2 AGC gain`: channel's AGC gain
* `3 Saturated AGC out`: Saturated Output of the AGC stage, used for LMS stage after.
* `4 AGC/LMS out`: Ouput of the LMS stage. If LMS option is turned off, this is equivalent to AGC stage.
* `5 x1(n-1)/LMS out`: Output of the LMS stage, input to the first IIR biquad.
* `6 x1(n-2)`: Last time step's input to the first IIR biquad.
* `7 y1(n-1) LPF`: Output of the first IIR biquad, equivalent to input to the second IIR biquad.
* `8 y1(n-2) LPF`: Last time step's output of the first IIR biquad/input to the second IIR biquad.
* `9 y2(n-1)/final output HPF`: Output of the second IIR biquad, final output of the signal chain. It's the default option.
* `10 y2(n-2) HPF`: Last time step's output of the second IIR biquad.
    
## <a name="SpikeSorting">Spike sorting</a>
Below is a screenshot showing the RHDgtkclient-gain's `spikes` tab. Other than the absence of AGC setting, it is identical to RHAclient's.

![spike_page](https://github.com/allenyin/allen_wireless/raw/master/images/RHDgtkclient_spikes.png)

Spike sorting in gtkclient is very similar to that used by Plexon's SortClient and OfflineSorter. Four channels can be sorted at a time, corresponding to the channels being transmitted. 

Waveforms are extracted around threshold and overlaid in each channel's window. The threshold can be set in both time and voltage domain. The threshold point is indicated by the intersecting gray lines in each channel's windows. Threshold is positive-only. The threshold values will be displayed on the left when the `sort` tab is selected.

The PCA-projection of the overlaid waveforms are shown as dots to the right of the waveforms. Holding down the right mouse-button while moving through the points will show the corresponding waveform in green. Double-clicking at the area will recalculate the PCA-projection, this is useful to reset the PCA view.

The PCA-projected points can be selected via a polygonal lasso - left-clicking and tracing out a loop. Right-clicking the selected area gives the options of `set as template 1 (cyan)` and `set as template 2 (red)`. If one of these options is selected, the mean and L1 norm of the selected points are computed, and sent to the headstage as the template and aperture for spike-matching.

The selected points, and any future points in the future that fall within the selected area will then turn cyan or red. The aperture of the set templates are also displayed on the left side of the `spikes` page. The overlaid waveforms that correspond to the colored PCA-projections will also change from gray to assume their corresponding template colors. Waveforms that don't belong to either sorted clusters will remain gray.

In the screenshot, both templates have been set for channel 71 (not really spikes, but just demonstrating the sorting tool). In all the overlaid waveform windows, there are also thicker orange and purple traces. Those are the 16-points waveform templates for those channels (as a result of calculating the mean of PCA-projections). These traces change whenever a template is set through the polygonal lasso. Note that the width of the 16-points templates are limited within the faint blue strip, which is more narrow than the overlaid waveforms.

After the template and apertures for a channel are set and sent to the headstage, they will be used to detect spike onboard the headstage. The detection of spikes are sent along with raw waveform data in the radio packets, and these spikes are plotted in the raster window. When a channel is also one of the four channels whose raw samples are streamed, gtkclient will convolve the templates for that channel with the last 24 continuously transmitted samples and compare against the corresponding aperture to check for spikes. If gtkclient detects a spike but the radio packet does not report one, then `missed spike` message will be printed out in the terminal. If gtkclient does not detect a spike, but the radio packet does, then `false positive` message will be printed out instead.

When spikes are detected, the interspike interval, binned at 768us spike sample rate is plotted on the bottom of the PCA-projection window. This can be seen in the screenshot as well for channel 71. 

## <a name="SavingRecordings">Saving recordings</a>

Below is a screenshot showing the client's `file` tab, identical for both RHAgtkclient and RHDgtkclient.

![files_page](https://github.com/allenyin/allen_wireless/raw/master/images/RHDgtkclient_filePage.png)

The right side of the window for the `file` tab is identical to that of `raster` tab. In the screenshot, the spikes detected on channel 31 are more visible as yellow and blue dots on the raster display.

On the left side, the `files` tab provides several configurations:

1. `cycle channels` checkbox. When checked, each of the four selected channel number will increment by 1 every 3 seconds.

2. `draw mode` box:
    * When `lines` option is selected, the raw waveforms displayed here, as well as the overlaid waveforms in the spike-sorting view will have lines connecting the received sample points. This is default option, and is the most intuitive plotting option.
    * When `points` option is selected, the raw waveforms displayed here, as well as the overlaid waveforms in the spike-sorting view will simply plot the individual sample points. The display results will be more difficult to inspect, but it is useful when the amount of rendering needed increases, as a result of rapidly varying waveforms with high amplitudes and/or a lot of spike detections to plot.

3. The text `configuration.bin` is displayed below the `draw mode` box. This indicates the configuration file we are reading from/writing to. See [Configuration files section](#gtkclientConfigurations).

4. The `sync headstage` button, when pressed, will reset the signal-chain and spike-matching parameters stored on the headstage to match those set in gtkclient. A noteable exception is that the IIR filters will be reset to 500Hz to 6.7kHz bandpass for RHAgtkclient, and 500Hz to 9kHz bandpass for RHDgtkclient, regardless of what option is set in the `filters` box, which will not change.

5. The `record` button initiates the recording process. When pressed, file dialog will prompt for where to save the file.

6. The `stop` button stops the recording to file. The size of the recording is shown under the `sync headstage` button.

7. The `pause` checkbox pauses an ongoing recording.

The recording files are saved as a binary file. The data recorded include:

* Received incoming packets in addition to client clock and bridge clock.
* Outgoing packets, along with ASCII versions of the commands.

The program `convert` is compiled along with gtkclient from `src/convert.cpp`. Running `convert infile.bin` yields four output files from the saved recordings, which can be easily processed in Matlab.

* $.mat, which contains the vectors:
    * `time` - wall time within the client, synchronous to the BMI (or any program requesting firing rates through TCP/IP).
    * `mstimer` - hardware clock on bridge, runs at 9155.273438Hz. One entry per received packet. Timestamps for spikes should be pretty accurate.
    * `spike_ts` - spike times, indexes time and mstimer. The spikes are matched on the headstages but only timestamped on teh bridge to conserve bandwidth.
    * `spike_ch` - Channel of the spikes detected, same length as `spike_ts`.
    * `spike_unit` - Unit of the spike detected in a channel. Same length as `spike_ts`. Note this wireless system assumes that both units in a channel can't fire at once.

* $.nlg: Contains raw signed 8-bit integer matrix of analog traces (the continuously transmitted four selected channels). It's a matrix of 4x(received packets x 6). Each packet contains 6 samples from 4 channels. The script `plot_raw_channels.m` will plot these signals given the name of the nlg file.

* $.chn.gz: gzipped version of $.nlg files.

* $.log.gz: gzipped text of messages within the file (i.e. ASCII versions of the outgoing commands)

To plot the analog traces, see [plot_raw_channels.m](https://github.com/allenyin/recording_validation/blob/master/analysis/plot_raw_channels.m).

To convert the spike times relative to recording start, the following scheme can be used:

```matlab
load(spikeFile);                % load .mat file
f_mstimer = 9155.273438;        % hardware clock in Hz
mstimer = double(mstimer);      % convert from uint32 to double
spike_ts = double(spike_ts);    % convert from uint32 to double

mstimer = mstimer - mstimer(1); % process mstimer to start at 0
mstimer = mstimer/f_mstimer;    % convert values in mstimer to seconds
spikeTimes = mstimer(spike_ts); % spike times in bridge time, in seconds
```

## <a name="TestRecording">Test recording</a>

Below is a screenshot showing the client's `test` tab, identical for both RHAgtkclient and all versions of RHDgtkclient.

![test_page](https://github.com/allenyin/allen_wireless/raw/master/images/RHDclientfinal_testPage.png)

The right side of the window for the `test` tab is identical to that of `raster` tab. In the screenshot, the spikes detected on channle 71 are more visible as yellow and blue dots on the raster display.

On the left side, the `test` tab provides several buttons. The purpose of this tab is to facilitate [recording from simulated signals](http://allenyin.github.io/reading_list/2016/03/WirelessValidationPktLoss). Simulated signals can be saved as audio files and played from the same computer where gtkclient runs. The audio signal can be passed to the wireless headstages (RHA or RHD) via [Plexon headstage tester units](http://www.plexon.com/products/headstage-tester-units). This page allows the user to specifiy the audio file, and start both the recording and audio with minimal delay.

1. `Load Audio File`: Brings up a dialog asking the user to specify the desired audio file. After selection the file's name will be displayed under the `Audio File:` field.

2. `Record with Audio`: Brings up a dialog asking the user where to save the recording file. After selection, the recording will be started, along with vlc playing the selected audio file. There is roughly a 0.4-0.5 seconds delay between recording start and vlc playback start. The recording file size as well as audio time (min:sec) will be displayed. While a recording initialized this way is happening, pressing the `record` button in the `file` page triggers nothing, and vice versa.

3. `Stop`: Stops the recording. Allows new recording to be initiated subsequently from either `test` or `file` page.

## <a name="gtkclientConfigurations">Configuration files</a>

Instances of gtkclient saves information about its state via [protobuf](https://developers.google.com/protocol-buffers/docs/cpptutorial) in two binary files: `state.bin` and `configuration.bin`.

Upon launching, gtkclient looks for these two files. Sorted spike templates, channel threshold, signal chain settings and radio channel information settings are all saved in `configuration.bin`. If these files do not exist when gtkclient is launched, they will be created.

It's important to remember that the session's settings will only be saved at the end if gtkclient is terminated by clicking the 'x' button on the GUI. Forced termination using `ctrl-c` will result in losing these setting information.

If you want to use the sorted templates created from a different gtkclient instance (from another computer or in a different folder), simply copy the `configuration.bin` from that instance to the current gtkclient directory. Upon launching, the copied over `configuration.bin` file will be read instead. You might want to back up the replaced file for later.

**Caution**
However, be careful to not mix the usage of RHA-headstage/gtkclient generated templates with that generated by RHD-headstage/gtkclient. This is because not only the headstages differ in electrode layout (e.g. channel 31 on RHA-headstage is actually channel 23 on RHD-headstage), but also since the DSP signal-chains differ, the user would have to go through all the channels to resort.

In general, do not mix using RHA-headstages with RHD-headstages.

## <a name="TCPgtkclient">Communication with other programs</a>

gtkclient can send firing rates information to requester throgh a TCP/IP socket. See Skunkape for implementation.

## <a name="gtkclient-TODO">TODO/possible improvements</a>
* More threadsafe

* Redsicovery of bridges after gtkclient is killed...maybe implement by remembering IP of bridges?

* Spatial bridge redundancy to improve range? Need to rewrite the multicast protocol to bind then..

* Graphics improvements --opengl??

* UI improvements. For example, doing `sync headstage` result in resetting the radio buttons in `filters` box..will require exposing the buttons pointer.

---------------------
This section has only covered using gtkclient compiled with `HEADSTAGE_TIM` option (for RHA-headstage), and that compiled with `RADIO_AGC_IIR_SAA` option (for RHD-headstage). 

Compilations of gtkclient with the other RHD-headstage options contains a subset of the `RADIO_AGC_IIR_SAA` gtkclient's functions.

## <a name="quickstart">Quick Start</a>

Here are steps to use 2 RHD-headstages with deployment firmware:

1. Follow the [bridge flashing instructions](#Bridge-hw) to flash two bridges, making sure they have different MAC addresses. It is ok to skip this step if there are two existing bridges known to be flashed already and have different MAC addresses.

2. Follow the [RHD-headstage flashing instructions](#RHD-fw) to flash two RHD-headstages, making sure they have different radio channels. Modify gtkclient as instructed and recompile. When connected to battery, the LED should blink.

3. Launch gtkclient inside the `myopen_multi/gtkclient_multi` with `./gtkclient`.

4. Turn on the bridges. The terminal from which gtkclient is launched should print out two sets of messages such as

    ```
    Thread 140129180382976: rxed buf= neurobrdg
    Thread 140129180382976: a wild bridge appears at 10.145.39.253
    Thread 140129180382976: radio channel set to 84. It's super effective!
    ```
    indicating the bridges have been discovered by gtkclient.

5. Waveforms should start appearing on the waveform plots when gtkclient's `raster` or `files` tab is selected. If not, check if the heastages are powered, and if the radio-channels were set up correctly.
