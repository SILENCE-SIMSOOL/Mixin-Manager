# 📋 Mixin Manager (for Fabric/Forge)
This is an automation tool designed to eliminate the tedious task of manually registering Mixin classes in your `mixins.json` during Minecraft mod development. It scans `.java` files in a specified directory and automatically updates the designated section (`client` or `mixins`) in your JSON file.

마인크래프트 모드 개발 시 `mixins.json` 파일에 믹스인 클래스들을 일일이 수동으로 등록해야 하는 번거로움을 해결하기 위한 자동화 도구입니다. 지정된 폴더 내의 `.java` 파일을 탐색하여 JSON 파일의 특정 섹션(`client` 또는 `mixins`)을 자동으로 갱신합니다.

## 🛠️ Usage (설치 및 사용법)
1. 컴파일 (Compile)
GCC를 사용하여 소스 코드를 빌드합니다.
Build the source code using GCC.
```bash
gcc main.c -o mixin_updater.exe
```
