# Directory Structures
**Languages**: [한국어](ko_kr/directory_structures.md)

```
─ CRSF
  ├ bin                     # DLL & EXE files
  │ ├ CRLauncher.exe
  │ └ CRSeedLib.dll
  ├ cmake                   # cmake files
  ├ crmodules               # CRSF internal modules
  ├ examples                # Example projects
  │ ├ 1-Graphics                # Graphics examples
  │ ├ 2-Interface               # Interface examples
  │ ├ 4-Network                 # Network examples
  │ ├ 5-Physics                 # Physics examples
  │ ├ 6-Hand                    # Hand manipulation examples
  │ └ 7-Applications            # Tutorial projects
  └ thirdparty              # Third-party
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

## Third-Party
### Visual Studio 2013
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

### Visual Studio 2017
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
