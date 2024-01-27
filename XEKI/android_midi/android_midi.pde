import android.app.Activity;
import android.content.Context;
import android.os.Bundle ;
import android.media.midi.* ;
import android.content.pm.PackageManager ;
import android.os.Handler ;
import android.os.Looper ;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;

//phone accelerometer sensor setup
Activity act;
Context context;
SensorManager manager;
Sensor sensor;
AccelerometerListener listener;
float ax, ay, az, mapr, mapg, mapv;

//androif midi manager setup
MidiManager m ;
MidiDeviceInfo[] deviceInfo ;
MidiDeviceInfo.PortInfo[] portInfo ;
MidiInputPort inputPort;
MidiOutputPort outputPort;
Bundle properties ;

int p; //ports
String name_midi; //midi device name
int inputs_midi;
int outputs_midi;

//MIDI message variables
byte[] msg_midi;
int note;
int vel;

//message buffer
byte[] buffer = new byte[32];
int numBytes = 0;
int channel = 3; // MIDI channels 1-16 are encoded as 0-15.
int offset = 0;

void setup() {
  fullScreen();
  textFont(createFont("SansSerif", 30 * displayDensity));

  //set MIDI activity
  act = this.getActivity();///////////
  context = act.getApplicationContext();
  m = (MidiManager)context.getSystemService(Context.MIDI_SERVICE);
  checkMIDI();
  buffer[numBytes++] = (byte)(0xB0); // continuous functio
  buffer[numBytes++] = (byte)0x17; // //set in undefined function number 0x15
  buffer[numBytes++] = (byte)0; // max velocity

  //set accelerometer activity
  context = getActivity();
  manager = (SensorManager)context.getSystemService(Context.SENSOR_SERVICE);
  sensor = manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
  listener = new AccelerometerListener();
  manager.registerListener(listener, sensor, SensorManager.SENSOR_DELAY_GAME);
}

void draw() {
  background(0);

  text("P: " + p, 10, 10, width, height);
  text("Name: " + name_midi, 10, 100, width, height);

  text("INPUTS: " + inputs_midi, 10, 400, width, height);
  text("OUTPUTS: " + outputs_midi, 10, 500, width, height);

  text("Note: " + note, 10, 700, width, height);
  text("Vel: " + vel, 10, 800, width, height);
  text("RAW: " + msg_midi, 10, 900, width, height);

  //if (mousePressed) {///////////////
  //if (note == 9) {
  //  if (vel == 0) {

      try {
        float ap; //accelerometer value roundup using range (-45, +45)
        if (ay<-5) ap = 0.0;
        else if (ay>5) ap = 10.0;
        else ap = ay + 5.0;

        if ((int(ap*100)%8) == 0) {
          //map accelerometer data to MIDI
          int dm;
          dm = int(map(ap, 0, 10.0, 52, 0));
          text("dm"+dm, 10, 1000, width, height);
          buffer[2] = (byte)dm; // function value
          inputPort.send(buffer, offset, numBytes);//6
          //delay(10);
        }
      }
      catch(IOException ie) {
        ie.printStackTrace();
      }
  //  }
  //}
  ////}
}

void mousePressed() {
  checkMIDI();
}

void checkMIDI() {
  //check if Android supports MIDI
  if (context.getPackageManager().hasSystemFeature(PackageManager.FEATURE_MIDI)) {
    println("MIDI avaiable");
    //search for attached devices
    deviceInfo = m.getDevices();
    if (deviceInfo.length == 0) {
      println("No MIDI devices found  .. " + deviceInfo.length);
      p = deviceInfo.length;
    } else {
      println("Num MIDI  Devices found  = " + deviceInfo.length);
      p = deviceInfo.length;
      openMIDI(deviceInfo);
    }
  } else {
    println("MIDI not avaiable");
  }
}

void openMIDI(MidiDeviceInfo[] info) {
  for (int i=0; i<info.length; i++) {
    println("Device " + i);
    properties = info[i].getProperties();
    println("  Name = " + properties.getString(MidiDeviceInfo.PROPERTY_NAME));
    name_midi = properties.getString(MidiDeviceInfo.PROPERTY_NAME);

    println("  Num MIDI inputs = " + info[i].getInputPortCount());
    inputs_midi = info[i].getInputPortCount();
    println("  Num MIDI outputs = " + info[i].getOutputPortCount());
    outputs_midi = info[i].getOutputPortCount();


    //Port Information
    portInfo = info[i].getPorts();
    println("    Num ports found = " + portInfo.length);
    for (int j=0; j<portInfo.length; j++) {
      println("    port " + j + " Name = " + portInfo[j].getName() + "Type = " + portInfo[j].getType());
    }

    //Open MIDI Device
    m.openDevice(info[0], new MidiManager.OnDeviceOpenedListener() {//info.length-1
      @Override
        public void onDeviceOpened(MidiDevice device) {
        if (device == null) {
          println("could not open device");
        } else {
          println("device opened and ready for use");
          //outputPort = device.openOutputPort(0);
          //if (info.length==1) 
          inputPort = device.openInputPort(0);//connection with sn
          //if (info.length==2) {
          //  //inputPort = device.openInputPort(0);
          //  outputPort = device.openOutputPort(0);//connection for acc trig

          //  outputPort.connect(new MyReceiver());
          //}
        }
      }
    }
    , new Handler(Looper.getMainLooper())
      );
  }
}

class MyReceiver extends MidiReceiver {
  public void onSend(byte[] data, int offset,
    int count, long timestamp) throws IOException {
    // MIDI Data:
    //for (int i=1; i<4; i++) {
    //  print(data[i]);
    //}
    //println();
    msg_midi = data;
    note = (int)(data[2] & 0xFF);
    vel = (int)(data[3] & 0xFF);
  }
}

class AccelerometerListener implements SensorEventListener {
  public void onSensorChanged(SensorEvent event) {
    ax = event.values[0];
    ay = event.values[1];
    az = event.values[2];
  }
  public void onAccuracyChanged(Sensor sensor, int accuracy) {
  }
}
