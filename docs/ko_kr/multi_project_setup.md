# 여러 프로젝트 구성하기
**Languages**: [English](../multi_project_setup.md)

여러 프로젝트를 [프로젝트 구성하기](project_setup.md)에서 설명한 것과 같은 방식을 사용할 경우 여러 CRSF 가 필요하며, 용량도 많이 필요로 합니다.

만일, **같은 버전의 CRSF** 를 사용할 경우에는 하나의 CRSF SDK 폴더에서 여러 프로젝트를 동시에 구성할 수 있습니다.

### 폴더 구조
```
─ CRSF-Projects
  ├ build                   # CMake 빌드 폴더
  ├ crsf
  │ ├ ...
  │ └ .crsf-root
  ├ project 1
  ├ project 2
  ├ ...
  ├ project N
  ├ vcpkg                   # Visual Studio 2017 사용 시
  └ CMakeLists.txt
```

폴더 구조는 위와 같으며, 그 외의 구성 단계는 [프로젝트 구성하기](project_setup.md)와 동일합니다.

그리고 `CMakeLists.txt` 파일에는 사용하고자 하는 프로젝트의 이름들을 아래와 같이 추가하면 됩니다.
```cmake
cmake_minimum_required(VERSION 3.13)
project(CRSF-Projects LANGUAGES CXX)

# === configure ====================================================================================
set(CRSF_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/crsf" CACHE PATH "Directory Path for CRSF SDK.")

if(NOT EXISTS ${CRSF_SDK_DIR})
    message(FATAL_ERROR "'crsf' directory does NOT exist!\nYou can download from http://crseed.org")
endif()

# list projects
add_subdirectory("project 1")
add_subdirectory("project 2")
# ...
add_subdirectory("project N")
# ==================================================================================================
```
