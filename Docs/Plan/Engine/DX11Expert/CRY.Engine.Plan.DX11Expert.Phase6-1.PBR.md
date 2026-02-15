---
aliases:
---
## PBR ( Physically Based Rendering )
---

Blinn-Phong에서 물리 기반 렌더링으로 전환.

<br />

<br />

## 학습 포인트
---

- Metallic-Roughness Workflow
- BRDF ( Bidirectional Reflectance Distribution Function )
- IBL ( Image Based Lighting )

^point

<br />

<br />

## TODO
---

- [ ] PBR 머티리얼 파라미터 CB 정의
    - Albedo, Metallic, Roughness, AO
    - `ICRRHIMaterial.h`, `CRD11Material.h/.cpp`
- [ ] PBR 셰이더 작성
    - Cook-Torrance BRDF ( D: GGX, F: Schlick, G: Smith )
    - 신규: `pbr.hlsl`
- [ ] Metallic/Roughness 텍스처 맵 지원
    - 텍스처 슬롯 t4, t5
- [ ] IBL Diffuse Irradiance Map + Specular Pre-filtered Map 생성
    - Compute Shader 또는 오프라인
- [ ] Environment BRDF LUT 생성 ( 2D 텍스처 )
- [ ] PBR 렌더링 결과 검증
    - 금속/비금속, 거칠기 변화

^todo

<br />

<br />
