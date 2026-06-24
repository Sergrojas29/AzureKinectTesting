# Edge Node Setup Guide: Tempo Studio PC (Ubuntu 24.04)

This guide documents the complete setup process for configuring an embedded PC running Ubuntu 24.04 as a dedicated Edge-AI node. It covers the configuration of the integrated touch screen, the Nvidia GTX 1050 Ti, and the embedded Azure Kinect Time-of-Flight (ToF) sensor.

---

## Chapter 1: Touch Screen Configuration
The integrated touch panel usually communicates via internal USB. On Ubuntu 24.04, it should be recognized automatically by the standard HID multi-touch drivers. If the touch inputs map to the wrong display or require calibration:

1. **Install Touch Utilities:**
   `sudo apt-get update`
   `sudo apt-get install xinput x11-xserver-utils`

2. **Identify the Display and Touch Device:**
   `xrandr` # Find your display name (e.g., HDMI-1 or DP-1)
   `xinput` # Find the ID or name of the touch interface

3. **Map the Touch Input to the Display:**
   `xinput map-to-output <Touch_Device_ID> <Display_Name>`
   *(Hint: If the screen is rotated vertically, use the Ubuntu Display Settings GUI to set the orientation before mapping the input).*

---

## Chapter 2: GPU Acceleration Setup (GTX 1050 Ti)
To ensure real-time computer vision processing, the system must utilize the dedicated Nvidia GPU rather than the CPU.

1. **Install Proprietary Drivers:**
   `sudo ubuntu-drivers install`

2. **Reboot the System:**
   `sudo reboot`

3. **Verify the GPU is Active:**
   `nvidia-smi`
   *(You should see a table displaying the driver version and memory usage).*

4. **Install the CUDA Compiler Toolkit:**
   `sudo apt-get install nvidia-cuda-toolkit`
   `nvcc --version`

---

## Chapter 3: Azure Kinect SDK Installation
Because Ubuntu 24.04 relies on modernized package management and official support halted after Ubuntu 20.04, strict version locking and manual dependency resolution are required.

### Phase A: Add Microsoft Repositories Safely
`sudo mkdir -p /etc/apt/keyrings`
`curl -sSL https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor | sudo tee /etc/apt/keyrings/microsoft-prod.gpg > /dev/null`
`echo "deb [arch=amd64 signed-by=/etc/apt/keyrings/microsoft-prod.gpg] https://packages.microsoft.com/ubuntu/18.04/prod bionic main" | sudo tee /etc/apt/sources.list.d/microsoft-prod.list`
`sudo apt-get update`

### Phase B: Fix Missing Audio Dependency
Modern Ubuntu drops libsoundio1, which the Kinect SDK requires to install.
`wget http://archive.ubuntu.com/ubuntu/pool/universe/libs/libsoundio/libsoundio1_1.1.0-1_amd64.deb`
`sudo dpkg -i libsoundio1_1.1.0-1_amd64.deb`

### Phase C: Install the Core Drivers & Tracking SDK
*CRITICAL: The libk4abt tracker requires exactly version 1.4.1 of the base SDK. You must force these versions to prevent an apt dependency standoff.*
`sudo apt-get install libk4a1.4=1.4.1 libk4a1.4-dev=1.4.1 libk4abt1.1 libk4abt1.1-dev k4a-tools=1.4.1`

### Phase D: Lock Packages (Prevent Accidental Updates)
`sudo apt-mark hold libk4a1.4 libk4a1.4-dev k4a-tools`

---

## Chapter 4: Hardware Permissions (UDEV Rules)
By default, Linux will block standard user accounts from accessing raw USB nodes. We must overwrite the USB rules to grant universal access (MODE="0666") to avoid requiring sudo for C++ compilation.

1. **Create the Rules File:**
   `sudo bash -c 'cat << EOF > /etc/udev/rules.d/99-k4a.rules`
   `# Azure Kinect UDEV Rules`
   `SUBSYSTEM=="usb", ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097a", MODE="0666"`
   `SUBSYSTEM=="usb", ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097b", MODE="0666"`
   `SUBSYSTEM=="usb", ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097c", MODE="0666"`
   `SUBSYSTEM=="usb", ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097d", MODE="0666"`
   `SUBSYSTEM=="usb", ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097e", MODE="0666"`
   `EOF'`

2. **Reload the Hardware Rules:**
   `sudo udevadm control --reload-rules`
   `sudo udevadm trigger`

> **💡 CRITICAL HARDWARE RESET HINT:**
> UDEV rules only apply at the exact moment of physical connection. **You must physically unplug the Azure Kinect USB cable from the PC, wait 3 seconds, and plug it back into a Blue SuperSpeed (SS) USB 3.0 port** before the permissions will take effect. 

---

## Chapter 5: Verifying the Setup
*Note: Only one application can access the camera streams at a time. Close one before opening the other.*

* **Test Raw Streams (Color & 3D Depth Map):**
  Run `k4aviewer` in the terminal. Click "Open Device" -> "Start".
* **Test AI Skeletal Tracking:**
  Run `k4abt_simple_3d_viewer` in the terminal. Step back 3-4 feet from the cabinet to allow the sensor to map the 32-joint wireframe onto your body.
