---
aliases:
  - CRD11RenderTarget2D::IsValid()
tags:
  - function
  - DX11
---
## 설명
---
**렌더 타겟이 사용 가능한 상태인지 확인**하는 함수이다.

[[CRD11RenderTarget2D.GetRTV|GetRTV()]]와 [[CRD11RenderTarget2D.GetSRV|GetSRV()]]가 모두 유효한 포인터를 반환할 때만 `true`를 반환한다.

<br />

<br />

## 시그니처
---
#### Input
---
- 없음

<br />

#### Output
---

| 타입     | 설명                                                                    |
| ------ | --------------------------------------------------------------------- |
| `bool` | [[RenderTarget View|RenderTargetView]]와 [[ShaderResource View|ShaderResourceView]]가 모두 유효하면 `true`, 하나라도 없으면 `false`. |

