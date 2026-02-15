---
aliases:
---
## 파티클 시스템
---

GPU 기반 파티클 생성, 업데이트, 렌더링.

<br />

<br />

## 학습 포인트
---

- CPU vs GPU 파티클
- Emitter 패턴
- Billboard 렌더링

^point

<br />

<br />

## TODO
---

- [ ] `CRParticle` 구조체
    - Position, Velocity, Color, Life, Size
    - 신규: `Engine/Source/Object/CRParticle.h`
- [ ] `CRParticleEmitter`
    - 생성 패턴 ( burst, continuous ), 초기화 파라미터
- [ ] GPU 파티클 업데이트 CS
    - 위치/수명 갱신, 죽은 파티클 재활용
    - 신규: `particle_update.hlsl`, 6.3 CS 인프라 활용
- [ ] Billboard 렌더링
    - 6.2 GS 활용 또는 Instancing
    - 신규: `particle_render.hlsl`
- [ ] Blend State 설정
    - Additive, Alpha Blend
    - `D3D11_BLEND_DESC`

^todo

<br />

<br />
