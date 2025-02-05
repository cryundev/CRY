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
    
    /// Add UI.
    void AddUI( ICRUI* InUI );

    /// Clear.
    void Clear();

    /// Draw.
    void Draw() const;
    
};
