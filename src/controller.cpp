#include "joytime-core.hpp"
#include <cstdint>
#include <iostream>
#include <vector>
#include <exception>

Joytime::Controller::Controller():
  initializable(false) {};

Joytime::Controller::Controller(const Joytime::Controller& controller):
  transmitBuffer(controller.transmitBuffer),
  receiveBuffer(controller.receiveBuffer),
  transmitBufferC(controller.transmitBufferC),
  receiveBufferC(controller.receiveBufferC),
  type(controller.type),
  handle(controller.handle),
  initializable(controller.initializable),
  usable(controller.usable) {};

Joytime::Controller::Controller(Joytime::ControllerType _type, void* _handle, Joytime::TransmitBufferFunction* _transmitBuffer, Joytime::ReceiveBufferFunction* _receiveBuffer):
  transmitBuffer(_transmitBuffer),
  receiveBuffer(_receiveBuffer),
  type(_type),
  handle(_handle) {};

Joytime::Controller::Controller(Joytime::ControllerType _type, void* _handle, Joytime::CTransmitBufferFunction* _transmitBufferC, Joytime::CReceiveBufferFunction* _receiveBufferC):
  transmitBufferC(_transmitBufferC),
  receiveBufferC(_receiveBufferC),
  type(_type),
  handle(_handle) {};

void Joytime::Controller::_transmitBuffer(std::vector<uint8_t> buffer) {
  if (transmitBuffer != nullptr) {
    transmitBuffer(handle, buffer);
  } else if (transmitBufferC != nullptr) {
    transmitBufferC(handle, buffer.data(), buffer.size());
  } else {
    throw std::runtime_error("Could not send command: no transmission function is set.");
  }
};

std::vector<uint8_t> Joytime::Controller::_receiveResponse() {
  std::vector<uint8_t> response;

  if (receiveBuffer != nullptr) {
    std::vector<uint8_t> tmp;
    while ((tmp = receiveBuffer(handle, 50)), tmp.size() > 0) {
      response.insert(response.end(), tmp.begin(), tmp.end());
    }
  } else if (receiveBufferC != nullptr) {
    uint8_t* tmp;
    int bytesRead = 0;
    while ((tmp = receiveBufferC(handle, 50, &bytesRead)), bytesRead > 0) {
      for (int i = 0; i < bytesRead; i++) {
        response.push_back(tmp[i]);
      }
    }
  } else {
    throw std::runtime_error("Could not send command: no receive function is set.");
  }

  return response;
};

std::vector<uint8_t> Joytime::Controller::sendCommand(Joytime::ControllerCommand command, std::vector<uint8_t> buffer) {
  std::vector<uint8_t> buf;

  buf.push_back((uint8_t)command);
  buf.insert(buf.begin() + 1, buffer.begin(), buffer.end());

  _transmitBuffer(buf);

  // read until a reply is received
  std::vector<uint8_t> reply;
  while ((reply = _receiveResponse()), reply.size() < 1);

  return reply;
};

std::vector<uint8_t> Joytime::Controller::sendSubcommand(Joytime::ControllerCommand command, Joytime::ControllerSubcommand subcommand, std::vector<uint8_t> buffer) {
  std::vector<uint8_t> buf;

  buf.push_back((uint8_t)command);

  buf.push_back(counter);
  counter++;
  if (counter > 0xf) counter = 0;

  buf.insert(buf.begin() + 2, Joytime::neutralRumbleVector.begin(), Joytime::neutralRumbleVector.end());
  buf.insert(buf.begin() + 6, Joytime::neutralRumbleVector.begin(), Joytime::neutralRumbleVector.end());

  buf.push_back((uint8_t)subcommand);

  buf.insert(buf.begin() + 11, buffer.begin(), buffer.end());

  _transmitBuffer(buf);

  // read until the *correct subcommand reply* is received
  std::vector<uint8_t> reply;
  while (true) {
    reply = _receiveResponse();
    if (reply.size() < 1) continue;
    if (reply[0] != (uint8_t)Joytime::ControllerReportCode::SubcommandReply) continue;
    if (reply[14] != (uint8_t)subcommand) continue;
    break;
  };

  return reply;
};

std::vector<uint8_t> Joytime::Controller::readSPIFlash(int32_t address, uint8_t length) {
  std::vector<uint8_t> buf = {
    (uint8_t)((address) & 0xff),
    (uint8_t)((address >> 8) & 0xff),
    (uint8_t)((address >> 16) & 0xff),
    (uint8_t)((address >> 24) & 0xff),
    length
  };

  std::vector<uint8_t> res = sendSubcommand(Joytime::ControllerCommand::RumbleAndSubcommand, Joytime::ControllerSubcommand::ReadSPIFlash, buf);
  if (res.size() < 20) return std::vector<uint8_t>();

  // offset 20, explained:
  // 15 - subcommand response data starts at 15
  // 4  - 4 bytes for the address read
  // 1  - 1 byte for length
  return std::vector<uint8_t>(res.begin() + 20, res.end());
};

void Joytime::Controller::initialize(bool calibrate) {
  if (!initializable) throw std::runtime_error("This Controller cannot be initialized");

  usable = true;

  setInputReportMode(Joytime::ControllerInputReportMode::StandardReport);
  setVibration(true);
  setSixAxisEnabled(true);

  if (calibrate) {
    std::vector<uint8_t> sixAxisCalibrationBuf = readSPIFlash(0x6020, 24);
    std::vector<uint8_t> leftStickCalibrationBuf = readSPIFlash(0x603d, 9);
    std::vector<uint8_t> rightStickCalibrationBuf = readSPIFlash(0x6046, 9);
    std::vector<uint8_t> sixAxisParametersBuf = readSPIFlash(0x6080, 6);
    std::vector<uint8_t> stickParameters1Buf = readSPIFlash(0x6086, 18);
    std::vector<uint8_t> stickParameters2Buf = readSPIFlash(0x6098, 18);

    if (leftStickCalibrationBuf.size() >= 0x09) {
      uint16_t leftStickData[6] = {
        (leftStickCalibrationBuf[1] << 8) & 0xf00 | leftStickCalibrationBuf[0],
        (leftStickCalibrationBuf[2] << 4) | (leftStickCalibrationBuf[1] >> 4),
        (leftStickCalibrationBuf[4] << 8) & 0xf00 | leftStickCalibrationBuf[3],
        (leftStickCalibrationBuf[5] << 4) | (leftStickCalibrationBuf[4] >> 4),
        (leftStickCalibrationBuf[7] << 8) & 0xf00 | leftStickCalibrationBuf[6],
        (leftStickCalibrationBuf[8] << 4) | (leftStickCalibrationBuf[7] >> 4)
      };

      uint16_t leftStickParameters[12] = {
        (stickParameters1Buf[1] << 8) & 0xf00 | stickParameters1Buf[0],
        (stickParameters1Buf[2] << 4) | (stickParameters1Buf[1] >> 4),
        (stickParameters1Buf[4] << 8) & 0xf00 | stickParameters1Buf[3],
        (stickParameters1Buf[5] << 4) | (stickParameters1Buf[4] >> 4),
        (stickParameters1Buf[7] << 8) & 0xf00 | stickParameters1Buf[6],
        (stickParameters1Buf[8] << 4) | (stickParameters1Buf[7] >> 4),
        (stickParameters1Buf[10] << 8) & 0xf00 | stickParameters1Buf[9],
        (stickParameters1Buf[11] << 4) | (stickParameters1Buf[10] >> 4),
        (stickParameters1Buf[13] << 8) & 0xf00 | stickParameters1Buf[12],
        (stickParameters1Buf[14] << 4) | (stickParameters1Buf[13] >> 4),
        (stickParameters1Buf[16] << 8) & 0xf00 | stickParameters1Buf[15],
        (stickParameters1Buf[17] << 4) | (stickParameters1Buf[16] >> 4),
      };

      leftStickCalibration.xCenter = leftStickData[2];
      leftStickCalibration.yCenter = leftStickData[3];
      leftStickCalibration.xMax = leftStickData[0];
      leftStickCalibration.yMax = leftStickData[1];
      leftStickCalibration.xMin = leftStickData[4];
      leftStickCalibration.yMin = leftStickData[5];
      leftStickCalibration.deadZone = leftStickParameters[2];
      leftStickCalibration.rangeRatio = leftStickParameters[3];
    }

    if (rightStickCalibrationBuf.size() >= 0x09) {
      uint16_t rightStickData[6] = {
        (rightStickCalibrationBuf[1] << 8) & 0xf00 | rightStickCalibrationBuf[0],
        (rightStickCalibrationBuf[2] << 4) | (rightStickCalibrationBuf[1] >> 4),
        (rightStickCalibrationBuf[4] << 8) & 0xf00 | rightStickCalibrationBuf[3],
        (rightStickCalibrationBuf[5] << 4) | (rightStickCalibrationBuf[4] >> 4),
        (rightStickCalibrationBuf[7] << 8) & 0xff | rightStickCalibrationBuf[6],
        (rightStickCalibrationBuf[8] << 4) | (rightStickCalibrationBuf[7] >> 4)
      };

      uint16_t rightStickParameters[12] = {
        (stickParameters2Buf[1] << 8) & 0xf00 | stickParameters2Buf[0],
        (stickParameters2Buf[2] << 4) | (stickParameters2Buf[1] >> 4),
        (stickParameters2Buf[4] << 8) & 0xf00 | stickParameters2Buf[3],
        (stickParameters2Buf[5] << 4) | (stickParameters2Buf[4] >> 4),
        (stickParameters2Buf[7] << 8) & 0xf00 | stickParameters2Buf[6],
        (stickParameters2Buf[8] << 4) | (stickParameters2Buf[7] >> 4),
        (stickParameters2Buf[10] << 8) & 0xf00 | stickParameters2Buf[9],
        (stickParameters2Buf[11] << 4) | (stickParameters2Buf[10] >> 4),
        (stickParameters2Buf[13] << 8) & 0xf00 | stickParameters2Buf[12],
        (stickParameters2Buf[14] << 4) | (stickParameters2Buf[13] >> 4),
        (stickParameters2Buf[16] << 8) & 0xf00 | stickParameters2Buf[15],
        (stickParameters2Buf[17] << 4) | (stickParameters2Buf[16] >> 4),
      };

      rightStickCalibration.xCenter = rightStickData[0];
      rightStickCalibration.yCenter = rightStickData[1];
      rightStickCalibration.xMax = rightStickData[4];
      rightStickCalibration.yMax = rightStickData[5];
      rightStickCalibration.xMin = rightStickData[2];
      rightStickCalibration.yMin = rightStickData[3];
      rightStickCalibration.deadZone = rightStickParameters[2];
      rightStickCalibration.rangeRatio = rightStickParameters[3];
    }

    if (sixAxisCalibrationBuf.size() >= 0x18) {
      accelerometerCalibration.originX = (sixAxisCalibrationBuf[1] << 8) | sixAxisCalibrationBuf[0];
      accelerometerCalibration.originY = (sixAxisCalibrationBuf[3] << 8) | sixAxisCalibrationBuf[2];
      accelerometerCalibration.originZ = (sixAxisCalibrationBuf[5] << 8) | sixAxisCalibrationBuf[4];
      accelerometerCalibration.rawCoeffX = (sixAxisCalibrationBuf[7] << 8) | sixAxisCalibrationBuf[6];
      accelerometerCalibration.rawCoeffY = (sixAxisCalibrationBuf[9] << 8) | sixAxisCalibrationBuf[8];
      accelerometerCalibration.rawCoeffZ = (sixAxisCalibrationBuf[11] << 8) | sixAxisCalibrationBuf[10];

      gyroscopeCalibration.originX = (sixAxisCalibrationBuf[13] << 8) | sixAxisCalibrationBuf[12];
      gyroscopeCalibration.originY = (sixAxisCalibrationBuf[15] << 8) | sixAxisCalibrationBuf[14];
      gyroscopeCalibration.originZ = (sixAxisCalibrationBuf[17] << 8) | sixAxisCalibrationBuf[16];
      gyroscopeCalibration.rawCoeffX = (sixAxisCalibrationBuf[19] << 8) | sixAxisCalibrationBuf[18];
      gyroscopeCalibration.rawCoeffY = (sixAxisCalibrationBuf[21] << 8) | sixAxisCalibrationBuf[20];
      gyroscopeCalibration.rawCoeffZ = (sixAxisCalibrationBuf[23] << 8) | sixAxisCalibrationBuf[22];

      accelerometerCalibration.offsetX = (sixAxisParametersBuf[1] << 8) | sixAxisParametersBuf[0];
      accelerometerCalibration.offsetY = (sixAxisParametersBuf[3] << 8) | sixAxisParametersBuf[2];
      accelerometerCalibration.offsetZ = (sixAxisParametersBuf[5] << 8) | sixAxisParametersBuf[4];

      accelerometerCalibration.coeffX = (1.0 / (accelerometerCalibration.rawCoeffX - accelerometerCalibration.originX)) * 4.0;
      accelerometerCalibration.coeffY = (1.0 / (accelerometerCalibration.rawCoeffY - accelerometerCalibration.originY)) * 4.0;
      accelerometerCalibration.coeffZ = (1.0 / (accelerometerCalibration.rawCoeffZ - accelerometerCalibration.originZ)) * 4.0;

      gyroscopeCalibration.coeffX = 816.0 / (gyroscopeCalibration.rawCoeffX - gyroscopeCalibration.offsetX);
      gyroscopeCalibration.coeffY = 816.0 / (gyroscopeCalibration.rawCoeffY - gyroscopeCalibration.offsetY);
      gyroscopeCalibration.coeffZ = 816.0 / (gyroscopeCalibration.rawCoeffZ - gyroscopeCalibration.offsetZ);
    }
  }
};

void Joytime::Controller::performUsabilityCheck() {
  if (!usable) throw std::runtime_error("This Controller cannot be used (yet)");
}

void Joytime::Controller::setInputReportMode(Joytime::ControllerInputReportMode reportMode = Joytime::ControllerInputReportMode::StandardReport) {
  performUsabilityCheck();
  std::vector<uint8_t> buf = { (uint8_t)(reportMode) };

  sendSubcommand(Joytime::ControllerCommand::RumbleAndSubcommand, Joytime::ControllerSubcommand::SetInputReportMode, buf);
};

void Joytime::Controller::setSixAxisEnabled(bool sixAxis) {
  performUsabilityCheck();
  std::vector<uint8_t> buf = { (uint8_t)((sixAxis) ? 1 : 0) };

  sendSubcommand(Joytime::ControllerCommand::RumbleAndSubcommand, Joytime::ControllerSubcommand::SetSixAxisSensor, buf);
};

void Joytime::Controller::setVibration(bool vibrate) {
  performUsabilityCheck();
  std::vector<uint8_t> buf = { (uint8_t)((vibrate) ? 1 : 0) };

  sendSubcommand(Joytime::ControllerCommand::RumbleAndSubcommand, Joytime::ControllerSubcommand::SetVibration, buf);
};

void Joytime::Controller::rumble(uint8_t timing, Joytime::Rumble* _rumble) {
  performUsabilityCheck();
  std::vector<uint8_t> buf = { timing };

  std::vector<uint8_t> leftRumble = Joytime::neutralRumbleVector;
  std::vector<uint8_t> rightRumble = Joytime::neutralRumbleVector;

  switch (type) {
    case Joytime::ControllerType::LeftJoycon:
      leftRumble = _rumble->toVector();
      break;
    case Joytime::ControllerType::RightJoycon:
      rightRumble = _rumble->toVector();
      break;
    case Joytime::ControllerType::Pro:
      leftRumble = _rumble->toVector();
      rightRumble = _rumble->toVector();
      break;
  }

  // 1-8
  buf.insert(buf.begin() + 1, leftRumble.begin(), leftRumble.end());
  buf.insert(buf.begin() + 5, rightRumble.begin(), rightRumble.end());

  sendCommand(Joytime::ControllerCommand::SendRumble, buf);
};

void Joytime::Controller::rumble(uint8_t timing, Joytime::Rumble* leftRumble, Joytime::Rumble* rightRumble) {
  performUsabilityCheck();
  std::vector<uint8_t> buf = { timing };


  std::vector<uint8_t> leftRumbleVector = leftRumble->toVector();
  std::vector<uint8_t> rightRumbleVector = rightRumble->toVector();

  // 1-8
  buf.insert(buf.begin() + 1, leftRumbleVector.begin(), leftRumbleVector.end());
  buf.insert(buf.begin() + 5, rightRumbleVector.begin(), rightRumbleVector.end());

  sendCommand(Joytime::ControllerCommand::SendRumble, buf);
};

uint8_t ledStateToFlag(Joytime::ControllerLEDState led, uint8_t position) {
  switch (led) {
    case Joytime::ControllerLEDState::Off:
      return 0;
    case Joytime::ControllerLEDState::On:
      return 1 << position - 1;
    case Joytime::ControllerLEDState::Flashing:
      return 1 << (position - 1) + 4;
    default:
      return 0; // Off
      break;
  }
};

void Joytime::Controller::setLEDs(Joytime::ControllerLEDState led1, Joytime::ControllerLEDState led2, Joytime::ControllerLEDState led3, Joytime::ControllerLEDState led4) {
  performUsabilityCheck();
  uint8_t flag = 0;

  flag |= ledStateToFlag(led1, 1);
  flag |= ledStateToFlag(led2, 2);
  flag |= ledStateToFlag(led3, 3);
  flag |= ledStateToFlag(led4, 4);

  std::vector<uint8_t> buf = { flag };

  sendSubcommand(Joytime::ControllerCommand::RumbleAndSubcommand, Joytime::ControllerSubcommand::SetPlayerLights, buf);
};

void Joytime::Controller::setPowerState(Joytime::ControllerPowerState state) {
  performUsabilityCheck();
  std::vector<uint8_t> buf = { (uint8_t)state };

  sendSubcommand(Joytime::ControllerCommand::RumbleAndSubcommand, Joytime::ControllerSubcommand::SetPowerState, buf);
};

void Joytime::Controller::update() {
  performUsabilityCheck();
  std::vector<uint8_t> buf = sendCommand(Joytime::ControllerCommand::RumbleAndSubcommand, std::vector<uint8_t>());
  return update(buf);
};

void Joytime::Controller::update(std::vector<uint8_t> buf) {
  performUsabilityCheck();
  if (buf.size() < 1) return;

  switch (buf[0]) {
    case (uint8_t)Joytime::ControllerReportCode::StandardOSController:
      break;
    case (uint8_t)Joytime::ControllerReportCode::NFCIR:
      // no break; the packet also contains a standard input report
    case (uint8_t)Joytime::ControllerReportCode::SubcommandReply:
      // no break; the packet also (partially) contains a standard input report
    case (uint8_t)Joytime::ControllerReportCode::Standard:
      uint8_t _battery = (buf[2] & 0xf0) >> 4;
      if (_battery & 0x01) {
        battery = Joytime::ControllerBatteryStatus::Charging;
      } else {
        battery = (Joytime::ControllerBatteryStatus)_battery;
      }

      buttons.a = buf[3] & 0x08;
      buttons.b = buf[3] & 0x04;
      buttons.x = buf[3] & 0x02;
      buttons.y = buf[3] & 0x01;

      buttons.up = buf[5] & 0x02;
      buttons.down = buf[5] & 0x01;
      buttons.left = buf[5] & 0x08;
      buttons.right = buf[5] & 0x04;

      buttons.l = buf[5] & 0x40;
      buttons.r = buf[3] & 0x40;

      buttons.zl = buf[5] & 0x80;
      buttons.zr = buf[3] & 0x80;

      buttons.sl = (buf[5] & 0x20) | (buf[3] & 0x20);
      buttons.sr = (buf[5] & 0x10) | (buf[3] & 0x10);

      buttons.plus = buf[4] & 0x02;
      buttons.minus = buf[4] & 0x01;

      buttons.lStick = buf[4] & 0x08;
      buttons.rStick = buf[4] & 0x04;

      buttons.home = buf[4] & 0x10;
      buttons.capture = buf[4] & 0x20;

      int16_t rawLeftX = (((buf[7] & 0xf) << 8) | buf[6]);
      int16_t rawLeftY = ((buf[8] << 4) | (buf[7] >> 4));
      int16_t rawRightX = (((buf[10] & 0xf) << 8) | buf[9]);
      int16_t rawRightY = ((buf[11] << 4) | (buf[10] >> 4));

      leftStick.x = rawLeftX - leftStickCalibration.xCenter;
      leftStick.y = rawLeftY - leftStickCalibration.yCenter;

      rightStick.x = rawRightX - rightStickCalibration.xCenter;
      rightStick.y = rawRightY - rightStickCalibration.yCenter;

      if (buf[0] != (uint8_t)Joytime::ControllerReportCode::SubcommandReply) {
        int16_t rawAccelX = (buf[14] << 8) | buf[13];
        int16_t rawAccelY = (buf[16] << 8) | buf[15];
        int16_t rawAccelZ = (buf[18] << 8) | buf[17];
        int16_t rawGyroX = (buf[20] << 8) | buf[19];
        int16_t rawGyroY = (buf[22] << 8) | buf[21];
        int16_t rawGyroZ = (buf[24] << 8) | buf[23];

        accelerometer.x = (rawAccelX - accelerometerCalibration.offsetX) * accelerometerCalibration.coeffX;
        accelerometer.y = (rawAccelY - accelerometerCalibration.offsetY) * accelerometerCalibration.coeffY;
        accelerometer.z = (rawAccelZ - accelerometerCalibration.offsetZ) * accelerometerCalibration.coeffZ;

        gyroscope.x = rawGyroX * gyroscopeCalibration.coeffX;
        gyroscope.y = rawGyroY * gyroscopeCalibration.coeffY;
        gyroscope.z = rawGyroZ * gyroscopeCalibration.coeffZ;
      }

      break;
  }

  updated.emit(this);
};
