#pragma once


//---------------------------------------------------------------------------------------------------------------------
/// ICRComponent
//---------------------------------------------------------------------------------------------------------------------
class ICRComponent
{
protected:
    bool bEnabled    = true;
    bool bWasEnabled = true;

public:
    /// Destructor
    virtual ~ICRComponent() = 0 {}

    /// Set enabled.
    void SetEnabled( bool InEnabled ) { bEnabled = InEnabled; }

    /// Get enabled.
    bool IsEnabled() const { return bEnabled; }

    /// Initialize component.
    virtual void InitializeComponent() {}

    /// Destroy component.
    virtual void DestroyComponent() {}

    /// Called when component is disabled.
    virtual void OnDisabled() {}

    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) {}
};