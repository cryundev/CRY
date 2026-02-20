---
aliases:
  - CRD11RenderTarget2D::GetRTV()
tags:
  - function
  - DX11
---
## 설명
---
**렌더 타겟 뷰의 원시 포인터를 반환**하는 함수이다.

[[RenderTarget View|RenderTargetView]]가 유효하지 않은 경우 `nullptr`을 반환한다.

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

| 타입                       | 설명                                              |
| ------------------------ | ----------------------------------------------- |
| `ID3D11RenderTargetView*` | 유효한 경우 렌더 타겟 뷰 포인터. 유효하지 않은 경우 `nullptr`. |

