#include "word-clock.h"
#include "esphome/components/light/esp_color_view.h"

esphome::Color WordClock::get_pixel_color_(uint32_t i) {
  uint8_t pos;
  pos = (this->rainbow + i) & 255;
  if (pos < 85) {
   return esphome::Color(pos * 3, 255 - pos * 3, 0);
  } else if (pos < 170) {
   pos -= 85;
   return esphome::Color(255 - pos * 3, 0, pos * 3);
  } else {
   pos -= 170;
   return esphome::Color(0, pos * 3, 255 - pos * 3);
  }
}

void WordClock::lit_pixel_color_(esphome::light::AddressableLight &addressable_light, uint32_t pos, esphome::Color color) {
  addressable_light[pos] = color;
}

void WordClock::lit_pixel_color_(uint32_t pos, esphome::Color color) {
  lit_pixel_color_(*addressable_light, pos, color);
}

void WordClock::lit_pixels_color_(uint32_t from, uint32_t to, esphome::Color color) {
  for(uint32_t i=from ; i<=to ; i++)
    lit_pixel_color_(i, color);
}

void WordClock::lit_pixel_rainbow_(uint32_t pos) {
  lit_pixel_color_(pos, get_pixel_color_(pos));
}

void WordClock::lit_pixels_rainbow_(uint32_t from, uint32_t to) {
  for(uint32_t i=from ; i<=to ; i++)
    lit_pixel_rainbow_(i);
}

void WordClock::lit_word_its_() {
  lit_pixels_rainbow_(74, 76);
}

void WordClock::lit_word_half_() {
  lit_pixels_rainbow_(71, 73);
}

void WordClock::lit_word_ten_hours_() {
  lit_pixels_rainbow_(14, 15);
}

void WordClock::lit_word_quarter_() {
  lit_pixels_rainbow_(58, 63);
}

void WordClock::lit_word_twenty_() {
  lit_pixels_rainbow_(64, 68);
}

void WordClock::lit_word_five_hours_() {
  lit_pixels_rainbow_(29, 30);
}

void WordClock::lit_word_minutes_() {
  lit_pixels_rainbow_(49, 54);
}

void WordClock::lit_word_to_() {
  lit_pixels_rainbow_(47, 48);
}

void WordClock::lit_word_past_() {
  lit_pixels_rainbow_(37, 39);
}

void WordClock::lit_word_one_() {
  lit_pixels_rainbow_(40, 42);
}

void WordClock::lit_word_three_() {
  lit_pixels_rainbow_(43, 46);
}

void WordClock::lit_word_two_() {
  lit_pixels_rainbow_(34, 36);
}

void WordClock::lit_word_four_() {
  lit_pixels_rainbow_(31, 33);
}

void WordClock::lit_word_five_minutes_() {
  lit_pixels_rainbow_(55, 57);
}

void WordClock::lit_word_six_() {
  lit_pixels_rainbow_(19, 20);
}

void WordClock::lit_word_seven_() {
  lit_pixels_rainbow_(21, 24);
}

void WordClock::lit_word_eight_() {
  lit_pixels_rainbow_(25, 28);
}

void WordClock::lit_word_nine_() {
  lit_pixels_rainbow_(16, 18);
}

void WordClock::lit_word_ten_minutes_() {
  lit_pixels_rainbow_(69, 70);
}

void WordClock::lit_word_eleven_() {
  lit_pixels_rainbow_(10, 13);
}

void WordClock::lit_word_twelve_() {
  lit_pixels_rainbow_(0, 4);
}

void WordClock::lit_word_o_clock_() {
  lit_pixels_rainbow_(5, 9);
}

void WordClock::lit_hour_(uint8_t hour) {
  switch(hour) {
    case 1:
      lit_word_one_();
      break;
    case 2:
      lit_word_two_();
      break;
    case 3:
      lit_word_three_();
      break;
    case 4:
      lit_word_four_();
      break;
    case 5:
      lit_word_five_hours_();
      break;
    case 6:
      lit_word_six_();
      break;
    case 7:
      lit_word_seven_();
      break;
    case 8:
      lit_word_eight_();
      break;
    case 9:
      lit_word_nine_();
      break;
    case 10:
      lit_word_ten_hours_();
      break;
    case 11:
      lit_word_eleven_();
      break;
    case 12:
    case 0:
      lit_word_twelve_();
      break;
  }
}

void WordClock::set_time(esphome::ESPTime esp_time) {
  if (!esp_time.is_valid()) {
    rainbow = 0;
    lit_pixels_rainbow_(0, 75);
    this->addressable_light->schedule_show();
    return;
  }

  uint8_t hour = esp_time.hour;
  uint8_t min = esp_time.minute;

  this->addressable_light->all() = esphome::Color::BLACK;

  lit_word_its_();

  if (0==hour) {
    hour = 12;
  } else {
    hour = hour%12;
  }

  if (min < 35) {
    lit_hour_(hour);
    if (min >= 0 && min <5) {
      lit_word_o_clock_();
    } else {
      if (min < 30) {
        if (min >= 5 && min <10) {
          lit_word_five_minutes_();
          lit_word_minutes_();
        } else if (min >= 10 && min <15) {
          lit_word_ten_minutes_();
          lit_word_minutes_();
        } else if (min >= 15 && min <20) {
          lit_word_quarter_();
        } else if (min >= 20 && min <25) {
          lit_word_twenty_();
          lit_word_minutes_();
        } else if (min >= 25 && min <30) {
          lit_word_twenty_();
          lit_word_five_minutes_();
          lit_word_minutes_();
        }
      } else if (min >=30 && min < 35) {
        lit_word_half_();
      }
      lit_word_past_();
    }
  } else {
    if (hour<12) {
      lit_hour_(hour+1);
    } else {
      lit_hour_(1);
    }
    if (min >= 35 && min < 40) {
      lit_word_twenty_();
      lit_word_five_minutes_();
      lit_word_minutes_();
    } else if (min >= 40 && min < 45) {
      lit_word_twenty_();
      lit_word_minutes_();
    } else if (min >= 45 && min < 50) {
      lit_word_quarter_();
    } else if (min >= 50 && min < 55) {
      lit_word_ten_minutes_();
      lit_word_minutes_();
    } else if (min >= 55 && min < 60) {
      lit_word_five_minutes_();
      lit_word_minutes_();
    }
    lit_word_to_();
  }

  rainbow += 1;
  this->addressable_light->schedule_show();
}
