import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID, CONF_FREQUENCY
from esphome import pins

from . import ps_pwm_ns

DEPENDENCIES = ["esp32"]

PsPwmWrapper = ps_pwm_ns.class_("PsPwmWrapper", output.FloatOutput, cg.Component)

CONF_PIN_LEAD_A = "pin_lead_a"
CONF_PIN_LEAD_B = "pin_lead_b"
CONF_PIN_LAG_A = "pin_lag_a"
CONF_PIN_LAG_B = "pin_lag_b"

CONF_LEAD_RED = "lead_red"
CONF_LEAD_FED = "lead_fed"
CONF_LAG_RED = "lag_red"
CONF_LAG_FED = "lag_fed"

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(PsPwmWrapper),
        cv.Required(CONF_PIN_LEAD_A): pins.internal_gpio_output_pin_number,
        cv.Required(CONF_PIN_LEAD_B): pins.internal_gpio_output_pin_number,
        cv.Required(CONF_PIN_LAG_A): pins.internal_gpio_output_pin_number,
        cv.Required(CONF_PIN_LAG_B): pins.internal_gpio_output_pin_number,
        cv.Required(CONF_FREQUENCY): cv.frequency,
        cv.Optional(CONF_LEAD_RED, default=125e-9): cv.float_,
        cv.Optional(CONF_LEAD_FED, default=125e-9): cv.float_,
        cv.Optional(CONF_LAG_RED, default=125e-9): cv.float_,
        cv.Optional(CONF_LAG_FED, default=125e-9): cv.float_,
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)

    cg.add(var.set_pins(
        config[CONF_PIN_LEAD_A],
        config[CONF_PIN_LEAD_B],
        config[CONF_PIN_LAG_A],
        config[CONF_PIN_LAG_B]
    ))
    
    cg.add(var.set_frequency(config[CONF_FREQUENCY]))
    
    cg.add(var.set_deadtimes(
        config[CONF_LEAD_RED],
        config[CONF_LEAD_FED],
        config[CONF_LAG_RED],
        config[CONF_LAG_FED]
    ))
