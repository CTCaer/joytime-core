#include "joytime-core.h"
#include "joytime-core.hpp"
#include <string.h> /* memcpy */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JOYTIME_CORE_EXPORT Joytime_Rumble* Joytime_Rumble_newFromFreqAndAmpSame(double frequency, double amplitude) {
  Joytime::Rumble* rumble = new Joytime::Rumble(frequency, amplitude);
  return (Joytime_Rumble*)rumble;
};

JOYTIME_CORE_EXPORT Joytime_Rumble* Joytime_Rumble_newFromFreqAndAmpDiff(double highFrequency, double highAmplitude, double lowFrequency, double lowAmplitude) {
  Joytime::Rumble* rumble = new Joytime::Rumble(highFrequency, highAmplitude, lowFrequency, lowAmplitude);
  return (Joytime_Rumble*)rumble;
};

JOYTIME_CORE_EXPORT Joytime_Rumble* Joytime_Rumble_newFromPreencoded(uint16_t highFrequency, uint8_t highAmplitude, uint8_t lowFrequency, uint16_t lowAmplitude) {
  Joytime::Rumble* rumble = new Joytime::Rumble(highFrequency, highAmplitude, lowFrequency, lowAmplitude);
  return (Joytime_Rumble*)rumble;
};

JOYTIME_CORE_EXPORT uint8_t* Joytime_Rumble_toBuffer(Joytime_Rumble* _rumble) {
  Joytime::Rumble* rumble = (Joytime::Rumble*)_rumble;
  return rumble->toBuffer();
};

JOYTIME_CORE_EXPORT uint16_t* Joytime_Rumble_getHighFrequency(Joytime_Rumble* _rumble) {
  Joytime::Rumble* rumble = (Joytime::Rumble*)_rumble;
  return &(rumble->highFrequency);
};
JOYTIME_CORE_EXPORT uint8_t* Joytime_Rumble_getHighAmplitude(Joytime_Rumble* _rumble) {
  Joytime::Rumble* rumble = (Joytime::Rumble*)_rumble;
  return &(rumble->highAmplitude);
};
JOYTIME_CORE_EXPORT uint8_t* Joytime_Rumble_getLowFrequency(Joytime_Rumble* _rumble) {
  Joytime::Rumble* rumble = (Joytime::Rumble*)_rumble;
  return &(rumble->lowFrequency);
};
JOYTIME_CORE_EXPORT uint16_t* Joytime_Rumble_getLowAmplitude(Joytime_Rumble* _rumble) {
  Joytime::Rumble* rumble = (Joytime::Rumble*)_rumble;
  return &(rumble->lowAmplitude);
};

JOYTIME_CORE_EXPORT uint16_t Joytime_Rumble_frequencyToHF(double frequency) {
  return Joytime::Rumble::frequencyToHF(frequency);
};

JOYTIME_CORE_EXPORT uint8_t Joytime_Rumble_frequencyToLF(double frequency) {
  return Joytime::Rumble::frequencyToLF(frequency);
};

JOYTIME_CORE_EXPORT uint8_t Joytime_Rumble_amplitudeToHA(double amplitude) {
  return Joytime::Rumble::amplitudeToHA(amplitude);
};

JOYTIME_CORE_EXPORT uint16_t Joytime_Rumble_amplitudeToLA(double amplitude) {
  return Joytime::Rumble::amplitudeToLA(amplitude);
};

JOYTIME_CORE_EXPORT Joytime_Controller* Joytime_Controller_new(Joytime_ControllerType type, void* handle, Joytime_TransmitBufferFunction* transmitBuffer, Joytime_ReceiveBufferFunction* receiveBuffer) {
  Joytime::Controller* controller = new Joytime::Controller((Joytime::ControllerType)type, handle, transmitBuffer, receiveBuffer);
  return (Joytime_Controller*)controller;
};

JOYTIME_CORE_EXPORT void Joytime_Controller_free(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  delete controller;
};

JOYTIME_CORE_EXPORT void Joytime_Controller_initialize(Joytime_Controller* _controller, bool calibrate) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->initialize(calibrate);
};

JOYTIME_CORE_EXPORT void Joytime_Controller_setVibrate(Joytime_Controller* _controller, bool vibrate) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->setVibration(vibrate);
};

JOYTIME_CORE_EXPORT void Joytime_Controller_setSixAxisEnabled(Joytime_Controller* _controller, bool enabled) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->setSixAxisEnabled(enabled);
};

JOYTIME_CORE_EXPORT void Joytime_Controller_setInputReportMode(Joytime_Controller* _controller, Joytime_ControllerInputReportMode mode) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->setInputReportMode((Joytime::ControllerInputReportMode)mode);
};

JOYTIME_CORE_EXPORT void Joytime_Controller_rumbleSame(Joytime_Controller* _controller, uint8_t timing, Joytime_Rumble* rumble) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->rumble(timing, (Joytime::Rumble*)rumble);
};

JOYTIME_CORE_EXPORT void Joytime_Controller_rumbleEach(Joytime_Controller* _controller, uint8_t timing, Joytime_Rumble* rumble1, Joytime_Rumble* rumble2) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->rumble(timing, (Joytime::Rumble*)rumble1, (Joytime::Rumble*)rumble2);
};

JOYTIME_CORE_EXPORT void Joytime_Controller_setLEDs(Joytime_Controller* _controller, Joytime_ControllerLEDState led1, Joytime_ControllerLEDState led2, Joytime_ControllerLEDState led3, Joytime_ControllerLEDState led4) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->setLEDs((Joytime::ControllerLEDState)led1, (Joytime::ControllerLEDState)led2, (Joytime::ControllerLEDState)led3, (Joytime::ControllerLEDState)led4);
};

JOYTIME_CORE_EXPORT void Joytime_Controller_setPowerState(Joytime_Controller* _controller, Joytime_ControllerPowerState state) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->setPowerState((Joytime::ControllerPowerState)state);
};

JOYTIME_CORE_EXPORT int Joytime_Controller_readSPIFlash(Joytime_Controller* _controller, int32_t address, uint8_t length, uint8_t* buf) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  std::vector<uint8_t> tmp = controller->readSPIFlash(address, length);

  int size = 0;
  if (tmp.size() < length) {
    size = tmp.size();
  } else {
    size = length;
  }

  memcpy(buf, tmp.data(), size);

  return size;
};

JOYTIME_CORE_EXPORT void Joytime_Controller_update(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->update();
};

JOYTIME_CORE_EXPORT Joytime_UpdateListenerID Joytime_Controller_registerUpdateListener(Joytime_Controller* _controller, Joytime_UpdateListener* listener) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  return controller->updated.on([listener](Joytime::Controller* ctrl) {
    listener((Joytime_Controller*)ctrl);
  });
};

JOYTIME_CORE_EXPORT void Joytime_Controller_removeUpdateListener(Joytime_Controller* _controller, Joytime_UpdateListenerID id) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;

  controller->updated.removeHandler(id);
};

JOYTIME_CORE_EXPORT int* Joytime_Controller_getInterval(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return &(controller->interval);
};
JOYTIME_CORE_EXPORT void** Joytime_Controller_getHandle(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return &(controller->handle);
};
JOYTIME_CORE_EXPORT uint8_t* Joytime_Controller_getType(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (uint8_t*)(&(controller->type));
};
JOYTIME_CORE_EXPORT uint8_t* Joytime_Controller_getBattery(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (uint8_t*)(&(controller->battery));
};
JOYTIME_CORE_EXPORT Joytime_StickCalibrationData* Joytime_Controller_getLeftStickCalibration(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (Joytime_StickCalibrationData*)(&(controller->leftStickCalibration));
};
JOYTIME_CORE_EXPORT Joytime_StickCalibrationData* Joytime_Controller_getRightStickCalibration(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (Joytime_StickCalibrationData*)(&(controller->rightStickCalibration));
};
JOYTIME_CORE_EXPORT Joytime_SixAxisCalibrationData* Joytime_Controller_getAccelerometerCalibration(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (Joytime_SixAxisCalibrationData*)(&(controller->accelerometerCalibration));
};
JOYTIME_CORE_EXPORT Joytime_SixAxisCalibrationData* Joytime_Controller_getGyroscopeCalibration(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (Joytime_SixAxisCalibrationData*)(&(controller->gyroscopeCalibration));
};
JOYTIME_CORE_EXPORT Joytime_Buttons* Joytime_Controller_getButtons(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (Joytime_Buttons*)(&(controller->buttons));
};
JOYTIME_CORE_EXPORT Joytime_Stick* Joytime_Controller_getLeftStick(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (Joytime_Stick*)(&(controller->leftStick));
};
JOYTIME_CORE_EXPORT Joytime_Stick* Joytime_Controller_getRightStick(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (Joytime_Stick*)(&(controller->rightStick));
};
JOYTIME_CORE_EXPORT Joytime_SixAxis* Joytime_Controller_getAccelerometer(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (Joytime_SixAxis*)(&(controller->accelerometer));
};
JOYTIME_CORE_EXPORT Joytime_SixAxis* Joytime_Controller_getGyroscope(Joytime_Controller* _controller) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  return (Joytime_SixAxis*)(&(controller->gyroscope));
};

JOYTIME_CORE_EXPORT Joytime_Rumble* Joytime_neutralRumble = Joytime_Rumble_newFromFreqAndAmpDiff(320.0, 0.0, 160.0, 0.0);
JOYTIME_CORE_EXPORT uint8_t* Joytime_neutralRumbleBufffer = Joytime_Rumble_toBuffer(Joytime_neutralRumble);

#ifdef __cplusplus
}
#endif /* __cplusplus */
