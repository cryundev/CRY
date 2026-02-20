---
aliases:
  - CRD11RenderTarget2D::Resize()
tags:
  - function
  - DX11
---
## 설명
---
**렌더 타겟의 크기 또는 포맷을 변경**하는 함수이다.

기존 DX 리소스를 [[CRD11RenderTarget2D.Release|Release()]]로 해제한 뒤, 새 파라미터로 [[CRD11RenderTarget2D.Create|Create()]]를 호출하여 리소스를 재할당한다.

<br />

<br />

## 시그니처
---
#### Input
---

| 타입              | 이름       | 설명                                                                                   |
| --------------- | -------- | ------------------------------------------------------------------------------------ |
| `const CRName&` | BaseName | DX 리소스 할당 및 관리를 위한 이름. <br>이 클래스에서 사용하는 각 리소스는 이 기본 이름에 Postfix가 추가된 이름으로 할당되고 관리된다. |
| `u32`           | InWidth  | 변경할 렌더 타겟 텍스처의 가로 크기.                                                                |
| `u32`           | InHeight | 변경할 렌더 타겟 텍스처의 세로 크기.                                                                |
| `DXGI_FORMAT`   | InFormat | 변경할 렌더 타겟 포맷.                                                                        |

<br />

#### Output
---
- 없음

