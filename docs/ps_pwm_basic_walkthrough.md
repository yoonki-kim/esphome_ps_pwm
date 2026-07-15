# 웹 서버(v3) 및 Wi-Fi 통합 가이드

`ps_pwm_basic.yaml`에 ESPHome의 최신 **웹 서버(버전 3)** 기능과 대시보드 무선 통신을 위한 네트워크 기반 설정들을 모두 추가했습니다.

## 추가된 주요 기능

1. **Web Server v3 (`web_server: version: 3`)**
   - 브라우저를 통해 기기를 제어하고 실시간 상태를 볼 수 있는 최신 버전의 자체 웹 UI를 제공합니다.
   
2. **Wi-Fi 연결 및 분리된 보안 설정 (`wifi:`)**
   - 인터넷 연결을 위해 Wi-Fi 컴포넌트를 추가했습니다.
   - 보안을 위해 아이디와 비밀번호를 `secrets.yaml`로 분리하였으며, 해당 파일은 `.gitignore`에 등록되어 Git 추적에서 제외되도록 안전하게 구성했습니다.

3. **Fallback AP (`captive_portal:`)**
   - 만약 입력해둔 Wi-Fi 공유기가 고장나거나 접속할 수 없을 경우, ESP32가 스스로 'PS-PWM-Basic Fallback Hotspot' 이라는 이름의 임시 Wi-Fi를 엽니다. 이 임시 Wi-Fi에 연결하면 브라우저를 통해 새로운 Wi-Fi 정보를 다시 잡아줄 수 있습니다.

4. **ESPHome 대시보드 원격 관리 (`api:`, `ota:`)**
   - 이제 띄워두신 ESPHome 대시보드 웹 화면에서 기기가 온라인(초록색)으로 나타나며, 더 이상 USB를 연결하지 않아도 무선으로 로그를 보거나 코드를 업데이트(OTA)할 수 있게 되었습니다.

## 향후 진행 순서 (테스트 방법)

1. **`secrets.yaml` 수정**
   - 프로젝트 폴더 내에 생성된 `secrets.yaml` 파일을 여시고, 본인이 사용 중인 실제 Wi-Fi `ssid`와 `password`로 값을 변경 후 저장해 주세요.

2. **최초 펌웨어 업로드 (USB)**
   - 대시보드에서 `INSTALL` 버튼을 누르거나, 터미널에서 기존처럼 펌웨어를 올립니다.
   - `esphome run ps_pwm_basic.yaml --device /dev/ttyACM0`

3. **웹 서버 v3 접속**
   - 터미널(또는 대시보드 로그)에 "IPv4 address: 192.168.x.x" 와 같이 할당받은 IP 주소가 표시됩니다.
   - 브라우저에서 해당 `http://할당받은IP주소` 로 접속하시면 최신 웹 인터페이스를 만나보실 수 있습니다!
