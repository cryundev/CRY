---
aliases:
---
## 컴퓨트 셰이더
---

GPU Compute 파이프라인 추가 및 범용 계산 활용.

<br />

<br />

## 학습 포인트
---

- GPU Compute의 활용
- Thread Group 구조
- UAV ( Unordered Access View )

^point

<br />

<br />

## TODO
---

- [ ] `CRD11ComputeShader` 리소스 래퍼 + 리소스 매니저 등록
- [ ] `CRD11RenderingPipeline::SetComputeShader()`, `Dispatch()` 추가
- [ ] UAV + Structured Buffer 래퍼
- [ ] 간단한 CS 테스트
    - 텍스처 밝기 조절 등
    - 신규: `test_compute.hlsl`
- [ ] Post-Process를 CS로 구현
    - Bloom 또는 Blur
    - 4.3과 비교

^todo

<br />

<br />
