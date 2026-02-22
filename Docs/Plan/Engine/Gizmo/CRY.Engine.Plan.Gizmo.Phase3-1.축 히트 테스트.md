---
aliases:
---
## 축 히트 테스트
---

기즈모가 렌더링되더라도 어느 축 핸들을 마우스로 가리키고 있는지 판별하는 히트 테스트 로직이 없음. \
축 핸들은 얇고 길기 때문에 삼각형 교차보다 레이-선분(또는 캡슐) 최소거리 기반 판정이 구현/성능 측면에서 유리하다. \
핵심은 픽셀 허용 오차를 카메라 거리와 투영값으로 월드 거리로 변환하고, 프레임 간 하이라이트 깜빡임을 줄이는 것이다.

<br />

<br />

## 학습 포인트
---

- 레이-선분 최소거리 공식 ( 두 직선의 최근접점, `dot` / `cross` 활용 )
- 스크린 스페이스 픽셀 허용 오차를 월드 공간 거리로 변환하는 방법
- 마우스 오버 시 하이라이트 색상을 Constant Buffer로 실시간 반영하는 패턴
- 히스테리시스(hysteresis)로 hover axis 튐 현상 완화

^point

<br />

<br />

## TODO
---

- [ ] `Engine/Source/Core/Math/CRRayUtil.h`에 `RayToSegmentDistance()` 함수 추가
    - `f32 RayToSegmentDistance(const CRRay& Ray, const CRVector& SegStart, const CRVector& SegEnd)`
    - 두 직선 파라미터 `s`, `t` 계산 → 범위 클램프 → 최근접점 거리 반환
- [ ] `Engine/Source/RHI/Gizmo/CRGizmoSystem.h`에 `EGizmoAxis` 열거형 추가
    - `enum class EGizmoAxis { None, X, Y, Z };`
    - `EGizmoAxis HoveredAxis = EGizmoAxis::None;` 상태 보유
- [ ] `CRGizmoSystem::TestAxisHit(const CRRay& Ray)` 함수 구현
    - 각 축 선분 ( 피벗 → 피벗 + 축방향 × 화살표 길이 ) 에 대해 `RayToSegmentDistance` 호출
    - 픽셀 허용 오차 → 월드 거리 변환: `tolerance = (2 * dist / projM22) * (pixelThreshold / viewportHeight)`
    - 가장 가까운 축 반환, 임계값 초과 시 `EGizmoAxis::None`
- [ ] hover 안정화 로직 추가
    - 직전 `HoveredAxis`가 새 후보보다 약간 불리한 수준이면 유지(예: 1.2x 임계)
    - 드래그 중에는 hover 재판정 정지
- [ ] `EngineDLL/InputAPI.cpp`의 `OnViewportMouseMove`에서 `CRGizmoSystem::TestAxisHit` 호출
    - 결과를 `GGizmoSystem.HoveredAxis`에 저장
- [ ] `CRD11GizmoPass::OnPreDraw()`에서 `HoveredAxis`를 Constant Buffer에 반영
    - 해당 축 색상을 노란색 `(1,1,0,1)`으로 오버라이드
    - `gizmo.hlsl`의 `cbuffer GizmoBuffer`에 `int HoveredAxis` 필드 추가
- [ ] 성능 최적화(옵션)
    - 선택 액터 없음/기즈모 비활성 시 히트 테스트 skip
    - 카메라/피벗 변화가 없고 마우스 좌표 동일하면 테스트 skip

^todo

<br />

<br />
