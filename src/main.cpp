#include <Arduino.h>
#include <SAMD21turboPWM.h>

// const int ledPin = 13;
const int brake_enable_pin = 5;
const int bus_voltage_pin = A5; //TODO: Change this pin
const float overvoltage_ramp_start = 28.0f; // NB: must be above supply voltage
const float overvoltage_ramp_end = 36.0f;
TurboPWM pwm;

float brake_duty = 0.0f;
float bus_voltage = 0.0f;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(brake_enable_pin, OUTPUT);
  pinMode(bus_voltage_pin, INPUT);
  pwm.setClockDivider(255 , false); // set clock to 188kHz;
  pwm.timer(0, 8, 2, true);
  pwm.analogWrite(brake_enable_pin, 0);
  analogReadResolution(12);
  Serial.begin(115200);
  // digitalWrite(LED_BUILTIN, HIGH); // ready
}

void loop() {
  bus_voltage = map((float)analogRead(bus_voltage_pin), 0, 4095, 0.0f, float(3.3*21)) ;
  Serial.println(analogRead(bus_voltage_pin));
  delay(100);
  brake_duty = min(max((bus_voltage - overvoltage_ramp_start)/(overvoltage_ramp_end - overvoltage_ramp_start), 0.0f), 1.0f);
  pwm.analogWrite(brake_enable_pin, (int)(1000*(1-brake_duty))); // MOSFET on when low
  if ((1-brake_duty)>0){
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
