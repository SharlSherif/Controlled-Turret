
#include <Servo.h>
#include <IRremote.h>

const int RECV_PIN = 12; // define input pin on Arduino 
const int SERVO_PIN = 9;
const int BUZZER_PIN = 6;
const int PING_PIN = 4; // Trigger Pin of Ultrasonic Sensor
const int ECHO_PIN = 7; // Echo Pin of Ultrasonic Sensor

IRrecv irrecv(RECV_PIN); 
decode_results results; // decode_results class is defined in IRremote.h

Servo myservo;  // create servo object to control a servo

void setup() {
  Serial.begin(9600);
  pinMode(RECV_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PING_PIN, OUTPUT);
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  irrecv.enableIRIn(); // Start the receiver 
}


int pos = 0;    // variable to store the servo position
String previousCode = "";

void checkAndCorrectPos (){
  if(pos >= 180 | pos <0){
    pos = 0;
  }
}


void ControlTurret() {
   if (irrecv.decode(&results)) {
    String value = translateIR();
    Serial.println(value+" "+pos); 

    if (value == "REPEAT") {
      value = previousCode;
    }else {
      previousCode=value;
    }
    irrecv.resume(); // Receive the next value 
    if(value == "LEFT"){
      pos=100;
    }
    if(value == "RIGHT"){
      pos=0;
    }
    if(value == "-OK-"){
      pos=40;
    }
    myservo.write(pos);
    delay(15);
    CalculateDistance();
  }
}

void CalculateDistance() {
   long duration, inches, cm;
   digitalWrite(PING_PIN, LOW);
   delayMicroseconds(2);
   digitalWrite(PING_PIN, HIGH);
   delayMicroseconds(10);
   digitalWrite(PING_PIN, LOW);
   pinMode(ECHO_PIN, INPUT);
   duration = pulseIn(ECHO_PIN, HIGH);
   cm = microsecondsToCentimeters(duration);
   Serial.print(cm);
   Serial.print(" cm");
   Serial.println();
}

void loop() {
  ControlTurret(); 
}



long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
String translateIR()
{
  Serial.println(results.value);
  switch (results.value)
  {
  case 0xFF18E7:
    return("FORWARD");
    break;
  case 0xFF10EF:
    return("LEFT");
    break;
  case 0xFF38C7:
    return("-OK-");
    break;
  case 0xFF5AA5:
    return("RIGHT");
    break;
  case 0xFF4AB5:
    return ("REVERSE");
    break;
  case 0xFFA25D:
    return ("1");
    break;
  case 0xFF629D:
    return ("2");
    break;
  case 0xFFE21D:
    return ("3");
    break;
  case 0xFF22DD:
    return ("4");
    break;
  case 0xFF02FD:
    return ("5");
    break;
  case 0xFFC23D:
    return ("6");
    break;
  case 0xFFE01F:
    return ("7");
    break;
  case 0xFFA857:
    return ("8");
    break;
  case 0xFF906F:
    return ("9");
    break;
  case 0xFF6897:
    return ("*");
    break;
  case 0xFF9867:
    return ("0");
    break;
  case 0xFFB04F:
    return ("#");
    break;
  case 0xFFFFFFFF:
    return ("REPEAT");
    break;

  default:
    return ("Unidentified button");
  }
}
