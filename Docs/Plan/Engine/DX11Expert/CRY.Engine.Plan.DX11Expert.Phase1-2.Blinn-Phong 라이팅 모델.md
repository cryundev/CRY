---
aliases:
---
## Blinn-Phong 라이팅 모델
---

현재 셰이더는 단순 NdotL 디퓨즈만 수행. \
Specular 계산 없음.

<br />

<br />

## 학습 포인트
---

- Phong vs Blinn-Phong 차이
- Half Vector 기반 Specular 계산
- 라이팅 수식의 최적화

^point

<br />

<br />

## TODO
---

- [ ] PS에 카메라 위치 전달을 위한 CB 추가
    - `ECPS` enum에 `CameraPosition` 추가 ( `CRD11Types.h`, `CRD11Renderer.cpp` )
- [ ] `CRD11Renderer::Draw()`에서 카메라 위치 CB 갱신
- [ ] `shader.hlsl`에 Blinn-Phong Specular 계산 구현
- [ ] Ambient + Diffuse + Specular 조합 구현 
    - `MaterialBuffer`의 색상 값 활용
- [ ] Shininess 값 변경 테스트

^todo

<br />

<br />
