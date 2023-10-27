#include <Arduino.h>
#include <SAMD21turboPWM.h>

// const int ledPin = 13;
const int brake_enable_pin = 5;
const int bus_voltage_pin = A6;
const float overvoltage_ramp_start = 28.0f; // NB: must be above supply voltage
const float overvoltage_ramp_end = 36.0f;
TurboPWM pwm;

float brake_duty = 0.0f;
float bus_voltage = 0.0f;
int output_duty = 1000;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(brake_enable_pin, OUTPUT);
  // pinMode(10, OUTPUT);
  pinMode(bus_voltage_pin, INPUT);

  pwm.setClockDivider(20 , false); // set clock to 188kHz;
  pwm.timer(0, 256, 8, false);
  pwm.enable(0, true);
  // pwm.analogWrite(brake_enable_pin, output_duty);
  analogReadResolution(12);
  Serial.begin(115200);
  // digitalWrite(LED_BUILTIN, HIGH); // ready
  analogWrite(10, 128);
  
}

void loop() {
  bus_voltage = map((float)analogRead(bus_voltage_pin), 0, 4095, 0.0f, float(3.3*21)) ;
  // Serial.println(analogRead(bus_voltage_pin));
  
  Serial.println(pwm.frequency(0));
  brake_duty = min(max((bus_voltage - overvoltage_ramp_start)/(overvoltage_ramp_end - overvoltage_ramp_start), 0.0f), 1.0f);
  output_duty = (int)(1000*(1-brake_duty));
  // pwm.analogWrite(brake_enable_pin, 1000);
  // Serial.println(output_duty);
  // delay(100);
  pwm.analogWrite(brake_enable_pin, output_duty); // MOSFET on when low
  if ((brake_duty)>0){
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
