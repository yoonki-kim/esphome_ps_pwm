#pragma once
#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "ps_pwm.h"

namespace esphome {
namespace ps_pwm {

class PsPwmWrapper : public Component, public output::FloatOutput {
 public:
  void set_pins(int lead_a, int lead_b, int lag_a, int lag_b) {
    pin_lead_a_ = lead_a;
    pin_lead_b_ = lead_b;
    pin_lag_a_ = lag_a;
    pin_lag_b_ = lag_b;
  }
  
  void set_frequency(float freq) { frequency_ = freq; }
  
  void set_deadtimes(float lead_red, float lead_fed, float lag_red, float lag_fed) {
    lead_red_ = lead_red;
    lead_fed_ = lead_fed;
    lag_red_ = lag_red;
    lag_fed_ = lag_fed;
  }

  void setup() override;
  void write_state(float state) override;
  void dump_config() override;

 protected:
  int pin_lead_a_, pin_lead_b_, pin_lag_a_, pin_lag_b_;
  float frequency_;
  float lead_red_{125e-9f}, lead_fed_{125e-9f}, lag_red_{125e-9f}, lag_fed_{125e-9f};
};

} // namespace ps_pwm
} // namespace esphome
