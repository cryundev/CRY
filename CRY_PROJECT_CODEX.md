# CRY Project Codex: DX11 Forward Renderer -> Expert Track (Project-Tailored)

## Baseline (Current)
- [x] D3D11 device/swapchain/backbuffer RTV
- [x] Forward rendering (backbuffer direct)
- [x] Texture sampling, camera, basic lighting
- [ ] Multipass (offscreen RT -> post -> backbuffer)
- [ ] State (Depth/Blend/Raster) systemization/cache
- [ ] Shadow/post/debug/profiling/compute/indirect

---

## Global Rules

### Rule A: Create at Init/Resize, Bind per-frame
- Create/Release for textures, views, and states should occur only during init/resize.
- Per-frame should only bind and draw.

### Rule B: RTV/SRV binding conflict must be avoided
- If a texture is used as RTV in one pass and SRV in the next, unbind RTV first.

### Rule C: Think in passes
- Organize frame as ScenePass / ShadowPass / PostPass, not a single monolithic draw.

---

# STEP 0) Minimal Structure Prep (Required before Step 1)
Goal: ensure Step 1 is safe with current engine structure.

- [ ] Add `RenderTarget2D` struct/class (Texture2D + RTV + SRV + Format + Width/Height + Create/Release/Resize).
- [ ] Add a render pipeline helper to unbind RTVs (e.g., `UnsetRenderTargets()`), because current pipeline has no null-safe path.
- [ ] Begin separating hardcoded texture/material usage from `CRD11Mesh::InitializeMaterial()` so material data can be injected later.

Completion:
- [ ] Clean RTV unbind is possible without hand-coded local hacks.
- [ ] SceneColor creation/recreation locations are clear.

---

# STEP 1) Offscreen SceneColor + PostPass
Goal: render to SceneColor, then output to backbuffer via PostPass.

ScenePass:
- [ ] `OMSetRenderTargets(SceneColorRTV, DepthDSV)`
- [ ] Clear SceneColor + Depth
- [ ] Forward draw

RTV Unbind (Important):
- [ ] Call `UnsetRenderTargets()` after ScenePass to avoid RTV/SRV conflict.

PostPass:
- [ ] `OMSetRenderTargets(BackBufferRTV, nullptr)`
- [ ] Depth OFF state
- [ ] Bind `SceneColorSRV` and draw fullscreen

Completion:
- [ ] PostPass output matches backbuffer-direct output (gamma/tonemap differences allowed).

---

# STEP 2) Fullscreen Triangle (No VB)
Goal: reliable post with minimal IA.

- [ ] VS uses `SV_VertexID` to generate full-screen triangle.
- [ ] `IASetPrimitiveTopology(TRIANGLELIST)` + `Draw(3,0)`
- [ ] Explicitly clear IA bindings in PostPass (InputLayout/VB/IB) to avoid stale state.

Completion:
- [ ] PostPass works with no vertex buffer or input layout.

---

# STEP 3) State Cache (Moved earlier than gamma)
Goal: stable pass-based state control.

- [ ] Rasterizer: `CullBack`, `CullNone`, `Wireframe`
- [ ] Depth: `DepthOnWriteOn`, `DepthOnWriteOff`, `DepthOff`
- [ ] Blend: `Opaque`, `Alpha`, `Additive`
- [ ] Enum-based runtime binding, no per-frame creation

Completion:
- [ ] All states created on init/resize only.
- [ ] Runtime uses enums for binding.

---

# STEP 4) sRGB / Gamma / Tonemap
Goal: linear lighting with correct display output.

- [ ] Albedo textures sampled as sRGB
- [ ] Lighting in linear space
- [ ] PostPass applies tonemap + gamma for HDR outputs

Completion:
- [ ] Consistent look between direct and post output.
- [ ] Reduced clipping in bright areas.

---

# STEP 5) Minimal Material System (Required for scaling)
Goal: stop hardcoding materials in mesh.

- [ ] Material = VS/PS/InputLayout/SRV/Sampler/States
- [ ] Mesh references Material instead of owning shaders/textures directly

Completion:
- [ ] Swap materials without editing mesh code.

---

# STEP 6) Shadow Mapping
- [ ] Shadow depth texture + DSV + SRV
- [ ] ShadowPass (depth only)
- [ ] MainPass sampling with bias + PCF

Completion:
- [ ] Stable shadows and bias control for acne/peter-panning.

---

# STEP 7) Postprocess + Debug Views
- [ ] At least two post effects (Bloom + FXAA or Sharpen)
- [ ] Debug views: depth, normal, shadow map, scene color

Completion:
- [ ] Quick visual debugging via toggle modes.

---

# STEP 8) Profiling
- [ ] GPU timestamps per pass
- [ ] CPU timings + draw call stats

Completion:
- [ ] Can name bottleneck pass with numbers.

---

# STEP 9) GPU-driven (DX11)
- [ ] Compute culling list
- [ ] `DrawIndexedInstancedIndirect`
- [ ] Apply to particles/instances

Completion:
- [ ] GPU builds draw list with measurable CPU reduction.

---

## Next Recommended Commit (Short-Term)
Priority: STEP 1 + STEP 2
- Offscreen SceneColor + PostPass
- Fullscreen Triangle
- RTV/SRV conflict safety

## Notes
- This plan is tailored to current code structure: `CRD11Renderer`, `CRD11RenderingPipeline`, `CRD11Mesh`.
- Keep changes minimal but structural enough to avoid rework later.
