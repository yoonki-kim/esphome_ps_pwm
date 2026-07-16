# ps_pwm_basic 설정 및 동작 요약

`ps_pwm_basic.yaml`은 ESP32-S3 기반의 Phase-Shift PWM(PS-PWM) 기능을 테스트하기 위해, 사용자의 개입 없이 자동으로 주파수와 듀티비를 변경하며 동작을 검증하는 **자동화 시연(Demonstration) 예제**입니다.

## 1. 주요 기능 및 동작 원리

이 파일은 크게 **자동 스윕(Sweep) 로직**, **하드웨어 결함 보호 로직**, 그리고 **물리적 제어 버튼**으로 구성되어 있습니다.

### 자동 상태 순환 (Auto Sweep Logic)
5초마다(5s Interval) 지정된 4가지 상태를 순환하며 PWM 출력을 제어합니다. 각 상태로 넘어갈 때 `pspwm_set_duty_soft` 함수를 호출하여 2초 동안 부드럽게 듀티를 변경(Soft-start)합니다. 상태 변화는 온보드 RGB LED의 색상으로 시각적으로 확인할 수 있습니다.
* **State 0 (GREEN)**: 주파수 100kHz 설정. 듀티 25% ➔ 75% 부드럽게 상승
* **State 1 (YELLOW)**: 주파수 100kHz 유지. 듀티 75% ➔ 25% 부드럽게 하강
* **State 2 (BLUE)**: 주파수 200kHz 설정. 듀티 25% ➔ 75% 부드럽게 상승
* **State 3 (PURPLE)**: 주파수 200kHz 유지. 듀티 75% ➔ 25% 부드럽게 하강

### 하드웨어 결함 보호 (Hardware Fault Latch)
100ms 주기로 `pspwm_get_hw_fault_shutdown_occurred` 상태를 검사합니다.
* GPIO 8 핀에 하드웨어 결함(LOW 신호)이 감지되면 즉시 PWM 출력이 안전하게 차단(Latched LOW)됩니다.
* 결함 상태에 돌입하면 온보드 LED가 **적색(RED)으로 깜빡이며** 경고를 표시하고, 자동 스윕 로직이 일시 정지됩니다.

### 물리 제어 (BOOT Button / GPIO 0)
* **결함 상태일 때**: 버튼을 누르면 하드웨어 결함이 해제되었는지 확인 후, 락(Latch)을 풀고 초기 비활성화 상태로 복귀합니다.
* **정상 상태일 때**: 버튼을 누르면 전체 PWM 출력을 켜거나 끕니다(Toggle). 켤 때는 10초에 걸쳐 천천히 100%까지 올라가는 소프트 스타트가 발동됩니다.

## 2. 하드웨어 연결 정보
* **LEAD 핀**: A(High-side)=GPIO 5, B(Low-side)=GPIO 4
* **LAG 핀**: A(High-side)=GPIO 7, B(Low-side)=GPIO 6
* **Fault 입력**: GPIO 8 (Active LOW)
* **제어 버튼**: GPIO 0 (BOOT Button)
* **상태 표시**: GPIO 48 (WS2812 RGB LED)

## 3. 네트워크 및 ESPHome 기본 기능
* **Web Server v3**: 브라우저를 통해 기기를 모니터링할 수 있는 로컬 웹 UI를 제공합니다.
* **Wi-Fi & Fallback AP**: `secrets.yaml`을 참조하여 Wi-Fi에 접속하며, 실패 시 `PS-PWM-Basic Fallback Hotspot` 이라는 이름의 복구용 핫스팟을 생성합니다.
* **OTA & API**: ESPHome 대시보드와 무선으로 통신하여 로그를 모니터링하거나 무선 펌웨어 업데이트를 진행할 수 있습니다.

## 4. 활용 방법
1. 코드를 빌드하여 기기에 올린 후, 터미널 로그나 온보드 LED의 색상을 관찰합니다.
2. LED 색상이 일정 주기마다 바뀌면서 주파수와 듀티가 정상적으로 스윕되는지 오실로스코프로 파형을 찍어 검증합니다.
3. GPIO 8번을 GND와 잠시 쇼트시켜, 붉은색 점멸과 함께 즉각적인 하드웨어 차단이 이루어지는지 안전 기능을 테스트합니다.
