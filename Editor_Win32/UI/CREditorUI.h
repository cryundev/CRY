#pragma once


#include "CRCameraUI.h"
#include "CRLogUI.h"


//---------------------------------------------------------------------------------------------------------------------
/// CREditorUI
//---------------------------------------------------------------------------------------------------------------------
class CREditorUI : public ICRUI
{
private:
    CRCameraUI CameraUI;
    CRLogUI    LogUI;

public:
    /// Initialize.
    virtual void Initialize() override;
    
    /// Draw.
    virtual void Draw() override;
};
