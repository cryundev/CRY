---
aliases:
---
## 포스트 프로세싱
---

HDR Render Target 기반 후처리 효과 체인.

<br />

<br />

## 학습 포인트
---

- Render Target Ping-Pong
- Bloom, HDR Tone Mapping
- Color Grading

^point

<br />

<br />

## TODO
---

- [ ] Fullscreen Quad 렌더링 유틸리티 구현
    - 재사용 가능한 공용 시스템
    - 신규: `CRD11FullscreenQuad.h/.cpp`
- [ ] HDR Render Target 사용
    - `DXGI_FORMAT_R16G16B16A16_FLOAT`
    - 기존 백버퍼 대신 HDR RT에 렌더링
- [ ] Bright Pass Filter 셰이더 ( Bloom 추출 )
    - 신규: `bloom_extract.hlsl`
- [ ] Gaussian Blur 셰이더
    - 2-pass separable, 수평/수직 분리
    - 신규: `gaussian_blur.hlsl`
- [ ] Tone Mapping 셰이더
    - Reinhard / ACES Filmic
    - 신규: `tonemap.hlsl`
- [ ] Post-Processing 체인 시스템
    - 순서대로 패스 실행, Ping-Pong RT

^todo

<br />

<br />
