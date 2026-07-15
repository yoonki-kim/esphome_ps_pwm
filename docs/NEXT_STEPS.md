# 다음 개발 단계 (Next Steps)

오늘(`ps_pwm_basic.yaml` 기반 웹 서버 및 하드웨어 Fault 기능 연동)까지 진행된 사항 이후, 다음에 이어서 진행할 수 있는 개발 마일스톤 및 체크리스트입니다.

## 1. 하드웨어 검증 (Hardware Verification)
- [ ] **오실로스코프 파형 측정**: 현재 `test_node.yaml` 및 `ps_pwm_basic.yaml`을 통해 제어되는 GPIO 4, 5, 6, 7번의 Phase-Shift PWM 출력 파형이 의도한 주파수(100kHz/200kHz)와 듀티 사이클, 그리고 설정된 데드타임(Dead-time)에 맞게 정확히 출력되는지 오실로스코프로 확인합니다.
- [ ] **하드웨어 Fault 동작 검증**: GPIO 8번(OCP_PULSE) 핀을 물리적으로 Active 시켰을 때(GND 쇼트), 즉시 하드웨어단에서 PWM 출력이 차단(Brake)되는지 오실로스코프를 통해 검증합니다.

## 2. ESPHome 아키텍처 고도화 (Advanced ESPHome Integration)
- [ ] 현재는 C++ 람다(Lambda) 구문 내부에서 `pspwm_set_frequency` 등의 C 함수를 직접 호출하고 있습니다. 향후 ESPHome 대시보드(또는 Home Assistant) 화면에서 슬라이더나 버튼 UI를 통해 실시간으로 주파수나 위상 천이(Phase-shift) 값을 조절하고 싶다면, `ps_pwm` 컴포넌트 내부(Python 및 C++ 래퍼)에 **Custom Number / Custom Sensor / Custom Action** 인터페이스를 추가로 노출하는 작업을 진행할 수 있습니다.

## 3. 다른 예제 이식 (Porting other examples)
- [ ] `esp32_ps_pwm` 라이브러리의 `examples/` 디렉토리에 있는 또 다른 고급 예제인 **`zvs_resonant_tracking`** (공진 주파수 자동 추적 로직)을 ESPHome 환경으로 이식하는 작업을 시작할 수 있습니다.
