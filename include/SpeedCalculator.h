#pragma once

#include <Arduino.h>

struct SpeedCalculatorConfig {
	float wheelCircumferenceMeters = 2.105f;
	uint8_t magnetsPerRevolution = 1;
	float emaAlpha = 0.2f;
};

struct SpeedState {
	float speedMetersPerSecond = 0.0f;
	float speedKilometersPerHour = 0.0f;
	float rpm = 0.0f;
	uint32_t lastPulseCount = 0;
	uint32_t lastWindowMicros = 0;
};

class SpeedCalculator {
public:
	void begin(const SpeedCalculatorConfig &config);
	void onPulses(uint32_t pulseCount, uint32_t windowMicros);
	SpeedState getState() const { return _state; }

private:
	SpeedCalculatorConfig _cfg;
	SpeedState _state;
	float _emaMps = 0.0f;
};


