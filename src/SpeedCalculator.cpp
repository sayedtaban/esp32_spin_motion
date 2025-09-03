#include "SpeedCalculator.h"

void SpeedCalculator::begin(const SpeedCalculatorConfig &config) {
	_cfg = config;
	_state = {};
	_emaMps = 0.0f;
}

void SpeedCalculator::onPulses(uint32_t pulseCount, uint32_t windowMicros) {
	_state.lastPulseCount = pulseCount;
	_state.lastWindowMicros = windowMicros;
	if (pulseCount == 0 || windowMicros == 0) {
		// decay toward zero for smooth stop
		_emaMps *= 0.9f;
	} else {
		const float seconds = (float)windowMicros / 1e6f;
		const float revs = (float)pulseCount / max<uint8_t>(_cfg.magnetsPerRevolution, 1);
		const float meters = revs * _cfg.wheelCircumferenceMeters;
		const float mps = meters / seconds;
		_emaMps = _cfg.emaAlpha * mps + (1.0f - _cfg.emaAlpha) * _emaMps;
		_state.rpm = (revs / seconds) * 60.0f;
	}
	_state.speedMetersPerSecond = _emaMps;
	_state.speedKilometersPerHour = _emaMps * 3.6f;
}


