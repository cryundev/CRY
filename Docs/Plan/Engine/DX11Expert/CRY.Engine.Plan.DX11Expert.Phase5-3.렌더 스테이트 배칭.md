---
aliases:
---
## 렌더 스테이트 배칭
---

State Change 최소화를 위한 렌더 큐 시스템.

<br />

<br />

## 학습 포인트
---

- State Change의 비용
- Render Queue 정렬
- Material Batching

^point

<br />

<br />

## TODO
---

- [ ] `CRRenderQueue` 클래스
    - 렌더 요소 수집 + 정렬
    - 신규: `Engine/Source/RHI/CRRenderQueue.h/.cpp`
- [ ] Sort Key 설계
    - Shader ID | Material ID | Depth
    - 64비트 정수 키
- [ ] 불투명/투명 오브젝트 분리
    - 불투명: front→back, 투명: back→front
- [ ] `CRD11Renderer::Draw()`를 Queue 기반으로 재구조화
    - 직접 루프 → Queue 소비
- [ ] `CRD11RenderingPipeline`에 Dirty 상태 추적 추가
    - 중복 상태 변경 방지
- [ ] ImGUI에 Draw Call / State Change 횟수 표시

^todo

<br />

<br />
