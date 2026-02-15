---
aliases:
---
## 프러스텀 컬링
---

현재 Culling/Batching 없음. \
View Frustum 밖 오브젝트를 렌더링에서 제외.

<br />

<br />

## 학습 포인트
---

- View Frustum Plane 추출
- AABB vs Frustum 교차 테스트
- Occlusion Culling 개념

^point

<br />

<br />

## TODO
---

- [ ] `CRFrustum` 클래스
    - ViewProjection Matrix에서 6개 평면 추출
    - 신규: `Engine/Source/Core/CRFrustum.h`
- [ ] `CRBoundingBox` ( AABB )
    - 메시 로딩 시 자동 계산
    - 신규: `Engine/Source/Core/CRBoundingBox.h`, `CRD11Mesh.cpp`
- [ ] AABB vs Frustum 교차 테스트 함수
- [ ] `CRD11Renderer::Draw()`에서 컬링 적용
    - 교차하지 않는 오브젝트 스킵
- [ ] ImGUI에 컬링 통계 표시
    - 전체 vs 렌더된 오브젝트 수

^todo

<br />

<br />
