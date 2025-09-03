#include "HallSensor.h"

void HallSensor::begin(const HallSensorConfig &config) {
	_cfg = config;
	_pulseCount = 0;
	_lastPulseMicros = 0;
	_windowStartMicros = micros();
	pinMode(_cfg.hallGpioPin, INPUT_PULLUP);
	attachInterruptArg((uint8_t)_cfg.hallGpioPin, &HallSensor::isrHandler, this, FALLING);
}

void IRAM_ATTR HallSensor::isrHandler(void* arg) {
	reinterpret_cast<HallSensor*>(arg)->onPulse();
}

void HallSensor::onPulse() {
	const uint32_t now = micros();
	const uint32_t sinceLast = now - _lastPulseMicros;
	if (sinceLast >= _cfg.debounceMicros) {
		_pulseCount++;
		_lastPulseMicros = now;
	}
}

HallSensorSnapshot HallSensor::takeSnapshotAndReset() {
	noInterrupts();
	const uint32_t now = micros();
	HallSensorSnapshot s;
	s.pulseCount = _pulseCount;
	s.windowMicros = now - _windowStartMicros;
	_pulseCount = 0;
	_windowStartMicros = now;
	interrupts();
	return s;
}


