---
aliases:
---
## 인스턴싱 ( Instancing )
---

동일 메시 다수 렌더링 시 Draw Call 최적화.

<br />

<br />

## 학습 포인트
---

- Instanced Rendering의 장점
- Per-Instance Data 전달
- Instance ID 활용

^point

<br />

<br />

## TODO
---

- [ ] `CRInstanceData` 구조체
    - World Matrix, 머티리얼 인덱스 등
- [ ] Instance Buffer 생성/관리
    - `D3D11_BIND_VERTEX_BUFFER`, per-instance
- [ ] Input Layout 수정
    - Per-Instance 요소 추가 ( `D3D11_INPUT_PER_INSTANCE_DATA` )
    - `CRRHI.cpp`
- [ ] `DrawIndexedInstanced()` 호출 경로 추가
    - `CRD11Mesh.cpp`, 기존 `DrawIndexed`와 분기
- [ ] VS에서 `SV_InstanceID` 사용, Per-Instance World Matrix 적용
- [ ] 동일 메시 다수 배치 성능 테스트 ( 100+개 )

^todo

<br />

<br />
