#pragma once

#include <Arduino.h>

struct HallSensorConfig {
	int hallGpioPin = 4;
	uint32_t debounceMicros = 1000;
};

struct HallSensorSnapshot {
	uint32_t pulseCount = 0;
	uint32_t windowMicros = 0;
};

class HallSensor {
public:
	void begin(const HallSensorConfig &config);
	HallSensorSnapshot takeSnapshotAndReset();

private:
	static void IRAM_ATTR isrHandler(void* arg);
	void onPulse();

	HallSensorConfig _cfg;
	volatile uint32_t _pulseCount = 0;
	volatile uint32_t _lastPulseMicros = 0;
	volatile uint32_t _windowStartMicros = 0;
};


