import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, sun, switch
from esphome.const import (
    CONF_ID,
    CONF_LIGHT_ID,
    CONF_COLD_WHITE_COLOR_TEMPERATURE,
    CONF_WARM_WHITE_COLOR_TEMPERATURE,
    CONF_TRANSITION_LENGTH,
    CONF_SPEED
)

DEPENDENCIES = ['sun', 'light']
AUTO_LOAD = ['sun', 'light', 'switch']

CONF_SUN_ID = 'sun_id'
CONF_SUNRISE_ELEVATION = "sunrise_elevation"
CONF_SUNSET_ELEVATION = "sunset_elevation"


def elevation(value):
    value = cv.angle(value)
    return cv.float_range(min=-180, max=180)(value)


adaptive_lightning_ns = cg.esphome_ns.namespace('adaptive_lightning')
AdaptiveLightningComponent = adaptive_lightning_ns.class_(
    'AdaptiveLightningComponent',
    cg.PollingComponent,
    switch.Switch
)

# Schema for a single component
ADAPTIVE_LIGHTNING_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(AdaptiveLightningComponent),
    cv.GenerateID(CONF_SUN_ID): cv.use_id(sun.Sun),
    cv.Required(CONF_LIGHT_ID): cv.use_id(light.LightState),
    cv.Optional(CONF_COLD_WHITE_COLOR_TEMPERATURE): cv.color_temperature,
    cv.Optional(CONF_WARM_WHITE_COLOR_TEMPERATURE): cv.color_temperature,
    cv.Optional(CONF_SUNRISE_ELEVATION, default=sun.DEFAULT_ELEVATION): elevation,
    cv.Optional(CONF_SUNSET_ELEVATION, default=sun.DEFAULT_ELEVATION): elevation,
    cv.Optional(CONF_TRANSITION_LENGTH, default="1s"): cv.positive_time_period_milliseconds,
    cv.Optional(CONF_SPEED, default=2): cv.positive_float,
}).extend(switch.switch_schema(default_restore_mode="ALWAYS_ON", icon="mdi:blur-linear")).extend(
    cv.polling_component_schema("60s"))

# Main schema that allows multiple components
CONFIG_SCHEMA = cv.All(cv.ensure_list(ADAPTIVE_LIGHTNING_SCHEMA))


async def to_code(config):
    for conf in config:
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)

        sun_component = await cg.get_variable(conf[CONF_SUN_ID])
        light_component = await cg.get_variable(conf[CONF_LIGHT_ID])

        cg.add(var.set_light(light_component))
        cg.add(var.set_sun(sun_component))
        if CONF_COLD_WHITE_COLOR_TEMPERATURE in conf:
            cg.add(var.set_cold_white_temperature(conf[CONF_COLD_WHITE_COLOR_TEMPERATURE]))
        if CONF_WARM_WHITE_COLOR_TEMPERATURE in conf:
            cg.add(var.set_warm_white_temperature(conf[CONF_WARM_WHITE_COLOR_TEMPERATURE]))
        cg.add(var.set_sunrise_elevation(conf[CONF_SUNRISE_ELEVATION]))
        cg.add(var.set_sunset_elevation(conf[CONF_SUNSET_ELEVATION]))
        if CONF_TRANSITION_LENGTH in conf:
            cg.add(var.set_transition_length(conf[CONF_TRANSITION_LENGTH]))
        if CONF_SPEED in conf:
            cg.add(var.set_speed(conf[CONF_SPEED]))

        await switch.register_switch(var, conf)
