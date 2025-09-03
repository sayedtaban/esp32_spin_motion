## ESP32 Bicycle Speed Sensor → Unreal Engine Integration

This project provides firmware for an ESP32 (Arduino framework via PlatformIO) to measure bicycle wheel speed using a Hall-effect sensor (or reed switch) and stream speed to a PC over USB Serial. It also includes Unreal Engine C++ code to read the stream and expose the current speed to Blueprints.

### Hardware
- **ESP32**: `esp32dev` board profile
- **Sensor**: Hall-effect sensor or reed switch
- **Magnet(s)**: Mounted on wheel spoke(s)

### Wiring
```
Bike Magnet(s) → Hall Sensor → ESP32

Hall/Vout  → GPIO 4 (default)
VCC        → 3V3
GND        → GND

ESP32 pin is configured with INPUT_PULLUP. For open-drain sensors, ensure they pull to GND when triggered.
```

### Build (Firmware)
1. Install PlatformIO (VS Code extension or CLI).
2. Connect ESP32 via USB.
3. From this folder, build and upload:
```
pio run -t upload
pio device monitor
```

Default config (overridable in `platformio.ini`):
- `CONFIG_DEFAULT_WHEEL_CIRCUM_MM=2105` (700x35c typical) → circumference in mm
- `CONFIG_DEFAULT_MAGNETS_PER_REV=1`
- `CONFIG_DEFAULT_HALL_PIN=4`

### Telemetry Format (USB Serial)
CSV line at ~30 Hz:
```
mps,kmh,rpm,pulses,dt_us,t_ms
0.000,0.000,0.00,0,33000,123456
```

Fields:
- **mps**: meters per second (smoothed EMA)
- **kmh**: kilometers per hour (smoothed)
- **rpm**: wheel revolutions per minute
- **pulses**: pulses detected in the last integration window
- **dt_us**: microseconds of the last window
- **t_ms**: device uptime (ms)

### Unreal Engine Integration (Windows)
Included under `unreal/BikeSpeed/` is a minimal plugin:
- `UBikeSpeedComponent` (Blueprint component) exposes `CurrentSpeedKmh` and `CurrentSpeedMps`.
- Windows-only serial reader uses Win32 API to open a COM port (e.g. `COM5`) and parse the CSV.

Setup:
1. Copy `unreal/BikeSpeed` into your UE project's `Plugins/` folder.
2. Enable the plugin in your project.
3. In Project Settings → Plugins → BikeSpeed, set the COM port (or in code).
4. Add `BikeSpeedComponent` to your pawn/actor and bind tick to update speed.

Serial Port Tips on Windows:
- Find COM port via Device Manager under "Ports (COM & LPT)".
- Use the same baud as firmware (`115200`).

### BLE Option (Future)
The firmware contains a `TransportBLE` placeholder. To minimize initial complexity and latency variance, USB Serial is the default. BLE can be added later if needed.

### Repository Layout
```
include/
  Config.h
  HallSensor.h
  SpeedCalculator.h
  TransportSerial.h
  TransportBLE.h
src/
  main.cpp
  HallSensor.cpp
  SpeedCalculator.cpp
unreal/
  BikeSpeed/ (UE plugin: module, component, serial reader)
platformio.ini
README.md
```

### Safety Notes
- Test on a stand before riding.
- Securely mount the magnet and sensor.


