#pragma once

#include <string>
#include "esphome/core/defines.h"
#include "esphome/components/time/posix_tz.h"

// Parse a POSIX TZ string (eg: "IST-1GMT0,M10.5.0,M3.5.0/1") at runtime.
// ESPHome only parses timezones at codegen time (or from Home Assistant via
// API), so this is a port of aioesphomeapi.posix_tz.parse_posix_tz.
// Returns false if the string is invalid.
bool parse_posix_tz(const std::string &tz_string, esphome::time::ParsedTimezone &result);
