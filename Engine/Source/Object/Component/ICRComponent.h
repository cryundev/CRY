#pragma once


//---------------------------------------------------------------------------------------------------------------------
/// ICRComponent
//---------------------------------------------------------------------------------------------------------------------
class ICRComponent
{
public:
    /// Destructor
    virtual ~ICRComponent() = 0 {}

    /// Initialize component.
    virtual void InitializeComponent() {}

    /// Destroy component.
    virtual void DestroyComponent() {}

    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) {}
};