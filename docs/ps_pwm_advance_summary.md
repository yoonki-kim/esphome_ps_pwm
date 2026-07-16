# ps_pwm_advance 설정 및 동작 요약

`ps_pwm_advance.yaml`은 ESPHome의 웹 대시보드 및 UI 컴포넌트를 적극 활용하여, 사용자가 **실시간으로 PS-PWM의 주요 파라미터를 세밀하게 튜닝하고 제어할 수 있는 고급 환경**을 제공합니다.

## 1. 주요 기능 및 제어 인터페이스

이 예제는 코드로 고정된 시나리오가 아니라, ESPHome의 UI 엔티티(`switch`, `number`)를 통해 동적 제어가 가능하도록 설계되었습니다.

### 사용자 인터페이스 (UI Components)
* **PWM Output (Switch)**: 
  * 웹 대시보드나 앱에서 PWM의 출력을 활성화(Enable)하거나 비활성화(Disable)할 수 있는 스위치입니다.
  * 켤 때 사용자가 설정해둔 듀티(Duty)와 소프트스타트(Soft-Start) 시간이 적용되어 부드럽게 출력을 시작합니다.
* **Target Frequency (Number)**: 
  * 10kHz부터 최대 300kHz까지 1kHz 단위로 주파수를 실시간 변경할 수 있습니다. 
  * 변경 즉시 타이머 주기가 재계산되어 하드웨어에 반영됩니다.
* **Target Duty Cycle (Number)**: 
  * 0%부터 100%까지 1% 단위로 위상 천이(Phase-Shift) 듀티비를 조절합니다. 
  * 변경 시 곧바로 끊기는 것이 아니라 설정된 `Soft Start Time`에 맞춰 부드럽게 목표치로 이동합니다.
* **Soft Start Time (Number)**: 
  * 듀티가 변경되거나 출력이 켜질 때 목표 듀티까지 도달하는 데 걸리는 시간(0 ~ 10,000ms)을 설정합니다. 전력 전자 회로에서 돌입 전류(Inrush Current)를 방지하는 핵심 파라미터입니다.

### 상태 피드백 (LED 시각화)
온보드 WS2812 RGB LED(GPIO 48)를 활용하여 시스템 상태를 표시합니다.
* **출력 켜짐**: 백색(White)으로 점등되며, 밝기는 현재 설정된 듀티비(Duty Cycle)에 비례하여 변합니다.
* **출력 꺼짐**: LED 소등.
* **결함 발생**: 스위치가 강제로 OFF되며, 적색(Red) 최대 밝기로 켜집니다.

### 물리 제어 (BOOT Button / GPIO 0)
웹 대시보드에 접속하지 않은 상태에서도 기기 자체의 BOOT 버튼을 눌러 `PWM Output` 스위치를 물리적으로 켜고 끌 수 있습니다. 결함이 발생했을 때는 버튼이 결함 해제(Fault Clear) 버튼으로 동작합니다.

## 2. 하드웨어 연결 및 기본 설정
* **LEAD 핀**: A(High-side)=GPIO 5, B(Low-side)=GPIO 4
* **LAG 핀**: A(High-side)=GPIO 7, B(Low-side)=GPIO 6
* **Fault 입력**: GPIO 8 (Active LOW)
* 초기 부팅 시 안전을 최우선으로 하여 모든 출력은 **꺼진 상태(Disabled)**로 시작됩니다.

## 3. 활용 방법
1. 코드를 빌드하고 업로드한 뒤, 할당된 IP 주소나 ESPHome 대시보드를 통해 기기의 UI에 접속합니다.
2. `PWM Output` 스위치를 켜고, UI 슬라이더를 조작하여 `Target Frequency`와 `Target Duty Cycle`을 조절하면서 오실로스코프로 실제 파형의 변화를 확인합니다.
3. `Soft Start Time`을 길게(예: 5000ms) 설정한 후 듀티비를 극단적으로 변경해 보며, 파형이 단계적으로 천천히 넓어지고 좁아지는 과도 응답 특성을 관찰합니다.
4. 풀브릿지 인버터의 최적 주파수나 위상각을 찾기 위한 튜닝 및 디버깅 툴로 활용할 수 있습니다.
