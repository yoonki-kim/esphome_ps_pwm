#include "ps_pwm_wrapper.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ps_pwm {

static const char *const TAG = "ps_pwm";

void PsPwmWrapper::setup() {
  ESP_LOGCONFIG(TAG, "Setting up PS-PWM...");
  
  mcpwm_action_on_pwmxa_t disable_action_lead = MCPWM_FORCE_MCPWMXA_LOW;
  mcpwm_action_on_pwmxa_t disable_action_lag  = MCPWM_FORCE_MCPWMXA_LOW;
  
  bool init_power_pwm_active = false;
  float init_ps_duty = 0.0f; 
  
  esp_err_t err = pspwm_init(MCPWM_UNIT_0,
                             pin_lead_a_, pin_lead_b_,
                             pin_lag_a_, pin_lag_b_,
                             frequency_, init_ps_duty,
                             lead_red_, lead_fed_,
                             lag_red_, lag_fed_,
                             init_power_pwm_active,
                             disable_action_lead,
                             disable_action_lag);
  
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize PS-PWM");
    this->mark_failed();
    return;
  }

  // 초기화 완료 후 안전을 위해 출력을 명시적으로 비활성화(OFF) 상태로 유지.
  // 출력 활성화는 반드시 사용자의 명시적 호출(pspwm_resync_enable_output)로만 이루어져야 함.
  pspwm_disable_output(MCPWM_UNIT_0);
  ESP_LOGCONFIG(TAG, "PS-PWM setup complete. Output is initially DISABLED.");
}

void PsPwmWrapper::write_state(float state) {
  // ESPHome float output gives 0.0 to 1.0
  pspwm_set_ps_duty(MCPWM_UNIT_0, state);
}

void PsPwmWrapper::dump_config() {
  ESP_LOGCONFIG(TAG, "PS-PWM Output:");
  ESP_LOGCONFIG(TAG, "  LEAD Pins: A=%d, B=%d", pin_lead_a_, pin_lead_b_);
  ESP_LOGCONFIG(TAG, "  LAG Pins: A=%d, B=%d", pin_lag_a_, pin_lag_b_);
  ESP_LOGCONFIG(TAG, "  Frequency: %.1f Hz", frequency_);
  ESP_LOGCONFIG(TAG, "  Deadtimes: LEAD_RED=%.1e, LEAD_FED=%.1e, LAG_RED=%.1e, LAG_FED=%.1e", 
                lead_red_, lead_fed_, lag_red_, lag_fed_);
}

} // namespace ps_pwm
} // namespace esphome
