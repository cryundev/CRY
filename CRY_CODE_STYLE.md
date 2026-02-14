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
