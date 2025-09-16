# ESPHome Adaptive Lighting

Adaptive lighting component for [ESPHome](https://esphome.io). It sets the light color temperature based on the current
position of the sun.

If the light color is adjusted manually, the adaptive lighting will be disabled until the light is turned off and on
again. The adaptive lighting is also a switch, so it can be enabled or disabled manually, or by automation.

```yaml
adaptive_lighting:
  - light_id: cw_light
```

## Configuration variables

- **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types.html#id)): Manually specify the ID used for
  code generation. At least one of `id` or `name` must be specified.
- **name** (*Optional*, string): The name of the component.
- **icon** (*Optional*, icon): Manually specify the icon to use for this component.
  Used for enable switch in the frontend.
- **light_id** (*Required*, [ID](https://esphome.io/guides/configuration-types.html#id)): The light to control.
  It must support color temperature.
- **cold_white_color_temperature** (*Optional*, float): The color temperature
  (in [mireds](https://en.wikipedia.org/wiki/Mired) or Kelvin) of the cold white channel. This can differ from the
  configuration of the light, but it still must be within the supported range.
- **warm_white_color_temperature** (*Optional*, float): The color temperature
  (in [mireds](https://en.wikipedia.org/wiki/Mired) or Kelvin) of the warm white channel. This can differ from the
  configuration of the light, but it still must be within the supported range.
- **sunrise_elevation** (*Optional*, float): The elevation of the sun at sunrise. Default is `nautical` (`-12 deg`).
- **sunset_elevation** (*Optional*, float): The elevation of the sun at sunset. Default is `nautical` (`-12 deg`).
- **speed** (*Optional*, float): The speed of the transition between color temperatures. Default is `1.0`.
- **update_interval** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): The interval in
  which the color temperature is updated. Default is `60s`.
- **transition_duration** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): The duration
  of the transition between color temperatures. Default is `1s`.
- **restore_mode** (*Optional*, RestoreMode): The restore mode to use. Default is `ALWAYS_ON`.

## Example

```yaml
external_components:
  - source: github://mdvorak/esphome-adaptive-lighting@v1

adaptive_lighting:
  - light_id: cw_light
    name: "Adaptive Lighting"
    cold_white_color_temperature: 6000 K
    warm_white_color_temperature: 2700 K
    sunrise_elevation: -15.0 deg
    sunset_elevation: -15.0 deg

output:
  - platform: ledc
    id: ledc_cold
    # Set the pin and frequency
  - platform: ledc
    id: ledc_warm
    # Set the pin and frequency

light:
  - platform: cwww
    id: cw_light
    name: "Light"
    cold_white: ledc_cold
    warm_white: ledc_warm
    cold_white_color_temperature: 6500 K
    warm_white_color_temperature: 2700 K
    constant_brightness: true

sun:
  latitude: !secret latitude
  longitude: !secret longitude

time:
  - platform: sntp
    timezone: !secret timezone
```

## Credits

* Inspired by https://github.com/basnijholt/adaptive-lighting, thanks [@basnijholt](https://github.com/basnijholt)!

## License

[Apache License Version 2.0](https://www.apache.org/licenses/LICENSE-2.0)

## See Also

- [CWWW Light](https://esphome.io/components/light/cwww.html)
- [Light](https://esphome.io/components/light/index.html)
- [Sun](https://esphome.io/components/sun.html)
- [Time](https://esphome.io/components/time/index.html)
- [Switch](https://esphome.io/components/switch/index.html)
