#pragma once

#include "esphome.h"
#include "esphome/components/light/light_state.h"
#include "esphome/components/sun/sun.h"
#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"
#include "esphome/core/version.h"

namespace esphome::adaptive_lighting {

struct SunEvents {
  ESPTime today;
  optional<ESPTime> sunrise;
  optional<ESPTime> sunset;
  float sunrise_elevation;
  float sunset_elevation;
};

class AdaptiveLightingListenerAdapter;

class AdaptiveLightingComponent : public PollingComponent, public switch_::Switch
{
public:
  void setup() override;

  virtual ~AdaptiveLightingComponent();

  void set_sun(sun::Sun *sun) { ESP_LOGW("TAG", "Sun: %p and %p", static_cast<void *>(sun_), static_cast<void *>(listener_)); sun_ = sun;  }
  void set_light(light::LightState *light) { ESP_LOGW("TAG", "Light: %p TO %p", static_cast<void *>(light_), static_cast<void *>(light)); light_ = light; }
  void set_cold_white_temperature(float min_mireds) { min_mireds_ = min_mireds; }
  void set_warm_white_temperature(float max_mireds) { max_mireds_ = max_mireds; }
  void set_transition_length(uint32_t transition_length) { transition_length_ = transition_length; }
  void set_sunrise_elevation(float sunrise_elevation) { sunrise_elevation_ = sunrise_elevation; }
  void set_sunset_elevation(float sunset_elevation) { sunset_elevation_ = sunset_elevation; }
  void set_speed(float speed) { speed_ = speed; }

  void update() override;

  void write_state(bool state) override;

  void force_next_update() { last_requested_color_temp_ = 0; }

  void dump_config() override;

  void on_light_remote_values_update();

  SunEvents calc_sun_events(const ESPTime &now);

  float calc_color_temperature(const time_t now, const time_t sunrise, const time_t sunset) {
    return calc_color_temperature(now, sunrise, sunset, min_mireds_, max_mireds_, speed_);
  }

  static float calc_color_temperature(const time_t now, const time_t sunrise, const time_t sunset, float min_mireds,
                                      float max_mireds, float speed);

protected:
  sun::Sun *sun_{nullptr};
  light::LightState *light_{nullptr};
  float min_mireds_{0};
  float max_mireds_{0};
  float light_min_mireds_{0};
  float light_max_mireds_{0};
  float sunrise_elevation_{-0.83333};
  float sunset_elevation_{-0.83333};
  uint32_t transition_length_{0};
  float speed_{1};

  bool previous_light_state_{false};
  float last_requested_color_temp_{0};

#if ESPHOME_VERSION_CODE >= VERSION_CODE(2025, 12, 0)
  AdaptiveLightingListenerAdapter *listener_{nullptr};
#endif
};

} // namespace esphome::adaptive_lighting
