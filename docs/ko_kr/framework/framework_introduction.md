# CRSF 구조 소개

## CRSF 기본 구조
![crsf-arch](https://user-images.githubusercontent.com/937305/57493543-4ec15c80-7300-11e9-9c4c-c4c1cc624194.jpg)

CRSF 는 미리 만들어진 런처(CRLauncher.exe)를 사용하여, CRSF 를 자동으로 셋업합니다.
이 과정에서 모듈들이 제공되면, 모듈들을 실행하는 구조로 되어 있습니다.
이는 플러그인 구조를 사용하여 프로그래밍 하였으며, C++ 플러그인 사용 방법을 그대로 따릅니다.



## CRLauncher
CRLauncher 는 [sources-public/sources/CRLauncher](../../sources-public/sources/CRLauncher) 폴더에 있으며,
해당 프로그램을 빌드하여 생성됩니다.

여기 코드에 프로그램 시작점인 main 함수가 존재하며, 여기서부터 CRSF 를 셋업합니다. 기본 흐름은 아래와 같습니다:

1. [커맨드 라인 옵션 셋업 및 파싱](https://github.com/chic-yukim/crsf/blob/f64b113bc040442ceab664f1a2b62c0d115e6059/sources/CRLauncher/src/main.cpp#L36-L52)
2. [CRSF 로거 설정](https://github.com/chic-yukim/crsf/blob/f64b113bc040442ceab664f1a2b62c0d115e6059/sources/CRLauncher/src/main.cpp#L54)
3. [렌더링 엔진 설정](https://github.com/chic-yukim/crsf/blob/f64b113bc040442ceab664f1a2b62c0d115e6059/sources/CRLauncher/src/main.cpp#L56-L57)
4. [CRSF 셋업 task 추가](https://github.com/chic-yukim/crsf/blob/f64b113bc040442ceab664f1a2b62c0d115e6059/sources/CRLauncher/src/main.cpp#L59)
5. [렌더링 엔진 시작](https://github.com/chic-yukim/crsf/blob/f64b113bc040442ceab664f1a2b62c0d115e6059/sources/CRLauncher/src/main.cpp#L62) (무한루프)
6. [CRSF 종료](https://github.com/chic-yukim/crsf/blob/f64b113bc040442ceab664f1a2b62c0d115e6059/sources/CRLauncher/src/main.cpp#L64)
7. [렌더링 엔진 종료](https://github.com/chic-yukim/crsf/blob/f64b113bc040442ceab664f1a2b62c0d115e6059/sources/CRLauncher/src/main.cpp#L66-L69)
8. [CRSF 로거 릴리즈](https://github.com/chic-yukim/crsf/blob/f64b113bc040442ceab664f1a2b62c0d115e6059/sources/CRLauncher/src/main.cpp#L71)

5번 단계인 렌더링 엔진 시작 부분에서 프로그램 창이 열리고, Panda3D 에 추가된 Task 들이 순차적으로 수행됩니다.
4번 단계에서 어플리케이션 Task 를 추가했으므로, 어느 단계에서 해당 Task 가 실행되고, 이 과정에서 CRSF 를 셋업하게 됩니다.

### CRSF 셋업 단계(4번 단계)
CRSF 셋업 단계에서는 아래 흐름을 따릅니다:

1. TInterfaceManager 초기화
2. 모듈 DLL 파일 로드
3. 모듈 `OnLoad` 함수 수행
4. `TInterfaceManager::ConnectInputOutputInterface` 함수 수행
5. 모듈 `OnStart` 함수 수행
