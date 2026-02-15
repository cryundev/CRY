---
aliases:
---
## SSR ( Screen Space Reflection )
---

Screen Space에서 Ray Marching 기반 실시간 반사.

<br />

<br />

## 학습 포인트
---

- Ray Marching in Screen Space
- Depth Buffer 활용
- Fresnel 효과

^point

<br />

<br />

## TODO
---

- [ ] Screen Space Ray Marching 셰이더 구현
    - Depth Buffer + Normal 입력
    - 신규: `ssr.hlsl`
- [ ] Hi-Z Depth Buffer 생성
    - 가속용, 밉맵 생성
- [ ] Reflection → CubeMap Fallback 구현 ( 레이 미스 시 )
- [ ] Fresnel 기반 반사 블렌딩
- [ ] Temporal Filtering으로 노이즈 감소 ( 선택적 )

^todo

<br />

<br />
