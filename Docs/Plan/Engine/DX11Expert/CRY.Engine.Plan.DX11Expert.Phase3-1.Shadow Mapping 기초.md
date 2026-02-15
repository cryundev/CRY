---
aliases:
---
## Shadow Mapping 기초
---

현재 `CRD11RenderPass` 클래스 존재하나 미사용. \
이를 활용할 기회.

<br />

<br />

## 학습 포인트
---

- Shadow Map의 원리 ( 깊이 버퍼 재사용 )
- Light Space Transformation
- Shadow Acne 문제

^point

<br />

<br />

## TODO
---

- [ ] Shadow Map용 Depth-only 텍스처 생성
    - `DXGI_FORMAT_R32_TYPELESS`, SRV + DSV 이중 생성
- [ ] Light 시점 View/Projection Matrix 계산
    - Directional Light → Ortho Projection
- [ ] Shadow Pass 구현
    - `CRD11RenderPass` 상속
    - 신규: `shadow_depth.hlsl` ( depth-only 셰이더 )
- [ ] Main Pass에서 Shadow Map을 SRV로 바인딩
    - Light Space 좌표로 깊이 비교
- [ ] Depth Bias 적용으로 Shadow Acne 해결
    - `RasterizerState.DepthBias`
- [ ] 렌더 루프를 멀티패스로 재구조화
    - Shadow Pass → Scene Pass

^todo

<br />

<br />
