---
aliases:
---
## 스켈레탈 애니메이션
---

FBX에서 Bone 데이터를 추출하고 GPU Skinning 구현.

<br />

<br />

## 학습 포인트
---

- Bone Hierarchy
- Vertex Skinning
- Animation Blending

^point

<br />

<br />

## TODO
---

- [ ] `CRBone` 구조체
    - Index, ParentIndex, OffsetMatrix, LocalTransform
    - 신규: `Engine/Source/Object/CRBone.h`
- [ ] `CRSkeleton` 클래스
    - Bone 계층, Final Transform 계산
- [ ] `CRVertex`에 Skinning 데이터 추가
    - `BoneIndices` ( uint4 ) + `BoneWeights` ( float4 )
    - Skinned Vertex 별도 정의
- [ ] FBX 로더에서 Bone/Animation 데이터 추출
    - `CRFbxLoader.cpp`
- [ ] Bone Matrix Array를 Constant Buffer로 전달
    - VS CB, 최대 128 bone
- [ ] Skinning VS 작성
    - 가중치 기반 변환 합성
    - 신규: `skinned.hlsl`
- [ ] 애니메이션 블렌딩 ( 2개 클립 LERP )

^todo

<br />

<br />
