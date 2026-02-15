---
aliases:
---
## Specular & Emissive Mapping
---

1.1의 머티리얼 시스템 위에 텍스처 맵 레이어 추가.

<br />

<br />

## 학습 포인트
---

- Specular Map의 역할 ( 반사 강도 조절 )
- Emissive Map의 역할 ( 자체 발광 )
- 텍스처 채널 활용 최적화

^point

<br />

<br />

## TODO
---

- [ ] 텍스처 슬롯 enum 정의
    - Diffuse=t0, Normal=t1, Specular=t2, Emissive=t3
    - `ICRRHIMaterial.h` 또는 `CRD11Types.h`
- [ ] `CRD11Material`에 슬롯별 텍스처 바인딩 API
    - `SetTexture(ESlot, Path)`
- [ ] `shader.hlsl` PS에서 Specular Map으로 반사 강도 조절
    - `specularStrength = specularMap.Sample().r`
- [ ] `shader.hlsl` PS에서 Emissive Map 적용
    - 최종 색상에 가산
- [ ] 텍스처 유무에 따른 분기 처리
    - 플래그 또는 1x1 흰색/검정 기본 텍스처

^todo

<br />

<br />
