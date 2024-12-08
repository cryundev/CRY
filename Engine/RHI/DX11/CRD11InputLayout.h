#pragma once

#include "CRD11Include.h"
#include "../../CRTypes.h"


class CRD11InputLayout
{
public:
	ID3D11InputLayout* InputLayoutPtr = nullptr;

public:
	/// Create input layout.
	void CreateInputLayout();
};
