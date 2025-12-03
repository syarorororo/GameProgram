#include"pipline_state_object.h"
#include<cassert>

PiplineStateObject::~PiplineStateObject() {
	if (piplineState_) {
		piplineState_->Release();
		piplineState_ = nullptr;
	}
}
[[nodiscard]] bool PiplineStateObject::create(const Device& device, const Shader& shader, const RootSignature& rootSignature)noexcept {
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{"POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{   "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
