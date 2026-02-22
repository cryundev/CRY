---
aliases:
---
## 마우스 입력 브리지
---

`EngineViewportHost.cs`는 HWND를 호스팅하지만, 현재 뷰포트 마우스 입력을 엔진으로 넘기는 공식 브리지가 없음. \
또한 DPI/리사이즈 상황에서 좌표계 오차를 줄이려면 C#에서 NDC를 계산하기보다 픽셀 좌표를 엔진으로 전달해 변환하는 구조가 더 안전함.

<br />

<br />

## 학습 포인트
---

- HwndHost 자식 HWND에서 발생하는 `WM_MOUSEMOVE` / `WM_LBUTTONDOWN` 메시지 처리 방식
- C# P/Invoke를 통한 경량 입력 이벤트 전달 패턴 ( `[DllImport]` + `extern` )
- Win32 메시지 후킹과 WPF 이벤트 시스템의 경계 구분
- 드래그 중 뷰포트 밖으로 커서가 나가도 입력을 유지하는 `SetCapture` / `ReleaseCapture` 운용

^point

<br />

<br />

## TODO
---

- [ ] `EngineViewportHost.cs`에 `WndProc` 오버라이드 추가
    - `WM_MOUSEMOVE(0x0200)`, `WM_LBUTTONDOWN(0x0201)`, `WM_LBUTTONUP(0x0202)` 처리
    - 좌표는 `GET_X_LPARAM`/`GET_Y_LPARAM` 규칙(부호 보존)으로 추출
    - 드래그 시작 시 `SetCapture`, 종료 시 `ReleaseCapture`, 커서 이탈 시 `WM_MOUSELEAVE` 처리
- [ ] `Editor_WPF/DllWrappers/EngineAPI.Input.cs` 신규 파일 생성
    - `[DllImport("EngineDLL.dll", EntryPoint = "OnViewportMouseMove")] static extern void OnViewportMouseMove(int pixelX, int pixelY, int viewportW, int viewportH);`
    - `[DllImport("EngineDLL.dll", EntryPoint = "OnViewportMouseButton")] static extern void OnViewportMouseButton(int pixelX, int pixelY, int viewportW, int viewportH, int button, bool pressed);`
    - `button`은 좌/우/중 enum 값을 C++과 동일한 정수 값으로 고정
- [ ] `WndProc`에서 엔진 호출 전 뷰포트 크기 확보 로직 정리
    - `GetClientRect(_hwnd)` 기반 실제 클라이언트 픽셀 크기 사용
    - 0 크기 가드(초기화 전/리사이즈 중) 처리
- [ ] `EngineDLL/InputAPI.cpp` 신규 파일 생성
    - `CR_ENGINE_API void OnViewportMouseMove(i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH)` 구현
    - `CR_ENGINE_API void OnViewportMouseButton(i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, i32 Button, bool bPressed)` 구현
    - 입력 좌표를 공통 유틸에서 NDC로 변환한 뒤 기즈모 시스템으로 전달
- [ ] `EngineDLL/EngineDLL.vcxproj`에 `InputAPI.cpp` 빌드 포함 추가
- [ ] 성능 방어선 추가
    - `WM_MOUSEMOVE` 과다 호출 시 이전 좌표와 동일하면 early-out
    - 기즈모 비활성/선택 없음 상태에서는 입력 처리 최소화

^todo

<br />

<br />
