# Adaptive Lightning

Adaptive lighting component for ESPHome. It sets the light color temperature based on the current position of the sun.

```yaml
adaptive_lightning:
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
- **speed** (*Optional*, float): The speed of the transition between color temperatures. Default is `2.0`.
- **update_interval** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): The interval in
  which the color temperature is updated. Default is `60s`.
- **transition_duration** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): The duration
  of the transition between color temperatures. Default is `3s`.
- **restore_mode** (*Optional*, RestoreMode): The restore mode to use. Default is `ALWAYS_ON`.

## Example

```yaml
external_components:
  - source: github://mdvorak/esphome-components

adaptive_lightning:
  - light_id: cw_light
    name: "Adaptive Lightning"
    cold_white_color_temperature: 6500 K
    warm_white_color_temperature: 2700 K

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

* Loosely based on https://github.com/basnijholt/adaptive-lighting, thanks [basnijholt](https://github.com/basnijholt)!

## License

[Apache License Version 2.0](https://www.apache.org/licenses/LICENSE-2.0)

## See Also

- [CWWW Light](https://esphome.io/components/light/cwww.html)
- [Light](https://esphome.io/components/light/index.html)
- [Sun](https://esphome.io/components/sun.html)
- [Time](https://esphome.io/components/time/index.html)
- [Switch](https://esphome.io/components/switch/index.html)
