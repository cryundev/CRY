# CRY Code Style Guide

이 문서는 CRY 프로젝트의 공통 코드 스타일을 정의한다. 모든 신규 코드는 이 규칙을 따른다.

---

## C++ Style

### 파일 구조
- 헤더는 `#pragma once`
- include 블록은 그룹 사이 2줄 공백
- 파일 끝 1줄 공백 유지

### 주석 블록
- 구분선 + `///` 문서 주석
```cpp
//---------------------------------------------------------------------------------------------------------------------
/// CRActor
//---------------------------------------------------------------------------------------------------------------------
class CRActor : public CRObject
```
- 함수 구현부도 동일한 구분선 사용

### 네이밍
- 클래스: `CR` + PascalCase (예: `CRActor`)
- 메서드: PascalCase (예: `AddComponent`)
- private 메서드: `_` + PascalCase (예: `_InitializeRenderTarget`)
- 멤버 변수: PascalCase (예: `ViewportWidth`)
- bool 멤버: `b` 접두 (예: `bDirty`)
- 파라미터: PascalCase, 필요 시 `In` 접두 (예: `InLocation`)
- 지역 변수: camelCase (예: `actorId`)
- 전역: `G` 접두 (예: `GWorld`, `GD11`)
- enum: `E` 접두 (예: `EProjectionType`)
- 상수: UPPER_SNAKE_CASE
- 타입 별칭: PascalCase + `_t` (예: `id_t`)
- 스마트 포인터 접미: `SPtr`, `WPtr`, `UPtr`

### 포맷팅
- 들여쓰기 4 스페이스
- Allman 스타일 중괄호
- 괄호/대괄호/템플릿 내부 공백: `( x )`, `[ i ]`, `< T >`
- 연산자 양쪽 공백: `a = b`, `x + y`
- 간단 getter/setter는 한 줄 허용

### 클래스 선언 순서
```cpp
class CRSomething
{
public:
    friend class ...;

protected:
    // members

public:
    // ctor/dtor
    // virtual overrides
    // public methods
    // template methods

private:
    // private methods (_ prefix)
};
```

### 타입/컨테이너
- 고정폭 타입: `i8/i16/i32/i64`, `u8/u16/u32/u64`, `f32/f64`
- 수학 타입: `CRVector`, `CRVector2D`, `CRVector4D`, `CRQuaternion`, `CRMatrix`
- 컨테이너: `CRArray`, `CRList`, `CRMap`, `CRDeque`
- 문자열: `CRString`, `CRWString`, `CRName`
- 스마트 포인터: `CRSharedPtr`, `CRWeakPtr`, `CRUniquePtr`

---

## C# Style (Editor_WPF)

### 파일 구조
- `using` 블록
- 2줄 공백
- file-scoped `namespace`
- 2줄 공백
- 클래스

### 주석 블록
- C++와 동일한 구분선 + `///`

### 네이밍
- 클래스: PascalCase
- ViewModel: `ViewModel` 접미
- 에디터 객체: `Cr` 접두
- 인터페이스: `I` 접두
- private 필드: `_` + camelCase
- public 프로퍼티: PascalCase
- 파라미터/로컬: camelCase

### 포맷팅
- 들여쓰기 4 스페이스
- Allman
- 괄호/제네릭 공백: `( x )`, `< T >`
- 짧은 auto-property 허용

### MVVM 규칙
- `OnPropertyChanged( nameof( Property ) )`
- `RelayCommand<T>`
- `[DataContract]`, `[DataMember]`, `[OnDeserialized]`
- `ObservableCollection` + `ReadOnlyObservableCollection`
- `UndoRedoAction`

### 클래스 구성 순서
- Static members
- Basic info properties
- Collection properties
- Commands
- Methods
- Initialize / InitializeCommands
- OnDeserialized
- Constructor

---

## 공통 규칙
- Allman 중괄호
- 구분선 스타일 통일
- 렌더링 코드에서 매 프레임 리소스 생성 금지

---

## Codex 작업 지침
- 사용자가 지적한 한 줄만 고치지 말고, 같은 함수/파일의 동일 패턴과 후속 정리까지 같이 확인한다.
- 엔진 코드에서 나중에 교체 가능성이 있는 `std`/외부 API 직접 호출은 가능한 한 `CRMath`, `CRTypes`, CR 컨테이너 등 프로젝트 추상화를 경유한다.
- asset/cooked 데이터처럼 그래픽스 API 선택과 독립적이어야 하는 계층에는 `DXGI_FORMAT` 같은 API 타입을 저장하지 않는다. 엔진/RHI 중립 타입을 저장하고 백엔드 경계에서 실제 API 타입으로 변환한다.
- 그래픽스 API 포맷과 엔진 포맷 사이의 변환 함수는 호출부마다 만들지 말고 백엔드별 전용 파일에 취합한다.
- asset metadata/file layout을 변경하면 기존 `Asset/` 파일도 새 로더가 읽을 수 있게 함께 마이그레이션하거나 호환 로더를 제공한다.
- 새 추상화를 추가했으면 중복되는 로컬 헬퍼를 제거하고 주변 호출부를 함께 정리한다.
- 매직 넘버를 조건식/알고리즘 안에 직접 두지 않는다. 가장 좁은 의미 범위에 `static constexpr` 상수로 빼고 의도를 이름에 담는다.
- 조건 통합 시 동작 의미를 보존한다. 예를 들어 finite 검사만 필요한 곳을 “normalize 가능” 검사로 바꿔 fallback 흐름을 바꾸지 않는다.
- 삼항식 조건이 길어지면 조건을 의미 있는 `bool` 지역 변수로 먼저 분리한다. 변수명과 비교 연산자가 실제 의미와 맞지 않으면 조건을 뒤집어서라도 읽히게 만든다.
- 편집 후에는 오래된 심볼, 긴 조건식, 임시값에 대한 불필요한 reference, 의미 없어진 주석/헬퍼가 남았는지 다시 훑는다.
