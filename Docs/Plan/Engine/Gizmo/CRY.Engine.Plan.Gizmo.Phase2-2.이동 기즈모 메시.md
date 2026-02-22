---
aliases:
---
## 이동 기즈모 메시
---

현재 `CRPrimitiveAsset`과 `CRD11Mesh`로 메시 렌더링 파이프라인은 완성되어 있으나, 기즈모용 화살표(실린더 + 콘) 메시를 절차적으로 생성하는 빌더가 없음. \
현재 `CRD11Mesh`는 사실상 non-indexed 경로만 사용하므로(인덱스 버퍼 업로드 미구현), 기즈모 메시는 우선 삼각형 리스트 정점을 직접 생성하는 방식이 안전하다. \
또한 거리만 비례시키는 스케일 대신 FOV/뷰포트 높이를 포함한 식으로 화면상 고정 크기를 유지해야 한다.

<br />

<br />

## 학습 포인트
---

- 절차적 실린더·콘 메시 생성 ( 세그먼트 분할, `cos`/`sin` 루프로 정점 배치, 인덱스 생성 )
- 스크린 스페이스 일정 크기 유지: `FOV`, `viewportHeight`, `distance`를 함께 고려한 스케일 계산
- `CRPrimitiveAsset`의 `Positions`, `Normals`, `Indices` 배열에 데이터를 채우는 워크플로우
- 렌더러 제약(non-indexed) 하에서 triangle list 확장 정점 생성 전략

^point

<br />

<br />

## TODO
---

- [ ] `Engine/Source/RHI/Gizmo/CRGizmoMeshBuilder.h` / `.cpp` 신규 파일 생성
    - `static CRPrimitiveAsset BuildArrowTriangleList(u32 Segments, f32 ShaftLength, f32 ShaftRadius, f32 ConeLength, f32 ConeRadius)` 구현
    - 현재 렌더러 제약에 맞춰 인덱스 기반이 아닌 확장 정점(triangle list) 생성
    - 실린더: 측면 + 캡, 콘: 측면 + 바닥 포함
- [ ] 실린더 정점 생성 구현
    - `for (u32 i = 0; i < Segments; ++i)` 루프로 `cos(2π*i/N)`, `sin(2π*i/N)` 계산
    - 각 세그먼트마다 삼각형 2개를 직접 정점으로 푸시
- [ ] `Engine/Source/RHI/Gizmo/CRGizmoSystem.h` / `.cpp` 신규 파일 생성
    - 공통 화살표 메시 1개 + 축별 월드 변환(X/Y/Z) 재사용 구조로 구성
    - `Initialize()`: `CRGizmoMeshBuilder::BuildArrowTriangleList()` 호출 후 GPU 업로드
    - `SetPivot(const CRVector& WorldPos)`: 다음 프레임의 기즈모 위치 설정
- [ ] `gizmo.hlsl` VS에 스크린 스페이스 스케일 보정 추가
    - `cbuffer GizmoBuffer`에 `float3 PivotWorldPos`, `float GizmoPixelSize`, `float ViewportHeight`, `float ProjM22` 추가
    - `scale = (2 * dist / ProjM22) * (GizmoPixelSize / ViewportHeight)` 식 적용
    - 정점 로컬 좌표에 `scale` 적용 후 View/Projection 변환
- [ ] `CRD11GizmoPass::OnPreDraw()`에서 `CRGizmoSystem`으로부터 기즈모 월드 행렬·색상을 받아 Constant Buffer 업데이트
    - X축: `transform = RotationY(-90°) * Translation(pivot)`, Color = `(1,0,0,1)`
    - Y축: `transform = Identity * Translation(pivot)`, Color = `(0,1,0,1)`
    - Z축: `transform = RotationX(90°) * Translation(pivot)`, Color = `(0,0,1,1)`
- [ ] `CRPrimitiveAsset` 채우기 규칙 명시
    - `VertexCount`, `Positions`, `Normals`, `UVs` 길이를 반드시 동일하게 맞춤
    - 기즈모 셰이더가 UV를 쓰지 않아도 더미 UV `(0,0)`을 채워 현재 `CRVertex::LoadFromPrimitiveData`와 호환
- [ ] 최적화(옵션)
    - 세그먼트별 `sin/cos` lookup table 캐시
    - 카메라-피벗 거리 변화가 임계값 이내면 스케일 재계산 스킵

^todo

<br />

<br />
