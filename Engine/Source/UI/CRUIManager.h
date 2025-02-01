#pragma once


#include "Source/Core/Containers/CRContainerInc.h"


class ICRUI;


//---------------------------------------------------------------------------------------------------------------------
/// CRUIManager
//---------------------------------------------------------------------------------------------------------------------
class CRUIManager
{
private:
    CRArray< ICRUI* > UIs;

public:
    /// Constructor.
    CRUIManager() = default;

    /// Destructor.
    virtual ~CRUIManager();
    
    /// Initialize.
    void Initialize();

    /// Clear.
    void Clear();

    /// Draw.
    void Draw() const;
    
};
