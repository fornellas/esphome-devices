#include "posix-tz.h"
#include <cctype>

using esphome::time::DSTRule;
using esphome::time::DSTRuleType;
using esphome::time::ParsedTimezone;

static bool skip_tz_name_(const std::string &s, size_t &pos) {
  if (pos < s.size() && s[pos] == '<') {
    pos++;
    while (pos < s.size() && s[pos] != '>')
      pos++;
    if (pos >= s.size())
      return false;
    pos++;
    return true;
  }
  size_t start = pos;
  while (pos < s.size() && isalpha((unsigned char)s[pos]))
    pos++;
  return pos - start >= 3;
}

static bool parse_uint_(const std::string &s, size_t &pos, uint32_t &value) {
  size_t start = pos;
  value = 0;
  while (pos < s.size() && isdigit((unsigned char)s[pos])) {
    value = value * 10 + (s[pos] - '0');
    pos++;
  }
  return pos != start;
}

static bool parse_offset_(const std::string &s, size_t &pos, int32_t &offset) {
  int32_t sign = 1;
  if (pos < s.size() && s[pos] == '-') {
    sign = -1;
    pos++;
  } else if (pos < s.size() && s[pos] == '+') {
    pos++;
  }
  uint32_t hours, minutes = 0, seconds = 0;
  if (!parse_uint_(s, pos, hours))
    return false;
  if (pos < s.size() && s[pos] == ':') {
    pos++;
    if (!parse_uint_(s, pos, minutes))
      return false;
    if (pos < s.size() && s[pos] == ':') {
      pos++;
      if (!parse_uint_(s, pos, seconds))
        return false;
    }
  }
  offset = sign * (int32_t)(hours * 3600 + minutes * 60 + seconds);
  return true;
}

static bool parse_dst_rule_(const std::string &s, size_t &pos, DSTRule &rule) {
  rule = DSTRule{};
  if (pos >= s.size())
    return false;
  uint32_t value;
  if (s[pos] == 'M' || s[pos] == 'm') {
    rule.type = DSTRuleType::MONTH_WEEK_DAY;
    pos++;
    if (!parse_uint_(s, pos, value) || value < 1 || value > 12)
      return false;
    rule.month = value;
    if (pos >= s.size() || s[pos] != '.')
      return false;
    pos++;
    if (!parse_uint_(s, pos, value) || value < 1 || value > 5)
      return false;
    rule.week = value;
    if (pos >= s.size() || s[pos] != '.')
      return false;
    pos++;
    if (!parse_uint_(s, pos, value) || value > 6)
      return false;
    rule.day_of_week = value;
  } else if (s[pos] == 'J' || s[pos] == 'j') {
    rule.type = DSTRuleType::JULIAN_NO_LEAP;
    pos++;
    if (!parse_uint_(s, pos, value) || value < 1 || value > 365)
      return false;
    rule.day = value;
  } else if (isdigit((unsigned char)s[pos])) {
    rule.type = DSTRuleType::DAY_OF_YEAR;
    if (!parse_uint_(s, pos, value) || value > 365)
      return false;
    rule.day = value;
  } else {
    return false;
  }

  rule.time_seconds = 2 * 3600;
  if (pos < s.size() && s[pos] == '/') {
    pos++;
    if (!parse_offset_(s, pos, rule.time_seconds))
      return false;
  }
  return true;
}

bool parse_posix_tz(const std::string &s, ParsedTimezone &result) {
  result = ParsedTimezone{};
  size_t pos = 0;

  if (s.empty() || !skip_tz_name_(s, pos))
    return false;

  if (pos >= s.size() || !(isdigit((unsigned char)s[pos]) || s[pos] == '+' || s[pos] == '-'))
    return false;
  if (!parse_offset_(s, pos, result.std_offset_seconds))
    return false;

  if (pos >= s.size())
    return true;
  if (s[pos] == ',')
    return false;
  if (!isalpha((unsigned char)s[pos]) && s[pos] != '<')
    return true;

  if (!skip_tz_name_(s, pos))
    return false;

  if (pos < s.size() && (isdigit((unsigned char)s[pos]) || s[pos] == '+' || s[pos] == '-')) {
    if (!parse_offset_(s, pos, result.dst_offset_seconds))
      return false;
  } else {
    result.dst_offset_seconds = result.std_offset_seconds - 3600;
  }

  if (pos >= s.size() || s[pos] != ',')
    return true;
  pos++;
  if (!parse_dst_rule_(s, pos, result.dst_start))
    return false;

  if (pos >= s.size() || s[pos] != ',')
    return false;
  pos++;
  return parse_dst_rule_(s, pos, result.dst_end);
}
