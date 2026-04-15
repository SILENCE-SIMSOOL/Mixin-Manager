# 📋 Mixin Manager (for Fabric/Forge)

This is an automation tool designed to eliminate the tedious task of manually registering Mixin classes in your `mixins.json` during Minecraft mod development. It scans `.java` files in a specified directory and automatically updates the designated section (`client` or `mixins`) in your JSON file.

마인크래프트 모드 개발 시 `mixins.json` 파일에 믹스인 클래스들을 일일이 수동으로 등록해야 하는 번거로움을 해결하기 위한 자동화 도구입니다. 지정된 폴더 내의 `.java` 파일을 탐색하여 JSON 파일의 특정 섹션(`client` 또는 `mixins`)을 자동으로 갱신합니다.

---

## 🛠️ Usage (설치 및 사용법)

### 1. 컴파일 (Compile)
Build the source code using GCC.
GCC를 사용하여 소스 코드를 빌드합니다.
```bash
gcc main.c -o MixinManager-1.0.0.exe
```

### 2. Initial Setu (초기 설정)
Running the program for the first time will create `mixinmanager/config.json`.
프로그램을 처음 실행하면 `mixinmanager/config.json` 파일이 생성됩니다.
```bash
./MixinManager-1.0.0.exe
```

### 3. Configuration (설정 수정)
Edit `config.json` to match your project paths.
`config.json`을 열어 프로젝트 경로에 맞게 수정합니다.

```json
{
	"mixin_dir": "src/main/java/silence/simsool/project/mixin",
	"mixin_package": "silence.simsool.project.mixin",
	"json_file": "src/main/resources/project.mixins.json",
	"default_target": "client"
}
```

* `mixin_dir`: Path to Mixin files (믹스인 파일이 있는 폴더 경로)
* `mixin_package`: Base package name (믹스인 기준 패키지명)
* `json_file`: Target mixins.json path (업데이트할 믹스인 JSON 파일 경로)
* `default_target`: `mixins` or `client` or `server`

### 4. 실행 (Run)
After configuration, run it again to update your JSON file automatically.
설정 완료 후 다시 실행하면 JSON 파일이 자동으로 업데이트됩니다.

---

## 📝 License (라이선스)
이 프로젝트는 자유롭게 수정 및 배포가 가능합니다.
This project is free to modify and distribute.
