---
aliases:
---
## 회전 & 스케일 드래그 수학
---

이동 드래그와 달리 회전은 드래그 방향을 회전 각도로, 스케일은 드래그 거리를 스케일 배율로 변환해야 하며, 두 경우 모두 드래그 평면 정의와 델타 해석 방식이 다름. \
`CRTransformComponent`의 `SetRotation`·`SetScale`은 이미 있으므로 핵심은 안정적인 델타 계산과 값 제한(스케일 하한/스냅)이다. \
특히 회전은 벡터 정규화 실패와 180도 근처 불안정성, 스케일은 음수/0 근처 값이 주요 리스크다.

<br />

<br />

## 학습 포인트
---

- 회전 기즈모 드래그: 피벗 기준 링 접선 방향의 각도 델타를 `CRQuaternion::CreateFromAxisAngle`으로 변환
- 스케일 드래그: 축 방향 투영 거리 비율로 스케일 배율 계산 ( `newScale = baseScale * (1 + delta / refLength)` )
- 누적 부동소수점 오차 방지를 위한 스냅샷 기반 절대 갱신 패턴 ( 매 프레임 델타 누산 대신 시작값 + 오프셋 )
- 스냅(회전 각도/스케일 스텝)과 최소/최대 제한을 통한 UX 안정화

^point

<br />

<br />

## TODO
---

- [ ] `CRGizmoSystem::UpdateDrag()` 내부에 모드 분기 추가
    - `EGizmoMode::Rotate` 분기: 현재 레이와 드래그 평면 교점 → 피벗 기준 각도 계산
        - `angleDelta = atan2(dot(cross(startVec, currentVec), axisDir), dot(startVec, currentVec))`
        - `newRotation = ActorStartRotation * CRQuaternion::CreateFromAxisAngle(AxisDir, angleDelta)`
    - `EGizmoMode::Scale` 분기: 레이-축 방향 투영 거리로 스케일 배율 결정
        - `delta = dot(currentHit - DragStartWorld, AxisDir)`
        - `newScale = ActorStartScale * (1.0f + delta / GizmoArrowLength)`
- [ ] `CRGizmoSystem`에 회전·스케일 드래그용 스냅샷 멤버 추가
    - `CRQuaternion ActorStartRotation`, `CRVector ActorStartScale`
    - `BeginDrag()` 시 `CRTransformComponent`에서 현재 값 스냅샷
- [ ] 스케일 드래그 평면 법선 결정 로직 구현
    - 이동과 동일한 카메라-축 외적 방식 사용
    - `Engine/Source/Core/Math/CRRayUtil.h`의 `RayPlaneIntersect()` 재사용
- [ ] 값 안정화 가드 추가
    - 회전: `startVec/currentVec` 길이 ε 이하 시 업데이트 skip
    - 스케일: `newScale` 각 축을 최소값(예: `0.001f`) 이상으로 clamp
    - 필요 시 최대 스케일 상한도 적용
- [ ] 회전·스케일 결과 반영 경로 확정
    - 엔진 내부에서는 대상 액터 `CRTransformComponent`를 직접 갱신
    - 에디터 동기화는 드래그 종료 시 `GetActorTransform` 재조회로 최종값 반영
- [ ] 에디터 Undo/Redo 연결
    - 드래그 시작 스냅샷(회전/스케일)과 종료값으로 `UndoRedoAction` 생성
    - `MultiSelectionTransform.Refresh()` 호출로 인스펙터 표시 일치
- [ ] 스냅 옵션(권장)
    - 회전: `Shift` 누를 때 15도 스냅
    - 스케일: `Shift` 누를 때 0.1 단위 스텝 스냅
- [ ] 통합 테스트
    - 액터 선택 → 이동 → 회전 → 스케일 순서 조작
    - 드래그 중 hover/active axis 표시 정상 여부 확인
    - Undo/Redo 3회 이상 왕복 후 값 드리프트 없는지 확인

^todo

<br />

<br />
