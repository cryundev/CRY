---
aliases:
---
## 지오메트리 셰이더
---

Geometry Shader 스테이지 추가 및 활용.

<br />

<br />

## 학습 포인트
---

- Geometry Shader의 역할
- Primitive Expansion ( Point → Quad )
- Normal/Tangent 시각화

^point

<br />

<br />

## TODO
---

- [ ] `CRD11GeometryShader` 리소스 래퍼 + 리소스 매니저 등록
    - `CRD11VertexShader` 패턴 따르기
- [ ] `CRD11RenderingPipeline::SetGeometryShader()` 추가
- [ ] Normal 시각화 GS 작성
    - 삼각형 입력 → 라인 출력
    - 신규: `debug_normal.hlsl`
- [ ] Billboard GS 작성
    - Point → Quad 확장
    - 신규: `billboard.hlsl`

^todo

<br />

<br />
