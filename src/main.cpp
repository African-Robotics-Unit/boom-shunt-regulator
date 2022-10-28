#include <Arduino.h>

const int ledPin = 13;
const int brake_enable_pin = 9;
const int bus_voltage_pin = A6;
const float overvoltage_ramp_start = 28.0f; // NB: must be above supply voltage
const float overvoltage_ramp_end = 36.0f;
const float pwm_frequency = 20000;

float brake_duty = 0.0f;
float bus_voltage = 0.0f;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(brake_enable_pin, OUTPUT);
  pinMode(bus_voltage_pin, INPUT);
  analogReadResolution(12);
  analogWriteResolution(12); // max at 20kHz
  analogWriteFrequency(brake_enable_pin, pwm_frequency);
  digitalWrite(ledPin, HIGH); // ready
}

void loop() {
  bus_voltage = map((float)analogRead(bus_voltage_pin), 0, 4095, 0.0f, 69.3f);
  brake_duty = min(max((bus_voltage - overvoltage_ramp_start)/(overvoltage_ramp_end - overvoltage_ramp_start), 0.0f), 1.0f);
  analogWrite(brake_enable_pin, (int)(4095*(1-brake_duty))); // MOSFET on when low
}
