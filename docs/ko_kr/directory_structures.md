# CRSF 폴더 구조
**Languages**: [English](../directory_structures.md)

```
─ CRSF
  ├ bin                     # DLL 및 EXE 파일
  │ ├ CRLauncher.exe
  │ └ CRSeedLib.dll
  ├ cmake                   # cmake 파일들
  ├ crmodules               # CRSF 내장 모듈들
  ├ examples                # 예시 프로젝트들
  │ ├ 1-Graphics                # 그래픽스 예시
  │ ├ 2-Interface               # 인터페이스 예시
  │ ├ 4-Network                 # 네트워크 예시
  │ ├ 5-Physics                 # 물리 예시
  │ ├ 6-Hand                    # 손 동작 예시
  │ └ 7-Applications            # 튜토리얼 프로젝트 예시
  └ thirdparty              # 외부 라이브러리
    ├ boost
    ├ bullet
    ├ CRASN
    ├ Eigen3
    ├ fmt
    ├ imgui
    ├ opencv
    ├ openvr
    ├ panda3d
    ├ render_pipeline
    └ spdlog
```

## 외부 라이브러리
### Visual Studio 2013 버전
- Boost : 1.65.1
- bullet : 2.87
- CRASN : rev5639
- Eigen3 : 3.3.4
- fmt : 4.1.0
- imgui : 1.60
- OpenCV : 2.4.13.6
- OpenVR : 1.0.14
- Panda3d : b678bfa (https://github.com/bluekyu/panda3d)
- Render Pipeline C++ : 29b5af7 (https://github.com/chic-yukim/render_pipeline_cpp)
- spdlog : 0.16.3

### Visual Studio 2017 버전
- vcpkg : 0.0.113 (b07568d)
  - bullet3       : 2.87
  - bzip2         : 1.0.6-2
  - eigen3        : 3.3.4-2
  - fmt           : 4.1.0
  - imgui         : 1.60
  - jasper        : 2.0.14-1
  - jsoncpp       : 1.8.4
  - libjpeg-turbo : 1.5.3-1
  - liblzma       : 5.2.3-2
  - libpng        : 1.6.34-3
  - opencv        : 3.4.1
  - opengl        : 0.0-5
  - openvr        : 1.0.15
  - spdlog        : 0.16.3
  - tiff          : 4.0.9
  - tinyxml2      : 6.2.0
  - yaml-cpp      : 0.6.2
  - zlib          : 1.2.11-3
- CRASN : rev6565
- Panda3d : 0005dbc (https://github.com/bluekyu/panda3d)
- Render Pipeline C++ : e73a580 (https://github.com/bluekyu/render_pipeline_cpp)
