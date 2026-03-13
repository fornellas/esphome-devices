 #pragma once

#include "esphome/components/light/addressable_light.h"
#include "esphome/core/time.h"
#include "esphome/core/color.h"

class WordClock {
public:
	WordClock(esphome::light::AddressableLight *addressable_light) {
		this->addressable_light = addressable_light;
		this->rainbow = 0;
	}
	void set_time(esphome::ESPTime esp_time);
	esphome::light::AddressableLight *addressable_light;
private:
	uint8_t rainbow;

	esphome::Color get_pixel_color_(uint32_t i);
	void lit_pixel_color_(esphome::light::AddressableLight &addressable_light, uint32_t pos, esphome::Color color);
	void lit_pixel_color_(uint32_t pos, esphome::Color color);
	void lit_pixels_color_(uint32_t from, uint32_t to, esphome::Color color);
	void lit_pixel_rainbow_(uint32_t pos);
	void lit_pixels_rainbow_(uint32_t from, uint32_t to);
	void lit_word_its_();
	void lit_word_half_();
	void lit_word_ten_hours_();
	void lit_word_quarter_();
	void lit_word_twenty_();
	void lit_word_five_hours_();
	void lit_word_minutes_();
	void lit_word_to_();
	void lit_word_past_();
	void lit_word_one_();
	void lit_word_three_();
	void lit_word_two_();
	void lit_word_four_();
	void lit_word_five_minutes_();
	void lit_word_six_();
	void lit_word_seven_();
	void lit_word_eight_();
	void lit_word_nine_();
	void lit_word_ten_minutes_();
	void lit_word_eleven_();
	void lit_word_twelve_();
	void lit_word_o_clock_();
	void lit_hour_(uint8_t hour);
};
