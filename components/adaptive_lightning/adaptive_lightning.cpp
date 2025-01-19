#include "adaptive_lightning.h"

#include <cmath>
#include <utility>

#include "esphome/core/log.h"

static const char *TAG = "adaptive_lightning";

namespace esphome {
namespace adaptive_lightning {

void AdaptiveLightningComponent::update() {
  if (light_ == nullptr || sun_ == nullptr) {
    ESP_LOGW(TAG, "Light or Sun component not set!");
    return;
  }

  if (!this->state) {
    ESP_LOGD("adaptive_lightning", "Update skipped - automatic updates disabled");
    return;
  }

  if (!light_->remote_values.is_on()) {
    ESP_LOGD("adaptive_lightning", "Update skipped - light is off");
    return;
  }

  // Get current timestamp
  const auto now = sun_->get_time()->now();
  // Calculate start of day, to get today's events, not next events
  auto today = now;
  today.hour = today.minute = today.second = 0;
  today.recalc_timestamp_utc();

  auto sunrise = sun_->sunrise(today, sunrise_elevation_);
  auto sunset = sun_->sunset(today, sunset_elevation_);

  if (!sunrise || !sunset) {
    ESP_LOGW(TAG, "Could not determine sunrise or sunset");
    return;
  }

  // Calculate
  float mireds = calc_color_temperature(now.timestamp, sunrise->timestamp, sunset->timestamp, min_ct_, max_ct_);
  if (mireds == last_requested_color_temp_) {
    // This is mandatory to avoid infinite loops when the light is updated
    ESP_LOGD(TAG, "Skipping update, color temperature is the same as last requested");
    return;
  }
  last_requested_color_temp_ = mireds;

  ESP_LOGD(TAG, "Setting color temperature %.3f", mireds);
  auto call = light_->make_call();
  call.set_color_temperature(mireds);
  if (transition_length_ > 0) {
    call.set_transition_length_if_supported(transition_length_);
  }
  call.perform();
}

void AdaptiveLightningComponent::write_state(bool state) {
  if (this->state != state) {
    if (state) {
      ESP_LOGD(TAG, "Adaptive lighting enabled");
    } else {
      ESP_LOGD(TAG, "Adaptive lighting disabled");
    }

    this->force_next_update();
    this->publish_state(state);
    this->update();
    this->force_next_update(); // Force update again, to update color after turn-on transition
  }
}

void AdaptiveLightningComponent::handle_light_state_change() {
  if (light_ == nullptr)
    return;

  bool current_state = light_->remote_values.is_on();

  // Light is on
  if (current_state) {
    float current_temp = light_->remote_values.get_color_temperature();

    // Check if we have a previous temperature set and if it differs
    if (this->state && last_requested_color_temp_ > 0 && std::fabs(current_temp - last_requested_color_temp_) > 0.1) {
      ESP_LOGI(
          TAG,
          "Color temperature changed externally (current: %.3f, last requested: %.3f), disabling adaptive lighting",
          current_temp, last_requested_color_temp_);
      this->write_state(false);
    } else if (!previous_light_state_ && !this->state && // Light was just turned on
               this->restore_mode == switch_::SWITCH_ALWAYS_ON) {
      // Enable the switch when light turns on if restore mode is ALWAYS_ON
      this->write_state(true);
    }
  }

  previous_light_state_ = current_state;
}

void AdaptiveLightningComponent::handle_target_state_reached() {
  if (light_ == nullptr)
    return;

  // We rely on previous_light_state_ was set in handle_light_state_change
  if (previous_light_state_ && this->state) {
    // Update color temperature if adaptive lighting is enabled
    this->update();
  }
}

// Helper function to compute coefficients 'a' and 'b' for the tanh function.
static std::pair<float, float> findAB(float x1, float x2, float y1, float y2) {
  // This function mimics the scaled_tanh approach from color_and_brightness.py.
  // Keep it simple: we want color temperature -> [min_ct, max_ct].
  // We compute "a" and "b" so that tanh covers the range from y1 to y2 over
  // [x1, x2].
  const float eps = 0.00001f;

  // If x2 and x1 are extremely close, return default (a=1, b=0) to avoid
  // divide-by-zero.
  if (std::fabs(x2 - x1) < eps) {
    return std::make_pair(1.0f, 0.0f);
  }

  // a = [atanh(2*y2-1) - atanh(2*y1-1)] / (x2 - x1)
  // b = x1 - [atanh(2*y1-1) / a]
  float a_val = (std::atanh(2.0f * y2 - 1.0f) - std::atanh(2.0f * y1 - 1.0f)) / (x2 - x1);
  float b_val = x1 - (std::atanh(2.0f * y1 - 1.0f) / a_val);
  return std::make_pair(a_val, b_val);
}

// Main scaledTanh function computing the tanh-based interpolation.
// x, x1, x2 specify your input range for time (or some other parameter),
// y1, y2 let you customize which portion of tanh range you use.
static float scaledTanh(float x, float x1, float x2, float y1, float y2, float min_ct, float max_ct) {
  // Obtain the 'a' and 'b' values from findAB().
  auto ab = findAB(x1, x2, y1, y2);

  // Basic tanh interpolation: map tanh(...) from [-1,1] to [0,1], then scale to
  // [min_ct, max_ct].
  float t = std::tanh(ab.first * (x - ab.second));
  // Map result from [-1..1] to [0..1], then to [min_ct..max_ct].
  return min_ct + (max_ct - min_ct) * 0.5f * (t + 1.0f);
}

// This method is loosely based on https://github.com/basnijholt/adaptive-lighting
float AdaptiveLightningComponent::calc_color_temperature(const time_t now, const time_t sunrise, const time_t sunset,
                                                         float min_ct, float max_ct) {
  // If before sunrise or after sunset, default to min_ct
  // If between sunrise and sunset, apply scaledTanh to get a smoother
  // transition.
  if (now < sunrise || now > sunset) {
    return max_ct;
  } else {
    // We do a simple mirrored approach:
    // y1 = 0.05 (very warm at sunrise), y2 = 0.95 (very cool near midday),
    // then it transitions back. This is just an example input to scaledTanh.
    return scaledTanh(now, sunrise, sunset, 0.05f, 0.95f, min_ct, max_ct);
  }
}

} // namespace adaptive_lightning
} // namespace esphome
