---
aliases:
---
## 페이즈
---
#### 공통 선행 정리
---
- 에디터 선택 상태는 현재 `ProjectLayoutView`/`ActorView.Instance.DataContext` 중심으로 관리되므로, 뷰포트 피킹 결과를 이 경로와 동기화하는 선택 서비스가 먼저 필요함
- 입력 브리지는 NDC 직접 전달보다 `픽셀 좌표 + 뷰포트 크기`를 넘기고 엔진에서 변환하는 방식이 DPI/리사이즈 오차에 안전함
- 레이 생성은 렌더러 내부 캐시 의존보다 `GWorld->GetCamera()` 기준 언프로젝션을 우선으로 하고, 필요 시 역행렬 캐시를 추가 최적화로 적용

<br />

#### 마우스 입력 브리지
---
- [[CRY.Engine.Plan.Gizmo.Phase1-1.마우스 입력 브리지]]

###### Point
![[CRY.Engine.Plan.Gizmo.Phase1-1.마우스 입력 브리지#^point]]

<br />

#### 레이 캐스팅
---
- [[CRY.Engine.Plan.Gizmo.Phase1-2.레이 캐스팅]]

###### Point
![[CRY.Engine.Plan.Gizmo.Phase1-2.레이 캐스팅#^point]]

<br />

#### 오브젝트 피킹
---
- [[CRY.Engine.Plan.Gizmo.Phase1-3.오브젝트 피킹]]

###### Point
![[CRY.Engine.Plan.Gizmo.Phase1-3.오브젝트 피킹#^point]]

<br />

#### 기즈모 렌더 패스
---
- [[CRY.Engine.Plan.Gizmo.Phase2-1.기즈모 렌더 패스]]

###### Point
![[CRY.Engine.Plan.Gizmo.Phase2-1.기즈모 렌더 패스#^point]]

<br />

#### 이동 기즈모 메시
---
- [[CRY.Engine.Plan.Gizmo.Phase2-2.이동 기즈모 메시]]

###### Point
![[CRY.Engine.Plan.Gizmo.Phase2-2.이동 기즈모 메시#^point]]

<br />

#### 회전 & 스케일 기즈모 메시
---
- [[CRY.Engine.Plan.Gizmo.Phase2-3.회전 & 스케일 기즈모 메시]]

###### Point
![[CRY.Engine.Plan.Gizmo.Phase2-3.회전 & 스케일 기즈모 메시#^point]]

<br />

#### 축 히트 테스트
---
- [[CRY.Engine.Plan.Gizmo.Phase3-1.축 히트 테스트]]

###### Point
![[CRY.Engine.Plan.Gizmo.Phase3-1.축 히트 테스트#^point]]

<br />

#### 이동 드래그 수학
---
- [[CRY.Engine.Plan.Gizmo.Phase3-2.이동 드래그 수학]]

###### Point
![[CRY.Engine.Plan.Gizmo.Phase3-2.이동 드래그 수학#^point]]

<br />

#### 회전 & 스케일 드래그 수학
---
- [[CRY.Engine.Plan.Gizmo.Phase3-3.회전 & 스케일 드래그 수학]]

###### Point
![[CRY.Engine.Plan.Gizmo.Phase3-3.회전 & 스케일 드래그 수학#^point]]

<br />
