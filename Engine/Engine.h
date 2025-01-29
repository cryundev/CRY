#pragma once


#include "resource.h"
#include "Core/CRSmartPtrMacro.h"
#include "Extras/DirectXTK/Inc/Keyboard.h"
#include "Extras/DirectXTK/Inc/Mouse.h"
#include "Object/Camera/CRCamera.h"


extern CRSharedPtr< CRCamera > GCamera;

extern DirectX::Keyboard GKeyboard;
extern DirectX::Mouse    GMouse;