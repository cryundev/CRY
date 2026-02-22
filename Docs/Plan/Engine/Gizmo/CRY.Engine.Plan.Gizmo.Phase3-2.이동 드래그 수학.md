---
aliases:
---
## 이동 드래그 수학
---

축 히트 테스트에서 드래그 시작점을 감지해도, 마우스 이동량을 월드 공간의 이동 델타로 변환하는 수학이 없음. \
이동 기즈모는 레이-평면 교점 차이를 그대로 쓰기보다, 해당 델타를 축 방향으로 투영해 단일 축 이동으로 제한해야 안정적이다. \
또한 `axis`와 `cameraForward`가 거의 평행한 경우 드래그 평면이 퇴화하므로 fallback 법선이 필요하다.

<br />

<br />

## 학습 포인트
---

- 레이-평면 교차점 공식: `t = dot(PlaneN, PlaneP - RayO) / dot(PlaneN, RayD)`
- 드래그 제약 평면 선택 전략 ( 카메라 방향과 축 벡터를 조합하여 가장 수직인 평면 선택 )
- 이동 결과를 `SetActorTransform` DLL API로 즉시 반영하고 드래그 종료 시 Undo 스택 등록
- 델타를 축 방향으로 투영(`dot`)해 오프축 흔들림을 제거하는 기법

^point

<br />

<br />

## TODO
---

- [ ] `Engine/Source/Core/Math/CRRayUtil.h`에 `RayPlaneIntersect()` 추가
    - `bool RayPlaneIntersect(const CRRay& Ray, const CRVector& PlaneNormal, const CRVector& PlanePoint, f32& OutT)`
    - 평행 검사 ( `|dot| < ε` ) 포함
- [ ] `CRGizmoSystem`에 드래그 상태 추가
    - `bool bDragging`, `EGizmoAxis DragAxis`
    - `CRVector DragStartWorld`, `CRVector ActorStartLocation`
    - `BeginDrag(const CRRay& Ray)`: `DragStartWorld` = 드래그 축 평면과 레이의 첫 교점
    - `UpdateDrag(const CRRay& Ray)`: 현재 교점과 시작 교점의 차이를 축 방향으로 투영해 이동량 계산
    - `EndDrag()`: 드래그 상태 초기화
- [ ] 드래그 제약 평면 법선 계산 로직 구현
    - 1차: `planeN = normalize(cross(axisDir, cameraForward))`
    - fallback: 외적 길이가 작으면 `cameraUp` 또는 `cameraRight`와 외적해 재계산
- [ ] `EngineDLL/InputAPI.cpp`의 이벤트 핸들러 연결
    - `OnViewportMouseButton(pressed=true)` 에서 hovered axis 유효 시 `BeginDrag` 호출
    - `OnViewportMouseMove`에서 `bDragging` 시 `UpdateDrag` 호출 → 대상 액터 Transform 즉시 반영
    - `OnViewportMouseButton(pressed=false)` 에서 `EndDrag` 호출
- [ ] 에디터 Undo/Redo 및 UI 동기화 경로 추가
    - 드래그 시작 시 `ActorStartLocation` 스냅샷 저장
    - 드래그 종료 시 `EngineAPI.Actor.TryGetTransform`으로 최종값을 읽어 `UndoRedoAction` 등록
    - `ActorView.Instance.DataContext`의 `MultiSelectionTransform.Refresh()` 호출로 인스펙터 표시 갱신
- [ ] 최적화/UX(옵션)
    - `Ctrl` 입력 시 월드 그리드 스냅 적용
    - 드래그 중 미세 떨림 억제를 위한 dead-zone 거리 임계값 도입

^todo

<br />

<br />
