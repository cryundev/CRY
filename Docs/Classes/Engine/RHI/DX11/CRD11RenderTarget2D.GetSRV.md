---
aliases:
  - CRD11RenderTarget2D::GetSRV()
tags:
  - function
  - DX11
---
## 설명
---
**쉐이더 리소스 뷰의 원시 포인터를 반환**하는 함수이다.

[[ShaderResource View|ShaderResourceView]]가 유효하지 않은 경우 `nullptr`을 반환한다.

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

| 타입                          | 설명                                                 |
| --------------------------- | -------------------------------------------------- |
| `ID3D11ShaderResourceView*` | 유효한 경우 쉐이더 리소스 뷰 포인터. 유효하지 않은 경우 `nullptr`. |

