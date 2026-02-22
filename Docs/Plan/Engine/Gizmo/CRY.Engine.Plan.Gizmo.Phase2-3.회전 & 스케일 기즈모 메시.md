---
aliases:
---
## 회전 & 스케일 기즈모 메시
---

이동 기즈모에 이어 회전(링 메시)과 스케일(박스 핸들) 기즈모 메시가 추가로 필요하며, `CRGizmoMeshBuilder`에 해당 빌더가 없음. \
세 가지 모드(이동·회전·스케일) 전환과 하이라이트 정책도 함께 정의해야 한다. \
또한 현재 렌더러 제약(non-indexed)을 유지할지, 인덱스 버퍼 경로를 먼저 도입할지 선결정이 필요하다.

<br />

<br />

## 학습 포인트
---

- 원 세그먼트 분할로 납작한 링 메시를 만드는 방법 ( 내반지름·외반지름, 얇은 쿼드 스트립 )
- 단위 박스 메시 생성 ( 6면 × 2삼각형 = 12삼각형, 36 인덱스 )
- 기즈모 모드 열거형 설계와 에디터-엔진 간 모드 동기화 패턴
- 축 핸들(단일 축)과 중앙 핸들(균일 스케일)의 상호작용 UX 설계

^point

<br />

<br />

## TODO
---

- [ ] `CRGizmoMeshBuilder`에 `BuildRingTriangleList(u32 Segments, f32 Radius, f32 Thickness)` 추가
    - 내반지름·외반지름을 가진 납작한 링 메시 ( 각 세그먼트마다 얇은 쿼드 )
    - 세그먼트 수 기본값 32 추천
- [ ] `CRGizmoMeshBuilder`에 `BuildBoxTriangleList(f32 HalfSize)` 추가
    - 6면 × 2삼각형(총 12삼각형) 정점을 직접 생성
    - 스케일 핸들 끝에 배치될 작은 정육면체
- [ ] `Engine/Source/RHI/Gizmo/CRGizmoSystem.h`에 `EGizmoMode` 열거형 추가
    - `enum class EGizmoMode { Translate, Rotate, Scale, None };`
    - `void SetMode(EGizmoMode Mode)` API 추가
- [ ] `CRGizmoSystem::Initialize()`에서 회전·스케일 메시도 GPU에 업로드
    - X 링: `RotationZ(90°)` 적용 후 배치 → 빨강
    - Y 링: 기본 방향 → 초록
    - Z 링: `RotationX(90°)` 적용 후 배치 → 파랑
    - 스케일 박스: 각 축 끝점에 배치
- [ ] 스케일 모드에 중앙 균일 스케일 핸들 추가(옵션 권장)
    - `EGizmoAxis::Uniform` 축 추가 여부 결정
    - 중심 핸들 드래그 시 XYZ 동시 배율 적용
- [ ] `EngineDLL/InputAPI.cpp`에 `CR_ENGINE_API void SetGizmoMode(i32 Mode)` 추가
    - `Editor_WPF/DllWrappers/EngineAPI.Input.cs`에 대응 P/Invoke 선언
    - 에디터 툴바의 이동 / 회전 / 스케일 버튼과 연결
- [ ] 에디터/엔진 모드 enum 정합성 확보
    - C# `enum GizmoMode`와 C++ `EGizmoMode` 값을 고정 매핑
    - 매직 넘버 대신 공용 헤더 또는 명시적 변환 함수 사용
- [ ] 최적화(옵션)
    - 공통 메시 재사용 + 축별 transform/color만 교체
    - 모드 전환 시 GPU 리소스 재생성 금지(visible mask로 전환)

^todo

<br />

<br />
