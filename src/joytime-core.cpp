#include "joytime-core.hpp"
#include "joytime_core_EXPORTS.hpp"

namespace Joytime {
  JOYTIME_CORE_EXPORT Joytime::Rumble neutralRumble = Joytime::Rumble(320.0, 0.0, 160.0, 0.0);
  JOYTIME_CORE_EXPORT uint8_t* neutralRumbleBuffer = neutralRumble.toBuffer();
  JOYTIME_CORE_EXPORT std::vector<uint8_t> neutralRumbleVector = neutralRumble.toVector();
  JOYTIME_CORE_EXPORT EventEmitter<Joytime::Controller*> controllerAvailable;
  JOYTIME_CORE_EXPORT EventEmitter<Joytime::Controller*> controllerRemoved;
};
