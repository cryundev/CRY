---
aliases:
---
## 지형 렌더링
---

Heightmap 기반 지형 생성, Texture Splatting, LOD.

<br />

<br />

## 학습 포인트
---

- Heightmap 기반 지형
- Texture Splatting
- LOD 및 최적화

^point

<br />

<br />

## TODO
---

- [ ] Heightmap 로딩
    - 그레이스케일 이미지 → 높이 배열
    - 신규: `CRTerrain.h/.cpp`
- [ ] Heightmap에서 Grid Mesh 생성 ( 정점 + 인덱스 )
- [ ] 높이 기반 Normal 계산 ( 유한 차분법 )
- [ ] Texture Splatting 셰이더
    - Blend Map + 4개 레이어 텍스처
    - 신규: `terrain.hlsl`
- [ ] Quadtree 기반 LOD 시스템
- [ ] 지형 프러스텀 컬링
    - Quadtree 노드 단위
    - 5.1 활용

^todo

<br />

<br />
