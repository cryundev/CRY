---
aliases:
---
## Deferred Rendering
---

Forward 렌더링의 멀티 라이트 비용 문제를 G-Buffer 기반으로 해결.

<br />

<br />

## 학습 포인트
---

- Forward vs Deferred 차이
- G-Buffer 구조 ( Position, Normal, Albedo, Specular 등 )
- Light Pass 최적화

^point

<br />

<br />

## TODO
---

- [ ] G-Buffer 텍스처 생성
    - Position, Normal, Albedo+Specular
    - `OMSetRenderTargets()` MRT 설정
- [ ] G-Buffer Pass용 셰이더 작성
    - VS: 기존, PS: MRT 출력
    - 신규: `gbuffer.hlsl`
- [ ] Light Pass용 Fullscreen Quad 메시 + 셰이더 작성
    - 신규: `deferred_light.hlsl`
- [ ] 렌더 루프 재구조화
    - G-Buffer Pass → Light Pass → 최종 출력
- [ ] Forward Rendering과 전환 가능한 구조 ( 플래그 )

^todo

<br />

<br />
