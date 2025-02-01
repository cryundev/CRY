#pragma once


#include <Windows.h>


//---------------------------------------------------------------------------------------------------------------------
/// CRTime
//---------------------------------------------------------------------------------------------------------------------
class CRTime
{
private:
    LARGE_INTEGER Frequency;
    LARGE_INTEGER StartTime;
    LARGE_INTEGER FinishTime;

public:
    /// Constructor.
    CRTime();

    /// Start.
    void Start();

    /// Finish.
    float Finish();

    /// Get delta seconds.
    float GetDeltaSeconds() const;
};
