---
aliases:
---
## PCF ( Percentage Closer Filtering )
---

Shadow Map의 Hard Shadow를 Soft Shadow로 개선.

<br />

<br />

## 학습 포인트
---

- Hard Shadow vs Soft Shadow
- PCF 샘플링 패턴
- 성능과 품질 트레이드오프

^point

<br />

<br />

## TODO
---

- [ ] Comparison Sampler State 생성
    - `D3D11_COMPARISON_LESS_EQUAL`
- [ ] `shader.hlsl`에서 3×3 PCF 커널 구현
    - `SampleCmpLevelZero` 사용
    - Shadow Map texel size를 CB로 전달
- [ ] PCF 커널 크기 조절 가능하도록 파라미터화
- [ ] Hard Shadow vs Soft Shadow 비교 테스트

^todo

<br />

<br />
