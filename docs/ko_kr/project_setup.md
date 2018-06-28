# 프로젝트 구성하기
**Languages**: [English](../project_setup.md)

CRSF 에서는 CMake 를 사용해서 프로젝트를 구성합니다.

빈 프로젝트는 [crsf-empty 프로젝트](https://github.com/chic-yukim/crsf-empty)에서 찾을 수 있습니다.
프로젝트를 다운로드 받아서 사용하거나, 동일한 구조를 갖도록 프로젝트 폴더를 구성하면 됩니다.



## 빈 프로젝트에서 시작하기
1. [http://crseed.org](http://crseed.org) 에서 CRSF 다운로드합니다.
2. "crsf" 폴더에 압축 파일 해제합니다.

### 폴더 구조
```
─ PROJECT_DIR
  ├ build                   # CMake 빌드 폴더
  ├ crsf
  │ ├ ...
  │ └ .crsf-root
  ├ empty-module
  ├ empty-project
  ├ vcpkg                   # Visual Studio 2017 사용 시
  └ CMakeLists.txt
```

### Visual Studio 2013
3. CMake 를 사용하여 아래와 같이 설정합니다.
   1. Where is the source code: `PROJECT_DIR` 경로
   2. Where to build the binaries: `PROJECT_DIR/build` 경로
   3. Configure 버튼을 눌러서 CMake 프로젝트를 구성합니다.
      - "Use default native compilers" 를 선택하고 Finish 하면 됩니다.
   4. Generate 버튼을 눌러서 Visual Studio 프로젝트를 생성합니다.

4. 빌드 및 프로젝트를 실행합니다.
   - 프로젝트 Configuration 설정에서 RelWithDebInfo 나 Release 를 선택합니다.
   - 특정 프로젝트를 시작(Start Debugging)하려면, 해당 프로젝트를 `Set as StartUp Project` 로 설정하면 됩니다.

### Visual Studio 2017
3. `bootstrap.bat` 파일을 실행합니다. (**이 작업은 시스템 사양에 따라서 20분 이상 소요됩니다.**)
   - 이 배치 파일은 [vcpkg](https://github.com/Microsoft/vcpkg) 프로젝트를 clone 합니다.
   - 그런 다음 vcpkg 를 사용해서 외부 라이브러리들을 빌드 및 설치합니다.

4. CMake 를 사용하여 아래와 같이 설정합니다.
   1. Where is the source code: `PROJECT_DIR` 경로
   2. Where to build the binaries: `PROJECT_DIR/build` 경로
   3. **Specify toolchain file for cross-compiling** 을 선택합니다.
   4. `vcpkg/scripts/buildsystems/vcpkg.cmake` 에 있는 vcpkg toolchain 파일을 지정합니다.
   5. Finish

5. 빌드 및 프로젝트를 실행합니다.
   - 프로젝트 Configuration 설정에서 Debug, RelWithDebInfo 및 Release 를 선택합니다.
   - 특정 프로젝트를 시작(Start Debugging)하려면, 해당 프로젝트를 `Set as StartUp Project` 로 설정하면 됩니다.



## CRSF 예제 실행하기
CMakeLists.txt 파일에서 `add_subdirectory("crsf/examples")` 주석을 해제하면 됩니다.



## 새 프로젝트 생성하기
새 프로젝트는 빈 프로젝트나 예제로부터 복사해서 만들 수 있습니다.
그리고 아래 내용에 따라 설정을 하면 됩니다.

### 폴더
프로젝트 폴더를 새로 만들었다면, `CMakeLists.txt` 파일에 `add_subdirectory` 명령을 추가해주면 됩니다.

### 프로젝트 이름(모듈 ID)
1. `your-project/CMakeLists.txt` 파일에 있는 `CRMODULE_ID` 값을 수정합니다.
2. `your-project/config.json` 파일의 내용을 수정합니다.
3. `your-project/config/SystemConfiguration.xml` 파일에 있는 `<id>` 태그 값을 **모듈 ID** 로 수정합니다.
   - 이를 하지 않으면, CRSF 어플리케이션에서 해당 모듈을 몾 찾았다는 에러 메시지가 나옵니다.
   - 예시:
   ```xml
   <dynamic_modules>
     <module>
         ...
     </module>
     <application>
         <id>your-project</id>
     </application>
   </dynamic_modules>
   ```

4. `your-project/config/DynamicModuleConfiguration.xml` 파일에 모듈에 대한 태그가 존재한다면, 해당 태그도 변경합니다.
