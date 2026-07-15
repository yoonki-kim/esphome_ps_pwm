# ESPHome ps_pwm 네이티브 컴포넌트 통합 구현 계획서

본 문서는 `esp32_ps_pwm` ESP-IDF 컴포넌트를 ESPHome의 외부 네이티브 컴포넌트(Native External Component)로 통합하기 위한 구현 계획입니다.

## 검토 및 미결 질문 (Open Questions)

1. **프레임워크 버전:** ESP-IDF v5.3.1 및 PlatformIO v6.9.0 환경이라고 명시해 주셨습니다. `test_node.yaml`에서는 기본적으로 `framework: type: esp-idf`를 사용하도록 구성할 예정이나, PlatformIO에서 사용하는 별도의 `platform_packages` 버전 매핑이 있다면 알려주세요.
2. **동적 제어 항목:** 현재로서는 `FloatOutput`을 통해 Phase Shift Duty(`ps_duty`)를 제어하도록 계획했습니다. 주파수나 데드타임을 런타임 중에 동적으로 변경하는 기능이 필요한가요? 아니면 YAML 상에서 한 번 고정 설정하는 것으로 충분한가요?
3. **추가 기능(옵션):** 기존 컴포넌트에서 지원하던 ZVS 하드웨어 타이머 추적(Tracking) 기능이나 안전 종료(HW Fault Shutdown) 기능도 ESPHome 레벨의 YAML 옵션으로 노출해야 할까요?

## 제안된 구조 및 구현 내용 (Proposed Changes)

---

### ESPHome Python 설정 (Configuration)

이 파이썬 스크립트들은 설정 스키마를 정의하고 C++ 보일러플레이트 코드를 생성합니다.

#### 1. `components/ps_pwm/__init__.py`
- `ps_pwm_ns` 네임스페이스를 등록합니다.
- 공통 의존성을 정의합니다 (필요한 경우).

#### 2. `components/ps_pwm/output.py`
- `output.FLOAT_OUTPUT_SCHEMA` 및 `cv.COMPONENT_SCHEMA`를 상속합니다.
- 필수 YAML 설정 키: `pin_lead_a`, `pin_lead_b`, `pin_lag_a`, `pin_lag_b`, `frequency`.
- 선택적 YAML 설정 키: `lead_red`, `lead_fed`, `lag_red`, `lag_fed` (기본값 제공).
- `ps_pwm::PsPwmWrapper`를 생성하고, 속성을 설정한 뒤 ESPHome의 출력 객체로 등록하는 C++ 코드를 생성합니다.

---

### ESPHome C++ 래퍼 (Wrapper)

C++ 래퍼는 ESPHome의 `Component` 및 `FloatOutput` 클래스를 ESP-IDF의 `pspwm_*` C API와 연결합니다.

#### 3. `components/ps_pwm/ps_pwm_wrapper.h`
- `esphome::Component` 및 `esphome::output::FloatOutput`를 상속하는 `PsPwmWrapper` 클래스를 정의합니다.
- 핀 번호, 주파수, 데드타임 설정을 위한 setter 메서드를 포함합니다.
- ESPHome의 핵심 메서드인 `setup()`, `dump_config()`, `write_state(float state)`를 오버라이드합니다.

#### 4. `components/ps_pwm/ps_pwm_wrapper.cpp`
- **`setup()`**: YAML에 설정된 핀 번호, 주파수, 데드타임을 사용하여 `pspwm_init(...)` API를 호출합니다.
- **`write_state(float state)`**: ESPHome에서 전달되는 0.0 ~ 1.0 상태값을 받아 `pspwm_set_ps_duty(...)`로 전달합니다.

---

### ESPHome 테스트 설정 파일

#### 5. `test_node.yaml`
- ESP-IDF 프레임워크를 사용하는 ESP32-S3 보드를 정의합니다.
- `external_components`를 사용하여 로컬의 `components/` 디렉토리를 참조하도록 합니다.
- `platform: ps_pwm`을 사용한 테스트용 `output:` 블록을 작성하여 컴파일 가능 여부를 검증합니다.

## 검증 계획 (Verification Plan)

### 1. 자동화된 컴파일 검증
- 작성된 `test_node.yaml`을 기반으로 ESPHome 컴파일러(`esphome compile test_node.yaml`)를 실행하여 파이썬 코드 생성기와 ESP-IDF 빌드 과정에서 오류가 없는지 확인합니다.

### 2. 수동 검증 (사용자 측면)
- 성공적으로 컴파일되면 사용자는 보드에 플래싱(`esphome run test_node.yaml`)하여 오실로스코프로 지정된 4개의 GPIO 핀을 측정할 수 있습니다.
- Home Assistant 연동 및 ESPHome Web UI를 통해 Duty Cycle 값(0.0 ~ 1.0)을 변경했을 때 정상적으로 Phase-Shift PWM 출력이 가변되는지 확인합니다.
