#pragma once


//---------------------------------------------------------------------------------------------------------------------
/// ICRComponent
//---------------------------------------------------------------------------------------------------------------------
class ICRComponent
{
public:
    /// Destructor
    virtual ~ICRComponent() = 0 {}

    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) = 0 {}
};