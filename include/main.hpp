#pragma once

#include "scotland2/shared/modloader.h"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

#include "paper/shared/logger.hpp"

constexpr auto Logger = Paper::ConstLoggerContext("Play3rdPerson");

#define PLAY_3RD_PERSON_EXPORT extern "C" __attribute__((visibility("default")))