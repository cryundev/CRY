---
aliases:
---
## 멀티 라이트 시스템
---

현재 하드코딩된 단일 Directional Light → 구조화된 멀티 라이트로 전환.

<br />

<br />

## 학습 포인트
---

- Directional, Point, Spot Light 구현
- Light Attenuation ( 감쇠 ) 계산
- 다중 광원 처리 최적화

^point

<br />

<br />

## TODO
---

- [x] `CRLight` 구조체 정의
    - Type, Position, Direction, Color, Range, InnerAngle, OuterAngle
    - 신규: `Engine/Source/RHI/CRLight.h`, HLSL과 레이아웃 일치
- [x] Light 배열 Constant Buffer 정의 ( 최대 N개 )
    - `ECPS` enum에 `Lights` 추가 ( `CRD11Types.h`, `CRD11Renderer.h` )
- [x] `CRD11Renderer`에 Light 관리 API 추가
    - AddLight, RemoveLight, UpdateLight
    - 기존 하드코딩 제거
- [x] `shader.hlsl`에 Directional Light 루프 처리
    - 기존 단일 라이트 → for 루프
- [x] Point Light 감쇠 공식 구현
    - `1 / (constant + linear*d + quadratic*d²)`
- [x] Spot Light 구현
    - 내부/외부 각도, `smoothstep` 감쇠
- [ ] 여러 종류의 라이트를 동시 배치하여 렌더링 테스트

^todo

<br />

<br />
