# DX11 Rendering Architecture Review

## Review Date: 2026-02-15
## Purpose: DX11 Expert 커리큘럼 진행 전 구조 점검

---

## 현재 구현 상태

### 구현 완료
- DX11 디바이스/스왑체인 (Feature Level auto-select, 1920x1080, no MSAA)
- 단일 Diffuse 셰이더 (VS/PS 5.0, shader.hlsl)
- 버텍스 포맷: Position(float3) + TexCoord(float2) + Normal(float3)
- 상수 버퍼: Transform(VS b0), ViewProjection(VS b1), LightDirection(PS b0), LightColor(PS b1)
- 깊이/스텐실 버퍼 (D3D11_COMPARISON_LESS, 하드코딩)
- 래스터라이저: CULL_BACK, FILL_SOLID, 하드코딩
- WIC 텍스처 로딩, FBX 모델 로딩
- 카메라 (Perspective/Orthographic)
- ImGUI 통합

### 미구현
- 머티리얼 시스템, 멀티 셰이더, 멀티 라이트
- 블렌드 스테이트 (투명도 불가)
- MRT (Multiple Render Targets)
- 렌더 투 텍스처 (포스트프로세싱/섀도우맵 불가)
- 드로우 콜 정렬/배칭
- 인스턴싱, 프러스텀 컬링
- GS/HS/DS/CS 셰이더 (인프라만 존재, 구현 없음)

---

## 구조적 강점 (유지)

1. **CRD11RenderingPipeline** - 파이프라인 스테이트 트래커, 슬롯별 관리, 전 셰이더 스테이지 지원
2. **CRD11ResourceManager** - 매크로 기반 14종 리소스 타입 일관 관리
3. **CRD11BindingConstantBuffer<T>** - 타입 안전 상수 버퍼, 슬롯+스테이지 캡슐화
4. **ED11RenderingPipelineStage enum** - VS/GS/DS/HS/CS/PS 확장성
5. **RHI 추상화 레이어** - ICRRHIRenderer, ICRRHIMesh 인터페이스

---

## 구조적 문제 (개선 필요)

### P1 (커리큘럼 진행 전 필수)

#### 1. CRD11Mesh가 지오메트리+셰이더+텍스처 전부 소유
- **파일**: `Engine/Source/RHI/DX11/CRD11Mesh.h:14-22`
- **문제**: 같은 메시에 다른 머티리얼 적용 불가, 머티리얼 공유 불가
- **해결**: Mesh(VB/IB만) + Material(셰이더+텍스처+상수) 분리
- **영향**: ICRRHIMesh 인터페이스, CRPrimitiveComponent 변경 필요
- **차단**: Phase 1.1 (멀티 머티리얼)

#### 2. 셰이더 Normal 월드 변환 누락
- **파일**: `Engine/Source/RHI/DX11/HLSL/shader.hlsl:28`
- **문제**: `output.normal = normal;` → 로컬 공간 그대로, 회전 시 라이팅 깨짐
- **해결**: `output.normal = mul(normal, (float3x3)transform);` 또는 역전치 매트릭스
- **차단**: Phase 1.2 (Phong 라이팅)

### P2 (해당 Phase 진입 전 필요)

#### 3. Renderer가 World에 직접 의존
- **파일**: `Engine/Source/RHI/DX11/CRD11Renderer.cpp:5,116`
- **문제**: `#include "Engine.h"`, `GWorld->GetCamera()` 직접 접근
- **해결**: Draw()에 ViewMatrix/ProjectionMatrix를 파라미터로 전달
- **차단**: Phase 3.1 (Shadow Mapping - 라이트 뷰 매트릭스 필요)

#### 4. MRT 미지원 (단일 RenderTarget)
- **파일**: `CRD11RenderingPipeline.h:42`, `CRD11RenderingPipeline.cpp:135`
- **문제**: `OMSetRenderTargets( 1, ... )` 하드코딩
- **해결**: RT 배열(최대 8개) + 뷰포트 배열 지원
- **차단**: Phase 3.1 (Shadow Map RT), Phase 4.1 (Deferred G-Buffer)

#### 5. ResourceManager 자동 빈 리소스 생성
- **파일**: `CRD11ResourceManager.h:57-59`
- **문제**: 없는 이름 요청 시 초기화 안 된 빈 객체 반환, 에러 없음
- **해결**: Find(검색용, nullptr 반환) / GetOrCreate(생성용) 분리

### P3 (성능/품질 개선)

#### 6. weak_ptr 중복 lock()
- **파일**: `CRD11Renderer.cpp:120-123`, `CRD11Mesh.cpp:48-84`
- **문제**: 프레임당 오브젝트 x 6~8회 불필요한 ref count
- **해결**: lock() 결과를 shared_ptr로 캐시

#### 7. InitializeMaterial() 하드코딩
- **파일**: `CRD11Mesh.cpp:33-38`
- **문제**: "Diffuse" 셰이더, "../Asset/cryun_icon.png" 경로 하드코딩
- **해결**: Material 분리 시 자연히 해결

#### 8. 깊이 스테이트/래스터라이저 하드코딩
- **파일**: `CRD11DepthStencilBuffer.cpp`, `CRD11Renderer.cpp:50-55`
- **문제**: 투명 오브젝트(depth write off), 섀도우 캐스터 등 커스텀 불가
- **해결**: 여러 스테이트 프리셋 또는 머티리얼별 설정

---

## 권장 리팩토링 순서 (커리큘럼 연계)

```
[리팩토링 1] Mesh/Material 분리  ←── Phase 1.1 전 필수
[리팩토링 2] Normal 월드 변환    ←── Phase 1.2 전 필수
[리팩토링 3] Renderer World 디커플링 ←── Phase 3.1 전 필요
[리팩토링 4] MRT 지원            ←── Phase 3.1 전 필요
[리팩토링 5] ResourceManager 개선 ←── 아무 때나 (디버깅 편의)
```

---

## 커리큘럼 문서 위치
- `DX11_LEARNING_CURRICULUM.md` (프로젝트 루트)

## 관련 파일 빠른 참조
- 렌더러: `Engine/Source/RHI/DX11/CRD11Renderer.h/.cpp`
- 메시: `Engine/Source/RHI/DX11/CRD11Mesh.h/.cpp`
- 파이프라인: `Engine/Source/RHI/DX11/CRD11RenderingPipeline.h/.cpp`
- 리소스매니저: `Engine/Source/RHI/DX11/CRD11ResourceManager.h/.cpp`
- 상수버퍼: `Engine/Source/RHI/DX11/CRD11BindingConstantBuffer.h`
- 텍스처: `Engine/Source/RHI/DX11/CRD11ShaderResourceTexture.h/.cpp`
- 셰이더: `Engine/Source/RHI/DX11/HLSL/shader.hlsl`
- 프리미티브컴포넌트: `Engine/Source/Object/Component/CRPrimitiveComponent.h/.cpp`
- RHI인터페이스: `Engine/Source/RHI/CRRHI.h/.cpp`
- 메시인터페이스: `Engine/Source/RHI/ICRRHIMesh.h`
- 렌더러인터페이스: `Engine/Source/RHI/ICRRHIRenderer.h`
- 글로벌: `Engine/Source/RHI/DX11/CRD11.h/.cpp` (GD11, GD11RP, GD11RM)
