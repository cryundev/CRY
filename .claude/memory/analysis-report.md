# CRY Engine - 종합 프로젝트 분석 보고서

## Context

CRY는 DirectX 11 기반 3D 게임 엔진 학습 프로젝트로, C++ 렌더링 엔진 + C# WPF 에디터로 구성됨.
사용자가 프로젝트 전체 분석을 요청하여, 아키텍처/코드 품질/확장성/개선점을 종합 평가함.

---

## 1. 프로젝트 규모 및 구성

| 프로젝트 | 언어 | 역할 | 소스 파일 수 |
|----------|------|------|-------------|
| **Engine** | C++ (Static Lib) | 코어 렌더링 엔진 | ~113 (.h/.cpp) |
| **EngineDLL** | C++ (DLL) | 에디터↔엔진 인터페이스 | 4 |
| **Editor_WPF** | C# (WPF App) | MVVM 에디터 | ~62 (.cs/.xaml) |
| **Editor_Win32** | C++ (Win32 App) | 레거시 에디터 | ~12 |
| **EngineTest** | C++ (Console) | 테스트 프레임워크 | 3 |
| **DirectXTK** | C++ (Static Lib) | 외부 의존성 | 외부 라이브러리 |

**총 소스 파일: ~194개** | **빌드 구성: Debug/Release/DebugEditor/ReleaseEditor (x64)**

---

## 2. 아키텍처 개요

```
┌─────────────────────────────────────────────────┐
│                 Editor_WPF (C#)                  │
│  ┌───────────┐ ┌──────────┐ ┌────────────────┐  │
│  │ MVVM      │ │ GameDev  │ │ DllWrappers    │  │
│  │ ViewModels│ │ VS/Rider │ │ EngineAPI.cs   │  │
│  └─────┬─────┘ └────┬─────┘ └───────┬────────┘  │
│        │             │               │            │
└────────┼─────────────┼───────────────┼────────────┘
         │             │               │ P/Invoke
┌────────┼─────────────┼───────────────┼────────────┐
│        │      EngineDLL (C ABI)      │            │
│  ┌─────┴──────────────────────┴──────┴─────┐      │
│  │ EngineAPI.cpp | WorldAPI.cpp            │      │
│  └─────────────────┬───────────────────────┘      │
└────────────────────┼──────────────────────────────┘
                     │
┌────────────────────┼──────────────────────────────┐
│              Engine (C++ Core)                     │
│  ┌────────┐ ┌──────┴───┐ ┌───────┐ ┌──────────┐  │
│  │ Object │ │   RHI    │ │ Asset │ │ Utility  │  │
│  │ ECS    │ │ DX11     │ │ FBX   │ │ Log/Time │  │
│  │ Script │ │ Pipeline │ │ WIC   │ │ Math     │  │
│  └────────┘ └──────────┘ └───────┘ └──────────┘  │
└───────────────────────────────────────────────────┘
```

---

## 3. 핵심 시스템 분석

### 3.1 Entity-Component System (ECS) — 평가: ★★★★☆

**설계 패턴: CRTP + Sparse Set**

```
CRObject (ID, Name, Lifecycle)
  ├── CRActor (Component 컨테이너, World 참조)
  │   └── CRCamera (카메라 특화)
  ├── CRScript (게임 로직, DLL 동적 로딩)
  └── CRComponent<T> (CRTP 기반, 정적 스토리지)
      ├── CRTransformComponent (위치/회전/스케일)
      ├── CRPrimitiveComponent (메시 렌더링)
      └── CRScriptComponent (스크립트 연결)
```

**강점:**
- O(1) 컴포넌트 추가/조회/삭제 (Sparse Set)
- 캐시 친화적 순회 (Dense Storage)
- C++20 Concepts로 컴파일 타임 타입 안전성
- 48비트 인덱스 + 16비트 세대 카운터 (use-after-free 방지)
- REGISTER_SCRIPT 매크로로 DLL 동적 스크립트 로딩

**약점:**
- 액터당 같은 타입의 컴포넌트 1개 제한
- RemoveComponent<T>() 미구현 (빈 stub)
- 부모-자식 Transform 계층구조 없음
- 스레드 안전성 없음 (static 배열 동기화 없음)
- Actor despawn 시 Transform만 명시적 제거 (다른 컴포넌트 고아화 위험)

---

### 3.2 렌더링 파이프라인 (RHI/DX11) — 평가: ★★★☆☆

**구조: 2-Tier 추상화**
- Tier 1: 추상 인터페이스 (`ICRRHIRenderer`, `ICRRHIMesh`)
- Tier 2: DirectX 11 구현 (`CRD11Renderer`, `CRD11Mesh`)

**프레임 실행 흐름:**
```
RenderFrame()
  ├─ ClearRenderTarget (배경색 + 깊이)
  ├─ Draw()
  │   ├─ ViewProjection 상수 버퍼 갱신 (매 프레임)
  │   └─ 각 메시:
  │       ├─ 파이프라인 상태 설정 (VB, IB, Shader, Texture)
  │       └─ DrawIndexed()
  └─ ImGui 렌더링
Present() → SwapChain->Present()
```

**셰이더:** 단일 Diffuse shader (HLSL)
- VS: Transform/ViewProjection 상수 버퍼
- PS: 텍스처 샘플링 + 기본 디퓨즈 라이팅 (감마 보정)

**강점:**
- 팩토리 패턴으로 백엔드 전환 가능한 구조
- 매크로 기반 리소스 관리자 (이름 기반 중복 방지)
- ImGUI 통합으로 디버깅 편의

**약점:**
- DX11 타입이 추상 레이어를 넘어 누출
- 드로우콜 배칭/정렬 없음 (메시마다 전체 상태 재설정)
- 프러스텀 컬링 없음
- 머티리얼 시스템 없음 (셰이더 1개 고정)
- 텍스처 경로 하드코딩
- Vulkan/DX12 추가 시 대규모 리팩토링 필요 (난이도 8/10)

---

### 3.3 WPF 에디터 MVVM — 평가: ★★★★☆

**ViewModel 계층:**
```
ViewModelBase (INotifyPropertyChanged)
  ├── ProjectViewModel (프로젝트 관리, 빌드, DLL 로드)
  │   └── WorldViewModel (씬/월드, 액터 컬렉션)
  ├── CrActorViewModel (액터, 컴포넌트 컬렉션)
  ├── CrComponentViewModel (추상 컴포넌트 베이스)
  │   ├── CrTransformComponentViewModel
  │   └── CrScriptComponentViewModel
  └── CreateProjectViewModel (프로젝트 생성 워크플로우)
```

**강점:**
- 엄격한 MVVM 분리 (View ↔ ViewModel)
- RelayCommand<T> 패턴으로 데이터 바인딩
- DataContract 직렬화 (.cryproject XML 포맷)
- Undo/Redo 시스템 (Command 패턴)
- VS/Rider 통합 (Strategy 패턴)
- 컴포넌트 팩토리 (배열 기반 O(1) 디스패치)

**약점:**
- 정적 싱글턴 (ProjectViewModel.Current) → 테스트 어려움
- P/Invoke 에러 처리 부족 (바이너리 리턴 코드만)
- DLL 핫리로드 레이스 컨디션 가능성
- Undo/Redo 히스토리 무제한 성장
- 비동기 커맨드 미지원 (빌드 시 UI 블로킹)
- 직렬화 스키마 버전 관리 없음

---

### 3.4 DLL 인터페이스 — 평가: ★★★☆☆

**API 표면:**
```
EngineAPI.cpp:
  LoadGameCodeDLL(path) → i32     ScriptCreator(name) → 함수 포인터
  UnloadGameCodeDLL() → i32       ScriptNames() → SAFEARRAY

WorldAPI.cpp:
  SpawnActor() → id_t             DespawnActor(id) → void
```

**강점:** C ABI로 깔끔한 경계, SAFEARRAY로 C# 배열 마샬링
**약점:** 에러 코드만 반환 (에러 메시지 없음), World 상태 조회 API 없음

---

### 3.5 유틸리티 시스템 — 평가: ★★★☆☆

| 시스템 | 품질 | 주요 특징 | 주요 한계 |
|--------|------|-----------|-----------|
| FBX 로더 | 7/10 | 전체 메시+변환 지원 | 축/스케일 변환 주석처리, 탄젠트 미생성 |
| WIC 텍스처 | 8/10 | 110+ 포맷 지원 | 메모리 소유권 불명확, 스트리밍 없음 |
| 로깅 | 5/10 | 인메모리 + HRESULT | 파일 출력 없음, 타임스탬프 없음 |
| 시간 관리 | 7/10 | HPC 마이크로초 정밀도 | 고정 타임스텝만 |
| 에셋 시스템 | 5/10 | 바이너리 저장/로드 | 포맷 버전 없음, 압축 없음 |
| 타입 시스템 | 8/10 | DirectXTK 래핑 | 커스텀 SIMD 최적화 없음 |

---

## 4. 설계 패턴 종합

| 패턴 | 적용 위치 | 효과 |
|------|-----------|------|
| **CRTP** | CRComponent<T> | ✅ 컴파일타임 다형성, 제로 오버헤드 |
| **Sparse Set** | 컴포넌트 저장소 | ✅ O(1) 연산, 캐시 친화 |
| **Generational Index** | 오브젝트 ID | ✅ use-after-free 방지 |
| **Factory** | RHI 렌더러, 컴포넌트 | ✅ 확장 가능한 생성 |
| **Strategy** | 코드 에디터 (VS/Rider) | ✅ IDE 전환 유연성 |
| **MVVM** | 에디터 전체 | ✅ View↔Logic 분리 |
| **Command** | Undo/Redo, RelayCommand | ✅ 실행 취소 가능한 작업 |
| **Singleton** | GWorld, GLog, ResourceMgr | ⚠️ 편리하나 테스트/스레드 안전 이슈 |
| **Static Registration** | REGISTER_SCRIPT | ✅ DLL 동적 로딩 지원 |
| **Lazy Evaluation** | Transform 더티 플래그 | ✅ 불필요한 행렬 계산 방지 |

---

## 5. 종합 평가

### 점수표

| 영역 | 점수 | 설명 |
|------|------|------|
| **아키텍처** | ★★★★☆ | 잘 분리된 레이어, 명확한 책임 |
| **코드 품질** | ★★★½☆ | 일관된 네이밍, 모던 C++, 일부 미완성 |
| **성능** | ★★☆☆☆ | 배칭/컬링 없음, 상태 중복 설정 |
| **확장성** | ★★★☆☆ | RHI 추상화 있으나 DX11 종속 누출 |
| **에러 처리** | ★★☆☆☆ | Debug Assert 의존, 일관성 부족 |
| **스레드 안전** | ★☆☆☆☆ | 동기화 메커니즘 전무 |
| **에디터** | ★★★★☆ | 견고한 MVVM, DLL 핫리로드 |
| **테스트** | ★★☆☆☆ | 스트레스 테스트만, 유닛 테스트 없음 |
| **문서화** | ★★★☆☆ | Docs 폴더 존재, 인라인 주석 부족 |

### 총평: **학습용 엔진으로서 우수한 설계 (3.5/5)**

**핵심 강점:**
1. 모던 C++20 (Concepts, CRTP, Sparse Set) 활용한 고성능 ECS
2. 깔끔한 C++/C# 경계 분리와 DLL 핫리로드
3. 체계적인 MVVM 에디터 아키텍처
4. 일관된 네이밍 컨벤션 (CR 접두사, ViewModel 접미사)

**핵심 약점:**
1. 렌더링 성능 최적화 부재 (배칭, 컬링, 머티리얼)
2. 스레드 안전성 부재
3. 에러 처리 일관성 부족
4. 일부 시스템 미완성 (RemoveComponent, ScriptComponent)

---

## 6. 개선 우선순위 로드맵

### P0 — 크리티컬 (안정성)
1. `RemoveComponent<T>()` 구현 완료
2. Actor despawn 시 모든 컴포넌트 정리
3. DLL 핫리로드 레이스 컨디션 수정
4. 에셋 포맷 버전 관리 추가

### P1 — 높음 (기능)
5. 머티리얼 시스템 구축 (셰이더 변형)
6. 프러스텀 컬링 구현
7. 드로우콜 배칭/정렬
8. 부모-자식 Transform 계층구조

### P2 — 중간 (품질)
9. 에러 처리 통합 프레임워크
10. 로깅 시스템 고도화 (파일 출력, 레벨, 타임스탬프)
11. Undo/Redo 히스토리 제한
12. 유닛 테스트 프레임워크 도입

### P3 — 낮음 (확장)
13. RHI 추상화 강화 (DX11 타입 누출 제거)
14. 스레드 안전 싱글턴
15. 에셋 스트리밍/캐싱
16. LOD 시스템

---

## 7. 주요 파일 참조

### Engine (C++)
- ECS 핵심: `Engine/Source/Object/CRActor.h`, `Component/CRComponent.h`
- 스크립트: `Engine/Source/Object/CRScript.h` (REGISTER_SCRIPT 매크로)
- ID 시스템: `Engine/Source/Core/Identify/CRIdentity.h`
- RHI: `Engine/Source/RHI/CRRHI.h`, `DX11/CRD11Renderer.h`
- 셰이더: `Engine/Source/RHI/DX11/HLSL/shader.hlsl`
- 에셋: `Engine/Source/Asset/CRPrimitiveAsset.h`

### Editor (C#)
- MVVM 베이스: `Editor_WPF/Common/ViewModelBase.cs`
- 프로젝트: `Editor_WPF/GameProject/ProjectViewModel.cs`
- DLL 래퍼: `Editor_WPF/DllWrappers/EngineAPI.cs`
- IDE 통합: `Editor_WPF/GameDev/CodeEditorManager.cs`
- Undo/Redo: `Editor_WPF/Utilities/UndoRedo.cs`

### DLL 경계
- 엔진 API: `EngineDLL/EngineAPI.cpp`
- 월드 API: `EngineDLL/WorldAPI.cpp`
