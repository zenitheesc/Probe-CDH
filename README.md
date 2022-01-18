# Probe-CDH
<h1 align="center" style="color:white; background-color:black">Probe-CDH</h1>
<h4 align="center"> Telecommunication board for the Zenith's Stack architecture.</h4>

<p align="center">
	<a href="http://zenith.eesc.usp.br/">
    <img src="https://img.shields.io/badge/Zenith-Embarcados-black?style=for-the-badge"/>
    </a>
    <a href="https://eesc.usp.br/">
    <img src="https://img.shields.io/badge/Linked%20to-EESC--USP-black?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Probe-CDH/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/zenitheesc/Probe-CDH?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Probe-CDH/issues">
    <img src="https://img.shields.io/github/issues/zenitheesc/Probe-CDH?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Probe-CDH/commits/main">
    <img src="https://img.shields.io/github/commit-activity/m/zenitheesc/Probe-CDH?style=for-the-badge">
    </a>
    <a href="https://github.com/zenitheesc/Probe-CDH/graphs/contributors">
    <img src="https://img.shields.io/github/contributors/zenitheesc/Probe-CDH?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Probe-CDH/commits/main">
    <img src="https://img.shields.io/github/last-commit/zenitheesc/Probe-CDH?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Probe-CDH/issues">
    <img src="https://img.shields.io/github/issues-raw/zenitheesc/Probe-CDH?style=for-the-badge" />
    </a>
    <a href="https://github.com/zenitheesc/Probe-CDH/pulls">
    <img src = "https://img.shields.io/github/issues-pr-raw/zenitheesc/Probe-CDH?style=for-the-badge">
    </a>
</p>

<p align="center">
    <a href="#environment-and-tools">Environment and Tools</a> •
    <a href="#Block-Diagram">Block Diagram</a> •
    <a href="#Schematic">Schematic</a>
    <a href="#Board">Board</a>
</p>

## Environment and tools

This board was designed using KiCad 5.1.10.

## Block Diagram

<p align = "center">
<img src="https://github.com/zenitheesc/Probe-CDH/blob/main/Other_Files/Images/Diagram.png"/>
</p>


## Schematic

The design was separated in hierarchical sheets to accommodate all components in an orderly way.

The first page contains the STM32L476 with all its accompanying circuits; crystals, debugging LED, Boot solder jumper, decoupling capacitors and reset buttons. On the same page you find the stack connector along with a programming and debugging connector and an external GPS connector (can also be used for UART communication with external devices, it’s bad but it’s there).


<p align = "center">
<img src="https://github.com/zenitheesc/Probe-CDH/blob/main/Other_Files/Images/Schematic_Page_1.png"/>
</p>

The second page contains the two LoRa and (G)FSK radios, one capable of 2 W off transmission power when fed with 6 V (obtained with the linear regulator), and the other capable of 160 mW, used as a backup or low power option. Both of them are based on the Semtech SX1262 transceiver, for operation at 915 MHz. The u-blox NEO-M8N also lives on this page, with its backup super capacitor for hot starts.

<p align = "center">
<img src="https://github.com/zenitheesc/Probe-CDH/blob/main/Other_Files/Images/Schematic_Page_2.png"/>
</p>

The last page houses the remaining peripherals which include the SD Card, the 128 Mb Flash, the CAN Transceiver and the buzzer connector with its mosfet, which may also be used as an external actuator with a max current of 1 A (the mosfet actually handles up to 1,5 A but let’s not push it too far).  

<p align = "center">
<img src="https://github.com/zenitheesc/Probe-CDH/blob/main/Other_Files/Images/Schematic_Page_3.png"/>
</p>


## Board

<p align = "center">
<img src="https://github.com/zenitheesc/Probe-CDH/blob/main/Other_Files/Images/Top.png"/>
</p>

<p align = "center">
<img src="https://github.com/zenitheesc/Probe-CDH/blob/main/Other_Files/Images/Bottom.png"/>
</p>

<p align = "center">
<img src="https://github.com/zenitheesc/Probe-CDH/blob/main/Other_Files/Images/Perspective.png"/>
</p>


---

<p align="center">
    <a href="http://zenith.eesc.usp.br">
    <img src="https://img.shields.io/badge/Check%20out-Zenith's Oficial Website-black?style=for-the-badge" />
    </a> 
    <a href="https://www.facebook.com/zenitheesc">
    <img src="https://img.shields.io/badge/Like%20us%20on-facebook-blue?style=for-the-badge"/>
    </a> 
    <a href="https://www.instagram.com/zenith_eesc/">
    <img src="https://img.shields.io/badge/Follow%20us%20on-Instagram-red?style=for-the-badge"/>
    </a>

</p>
<p align = "center">
<a href="zenith.eesc@gmail.com">zenith.eesc@gmail.com</a>
</p>
