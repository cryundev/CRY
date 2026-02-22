---
aliases:
---
## 오브젝트 피킹
---

`CRWorld::GetActor()`로 개별 조회는 가능하지만, 월드 액터 전체를 순회해 피킹하는 경로는 아직 없음. \
또한 에디터 선택 상태는 `ProjectLayoutView`/`ActorView.Instance.DataContext` 중심이라, 피킹 결과를 반영할 명시적 동기화 경로가 필요함.

<br />

<br />

## 학습 포인트
---

- AABB(Axis-Aligned Bounding Box)의 정의와 슬래브법(Slab Method) 레이-AABB 교차 검사
- 여러 히트 결과 중 가장 가까운 것을 선택하는 깊이 정렬 기법 ( 최소 `t` 값 )
- 피킹 결과를 DLL 브리지를 통해 C# 에디터로 전달하는 ABI 설계
- 선택 모델(ListBox 선택/멀티 선택)과 뷰포트 피킹의 동기화 설계

^point

<br />

<br />

## TODO
---

- [ ] `Engine/Source/Core/Math/CRAABB.h` 신규 파일 생성
    - `struct CRAABB { CRVector Min; CRVector Max; };`
    - `static CRAABB FromCenterExtents(const CRVector& Center, const CRVector& HalfExtents)`
    - `bool Intersects(const CRRay& Ray, f32& OutT) const` — 슬래브법 구현
- [ ] `Engine/Source/World/CRWorld.h`에 월드 액터 순회 접근자 추가
    - `const CRList<CRActor*>& GetAllActors() const`
    - 구현은 기존 `Actors` 컨테이너를 직접 노출하되 const 제한 유지
- [ ] 1차 피킹용 AABB 정책 정의
    - 기본: `Transform.Location + Transform.Scale` 기반 축정렬 박스
    - 예외: 카메라 액터는 피킹 후보에서 제외
    - 확장: 추후 `CRPrimitiveComponent` 메시 bounds 기반으로 교체
- [ ] `EngineDLL/WorldAPI.Picking.cpp` 신규 파일 생성
    - `CR_ENGINE_API CRIdentity::id_t PickActorAtScreen(i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH)` 구현
    - 스크린 좌표 → 레이 생성 → 액터 순회 → `AABB.Intersects()` → 최소 t 액터 반환
    - 히트 없을 경우 `CRIdentity::IdMask` 반환
- [ ] `Editor_WPF/DllWrappers/EngineAPI.Input.cs`에 `PickActor` P/Invoke 추가
    - `[DllImport("EngineDLL.dll", EntryPoint = "PickActorAtScreen")] static extern long PickActor(int pixelX, int pixelY, int viewportW, int viewportH);`
- [ ] `EngineViewportHost.cs`의 `WM_LBUTTONDOWN`에서 `PickActor` 호출 후 선택 동기화
    - `WorldViewModel.SelectedActor` 같은 단일 선택 프로퍼티는 현재 없으므로 사용하지 않음
    - `ProjectLayoutView`/`ActorView`와 연결 가능한 선택 유틸(예: `ProjectViewModel.SelectActorById`)을 도입해 멀티 선택 모델과 일치시킴
- [ ] 후속 최적화(옵션)
    - 액터 수 증가 대비 Broad-phase(그리드/트리) 도입
    - 드래그 중에는 재피킹 생략, hover 상태에서만 경량 테스트 수행

^todo

<br />

<br />
