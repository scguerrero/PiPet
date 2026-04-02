# PiPet: Handheld Digital Pet Game
Senior capstone project by Luke C., Sasha G., Camden G., Tanya M., and Cesar R.

## Raspberry Pi Zero 2 W Pre-Compiled Binary
PiPet is designed for 32-bit Raspberry Pi Bookworm OS. 
Send `pipet` located in `PreCompiledBinary/` directory to the Pi.
```
# Use hostname -I to get the Pi's IP address.
hostname -I
# Use whoami to get the Pi's name.
whoami
# Assuming you have an SSH connection with the Pi, use scp to send the file.
# We send it to the Pi's desktop for ease of access. Not mandatory.
scp pipet <raspberry-pi-name>@<ip-address>:/home/<raspberry-pi-name>/Desktop
```

## How to Build
**OS Requirement**: PiPet is exclusively for Linux.

**Without QT Creator** (qmake and QT are assumed to be installed):
```
git clone https://github.com/scguerrero/PiPet.git
cd PiPet/GUI
qmake
make
./pipet
```
**With QT Creator**:
1. Clone repository: `git clone https://github.com/scguerrero/PiPet.git`. This should download the PiPet directory and all subdirectories.
2. "Welcome" -> "Open Project" -> Navigate to directory PiPet and select file `pipet.pro`.
3. Build and run.

