# ESPHome 기반 `ps_pwm_basic` 구현 계획서

이 문서는 `esp32_ps_pwm` 라이브러리에 포함된 `ps_pwm_basic` C 예제를 ESPHome 환경의 `ps_pwm_basic.yaml` 파일로 기본 기능을 이식(포팅)하는 계획을 담고 있습니다.

## 논의 사항 (Open Questions)
- `ps_pwm_basic` C 예제에서는 **GPIO 8번 핀**을 하드웨어 결함(Hardware Fault, OCP_PULSE) 감지 핀으로 사용합니다. 가지고 계신 ESP32-S3 DevKit 보드에 해당 핀(GPIO 8)이 실제로 결함 감지용으로 연결되어 있나요, 아니면 단순히 데모용으로 설정된 것인가요?
- ESPHome으로 이식할 때, 기존 C 코드처럼 10ms 단위의 무한 반복문(루프)을 엄격하게 돌리는 방식과, ESPHome의 자체 이벤트 방식(예: 버튼은 `on_press` 사용, 5초 주기 변경은 `interval` 컴포넌트 사용)을 혼합하여 YAML 코드를 더 깔끔하게 구성하는 방식이 있습니다. 저는 YAML 구성이 훨씬 깔끔해지는 **혼합형 방식(하이브리드)**을 제안합니다. 이 방향으로 진행해도 될까요?

## 제안하는 변경 사항

### [NEW] [ps_pwm_basic.yaml](file:///home/yoonki/esphome/esphome_ps_pwm/ps_pwm_basic.yaml)
1. **기본 환경 구성**: 기존에 정상 동작을 확인한 `test_node.yaml`의 기본 설정(ESP32 설정, USB 로거, PS-PWM 출력 핀 등)을 복사하여 시작합니다.
2. **C++ 헤더 삽입**: `esphome:` 블록 아래에 `includes: ["ps_pwm.h"]`를 추가하여, ESPHome 람다(Lambda) 내에서 C 드라이버 API(`pspwm_set_duty_soft`, `pspwm_set_frequency` 등)를 직접 호출할 수 있도록 만듭니다.
3. **하드웨어 Fault 설정**: `on_boot` 트리거를 추가하여 부팅 시 `pspwm_enable_hw_fault_shutdown()` 함수를 호출, GPIO 8번 핀을 결함 감지 핀으로 활성화합니다.
4. **전역 변수(Globals) 선언**: 람다 간에 상태를 공유할 수 있도록 `is_output_enabled`, `is_fault_active` 및 주기 카운터용 전역 변수를 선언합니다.
5. **버튼 제어 로직 (`on_press`)**: 
   - 만약 하드웨어 결함이 활성화된 상태라면 결함을 해제(`pspwm_clear_hw_fault_shutdown_occurred`)합니다.
   - 정상 상태라면 버튼을 누를 때마다 PWM 출력을 On/Off 토글하고, 켜질 경우 10초에 걸쳐 100%까지 도달하는 소프트 스타트(Soft-start)를 트리거합니다.
6. **5초 단위 상태 변경 (`interval` 컴포넌트)**:
   - 5초마다 실행되는 루프를 구성합니다.
   - 예제와 동일하게 다음 4가지 상태를 순환합니다:
     - **상태 0**: 100 kHz, 25% -> 75% 소프트 스타트, LED 초록색
     - **상태 1**: 100 kHz, 75% -> 25% 소프트 스타트, LED 노란색
     - **상태 2**: 200 kHz, 25% -> 75% 소프트 스타트, LED 파란색
     - **상태 3**: 200 kHz, 75% -> 25% 소프트 스타트, LED 보라색
7. **하드웨어 Fault 모니터링 (`interval` 컴포넌트)**:
   - 100ms 주기로 실행하며 `pspwm_get_hw_fault_shutdown_occurred()`를 검사합니다.
   - 결함이 감지되면 내부 상태를 락(Latch) 처리하고, PWM 출력을 끄고, LED가 붉은색으로 점멸하도록 설정합니다.

## 검증 계획
### 자동화 테스트 (빌드)
- `esphome compile ps_pwm_basic.yaml` 명령을 통해 C++ 람다 코드가 ESP-IDF 드라이버 함수들과 올바르게 연결(Link)되고 빌드되는지 확인합니다.

### 수동 검증 (동작 테스트)
- `esphome run ps_pwm_basic.yaml --device /dev/ttyACM0` 명령을 실행하여 펌웨어를 업로드합니다.
- 터미널 로그와 LED 색상 변화를 통해 5초 주파수 및 듀티 사이클 순환(Sweep)이 정상 작동하는지 확인합니다.
- BOOT 버튼을 통해 시스템 끄기/켜기가 토글되는지 테스트합니다.
- GPIO 8번 핀을 GND로 쇼트(Active Low일 경우)시켜 하드웨어 결함 동작(LED 붉은 점멸, 출력 강제 차단)이 정상적으로 트리거되는지 확인합니다.
