# CRY 프로젝트 코딩 스타일 가이드

## C++ 코딩 스타일

### 파일 구조
- 헤더 가드: `#pragma once` (include guard 대신)
- include 후 **빈 줄 2개**로 섹션 구분
- 클래스 선언 전 **빈 줄 2개**
- 파일 끝에 빈 줄 1개

### 주석 스타일
- 클래스/구조체 위: 대시 구분선 + `///` 주석
```cpp
//---------------------------------------------------------------------------------------------------------------------
/// CRActor
//---------------------------------------------------------------------------------------------------------------------
class CRActor : public CRObject
```
- 멤버 함수 구현 위: 대시 구분선 + `///` 주석 (클래스와 동일)
```cpp
//---------------------------------------------------------------------------------------------------------------------
/// Add component.
//---------------------------------------------------------------------------------------------------------------------
template < ComponentType T >
T* CRActor::AddComponent()
```
- 멤버 변수/함수 선언: `///` 한 줄 주석
```cpp
/// Components.
inline static CRArray< T > Components = {};
```
- 대시 구분선은 정확히 117자 (`//-...-`)

### 네이밍 규칙
- **클래스**: `CR` 접두사 + PascalCase → `CRActor`, `CRComponent`, `CRWorld`
- **네임스페이스**: PascalCase → `CRIdentity`, `UtilContainer`
- **함수/메서드**: PascalCase → `AddComponent()`, `SpawnActor()`, `GetObjectId()`
- **private 메서드**: `_` 접두사 + PascalCase → `_SetDirty()`, `_InitializeRenderTarget()`
- **멤버 변수**: PascalCase (접두사 없음) → `ObjectName`, `Location`, `ViewportWidth`
- **bool 멤버**: `b` 접두사 → `bDirty`
- **매개변수**: PascalCase, `In` 접두사 사용 가능 → `InLocation`, `InRotation`, `DeltaSeconds`
- **지역 변수**: camelCase → `actorId`, `newActor`, `worldIndex`, `lightDir`
- **상수**: UPPER_SNAKE_CASE → `THRESHOLD_FREE_IDS_RETURN`
- **타입 별칭**: PascalCase + `_t` 접미사 → `id_t`, `generation_t`
- **전역 변수**: `G` 접두사 → `GWorld`, `GLog`, `GD11RM`, `GD11RP`, `GD11`
- **열거형**: `E` 접두사 → `EProjectionType`, `ECVS`, `ECPS`
- **스마트 포인터 접미사**: `SPtr`(shared), `WPtr`(weak), `UPtr`(unique)

### 공백 및 포맷
- **들여쓰기**: 4 spaces (탭 아님)
- **중괄호**: Allman 스타일 (새 줄에 열림)
```cpp
if ( index >= IdMap.size() )
{
    IdMap.resize( index + 1 );
}
```
- **괄호 내부 공백**: 있음 → `( const CRName& Name )`, `( id )`, `[ index ]`
- **템플릿 공백**: `< T >`, `< CRComponent< T >, T >` (꺽쇠 내부에도 공백)
- **단일 행 함수**: 짧은 getter/setter는 한 줄 허용
```cpp
inline CRWorld* GetWorld() const { return World; }
void _SetDirty() { bDirty = true; }
```
- **연산자 주변 공백**: 있음 → `a = b`, `x + y`
- **정렬**: 관련 변수/멤버를 정렬해서 가독성 확보
```cpp
CRVector     Location = CRVector::Zero;
CRQuaternion Rotation = CRQuaternion::Identity;
CRVector     Scale    = CRVector::One;
```

### 접근 지정자 순서
```cpp
class CRSomething
{
public:
    friend class ...;

protected:
    // 멤버 변수

public:
    // 생성자/소멸자
    // virtual override 함수
    // public 메서드
    // 템플릿 메서드

private:
    // private 메서드 (_접두사)
};
```

### 타입 시스템
- 고정 크기 타입 사용: `i8/i16/i32/i64`, `u8/u16/u32/u64`, `f32/f64`
- 수학 타입: `CRVector`, `CRVector2D`, `CRVector4D`, `CRQuaternion`, `CRMatrix`
- 컨테이너: `CRArray<T>`, `CRList<T>`, `CRMap<K,V>`, `CRDeque<T>`
- 문자열: `CRString`(std::string), `CRWString`(std::wstring), `CRName`(std::string)
- 스마트 포인터: `CRSharedPtr<T>`, `CRWeakPtr<T>`, `CRUniquePtr<T>`

### 매크로
- `DECLARE_SMART_PTR( ClassName )` — SharedPtr/WeakPtr 타입 별칭 선언
- `DECLARE_TYPE_ID( name )` — 타입 안전 ID 래퍼 (Debug), 별칭 (Release)
- `REGISTER_SCRIPT( T )` — 스크립트 정적 등록

### C++ 모던 기능 사용
- C++20 Concepts: `template< typename T > concept ComponentType = ...`
- `inline static`: 클래스 내 static 멤버 인라인 초기화
- `constexpr`: 컴파일 타임 상수와 생성자
- `std::ranges::find_if`: 범위 기반 알고리즘
- `= default`, `= 0`: 기본/순수 가상 함수
- `auto&` range-for: `for ( auto& component : Components )`

---

## C# 코딩 스타일 (Editor_WPF)

### 파일 구조
- `using` 문 → 빈 줄 2개 → `namespace` (file-scoped) → 빈 줄 2개 → 클래스
```csharp
using System.Collections.ObjectModel;
using Editor_WPF.Common;


namespace Editor_WPF.GameProject;


public class ProjectViewModel : ViewModelBase
```

### 주석 스타일
- C++과 동일한 대시 구분선 패턴 (메서드 위)
```csharp
//-----------------------------------------------------------------------------------------------------------------
/// BuildGameCodeDll
//-----------------------------------------------------------------------------------------------------------------
private void BuildGameCodeDll( bool showWindow = true )
```
- C#의 대시 구분선은 113자 (`//-...-`)로 C++보다 약간 짧음
- 멤버 그룹 구분: `///` 주석 → `/// Commands`, `/// Basic Info Properties`

### 네이밍 규칙
- **클래스**: PascalCase → `ProjectViewModel`, `WorldViewModel`
- **ViewModel**: `ViewModel` 접미사 → `CrActorViewModel`, `CrComponentViewModel`
- **에디터 객체**: `Cr` 접두사 → `CrActor`, `CrScript`, `CrComponent`
- **인터페이스**: `I` 접두사 → `ICodeEditor`, `IMultiSelectionComponent`
- **private 필드**: `_` 접두사 + camelCase → `_worlds`, `_activeWorld`, `_buildConfig`
- **public 프로퍼티**: PascalCase → `ActiveWorld`, `BuildConfig`, `AvailableScripts`
- **static 필드**: `_` 접두사 + camelCase → `_buildConfigurationNames`
- **메서드**: PascalCase → `Initialize()`, `LoadGameCodeDll()`, `OnBuildCompleted()`
- **매개변수**: camelCase → `worldName`, `showWindow`, `component`
- **로컬 변수**: camelCase → `newWorld`, `worldIndex`, `configName`
- **람다 매개변수**: 한 글자 → `x`, `_` (discard)

### 공백 및 포맷
- **들여쓰기**: 4 spaces
- **중괄호**: Allman 스타일 (새 줄)
- **괄호 내부 공백**: C++과 동일하게 있음 → `( this )`, `( nameof( Worlds ) )`
- **제네릭 공백**: `< WorldViewModel >`, `< T >` (꺽쇠 내부 공백)
- **프로퍼티 패턴**:
```csharp
public WorldViewModel? ActiveWorld
{
    get => _activeWorld;
    set
    {
        if ( _activeWorld == value ) return;
        _activeWorld = value;
        OnPropertyChanged( nameof( ActiveWorld ) );
    }
}
```
- **간단한 프로퍼티**: 한 줄 가능 → `public static string Extension { get; } = ".cryproject";`
- **컬렉션 초기화**: `= []` (C# 12 collection expression)
- **정렬**: 관련 필드/속성 정렬
```csharp
[DataMember] public string Name { get; private set; }
[DataMember] public string Path { get; private set; }
```

### 클래스 구조 순서
```csharp
[DataContract]
public class SomeViewModel : ViewModelBase
{
    /// Static Members (static 필드, 프로퍼티)

    /// Basic Info Properties (데이터 멤버)

    /// Collection Properties (ObservableCollection + ReadOnly)

    /// Commands (ICommand 프로퍼티)

    /// Methods (private 헬퍼 → public API → 이벤트 핸들러)

    /// Initialize / InitializeCommands

    /// OnDeserialized

    /// Constructor (마지막)
}
```

### MVVM 패턴
- **프로퍼티 변경**: `OnPropertyChanged( nameof( PropertyName ) )`
- **커맨드**: `new RelayCommand< T >( execute, canExecute )`
- **직렬화**: `[DataContract]`, `[DataMember]`, `[OnDeserialized]`
- **컬렉션**: `private ObservableCollection` + `public ReadOnlyObservableCollection`
- **Undo/Redo**: `new UndoRedoAction( undo, redo, description )`

---

## 공통 스타일 원칙

1. **괄호 내부 공백**: C++, C# 모두 `( x )`, `[ i ]`, `< T >` 내부 공백 사용
2. **Allman 중괄호**: 모든 곳에서 새 줄에 중괄호
3. **대시 구분선 주석**: 클래스와 메서드 구분에 대시 라인 + `///` 패턴
4. **정렬**: 관련 변수들은 타입/이름 정렬하여 가독성 확보
5. **빈 줄**: 섹션 간 빈 줄 2개, 같은 섹션 내 빈 줄 1개
6. **간결한 함수**: 짧은 getter/setter는 한 줄, 나머지는 풀 바디
