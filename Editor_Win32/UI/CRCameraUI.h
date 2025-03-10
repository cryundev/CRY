#pragma once

#include "ICRUI.h"
#include <Source/Core/CRSmartPtrMacro.h>


class CRCamera;


//---------------------------------------------------------------------------------------------------------------------
/// CRCameraUI
//---------------------------------------------------------------------------------------------------------------------
class CRCameraUI : public ICRUI
{
private:
    const CRCamera* Camera = nullptr;
    
public:
    /// Set camera.
    void SetCamera( const CRCamera* InCamera ) { Camera = InCamera; }
    
    /// Draw.
    virtual void Draw() override;
    
};
