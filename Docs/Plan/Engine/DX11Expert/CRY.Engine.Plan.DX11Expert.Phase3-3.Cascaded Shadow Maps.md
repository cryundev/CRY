---
aliases:
---
## Cascaded Shadow Maps ( CSM )
---

Shadow Map 해상도 문제를 Frustum 분할로 해결.

<br />

<br />

## 학습 포인트
---

- Shadow Map 해상도 문제
- View Frustum을 여러 구간으로 분할
- Cascade 선택 로직

^point

<br />

<br />

## TODO
---

- [ ] Camera Frustum을 N개 구간으로 분할하는 로직 구현
    - Near/Far 분할 비율
- [ ] 각 Cascade별 Light View/Projection Matrix 계산
- [ ] Shadow Map `Texture2DArray` 생성 ( N개 슬라이스 )
- [ ] N번의 Shadow Pass 실행 ( 각 Cascade별 )
- [ ] PS에서 픽셀 깊이 기반 Cascade 선택 + 경계 블렌딩

^todo

<br />

<br />
