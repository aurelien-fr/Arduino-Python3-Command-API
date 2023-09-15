#include <Wire.h>

void Version(){
  Serial.println(F("V0.6"));
}

int servo_pins[] = {0, 0, 0, 0, 0, 0, 0, 0};
boolean connected = false;

int Str2int (String Str_value)
{
  char buffer[10]; //max length is three units
  Str_value.toCharArray(buffer, 10);
  int int_value = atoi(buffer);
  return int_value;
}

void split(String results[], int len, String input, char spChar) {
  String temp = input;
  for (int i=0; i<len; i++) {
    int idx = temp.indexOf(spChar);
    results[i] = temp.substring(0,idx);
    temp = temp.substring(idx+1);
  }
}

void DigitalHandler(int mode, String data){
      int pin = Str2int(data);
    if(mode<=0){ //read
        Serial.println(digitalRead(pin));
    }else{
        if(pin <0){
            digitalWrite(-pin,LOW);
        }else{
            digitalWrite(pin,HIGH);
        }
        //Serial.println('0');
    }
}

void AnalogHandler(int mode, String data){
     if(mode<=0){ //read
        int pin = Str2int(data);
        Serial.println(analogRead(pin));
    }else{
        String sdata[2];
        split(sdata,2,data,'%');
        int pin = Str2int(sdata[0]);
        int pv = Str2int(sdata[1]);
        analogWrite(pin,pv);
    }
}

void ConfigurePinHandler(String data){
    int pin = Str2int(data);
    if(pin <=0){
        pinMode(-pin,INPUT);
    }else{
        pinMode(pin,OUTPUT);
    }
}

void pulseInHandler(String data){
    int pin = Str2int(data);
    long duration;
    if(pin <=0){
          pinMode(-pin, INPUT);
          duration = pulseIn(-pin, LOW);
    }else{
          pinMode(pin, INPUT);
          duration = pulseIn(pin, HIGH);
    }
    Serial.println(duration);
}

void pulseInSHandler(String data){
    int pin = Str2int(data);
    long duration;
    if(pin <=0){
          pinMode(-pin, OUTPUT);
          digitalWrite(-pin, HIGH);
          delayMicroseconds(2);
          digitalWrite(-pin, LOW);
          delayMicroseconds(5);
          digitalWrite(-pin, HIGH);
          pinMode(-pin, INPUT);
          duration = pulseIn(-pin, LOW);
    }else{
          pinMode(pin, OUTPUT);
          digitalWrite(pin, LOW);
          delayMicroseconds(2);
          digitalWrite(pin, HIGH);
          delayMicroseconds(5);
          digitalWrite(pin, LOW);
          pinMode(pin, INPUT);
          duration = pulseIn(pin, HIGH);
    }
    Serial.println(duration);
}

void SerialParser(void) {
  char readChar[64];
  Serial.readBytesUntil(33,readChar,64);
  String read_ = String(readChar);
  //Serial.println(readChar);
  int idx1 = read_.indexOf('%');
  int idx2 = read_.indexOf('$');
  // separate command from associated data
  String cmd = read_.substring(1,idx1);
  String data = read_.substring(idx1+1,idx2);

  // determine command sent
  if (cmd == "dw") {
      DigitalHandler(1, data);
  }
  else if (cmd == "dr") {
      DigitalHandler(0, data);
  }
  else if (cmd == "aw") {
      AnalogHandler(1, data);
  }
  else if (cmd == "ar") {
      AnalogHandler(0, data);
  }
  else if (cmd == "pm") {
      ConfigurePinHandler(data);
  }
  else if (cmd == "ps") {
      pulseInSHandler(data);
  }
  else if (cmd == "pi") {
      pulseInHandler(data);
  }
  else if (cmd == "version") {
      Version();
  }
}

void setup()  {
  Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
  Serial.println("connected");
}

void loop() {
  SerialParser();
}
