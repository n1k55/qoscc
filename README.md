# qoscc
A Oscilloscope build in QT &amp; C++ for Jack audio server

forked from:

https://sourceforge.net/projects/heaven/files/Audio%20Applications/Jack%20Related/QOscC/

WIP:
- need to convert QT3 related code to QT5 or higher
- refactor old code with circular dependencies

TODO:
- get rid of handcrafted stringlist;
- unify locks usage;


History
==========
2022-03-15 Add libraries without Qt dependencies and MainWindow


Info from original README and TODO:
===========
QOscC V. 0.1.6

Description
-----------
QOscC is a highly flexible and configurable software  
Oscilloscope with a large number of features.  
This includes support for any number of audio  
devices (ALSA or OSS), each with any number of channels.
Each scope display can be configured individually to different  
display types and variants. e.g. you can chose from  
standard y-t mode (as on an usual oscilloscope), xy  
mode (e.g. for measuring the phase shift between  
two signals) of the FFT mode (to view a spectrum  
plot of the signal).
This software is intended for electronic hobyists, who  
cannot afford a hardware oscilloscope or need a  
simple spectrum analyzer as well as for musicans for 
doing basic signal analysis.
QOscC is still in development and therefor somehow buggy.
The 0.1.6 Version has some additional problems over Version 0.1.4,
so you might want to try both versions if you experience problems.


New in 0.1.6
------------
All parameters are fully run-time configurable
ALSA Support is implemented
Build process is now controlled by automake


Still to come in later releases
-------------------------------
Logarithmic frequency scale in FFT mode
Cursors for different measuring issues
Data and Image export
Input level adjustment
Backends for serial Multimeters and digital oscilloscope cards
Backend for audio file input
Fix spurious freezes
Fix buffer issue with the ALSA backend<

Features
--------
GUI in QT
The number of Soundcards and Soundcard channels is not limited
Supports spectrum display (FFT)
OSS and ALSA drivers work now.

TODO
----
- image exports
- backend for digital oscilloscope cards....
- backend for WAVs
