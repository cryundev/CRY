#pragma once


#include <windows.h>


/// Initialize RHI.
void InitializeRHI( HWND hWnd, unsigned int Width, unsigned int Height );

// Render frame RHI.
void RenderFrameRHI();