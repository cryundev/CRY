# CRY Project Memory

## Project Overview
- DirectX 11 3D game engine (learning project)
- C++ Engine (static lib) + C# WPF Editor (MVVM) + EngineDLL (C ABI interop)
- ~194 source files total, x64 only, VS/Rider IDE support
- Build: `msbuild CRY.sln /p:Configuration=DebugEditor /p:Platform=x64`

## Architecture Quick Reference
- **ECS**: CRTP + Sparse Set, O(1) ops, C++20 Concepts, Generational IDs
- **RHI**: 2-Tier (abstract + DX11), single Diffuse shader, no batching/culling
- **Editor**: MVVM ViewModels, DLL hot-reload, Undo/Redo, VS/Rider Strategy pattern
- **DLL Boundary**: C ABI exports, SAFEARRAY for arrays, P/Invoke in C#

## Key Conventions
- C++ classes: `CR` prefix (CRActor, CRComponent)
- C# ViewModels: `ViewModel` suffix (ProjectViewModel, WorldViewModel)
- C# editor objects: `Cr` prefix (CrActor, CrScript)
- Types: i8/i16/i32/i64, u8/u16/u32/u64, f32/f64
- Smart ptrs: CRSharedPtr, CRWeakPtr (std wrappers)
- Containers: CRArray(vector), CRMap(unordered_map), CRList(list)

## Known Issues (from analysis)
- RemoveComponent<T>() is empty stub
- No thread safety on static component arrays
- Actor despawn only removes Transform (other components orphaned)
- DLL hot-reload has potential race conditions
- No asset format versioning
- Rendering: no batching, no culling, no material system

## Key Files
- ECS: `Engine/Source/Object/CRActor.h`, `Component/CRComponent.h`
- Script: `Engine/Source/Object/CRScript.h` (REGISTER_SCRIPT macro)
- RHI: `Engine/Source/RHI/CRRHI.h`, `DX11/CRD11Renderer.h`
- Editor: `Editor_WPF/GameProject/ProjectViewModel.cs`
- DLL: `EngineDLL/EngineAPI.cpp`, `EngineDLL/WorldAPI.cpp`
- Interop: `Editor_WPF/DllWrappers/EngineAPI.cs`

## Coding Style
See [detailed coding style guide](coding-style.md)

### Quick Reference
- **괄호 내부 공백**: `( x )`, `[ i ]`, `< T >` — C++/C# 모두
- **중괄호**: Allman 스타일 (새 줄)
- **주석**: 대시 구분선 `//---...---` + `///` 설명
- **들여쓰기**: 4 spaces
- **C++ private 메서드**: `_` 접두사 (`_SetDirty()`, `_InitializeRenderTarget()`)
- **C++ bool 멤버**: `b` 접두사 (`bDirty`)
- **C++ 지역변수**: camelCase (`actorId`, `newActor`)
- **C++ 멤버/매개변수**: PascalCase (`Location`, `DeltaSeconds`)
- **C++ 전역**: `G` 접두사 (`GWorld`, `GLog`)
- **C# private 필드**: `_` 접두사 + camelCase (`_worlds`, `_activeWorld`)
- **C# 정렬**: 관련 변수 타입/이름 정렬
- **빈 줄**: 섹션 간 2줄, 섹션 내 1줄

## DX11 Learning Curriculum
- 커리큘럼 문서: `DX11_LEARNING_CURRICULUM.md` (8 Phase, 24 소단원)
- 렌더링 구조 리뷰: [dx11-rendering-review.md](dx11-rendering-review.md)
- **커리큘럼 진행 전 필수 리팩토링**:
  1. Mesh/Material 분리 (CRD11Mesh → 지오메트리만 + CRD11Material 신규)
  2. shader.hlsl Normal 월드 변환 추가
- **Phase별 추가 리팩토링**:
  3. Renderer→World 디커플링 (Phase 3.1 전)
  4. MRT 지원 (Phase 3.1 전)
  5. ResourceManager Find/Create 분리

## Detailed Analysis
See [full analysis report](analysis-report.md)
