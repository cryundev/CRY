---
aliases:
---
## 기즈모 렌더 패스
---

`CRD11Renderer::Initialize()`는 `CRD11ScenePass` → `CRD11CompositePass` 두 패스만 등록하며 기즈모용 별도 렌더 패스가 없음. \
기즈모는 Scene 결과 위에 합성되어야 하므로 ScenePass 이후, CompositePass 이전에 별도 패스로 그리는 것이 맞다. \
깊이 테스트는 유지하고 깊이 쓰기만 끄면(DepthWrite=0) 오클루전 감각을 유지하면서 씬 깊이 버퍼를 오염시키지 않을 수 있다.

<br />

<br />

## 학습 포인트
---

- `D3D11_DEPTH_STENCIL_DESC`로 Depth Write 비활성화 + Depth Test 활성화 설정 방법
- `ICRRHIRenderPass` 상속 구조와 `CRD11Renderer::RenderPasses` 배열에 삽입 순서의 의미
- 렌더 상태(Depth/Rasterizer/Shader Resource) 오염 방지와 패스 경계에서의 상태 복구

^point

<br />

<br />

## TODO
---

- [ ] `Engine/Source/RHI/DX11/Passes/CRD11GizmoPass.h` / `.cpp` 신규 파일 생성
    - `class CRD11GizmoPass : public CRD11RenderPass` 선언
    - `GetPassName()` → `"GizmoPass"` 반환
    - 멤버: `CRD11DepthStencilStateWPtr GizmoDepthState` (Depth Write 비활성), 필요 시 `CRD11RasterizerStateWPtr GizmoRasterState`(CullNone)
- [ ] `CRD11GizmoPass::Initialize()`에서 Depth Stencil State 생성
    - `D3D11_DEPTH_STENCIL_DESC`: `DepthEnable = TRUE`, `DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO`
    - `GD11RM.GetDepthStencilState("GizmoDepth")`로 리소스 관리자 등록
- [ ] `CRD11GizmoPass::OnPreDraw()`에서 Scene 컬러/깊이 대상 재바인딩
    - `SceneColor_RTV`, `DepthStencilView`를 리소스 이름으로 조회해 OM 바인딩
    - `GD11RP.SetDepthStencilState( GizmoDepthState.lock()->GetObjectPtr() )` 호출
- [ ] `CRD11GizmoPass::OnDrawRenderElements()` 구현 방침 확정
    - 현재 인터페이스는 `CRPackedArray<CRRenderElement>`만 전달되므로, 씬 요소를 재사용하지 않고 `CRGizmoSystem`의 draw list를 직접 조회해 렌더
    - `CRGizmoRenderElement` 별도 컨테이너를 렌더러 공용 배열에 혼합하지 않도록 분리
- [ ] `CRD11Renderer::Initialize()`에서 ScenePass 이후, CompositePass 이전에 GizmoPass 삽입
    - `Engine/Source/RHI/DX11/CRD11Renderer.cpp` 의 `Initialize()` 수정
- [ ] `Engine/Source/RHI/DX11/HLSL/gizmo.hlsl` 신규 파일 생성
    - 라이팅 없이 단색(축 색상) 출력하는 VS/PS 작성
    - 기본 Transform/ViewProjection 버퍼(`b0`,`b1`)를 그대로 사용하고, 기즈모 전용 데이터는 `cbuffer GizmoBuffer : register(b2)`로 분리
- [ ] `Engine/Source/RHI/DX11/CRD11Types.h`의 `EConstBufferSlotVS`에 `Gizmo = 2` 슬롯 추가
- [ ] 패스 종료 시 상태 정리
    - SRV/DepthState가 다음 패스에 누출되지 않도록 null 바인딩 또는 기본 상태 복원
- [ ] 최적화(옵션)
    - 선택 액터/기즈모 비활성 시 GizmoPass draw skip
    - 축별 draw call을 인스턴싱(후속 단계)로 축소

^todo

<br />

<br />
