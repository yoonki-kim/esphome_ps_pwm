# Next Steps & Future Tasks

## 1. 하위 라이브러리의 ZVS Resonant Tracking 예제를 ESPHome으로 포팅 (Porting)
- **목표**: `esp32_ps_pwm` 하위 라이브러리의 `examples` 폴더 등에 이미 구현되어 있는 `zvs_resonant_tracking` 예제 코드를 ESPHome 환경으로 포팅(Porting)하여 통합한다.
- **내용**: 
  - ESP-IDF 기반의 기존 C/C++ 예제 코드를 분석하여 ESPHome의 `external_components` (또는 `lambda`) 구조에 맞게 래핑(Wrapping) 및 이식.
  - 외부 피드백 신호(예: 공진 탱크 전류의 영교차점)와 구동 신호 간의 위상 지연(Phase delay)을 실시간으로 추적하는 기존 제어 로직을 ESPHome 생태계 내에서 동일하게 구동.

## 2. ESPHome 대시보드 기반 시뮬레이션 환경 구축
- **목표**: 포팅된 ZVS 공진 추적 기능의 동작 상태와 파라미터를 기존 터미널 CLI 명령어가 아닌, 사용자가 직관적으로 조작할 수 있는 **ESPHome 웹 대시보드(UI)**를 통해 제어하고 시뮬레이션하는 프로그램을 개발한다.
- **내용**:
  - `ps_pwm_advance.yaml`에서 사용된 템플릿 컴포넌트(`number`, `switch`, `sensor`) 패턴을 확장 적용.
  - **제어 항목**:
    - Tracking Enable/Disable 스위치
    - Tracking PI Controller의 Kp, Ki 파라미터 튜닝 슬라이더
    - 목표 위상 여유(Target Phase Margin) 설정
  - **모니터링 항목**:
    - 실시간 측정된 공진 주파수(Hz) 및 위상차(ns 또는 degree) 출력 (Sensor).
    - 추적 락(Lock) 달성 여부 표시 (Binary Sensor / Text Sensor).
  - **기대 효과**: 시리얼 터미널 창에 명령어를 타이핑할 필요 없이, 스마트폰이나 PC의 웹 대시보드에서 바로 슬라이더를 움직이며 공진 제어기를 튜닝할 수 있는 대화형 시뮬레이션 및 테스트 환경 완성.
