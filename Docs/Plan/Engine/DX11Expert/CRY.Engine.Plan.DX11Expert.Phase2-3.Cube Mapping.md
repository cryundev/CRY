---
aliases:
---
## Cube Mapping ( 환경 매핑 )
---

Skybox 렌더링 및 환경 반사 구현.

<br />

<br />

## 학습 포인트
---

- Cube Map 구조 이해
- Reflection Vector 계산
- Skybox 렌더링

^point

<br />

<br />

## TODO
---

- [ ] Cube Map 텍스처 로딩 기능 구현
    - DDS / 6면 개별 이미지
    - 신규: `CRD11TextureCube.h/.cpp`
- [ ] Skybox용 셰이더 작성
    - VS: 카메라 위치 고정, PS: CubeMap 샘플링
    - 신규: `skybox.hlsl`
- [ ] Skybox 메시 생성 및 렌더링
    - 단위 큐브, 깊이 1.0 고정, 가장 먼저 렌더
    - DepthFunc ≤ 사용
- [ ] 환경 반사 구현
    - Reflection Vector로 CubeMap 샘플링
    - 머티리얼의 Reflectivity 파라미터

^todo

<br />

<br />
