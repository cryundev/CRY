---
aliases:
---
## 레이 캐스팅
---

현재 엔진에는 마우스 좌표에서 월드 레이를 만드는 공용 유틸이 없음. \
레이 생성은 렌더러 내부 상태 의존보다 `GWorld->GetCamera()`의 View/Projection을 직접 사용하면 프레임 타이밍과 무관하게 안정적으로 계산할 수 있음.

<br />

<br />

## 학습 포인트
---

- NDC 좌표계에서 월드 공간 레이를 역산하는 언프로젝션(Unprojection) 수학
- ViewProjection 역행렬의 정의와 `DirectX::SimpleMath::Matrix::Invert()` 활용
- Near/Far 두 점을 언프로젝션해 방향을 얻는 방식과 단일 점 역변환 방식의 차이

^point

<br />

<br />

## TODO
---

- [ ] `Engine/Source/Core/Math/CRRay.h` 신규 파일 생성
    - `struct CRRay { CRVector Origin; CRVector Direction; };`
- [ ] `Engine/Source/Core/Math/CRRayUtil.h` / `.cpp` 신규 파일 생성
    - `CRRay CreateRayFromNDC(f32 NdcX, f32 NdcY, const CRMatrix& View, const CRMatrix& Projection)` 선언
    - Near/Far 클립 포인트 `(ndcX, ndcY, 0, 1)`, `(ndcX, ndcY, 1, 1)`를 각각 역변환
    - `origin = nearWorld`, `direction = normalize(farWorld - nearWorld)` 방식으로 계산
- [ ] `EngineDLL/InputAPI.cpp`에 좌표 변환 헬퍼 추가
    - `PixelToNDC(PixelX, PixelY, ViewportW, ViewportH, OutNdcX, OutNdcY)` 구현
    - `GRHI.GetRenderer()->GetViewportWidth/Height()`와 인자 뷰포트 크기 불일치 시 보정/가드
- [ ] `OnViewportMouseMove` / `OnViewportMouseButton`에서 `CreateRayFromNDC` 호출
    - `GWorld->GetCamera()`에서 View/Projection 획득
    - 카메라/월드 부재 시 early-out
    - 결과 `CRRay`를 `CRGizmoSystem`의 현재 입력 레이로 갱신
- [ ] 후속 최적화(옵션)
    - 매 이벤트마다 역행렬 계산하지 않도록 `CRGizmoSystem`에 `CachedInvViewProj` + dirty flag 도입
    - 카메라/프로젝션 변경 시에만 역행렬 갱신

^todo

<br />

<br />
