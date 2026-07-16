# ESPHome PS-PWM 사용 가이드

이 문서는 `esphome_ps_pwm` 프로젝트를 처음 설정하고 빌드, 실행, 모니터링하기 위한 종합 사용 가이드입니다. (Ubuntu 및 일반 터미널 환경 공통)

## 1. 프로젝트 클론 및 서브모듈 초기화
```bash
# 프로젝트를 복제합니다. (해당 브랜치 기준)
git clone -b esphome-2026.5 git@github.com:yoonki-kim/esphome_ps_pwm.git

# 프로젝트 폴더로 이동합니다.
cd esphome_ps_pwm

# 하위 하드웨어 제어 라이브러리(서브모듈) 소스를 다운로드합니다.
git submodule update --init --recursive
```

## 2. Python 가상 환경(venv) 설정 및 활성화
ESPHome과 필요한 파이썬 패키지들을 시스템 전체에 설치하는 대신, 이 프로젝트만을 위한 독립적인 가상 환경을 구성하는 것이 안전하고 좋습니다.

```bash
# 프로젝트 폴더 내에 'venv'라는 이름의 파이썬 가상 환경 생성
python3 -m venv venv

# 가상 환경 활성화 (성공하면 터미널 프롬프트 앞에 (venv)가 표시됩니다)
source venv/bin/activate

# 활성화된 가상 환경에 ESPHome 패키지 설치 (처음 1회만 수행)
pip install esphome
```
> **참고**: 터미널을 새로 열거나 재부팅했을 경우, ESPHome 명령어를 사용하기 전에 항상 프로젝트 폴더에서 `source venv/bin/activate` 명령어를 먼저 실행하여 가상 환경을 켜주어야 합니다. 작업을 마치고 가상 환경을 종료하려면 `deactivate`를 입력하세요.

## 3. ESPHome 명령어 가이드 (컴파일, 업로드, 디버깅)

### 컴파일 (Compile)
설정 파일(`.yaml`)에 문법적 오류가 없는지 확인하고 펌웨어를 빌드만 할 때 사용합니다.
```bash
esphome compile ps_pwm_basic.yaml
```

### 업로드 및 디버깅 (Run & Logs)
컴파일 후 장치에 펌웨어를 업로드하고, 즉시 로그를 확인(디버깅)합니다. USB나 Wi-Fi(OTA)가 연결되어 있어야 합니다.
```bash
# 기본 실행 (연결된 포트를 자동 감지하여 업로드 후 로그 출력)
esphome run ps_pwm_basic.yaml

# 특정 포트 지정 (예: USB 연결 시 /dev/ttyACM0)
esphome run ps_pwm_basic.yaml --device /dev/ttyACM0

# 로그만 확인할 때 (업로드는 생략하고 디버깅만 할 때)
esphome logs ps_pwm_basic.yaml
```

## 4. ESPHome 대시보드 (Web UI) 실행
ESPHome 대시보드는 로컬 환경에서 노드들을 한 눈에 모니터링하고 웹 브라우저 상에서 컴파일 및 무선 업데이트(OTA)를 시각적으로 편하게 진행할 수 있는 강력한 도구입니다.

```bash
# 현재 디렉토리(프로젝트 폴더)를 기준으로 대시보드를 실행합니다.
esphome dashboard .
```

* 실행 후 터미널에 표시되는 URL (보통 `http://localhost:6052` 또는 `http://0.0.0.0:6052`) 주소로 웹 브라우저를 통해 접속합니다.
* 대시보드 화면에서 기기의 온라인 상태 확인, 로그 뷰어(LOGS), 무선 펌웨어 업데이트(INSTALL) 등의 기능을 직관적으로 사용할 수 있습니다.
