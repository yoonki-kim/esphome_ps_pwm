# `ps_pwm_basic` ESPHome 구현 완료 가이드

기존 `esp32_ps_pwm` 라이브러리의 C 예제인 `ps_pwm_basic` 기능을 완벽하게 ESPHome 방식(YAML + C++ 람다)으로 이식했습니다.

## 주요 구현 내용

1. **이벤트 기반 아키텍처 도입**
   - 무한 루프(`while(1)`) 폴링(Polling) 방식을 제거하고, ESPHome의 자체 인터럽트 기반 이벤트(`on_press`, `interval`)로 구조를 완전히 개편했습니다. 
   - CPU 자원 소모를 줄이면서도 즉각적이고 안정적인 반응을 보장합니다.

2. **하드웨어 결함(Hardware Fault) 모니터링 적용 (GPIO 8)**
   - 예제의 의도대로 OCP_PULSE (Over-Current Protection) 핀인 GPIO 8번을 하드웨어 결함 감지 핀으로 활성화(`pspwm_enable_hw_fault_shutdown`)했습니다.
   - 100ms 주기로 동작하는 백그라운드 모니터(`interval`)가 결함을 감지하면 즉시 내부 상태를 차단 모드(Latch)로 잠그고, RGB LED를 붉은색으로 점멸시킵니다.

3. **버튼 제어 (GPIO 0)**
   - `on_press` 이벤트를 통해 결함(Fault) 래치를 초기화하거나, 정상 상태일 때는 PWM 출력을 켜고 끕니다(Soft-start 기능 연동).

4. **자동 순환(Sweep) 로직 (5초 단위)**
   - `interval`을 이용해 매 5초마다 주파수(100kHz/200kHz)와 듀티 사이클(25% -> 75% -> 25%)을 순환하며 소프트 스타트를 수행하도록 구성했습니다.
   - 각 상태마다 LED 색상이 Green -> Yellow -> Blue -> Purple 로 변하며 터미널에 로그를 남깁니다.

## 테스트 및 동작 확인 방법

터미널에서 이전에 실행 중이던 커맨드를 종료(`Ctrl + C`)한 후, 아래 명령어로 새 펌웨어를 업로드하세요.

```bash
esphome run ps_pwm_basic.yaml --device /dev/ttyACM0
```

> [!TIP]
> 펌웨어가 구동된 후:
> - **평상시**: 5초마다 LED 색상이 바뀌며 터미널 로그에 듀티와 주파수 변경 내역이 표시됩니다.
> - **버튼 테스트**: 보드의 BOOT 버튼을 눌러 출력이 정상적으로 정지/재개 되는지 확인하세요.
> - **Fault 테스트**: 점퍼선 등을 이용해 **GPIO 8번 핀을 GND로 짧게 쇼트**시켜 보세요. 하드웨어 결함이 트리거되며 즉시 붉은 LED가 점멸하는지 테스트할 수 있습니다. (리셋은 다시 BOOT 버튼 클릭)
