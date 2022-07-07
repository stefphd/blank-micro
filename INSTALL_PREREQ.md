# Installation of the pre-requisites

## Windows

### Arduino builder

You can either install the *arduino-builder* only or the *Arduino IDE*, which is shipped with the *arduino-builder* included. To install the *Arduino IDE*:

1. Download *Arduino IDE* from [here](https://www.arduino.cc/en/software).
2. Run the installer `arduino-X.X.X-windows.exe` (where `X.X.X` is the software version)
3. Follow the installation steps. Default installation folder should be `C:/Program Files (x86)/Arduino`
4. You can check if the installation finishes correctly by running in the terminal
   
   ```bash
    "C:/Program Files (x86)/Arduino/arduino-builder" -version
   ```

   where `C:/Program Files (x86)/Arduino` is your installation path.

\attention Default installation path should be `C:/Program Files (x86)/Arduino`. If you choose a different one, you need to change the `ARDUINO_FOLDER` variable in the `Makefile` according to your installation path.

### Visual Studio Code

To install *Visual Studio Code*:

1. Download the installer from [here](https://code.visualstudio.com/)
2. Run the installer `VSCodeUserSetup-x64-X.X.X.exe` (where `X.X.X` is the software version)
3. Follow the installation steps

You may also install the `C/C++` extension for *Visual Studio Code* for code completation.

### Make and Doxygen

For installation of *Make* and *Doxygen* you may employ *Chocolatey*, which is a package manager for Windows. First download and install *Chocolatey* by following the this [installation guide](https://chocolatey.org/install#individual). Basically, you need to run in the powershell (with administrative privileges) the following command

```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

To install *Make* and *Doxygen* just run in the command promp or powershell (with administrative privileges)

```powershell
choco install make doxygen.install
```

and follow the installation steps (basically, type `Y` when required).
Finally, add the installation directory (default is `C:\Program Files\doxygen\bin`) to the environment variable `PATH`.
To check the installation, just use in the command prompt (you need to open a new terminal)

```bash
doxygen -v
```

```bash
make -v
```

\note Alternatively, for *Doxigen* you may also download the installer from [here](https://www.doxygen.nl/download.html) and install the software. After, you need to add the installation directory to the environment variable `PATH`.

\note For *make* other installation process may exist.

### Teensyduino

Download and install *Teensyduino* from [here](https://www.pjrc.com/teensy/td_download.html), which is an Add-On for *Arduino IDE*.

\attention Installation of *Arduino IDE* is required.

### KiCAD

Download and install *KiCAD* from [here](https://www.kicad.org/download/windows/).

## Linux

Depending on your Linux distrubution, you can easily install the pre-requisites using your package manager: **you are using Linux, so you are supposed to know this stuff!** For example, in ArchLinux to install *arduino-builder*, *Make* and *Doxygen* use

```bash
pacman -Sy arduino-builder make doxygen
```

## MacOS

Not implemented yet.

## Embedded Coder Toolbox (MATLAB)

For installing the *Embedded Coder Toolbox* in MATLAB
0. Install MATLAB (if not installed)
1. Go to `Add-Ons > Get Add Ons` and look for the *Embedded Coder*
2. Click on *Embedded Coder* and install

Other (possibly necessary) MATLAB toolboxes may be installed in a simular way.