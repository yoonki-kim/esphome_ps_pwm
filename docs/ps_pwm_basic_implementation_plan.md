# 웹 서버(v3) 및 Wi-Fi 통합 구현 계획서

사용자의 요청에 따라 `ps_pwm_basic.yaml`에 최신 웹 서버(버전 3) 기능을 추가합니다. 웹 서버를 구동하기 위해서는 기기가 네트워크에 연결되어 있어야 하므로, Wi-Fi 설정과 ESPHome 대시보드 무선 관리를 위한 필수 컴포넌트들을 함께 추가해야 합니다.

## 논의 사항 (Open Questions)
- Wi-Fi 비밀번호 등 민감한 정보는 `secrets.yaml` 파일로 분리하여 관리하는 것이 원칙(보안 및 Git 노출 방지)입니다. 임시로 `secrets.yaml`을 생성하여 더미(Dummy) 값을 넣어두겠습니다. 이후 직접 실제 Wi-Fi 정보로 수정해 주시면 되는데, 이 방식이 괜찮으신가요?

## 제안하는 변경 사항

### [MODIFY] [ps_pwm_basic.yaml](file:///home/yoonki/esphome/esphome_ps_pwm/ps_pwm_basic.yaml)
다음 컴포넌트들을 파일 상단(또는 적절한 위치)에 추가합니다:
1. **`wifi:`**: `secrets.yaml`을 참조하도록 설정.
2. **`captive_portal:`**: Wi-Fi 접속 실패 시 기기 자체에서 임시 AP(공유기)를 열어 웹으로 Wi-Fi 설정을 잡을 수 있도록 하는 백폴(Fallback) 기능.
3. **`api:` 및 `ota:`**: 조금 전 활성화하신 ESPHome 대시보드에서 기기의 온라인 상태를 모니터링하고 무선으로 펌웨어를 업데이트(OTA)하기 위한 필수 컴포넌트.
4. **`web_server:`**: 요청하신 최신 웹 UI를 위해 `version: 3`으로 명시하여 추가.

### [NEW] [secrets.yaml](file:///home/yoonki/esphome/esphome_ps_pwm/secrets.yaml)
- `wifi_ssid`와 `wifi_password` 항목을 포함하는 기본 뼈대 파일을 생성합니다.
- *참고: 이 파일은 이미 생성해 둔 `.gitignore` 규칙에 의해 기본적으로 Git에서 무시되어야 하지만, 혹시 모르니 `.gitignore`에 `secrets.yaml`이 명시되어 있는지 확인하고 필요 시 추가하겠습니다.*

## 검증 계획
1. `secrets.yaml` 생성 후, `ps_pwm_basic.yaml`에 코드를 병합.
2. `esphome compile ps_pwm_basic.yaml`을 실행하여 모든 컴포넌트가 충돌 없이 정상적으로 빌드되는지 확인.
3. 빌드 성공 시, 직접 `secrets.yaml`의 Wi-Fi 정보를 수정하시고 `esphome run`으로 업로드하여 웹 브라우저로 기기의 IP에 접속(웹 서버 v3 화면 확인)해 볼 수 있도록 안내해 드립니다.
