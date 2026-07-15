# ESPHome ps_pwm 네이티브 컴포넌트 통합 구현 계획서

본 문서는 `esp32_ps_pwm` ESP-IDF 컴포넌트를 ESPHome의 외부 네이티브 컴포넌트(Native External Component)로 통합하기 위한 구현 계획입니다.

## 검토 및 결정 사항 (Decisions Made)

1. **컴포넌트 연동 방식:** `external_components` 방식(Python + C++ Wrapper)을 최종 채택하여 유지보수성 및 확장성을 확보했습니다.
2. **테스트 환경:** 터미널 환경에서의 원활한 ESPHome 컴파일을 위해 **Ubuntu OS** 기반에서 빌드를 진행합니다.
3. **프레임워크 및 메모리:** ESP-IDF v5.5.4, ESP32-S3 DevKit(N16R8, 16MB Flash / 8MB PSRAM)을 기준으로 설정이 완료되었습니다.

## 제안된 구조 및 구현 내용 (Implemented Changes)

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
