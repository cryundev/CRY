#include "CRD11ResourceManager.h"


//---------------------------------------------------------------------------------------------------------------------
/// Clear
//---------------------------------------------------------------------------------------------------------------------
void CRD11ResourceManager::Clear()
{
	VertexBuffers.clear();
	IndexBuffers .clear();
	InputLayouts .clear();
	VertexShaders.clear();
	PixelShaders .clear();
}
