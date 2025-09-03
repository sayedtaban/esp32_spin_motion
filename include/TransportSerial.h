#pragma once

#include <Arduino.h>

class TransportSerial {
public:
	void begin(uint32_t baud) { Serial.begin(baud); }
	void publishTelemetry(float mps, float kmh, float rpm, uint32_t pulses, uint32_t dtMicros, uint32_t tMs) {
		Serial.printf("%.3f,%.3f,%.2f,%u,%u,%u\n", mps, kmh, rpm, pulses, dtMicros, tMs);
	}
};


