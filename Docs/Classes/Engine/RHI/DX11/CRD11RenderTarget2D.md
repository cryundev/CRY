---
aliases:
tags:
---
**오프스크린 렌더 타겟 1장을 안전하게 관리하기 위한 래퍼**클래스이다. 

<br />

<br />

## 프로퍼티
---

| 타입                            | 이름                 | 설명                |
| ----------------------------- | ------------------ | ----------------- |
| `CRD11Texture2DWPtr`          | Texture            | 오프스크린 텍스처 본체      |
| `CRD11RenderTargetViewWPtr`   | RenderTargetView   | 렌더 타겟 뷰( 출력용 )    |
| `CRD11ShaderResourceViewWPtr` | ShaderResourceView | 쉐이더 리소스 뷰( 샘플링용 ) |
| `u32`                         | Width              | 텍스처 너비            |
| `u32`                         | Height             | 텍스처 높이            |
| `DXGI_FORMAT`                 | Format             | 텍스처 포맷            |

<br />

<br />

## 사용
---
###### 선행 패스

[[RenderTarget View|RenderTargetView]]를 바인딩하여 이 텍스처에 장면을 그린다.

<br />

###### 후행 패스

[[ShaderResource View|ShaderResourceView]]를 바인딩하여 장면이 그려진 텍스처를 샘플링한다. 

<br />

<br />

## 주의 사항
---
- 같은 텍스처를 **RTV로 사용한 직후 SRV로 바로 사용하면 충돌**이 발생할 수 있다.
- 패스 전환 시 RTV 언바인드가 필요하다.

<br />

<br />

## 함수
---
- [[CRD11RenderTarget2D.Create|Create]]
- [[CRD11RenderTarget2D.Release|Release]]
- [[CRD11RenderTarget2D.Resize|Resize]]
- [[CRD11RenderTarget2D.GetRTV|GetRTV]]
- [[CRD11RenderTarget2D.GetSRV|GetSRV]]
- [[CRD11RenderTarget2D.IsValid|IsValid]]