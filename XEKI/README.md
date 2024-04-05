# X.E.K.I.

<p align="center"> 
  <img src="https://github.com/OrpheasK/arduino/blob/main/XEKI/img/DSC01478.jpeg" width="52%"/>
</p>

X.E.K.I. (standing for Expressive Electronic Keyboard Instrument) is a MIDI instrument programmed on an Arduino Nano 33 IoT. 
Built around the concept of smartphone reusing, its [first iteration](https://github.com/OrpheasK/arduino/blob/main/XEKI/sketch_may1a.ino) made use of the Google Pixel 3, integrating it in its design and using the built-in accelerometer as complementary MIDI input through an [Android script](https://github.com/OrpheasK/arduino/blob/main/XEKI/android_midi/android_midi.pde). The phone also acted as the receiving end of the MIDI messages, having a 
music app run at the same time through split-screen mode, making use of its sound library. This setup allowed the instrument to have all the software in-unit and relied on connecting the phone to a speaker via Bluetooth for amplification.

<p align="center"> 
  <img src="https://github.com/OrpheasK/arduino/blob/main/XEKI/img/protmd.png" width="74%"/>
</p>

Since the Arduino functions as the MIDI device, it is possible to connect to any DAW directly, either through USB, OSC or BLE if available on
the microcontroller model. The smartphone setup allows for any microcontroller with 15 digital inputs to be used, if however the model also
has an IMU, such as the Nano 33 IoT, it can run all the necessary input functionalities [on its own](https://github.com/OrpheasK/arduino/blob/main/XEKI/sketch_feb14a.ino), making the process similar to that of a standard MIDI instrument.

[Demo video](https://www.youtube.com/embed/NObGi7amb2s?si=Cx3Abpt3CwYqlHW7)
[Documentation](https://www.notion.so/Final-Project-a1218a285a6d4cfa89196ad5ae3c9d70)
