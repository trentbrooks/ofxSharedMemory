ofxSharedMemory
===============

Openframeworks example of a memory mapped file (https://en.wikipedia.org/wiki/Memory-mapped_file) for sharing data between multiple applications. Example shows sharing an RGB video feed between 2 seperate OF apps. May be a good alternative to Syphon (http://syphon.v002.info/) for Windows.

 Thanks to sloopi & KJ1 on the openframeworks forum for introducing me to 'memory mapped files' - http://forum.openframeworks.cc/index.php/topic,11730.msg55510.html#msg55510
 
 KJ1 created an excellent Windows/VS specific example for kinect (https://github.com/Kj1/ofxKinectMemoryMapped) which shares kinect data from a C# app with an OF app. I made this because I needed a pure C++ example that worked on osx and windows, and not necessarily tied to kinect.

 Tested on OSX and Windows 7 (OF0074).
 
 TODO::
 - need to allow reconnection for the client when server closes the shared memory without having to connect() in update/every frame.
 - example for sharing non image data- eg. floats, ints, strings, etc.
