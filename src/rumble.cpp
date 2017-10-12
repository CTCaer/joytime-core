#include "joytime-core.hpp"
#include <cmath>

// clamps a number to make sure it doesn't exceed either boundary
double clamp(double n, double min, double max) {
  // we could just do it all on one line, but readability ftw
  if (n < min) return min;
  if (n > max) return max;
  return n;
};

double encodeFrequency(double frequency) {
  return round(log2(frequency / 10.0) * 32.0);
};

double encodeAmplitude(double amplitude) {
  double preEncode = (log2(amplitude * 1000) * 32) - 0x60;
  double ret = 0;
  if (amplitude == 0) return 0;
  if (amplitude < 0.117) ret = (preEncode / (5 - pow(2, amplitude))) - 1;
  if (amplitude >= 0.117 && amplitude < 0.23) ret = preEncode - 0x5c;
  if (amplitude >= 0.23) ret = (preEncode * 2) - 0xf6;
  return round(ret);
};

Joytime::Rumble::Rumble(double frequency, double amplitude):
  highFrequency(Rumble::frequencyToHF(frequency)),
  lowFrequency(Rumble::frequencyToLF(frequency)),
  highAmplitude(Rumble::amplitudeToHA(amplitude)),
  lowAmplitude(Rumble::amplitudeToLA(amplitude)) {};

Joytime::Rumble::Rumble(double high_frequency, double high_amplitude, double low_frequency, double low_amplitude):
  highFrequency(Rumble::frequencyToHF(high_frequency)),
  lowFrequency(Rumble::frequencyToLF(low_frequency)),
  highAmplitude(Rumble::amplitudeToHA(high_amplitude)),
  lowAmplitude(Rumble::amplitudeToLA(low_amplitude)) {};

Joytime::Rumble::Rumble(uint16_t hf, uint8_t hf_amp, uint8_t lf, uint16_t lf_amp):
  highFrequency(hf),
  lowFrequency(lf),
  highAmplitude(hf_amp),
  lowAmplitude(lf_amp) {};

uint8_t* Joytime::Rumble::toBuffer() {
  uint8_t buf[4] = {
    highFrequency & 0xff, // high frequency upper byte
    highAmplitude + ((highFrequency >> 8) & 0xff), // high frequency amplitude + high frequency lower byte
    lowFrequency + ((lowAmplitude >> 8) & 0xff), // low frequency + low frequency amplitude lower byte
    lowAmplitude & 0xff, // low frequency upper byte
  };

  return buf;
};

std::vector<uint8_t> Joytime::Rumble::toVector() {
  std::vector<uint8_t> buf = {
    (uint8_t)(highFrequency & 0xff), // high frequency upper byte
    (uint8_t)(highAmplitude + ((highFrequency >> 8) & 0xff)), // high frequency amplitude + high frequency lower byte
    (uint8_t)(lowFrequency + ((lowAmplitude >> 8) & 0xff)), // low frequency + low frequency amplitude lower byte
    (uint8_t)(lowAmplitude & 0xff), // low frequency upper byte
  };

  return buf;
};

uint16_t Joytime::Rumble::frequencyToHF(double frequency) {
  frequency = clamp(frequency, 0.0, 1253.0);

  return (encodeFrequency(frequency) - 0x60) * 4;
};

uint8_t Joytime::Rumble::frequencyToLF(double frequency) {
  frequency = clamp(frequency, 0.0, 1253.0);

  return (encodeFrequency(frequency) * 32.0) - 0x40;
};

uint8_t Joytime::Rumble::amplitudeToHA(double amplitude) {
  amplitude = clamp(amplitude, 0.0, 1.8);

  return encodeAmplitude(amplitude);
};

uint16_t Joytime::Rumble::amplitudeToLA(double amplitude) {
  uint8_t encoded = encodeAmplitude(amplitude);
  uint8_t evenOrOdd = encoded % 2;

  if (evenOrOdd > 0) encoded = encoded - 1;
  encoded = encoded / 2;

  return (((evenOrOdd > 0) ? 0x80 : 0) << 8) | (encoded + 0x40);
};
