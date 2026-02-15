---
aliases:
---
## SSAO ( Screen Space Ambient Occlusion )
---

Depth+Normal 버퍼를 활용한 Screen Space 기반 AO 계산.

<br />

<br />

## 학습 포인트
---

- Screen Space 기법의 장점
- Random Sampling Kernel
- Normal-Oriented Hemisphere

^point

<br />

<br />

## TODO
---

- [ ] Random Sample Kernel 생성
    - 반구 분포, 64개 샘플
    - CPU에서 생성 → CB 전달
- [ ] 4×4 Noise Texture 생성 및 타일링
- [ ] SSAO Pass 셰이더 작성
    - Depth+Normal → Occlusion Factor
    - 신규: `ssao.hlsl`
- [ ] Blur Pass로 SSAO 노이즈 제거
    - 신규: `ssao_blur.hlsl`
- [ ] 최종 라이팅에 AO Factor 적용

^todo

<br />

<br />
