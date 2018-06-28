# 모듈 소개
**Languages**: [English](../../framework/module_introduction.md)

CRSF 에서는 여러 기능들을 쉽게 사용하고, 조합해서 재사용하기 위해 모듈 기능을 제공합니다.
앞서 [프로젝트 구성](project_setup.md)에서 만든 프로젝트가 하나의 모듈이 됩니다.

CRSF 프로젝트는 모듈 단위로 구성이 되고, 어플리케이션을 구성하는 부분도 모듈로 작성할 수 있습니다.

일반적인 모듈은 보통 특정 기능을 쉽게 재사용하기 위해 만듭니다.
예를 들어, VR 모듈은 다른 개발자가 VR 기능을 작동시키기 위한 코드를 직접 추가하지 않고, VR 기능을 활성화 할 수 있도록 합니다.



## 모듈 구조
빈 모듈은 [crsf-empty 프로젝트](https://github.com/chic-yukim/crsf-empty) 에 있는 empty-module 과 같은 구조를 가집니다.

CRSF 에서 모듈을 실행하기 위해서는 DLL 파일과 `config.json` 파일이 필요합니다.
그리고 이를 생성하기 위해 위와 같은 프로젝트를 구성하게 됩니다.

crsf-empty 프로젝트에서 `config.json` 파일은 모듈의 설명을 담고 있으며, 단순 텍스트 파일입니다.
그 외의 파일들은 DLL 을 만들기 위한 소스로 구성이 됩니다.



## 모듈 코드 및 동작
모듈 기능은 [플러그인 개념](https://ko.wikipedia.org/wiki/%ED%94%8C%EB%9F%AC%EA%B7%B8%EC%9D%B8)을 사용해서
구현되어 있습니다. 따라서 CRSF 에서 모듈에 접근하기 위한 인터페이스를 제공해주어야 합니다.

crsf-empty 프로젝트에서 [empty_module.h](https://github.com/chic-yukim/crsf-empty/blob/master/empty-module/include/empty_module/empty_module.h)를 예로 들면,
`EmptyModule` 클래스를 통해 모듈 인스턴스를 제공합니다. 그리고 CRSF 에서 모듈을 제어하기 위해 Load, Start, Exit 함수가 필요합니다.

`OnLoad` 함수는 CRSF 에서 모듈을 로드할 때 부르는 함수이고, 일반적으로 모듈이 로드될 때 필요한 기능이 포함됩니다.

`OnStart` 함수는 CRSF 에서 모듈을 시작할 때 부르는 함수이고, 어플리케이션의 메인 루프가 시작되기 직전에 필요한 기능이 포함됩니다.

`OnExit` 함수는 CRSF 에서 어플리케이션이 종료되어, 모듈을 종료할 때 부르는 함수로, 모듈 기능을 정지하고 DLL 에 포함된 리소스들을
제거하는 기능이 포함됩니다.

그리고 각 함수는 모든 모듈이 순차적으로 실행한 후에, 다음 함수로 넘어갑니다. 즉, 아래와 같은 순서로 함수가 불리게 됩니다.
```
Moduel1::OnLoad()
Module2::OnLoad()
...
ModuleN::OnLoad()

// ...

Moduel1::OnStart()
Module2::OnStart()
...
ModuleN::OnStart()
```

또한, CRSF 에서 각 함수는 한번만 호출하므로, 반복적인 동작이 필요하다면, CRSF 의 Task 기능이나 Thread 기능을 사용해야 합니다.



## 어플리케이션 모듈
어플리케이션 모듈은 일반적인 모듈과 동일한 기능을 가지며, 추가적인 기능이 붙습니다.

먼저, CRSF 에서 어플리케이션으로 지정된 모듈은 해당 모듈이 있는 디렉토리를 작업 디렉토리(Working Directory)로 갖습니다.
그리고 config 폴더를 가져서 CRSF 에 대한 설정을 할 수 있습니다.

마지막으로 어플리케이션 모듈의 config 폴더 안에 있는 `SystemConfiguration.xml` 파일에서 다른 모듈들과 달리
`<application>` 태그를 갖습니다.

어플리케이션 모듈은 crsf-empty 프로젝트에서 `empty-project` 를 참고하면 됩니다.



## 모듈 사용하기
어플리케이션 모듈은 위에서 언급한 것처럼 `SystemConfiguration.xml` 파일에서 `<application>` 태그를 지정하면 됩니다.

일반적인 모듈은 `<module>` 태그를 사용하면 됩니다:
```
<dynamic_modules>
    ...
    <module>
        <id>MODULE_ID</id>
    </module>
    ...
</dynamic_modules>
```
"MODULE_ID" 는 `CMakeLists.txt` 파일에서 `CRMODULE_ID` 와 동일한 문자열을 사용하면 됩니다.
