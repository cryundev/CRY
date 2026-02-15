# DirectX 11 Expert Learning Curriculum

CRY 엔진 프로젝트를 통한 DirectX 11 마스터 커리큘럼.
각 단계는 독립적으로 구현 가능한 소규모 작업으로 구성되어 있으며, 이전 단계의 결과물 위에 쌓아 올린다.

---

## 현재 프로젝트 상태 (베이스라인)

구현 완료:
- [x] DX11 디바이스/스왑체인 초기화
- [x] 버텍스/인덱스 버퍼
- [x] 단일 디퓨즈 셰이더 (VS/PS 5.0)
- [x] 기본 디렉셔널 라이팅 (dot product 기반)
- [x] 텍스처 로딩 (WIC)
- [x] 깊이/스텐실 버퍼
- [x] 래스터라이저 스테이트
- [x] 상수 버퍼 시스템
- [x] FBX 모델 로딩
- [x] 카메라 시스템 (Perspective/Orthographic)
- [x] RHI 추상화 레이어

---

## Phase 1: 렌더링 기초 강화

현재 단일 셰이더/단일 머티리얼 구조를 확장하여 유연한 렌더링 기반을 만든다.

### 1.1 멀티 머티리얼 시스템
**목표**: 오브젝트마다 다른 머티리얼(색상, 텍스처, 프로퍼티)을 적용할 수 있게 한다.
**학습 포인트**:
- 상수 버퍼 설계: 머티리얼 프로퍼티를 GPU에 전달하는 구조
- 셰이더 리소스 바인딩: 텍스처 슬롯 관리
- 드로우 콜별 스테이트 변경 최소화 전략

**구현 항목**:
- [ ] `CRMaterial` 클래스 (diffuse color, specular, shininess, texture 참조)
- [ ] 머티리얼 상수 버퍼 (PS slot 추가)
- [ ] `CRPrimitiveComponent`에 머티리얼 바인딩
- [ ] 셰이더에 머티리얼 프로퍼티 반영

**핵심 DX11 API**: `ID3D11Buffer`, `Map/Unmap`, `PSSetConstantBuffers`

---

### 1.2 Phong/Blinn-Phong 라이팅
**목표**: Ambient + Diffuse + Specular 완전한 Phong 라이팅 모델 구현.
**학습 포인트**:
- 라이팅 모델의 수학적 원리 (반사 벡터, 하프 벡터)
- 월드 공간 vs 뷰 공간 라이팅 계산 차이
- 카메라 위치를 셰이더에 전달하는 패턴

**구현 항목**:
- [ ] Ambient light 상수 추가
- [ ] Specular highlight 계산 (Blinn-Phong half-vector)
- [ ] 카메라 위치 상수 버퍼
- [ ] 머티리얼별 specular power/intensity

**핵심 DX11 API**: HLSL `reflect()`, `pow()`, `normalize()`

---

### 1.3 멀티 라이트 시스템
**목표**: 여러 개의 광원(Directional, Point, Spot)을 동시에 지원한다.
**학습 포인트**:
- 구조화 버퍼(Structured Buffer) vs 상수 버퍼 배열
- 라이트 타입별 감쇠(attenuation) 공식
- 상수 버퍼 크기 제한과 설계 전략

**구현 항목**:
- [ ] Light 구조체 정의 (Type, Position, Direction, Color, Range, Cone 등)
- [ ] 라이트 배열 상수 버퍼 (최대 N개)
- [ ] Point light 감쇠 계산
- [ ] Spot light 콘 각도 계산
- [ ] 셰이더에서 동적 라이트 루프

**핵심 DX11 API**: HLSL `StructuredBuffer`, 패킹 규칙 (`packoffset`)

---

## Phase 2: 텍스처링 & 머티리얼 고급

### 2.1 Normal Mapping
**목표**: 탄젠트 공간 노말맵을 적용하여 표면 디테일을 추가한다.
**학습 포인트**:
- 탄젠트 공간(Tangent Space)과 TBN 매트릭스
- 노말맵 텍스처 포맷과 인코딩 (0~1 → -1~1)
- FBX에서 탄젠트/바이노말 추출 (이미 데이터 있음, 미사용)

**구현 항목**:
- [ ] 버텍스 포맷에 Tangent, Binormal 추가
- [ ] TBN 매트릭스 구성 (VS에서 계산)
- [ ] 노말맵 텍스처 샘플링 및 탄젠트 공간 변환
- [ ] 라이팅 계산에 노말맵 적용

**핵심 DX11 API**: 멀티 텍스처 바인딩, `PSSetShaderResources` 복수 슬롯

---

### 2.2 Specular/Emissive Map
**목표**: 스페큘러맵과 이미시브맵으로 머티리얼 표현력을 높인다.
**학습 포인트**:
- 텍스처 채널 패킹 전략 (RGB+A 활용)
- 멀티 텍스처 샘플링 패턴
- 이미시브(자체 발광) 렌더링

**구현 항목**:
- [ ] 스페큘러맵 텍스처 슬롯 추가
- [ ] 이미시브맵 텍스처 슬롯 추가
- [ ] 셰이더에서 각 맵 샘플링 및 적용
- [ ] 머티리얼 클래스에 텍스처 슬롯 관리

**핵심 DX11 API**: 다중 `SRV` 바인딩, HLSL `Texture2D` 배열

---

### 2.3 환경 매핑 (Cube Map)
**목표**: 큐브맵을 이용한 환경 반사/스카이박스를 구현한다.
**학습 포인트**:
- 큐브맵 텍스처 생성과 샘플링
- 반사 벡터 계산과 환경 매핑
- 스카이박스 렌더링 (깊이 트릭)

**구현 항목**:
- [ ] DDS/큐브맵 텍스처 로딩
- [ ] 스카이박스 메시 + 셰이더
- [ ] 환경 반사 셰이더 (reflect 벡터로 큐브맵 샘플링)
- [ ] 프레넬 효과 적용

**핵심 DX11 API**: `D3D11_TEXTURECUBE_FACE`, `TextureCube` HLSL, `SampleLevel`

---

## Phase 3: 그림자 시스템

### 3.1 기본 Shadow Mapping
**목표**: 디렉셔널 라이트의 그림자를 깊이맵 기반으로 렌더링한다.
**학습 포인트**:
- 2-pass 렌더링 개념 (섀도우 패스 + 메인 패스)
- 라이트 공간 변환 매트릭스
- 렌더 투 텍스처 (Render-to-Texture)
- 깊이 비교와 섀도우 아크네(acne) 문제

**구현 항목**:
- [ ] 섀도우맵용 깊이 전용 렌더 타겟 생성
- [ ] 라이트 뷰/프로젝션 매트릭스 계산
- [ ] 섀도우 패스 전용 셰이더 (깊이만 기록)
- [ ] 메인 패스에서 섀도우맵 샘플링 + 깊이 비교
- [ ] 바이어스(bias) 적용으로 섀도우 아크네 해결

**핵심 DX11 API**: `OMSetRenderTargets` (깊이 전용), `D3D11_COMPARISON_LESS`, DSV/SRV 전환

---

### 3.2 PCF (Percentage Closer Filtering)
**목표**: 섀도우 엣지를 부드럽게 만드는 PCF 필터링을 적용한다.
**학습 포인트**:
- 하드 섀도우 vs 소프트 섀도우
- PCF 커널 크기와 품질/성능 트레이드오프
- 비교 샘플러 (Comparison Sampler)

**구현 항목**:
- [ ] `SamplerComparisonState` 생성
- [ ] NxN PCF 커널 셰이더 구현
- [ ] 텍셀 크기 계산과 오프셋 적용

**핵심 DX11 API**: `D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT`, HLSL `SampleCmpLevelZero`

---

### 3.3 CSM (Cascaded Shadow Maps)
**목표**: 원거리까지 품질 높은 그림자를 제공하는 CSM을 구현한다.
**학습 포인트**:
- 프러스텀 분할(split) 전략
- 캐스케이드별 라이트 매트릭스 계산
- 캐스케이드 선택 및 블렌딩

**구현 항목**:
- [ ] 뷰 프러스텀을 N개 캐스케이드로 분할
- [ ] 캐스케이드별 라이트 직교 프로젝션 계산
- [ ] 텍스처 배열 또는 아틀라스로 섀도우맵 저장
- [ ] 셰이더에서 뷰 깊이 기반 캐스케이드 선택

**핵심 DX11 API**: `Texture2DArray`, `D3D11_VIEWPORT` 배열, 다중 렌더 패스

---

## Phase 4: 고급 렌더링 기법

### 4.1 디퍼드 렌더링 (Deferred Rendering)
**목표**: G-Buffer 기반 디퍼드 렌더링으로 다수의 라이트를 효율적으로 처리한다.
**학습 포인트**:
- 포워드 vs 디퍼드 렌더링 장단점
- MRT (Multiple Render Targets)
- G-Buffer 구성: Position, Normal, Albedo, Specular 등
- 라이팅 패스 설계

**구현 항목**:
- [ ] G-Buffer 렌더 타겟 생성 (3~4개 RT)
- [ ] 지오메트리 패스 셰이더 (G-Buffer에 데이터 기록)
- [ ] 라이팅 패스 셰이더 (풀스크린 쿼드, G-Buffer 읽기)
- [ ] MRT 바인딩 및 클리어
- [ ] 포워드/디퍼드 전환 가능한 구조 설계

**핵심 DX11 API**: `OMSetRenderTargets` (복수 RT), MRT 출력, `SV_Target0~3`

---

### 4.2 SSAO (Screen Space Ambient Occlusion)
**목표**: 화면 공간에서 주변 폐색을 계산하여 깊이감을 더한다.
**학습 포인트**:
- 뷰 공간 위치/노말 복원
- 반구 커널 샘플링
- 노이즈 텍스처와 타일링
- 블러 패스

**구현 항목**:
- [ ] 랜덤 샘플 커널 생성 (반구 분포)
- [ ] 노이즈 텍스처(4x4) 생성
- [ ] SSAO 셰이더 (깊이+노말로 폐색 계산)
- [ ] 블러 패스로 노이즈 제거
- [ ] 라이팅에 AO 팩터 적용

**핵심 DX11 API**: 렌더 투 텍스처 체인, 풀스크린 쿼드 렌더링

---

### 4.3 포스트 프로세싱 프레임워크
**목표**: 재사용 가능한 포스트 프로세싱 파이프라인을 구축한다.
**학습 포인트**:
- Ping-pong 버퍼 전략
- 풀스크린 삼각형/쿼드 렌더링
- HDR 렌더링과 톤 매핑

**구현 항목**:
- [ ] 포스트 프로세스 패스 베이스 클래스
- [ ] HDR 렌더 타겟 (R16G16B16A16_FLOAT)
- [ ] 톤 매핑 셰이더 (Reinhard, ACES)
- [ ] 감마 보정
- [ ] Bloom 효과 (다운샘플 → 블러 → 업샘플 → 합성)

**핵심 DX11 API**: Float 포맷 RT, `GenerateMips`, 멀티 패스 렌더링

---

## Phase 5: 최적화 기법

### 5.1 프러스텀 컬링
**목표**: 카메라 시야 밖의 오브젝트를 드로우 콜에서 제외한다.
**학습 포인트**:
- 프러스텀 6개 평면 추출
- AABB (Axis-Aligned Bounding Box) 교차 판정
- 바운딩 볼륨 계산

**구현 항목**:
- [ ] 뷰-프로젝션 매트릭스에서 프러스텀 평면 추출
- [ ] AABB 구조체 및 메시별 바운딩 박스 계산
- [ ] 프러스텀 vs AABB 교차 테스트
- [ ] 렌더 큐에서 컬링 적용

**핵심 DX11 API**: CPU 측 최적화 (DirectXMath `BoundingFrustum`)

---

### 5.2 인스턴싱 (Instanced Rendering)
**목표**: 같은 메시를 다수 렌더링할 때 드로우 콜을 1회로 줄인다.
**학습 포인트**:
- 인스턴스 버퍼 레이아웃
- 입력 레이아웃의 Per-Instance 데이터
- `DrawIndexedInstanced` 호출

**구현 항목**:
- [ ] 인스턴스 데이터 버퍼 (월드 매트릭스 배열)
- [ ] 입력 레이아웃에 인스턴스 요소 추가 (`D3D11_INPUT_PER_INSTANCE_DATA`)
- [ ] 버텍스 셰이더에서 인스턴스 매트릭스 적용
- [ ] `DrawIndexedInstanced` 호출 경로 추가

**핵심 DX11 API**: `D3D11_INPUT_ELEMENT_DESC::InputSlotClass`, `DrawIndexedInstanced`

---

### 5.3 렌더 스테이트 정렬 & 배칭
**목표**: 드로우 콜을 머티리얼/셰이더별로 정렬하여 스테이트 변경을 최소화한다.
**학습 포인트**:
- 렌더 큐 정렬 키 설계
- 스테이트 변경 비용 이해
- 불투명/반투명 분리 렌더링

**구현 항목**:
- [ ] 렌더 큐(Render Queue) 시스템
- [ ] 정렬 키: 셰이더 → 머티리얼 → 메시 순 정렬
- [ ] 불투명 큐 (앞에서 뒤로) + 반투명 큐 (뒤에서 앞으로)
- [ ] 스테이트 변경 시 이전 스테이트와 비교하여 불필요한 바인딩 스킵

**핵심 DX11 API**: 렌더링 아키텍처 설계 (API 자체보다 구조적 최적화)

---

## Phase 6: 고급 셰이더 기법

### 6.1 PBR (Physically Based Rendering)
**목표**: 물리 기반 렌더링으로 사실적인 머티리얼 표현을 구현한다.
**학습 포인트**:
- 미세면(Microfacet) 이론
- Cook-Torrance BRDF
- 메탈릭-러프니스 워크플로우
- 에너지 보존

**구현 항목**:
- [ ] PBR 머티리얼 (Albedo, Metallic, Roughness, AO 맵)
- [ ] Normal Distribution Function (GGX/Trowbridge-Reitz)
- [ ] Geometry Function (Smith's Schlick-GGX)
- [ ] Fresnel Function (Schlick approximation)
- [ ] IBL (Image-Based Lighting) - 환경맵 기반 간접 조명

**핵심 DX11 API**: 고급 HLSL 수학, 큐브맵 convolution, BRDF LUT 텍스처

---

### 6.2 지오메트리 셰이더 활용
**목표**: GS를 사용한 비주얼 디버깅 도구와 이펙트를 구현한다.
**학습 포인트**:
- 지오메트리 셰이더 파이프라인 위치와 역할
- 프리미티브 생성/삭제/변형
- 성능 특성과 적절한 사용 사례

**구현 항목**:
- [ ] 와이어프레임 오버레이 (삼각형 → 삼각형 + 라인)
- [ ] 노말/탄젠트 시각화 (버텍스 → 라인 생성)
- [ ] 빌보드 파티클 (포인트 → 쿼드 확장)
- [ ] Stream Output으로 데이터 캡처

**핵심 DX11 API**: `GSSetShader`, `ID3D11GeometryShader`, Stream Output

---

### 6.3 컴퓨트 셰이더 기초
**목표**: GPGPU 컴퓨팅의 기초를 이해하고 간단한 활용 사례를 구현한다.
**학습 포인트**:
- 스레드/스레드 그룹/디스패치 개념
- UAV (Unordered Access View)
- 공유 메모리(Shared Memory) 최적화
- GPU vs CPU 작업 분담 전략

**구현 항목**:
- [ ] 간단한 이미지 처리 (그레이스케일 변환, 블러)
- [ ] 파티클 시뮬레이션 (위치/속도 업데이트)
- [ ] GPU 기반 프러스텀 컬링
- [ ] 히스토그램/리덕션 연산

**핵심 DX11 API**: `Dispatch`, `ID3D11UnorderedAccessView`, `RWTexture2D`, `groupshared`

---

## Phase 7: 고급 이펙트

### 7.1 파티클 시스템
**목표**: GPU 기반 파티클 시스템으로 이펙트를 렌더링한다.
**학습 포인트**:
- 파티클 라이프사이클 관리
- GPU 파티클 시뮬레이션 (CS)
- Append/Consume 버퍼
- 알파 블렌딩과 정렬

**구현 항목**:
- [ ] 파티클 데이터 구조 (Position, Velocity, Life, Size, Color)
- [ ] Emit/Update 컴퓨트 셰이더
- [ ] Append/Consume 버퍼로 파티클 풀 관리
- [ ] 블렌드 스테이트 (Additive, Alpha Blend)
- [ ] GS 빌보드 렌더링

**핵심 DX11 API**: `AppendStructuredBuffer`, `D3D11_BLEND_DESC`, `OMSetBlendState`

---

### 7.2 테셀레이션
**목표**: 하드웨어 테셀레이션으로 동적 LOD를 구현한다.
**학습 포인트**:
- 테셀레이션 파이프라인 (HS → Tessellator → DS)
- 테셀레이션 팩터 계산 (거리 기반)
- 디스플레이스먼트 매핑

**구현 항목**:
- [ ] Hull Shader (테셀레이션 팩터 출력)
- [ ] Domain Shader (테셀레이트된 버텍스 위치 계산)
- [ ] 거리 기반 동적 테셀레이션 팩터
- [ ] 하이트맵 디스플레이스먼트

**핵심 DX11 API**: `ID3D11HullShader`, `ID3D11DomainShader`, `D3D11_PRIMITIVE_TOPOLOGY_*_CONTROL_POINT_PATCHLIST`

---

### 7.3 스크린 스페이스 반사 (SSR)
**목표**: 화면 공간에서 실시간 반사를 계산한다.
**학습 포인트**:
- 레이 마칭 알고리즘
- 뷰 공간 반사 벡터
- 히트 판정과 바이너리 서치 정밀화
- 페이드 아웃과 폴백 전략

**구현 항목**:
- [ ] 뷰 공간 노말/위치로 반사 방향 계산
- [ ] 화면 공간 레이 마칭 구현
- [ ] 바이너리 서치 리파인먼트
- [ ] 엣지 페이드 및 환경맵 폴백

**핵심 DX11 API**: 풀스크린 포스트 프로세스, 깊이 버퍼 읽기

---

## Phase 8: 애니메이션 & 지형

### 8.1 스켈레탈 애니메이션
**목표**: 본(bone) 기반 스켈레탈 애니메이션을 GPU에서 수행한다.
**학습 포인트**:
- 본 계층 구조와 바인드 포즈
- 스키닝 매트릭스 계산 (본 → 월드)
- 버텍스별 본 가중치
- FBX 애니메이션 데이터 추출

**구현 항목**:
- [ ] 본 구조체와 스켈레톤 로딩 (FBX)
- [ ] 애니메이션 클립 로딩 및 키프레임 보간
- [ ] 스키닝 상수 버퍼 (본 매트릭스 배열)
- [ ] 버텍스 셰이더에서 GPU 스키닝
- [ ] 애니메이션 블렌딩 (lerp/slerp)

**핵심 DX11 API**: 대형 상수 버퍼, `StructuredBuffer<matrix>` for bone matrices

---

### 8.2 지형 렌더링
**목표**: 하이트맵 기반 대규모 지형을 효율적으로 렌더링한다.
**학습 포인트**:
- 하이트맵에서 메시 생성
- 멀티 텍스처 스플래팅
- LOD (Level of Detail) 시스템
- 노말 계산 (하이트맵 기울기)

**구현 항목**:
- [ ] 하이트맵 로딩 및 그리드 메시 생성
- [ ] 텍스처 스플래팅 셰이더 (블렌드맵 기반)
- [ ] 청크 기반 LOD 시스템
- [ ] 테셀레이션 기반 지형 LOD (Phase 7.2 활용)

**핵심 DX11 API**: 대형 버텍스 버퍼, 텍스처 배열, 동적 인덱스 버퍼

---

## 진행 방법

1. **각 단계는 독립된 브랜치에서 작업** → 완료 후 메인에 병합
2. **작은 단위로 구현/테스트** → 각 체크박스 항목이 하나의 작업 단위
3. **구현 전 이론 학습** → 각 단계의 "학습 포인트"를 먼저 이해
4. **시각적 검증** → 각 단계마다 렌더링 결과를 눈으로 확인
5. **코드 리뷰** → 구현 후 DX11 API 사용이 올바른지 점검

## 권장 진행 순서

```
Phase 1 (기초 강화)  ──→  Phase 2 (텍스처링)  ──→  Phase 3 (그림자)
                                                        │
Phase 5 (최적화)  ←──  Phase 4 (고급 렌더링)  ←────────┘
       │
       └──→  Phase 6 (고급 셰이더)  ──→  Phase 7 (이펙트)  ──→  Phase 8 (애니메이션/지형)
```

Phase 1~3은 순서대로 진행. Phase 4 이후는 관심사에 따라 순서 조정 가능.
