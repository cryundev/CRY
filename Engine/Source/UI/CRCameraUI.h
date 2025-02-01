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
    CRWeakPtr< CRCamera > CameraPtr;
    
public:
    /// Set camera.
    void SetCamera( const CRWeakPtr< CRCamera >& InCameraPtr ) { CameraPtr = InCameraPtr; }
    
    /// Draw.
    virtual void Draw() override;
    
};
