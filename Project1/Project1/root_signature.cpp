#include"root_signature.h"
#include<cassert>

RootSignature::~RootSignature()
{
	if (rootSignature_) {
		rootSignature_->Release();
		rootSignature_ = nullptr;
	}

	
}
[[nodiscard]] bool RootSignature::create(const Device& device) noexcept {
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.NumParameters = 0;
	rootSignatureDesc.pParameters = nullptr;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
}