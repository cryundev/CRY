---
aliases:
---
## 테셀레이션
---

Hull/Domain Shader 스테이지 추가 및 적응형 LOD.

<br />

<br />

## 학습 포인트
---

- Hull Shader, Domain Shader
- Tessellation Factor 제어
- Adaptive LOD

^point

<br />

<br />

## TODO
---

- [ ] `CRD11HullShader`, `CRD11DomainShader` 리소스 래퍼
- [ ] `CRD11RenderingPipeline`에 HS/DS 바인딩 추가
- [ ] 기본 테셀레이션 셰이더 작성 ( 균일 분할 )
    - 신규: `tessellation.hlsl`
- [ ] 카메라 거리 기반 LOD 테셀레이션 팩터 조절
- [ ] 지형 메시에 적용하여 디테일 향상 테스트
    - 8.2와 연계

^todo

<br />

<br />
