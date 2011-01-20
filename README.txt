How to build VstBoard :

Steinberg SDKs :
================
Steinberg VST and ASIO SDKs can't be redistributed : you have to create an account and download them yourself.
We need "VST 2.4 Audio Plug-Ins SDK" and "ASIO SDK" available at http://www.steinberg.net/en/company/developer.html
Copy the VST SDK in ./libs/vstsdk2.4 (see README.txt in that folder)
Copy the Asio SDK in ./libs/portaudio/src/hostapi/asio/ASIOSDK (see README.txt in that folder)

Qt Creator :
============
Download and install "Qt SDK for Windows (based on MinGW)" (the Complete Development Environment) from http://qt.nokia.com/downloads

In Qt Creator, create a new session and open :
 ./src/vstboard/VstBoard.pro
 ./src/portaudio/portaudio.pro
 ./src/pormidi/portmidi.pro

- Compile portaudio and portmidi
- You can now compile the VstBoard project

x64 and msvc 2010 :
====================
hotfix needed : http://support.microsoft.com/kb/2280741
