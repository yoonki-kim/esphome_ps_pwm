# Ubuntu 환경에서의 실행 가이드

이 프로젝트를 새로운 Ubuntu 터미널 환경에서 시작하고 빌드하기 위한 명령어 모음입니다.

## 1. 프로젝트 클론 (Clone)
```bash
# esphome-2026.5 브랜치로 프로젝트를 복제합니다.
git clone -b esphome-2026.5 git@github.com:yoonki-kim/esphome_ps_pwm.git

# 프로젝트 폴더로 이동합니다.
cd esphome_ps_pwm

# 서브모듈(esp32_ps_pwm) 소스 코드를 다운로드합니다.
git submodule update --init --recursive
```

## 2. ESPHome 빌드 및 테스트
```bash
# test_node.yaml을 컴파일하여 오류가 없는지 확인합니다.
esphome compile test_node.yaml

# 보드가 연결되어 있다면 바로 플래싱(업로드)할 수 있습니다.
esphome run test_node.yaml
```
