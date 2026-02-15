---
aliases:
---
## Normal Mapping
---

현재 `CRVertex`에 Tangent/Bitangent 없음. \
Normal Map 미지원.

<br />

<br />

## 학습 포인트
---

- Tangent Space 이해
- Normal Map의 구조 ( RGB → XYZ )
- TBN Matrix 계산

^point

<br />

<br />

## TODO
---

- [ ] `CRVertex`에 `Tangent`, `Bitangent` 필드 추가
    - float3 × 2
- [ ] Input Layout 갱신
    - `TANGENT`, `BITANGENT` semantic 추가 ( `CRRHI.cpp` )
- [ ] FBX 로더에서 Tangent/Bitangent 데이터 추출 또는 CPU에서 계산
    - `CRFbxLoader.cpp`
- [ ] `CRD11Material`에 Normal Map 텍스처 슬롯 추가 ( t1 )
- [ ] `shader.hlsl`에 TBN 기반 Normal Mapping 구현
    - VS에서 TBN Matrix 계산
    - PS에서 Normal Map 샘플링 후 Tangent Space → World Space 변환
- [ ] Normal Map 적용 전/후 비교 테스트

^todo

<br />

<br />
