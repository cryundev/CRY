---
aliases:
---
## 멀티 머티리얼 시스템
---

현재 `CRD11Material`은 셰이더+텍스처 바인딩만 수행. \
머티리얼 파라미터( 색상, 강도 등 ) 개념 없음.

<br />

<br />

## 학습 포인트
---

- 머티리얼 데이터 구조 설계
- Constant Buffer를 통한 머티리얼 파라미터 전달
- 머티리얼별 셰이더 바인딩

^point

<br />

<br />

## TODO
---

- [ ] `CRMaterialProperties` 구조체 정의
    - Diffuse Color, Specular Color, Shininess, Emissive
- [ ] Material용 Constant Buffer 생성
    - `ECPS` enum에 `Material` 슬롯 추가 ( `CRD11Types.h` )
- [ ] `CRD11Material::SetInRenderingPipeline()`에서 머티리얼 CB 바인딩 추가
- [ ] `shader.hlsl` PS에 `MaterialBuffer` cbuffer 추가
    - 디퓨즈 계산에 머티리얼 색상 적용
- [ ] `CRPrimitiveComponent`에서 머티리얼 프로퍼티 설정 API 추가
- [ ] 여러 오브젝트에 서로 다른 머티리얼 적용하여 렌더링 테스트

^todo

<br />

<br />
