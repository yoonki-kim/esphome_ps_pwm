# ESPHome PS-PWM Project Context

이 프로젝트는 ESP32 및 ESP32-S3 기기를 위한 Phase-Shift PWM(PS-PWM) 기능을 제공하는 **ESPHome 커스텀 컴포넌트**입니다. 앞으로 AI 에이전트가 이 프로젝트에 기여할 때는 아래의 맥락과 가이드라인을 반드시 숙지하고 따라야 합니다.

## 1. 아키텍처 및 폴더 구조
* **ESPHome 커스텀 컴포넌트 래퍼 (`components/ps_pwm/`)**
  * `output.py`: YAML 설정을 파싱하고 C++ 코드를 생성하는 Python 스크립트. (`esphome.codegen`, `esphome.config_validation` 활용)
  * `ps_pwm_wrapper.cpp` & `ps_pwm_wrapper.h`: ESPHome의 `Component` 및 `FloatOutput` 인터페이스를 상속받아 하위 C 드라이버와 통신하는 C++ 래퍼 클래스.
* **ESP-IDF 핵심 드라이버 (`components/ps_pwm/esp32_ps_pwm/`)**
  * 실제 하드웨어 제어를 담당하는 C 기반 라이브러리. 
  * ESP-IDF v5.3.4 환경에 맞춰 최신 `driver/mcpwm_prelude.h` API를 사용.
  * Git 서브모듈로 관리될 수 있으며, 독립적인 ESP-IDF 컴포넌트로도 동작해야 함.

## 2. 개발 및 코딩 가이드라인
* **언어 및 스타일**: 
  * 래퍼 계층은 ESPHome 스타일의 C++17 및 Python 3 규칙을 따릅니다.
  * 하위 계층 드라이버는 Espressif의 공식 C 스타일 가이드를 따릅니다.
* **하드웨어 호환성**: 모든 변경 사항은 `ESP32` 및 `ESP32-S3` 칩 모두에서 호환되는지 확인해야 합니다. (MCPWM 주변장치 기능 차이 고려)
* **테스트 및 검증**: 새로운 기능이 추가될 경우 프로젝트 루트의 예제 YAML 파일(`ps_pwm_basic.yaml` 등)을 업데이트하여 테스트 가능하도록 유지해야 합니다.
* **안전 우선**: 전력 전자 기기(풀브릿지 등) 제어를 목적으로 하므로, 데드타임(Deadtime), 하드웨어 락(Fault Latch), 소프트 스타트(Soft-start) 등의 핵심 안전 로직을 우회하거나 해치는 변경을 해서는 안 됩니다.

## 3. 언어 규칙 (Language)
- 프로젝트와 관련된 모든 대화, 정보 제공, 문서 작성 및 코드 주석(필요 시)은 **한국어(Korean)**를 기본으로 사용합니다.
- 기술 용어의 경우, 이해를 돕기 위해 영문을 병기할 수 있습니다 (예: Phase-Shift PWM (위상 천이 PWM)).

## 4. 문서화 규칙 (Documentation)
- 프로젝트 진행 중 생성되는 모든 설계 문서, 구현 계획서(Implementation Plan), 개발 가이드 등은 프로젝트 루트의 `docs/` 디렉토리에 마크다운(`.md`) 파일로 저장합니다.
- 이를 통해 개발 이력을 추적하고 향후 유지보수 시 참고할 수 있도록 합니다.
