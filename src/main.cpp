#include <Arduino.h>
#include "Config.h"
#include "HallSensor.h"
#include "SpeedCalculator.h"
#include "TransportSerial.h"

static HallSensor g_hallSensor;
static SpeedCalculator g_speedCalc;
static TransportSerial g_serial;

static uint32_t g_lastPublishMs = 0;
static const uint32_t PUBLISH_INTERVAL_MS = 33; // ~30 Hz

void setup() {
	Serial.begin(115200);
	while (!Serial) {
		; // wait for native USB if applicable
	}

	g_serial.begin(115200);

	HallSensorConfig hallCfg;
	hallCfg.hallGpioPin = CONFIG_HALL_PIN;
	hallCfg.debounceMicros = 1000; // 1 ms debounce for Hall/reed
	g_hallSensor.begin(hallCfg);

	SpeedCalculatorConfig spdCfg;
	spdCfg.wheelCircumferenceMeters = (float)CONFIG_WHEEL_CIRCUMF_MM / 1000.0f;
	spdCfg.magnetsPerRevolution = CONFIG_MAGNETS_PER_REV;
	spdCfg.emaAlpha = 0.2f; // smoothing factor
	g_speedCalc.begin(spdCfg);
}

void loop() {
	const uint32_t nowMs = millis();
	HallSensorSnapshot snap = g_hallSensor.takeSnapshotAndReset();
	if (snap.pulseCount > 0) {
		g_speedCalc.onPulses(snap.pulseCount, snap.windowMicros);
	}

	if (nowMs - g_lastPublishMs >= PUBLISH_INTERVAL_MS) {
		g_lastPublishMs = nowMs;
		SpeedState s = g_speedCalc.getState();
		// CSV: mps,kmh,rpm,pulses,dt_us,t_ms
		g_serial.publishTelemetry(s.speedMetersPerSecond, s.speedKilometersPerHour, s.rpm,
				s.lastPulseCount, s.lastWindowMicros, nowMs);
	}

	delay(1);
}


