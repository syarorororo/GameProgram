#include "swap_chain.h"
#include <cassert>

SwapChain::~SwapChain() {
    if (swapChain_) {
        swapChain_->Release();
        swapChain_ = nullptr;
    }
}

[[nodiscard]] bool SwapChain::create(const DXGI& dxgi, const Window& window, const CommandQueue& commandQueue) noexcept {
   
    const auto [w, h] = window.size();
    swapChainDesc_ = {};
    swapChainDesc_.BufferCount = 2;                                // バックバッファの数（ダブルバッファ）
    swapChainDesc_.Width = w;                                // バックバッファの横幅
    swapChainDesc_.Height = h;                                // バックバッファの縦幅
    swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // バックバッファのフォーマット
    swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // レンダーターゲットとして使用
    swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    // 毎フレーム画面更新するので描画が終わったらバッファを破棄
    swapChainDesc_.SampleDesc.Count = 1;                                // マルチサンプリングなし
    
    IDXGISwapChain1* tempSwapChain{};
    {
        const auto hr = dxgi.factory()->CreateSwapChainForHwnd(commandQueue.get(), window.handle(),
            &swapChainDesc_, nullptr, nullptr, &tempSwapChain);
        if (FAILED(hr)) {
            assert(false && "スワップチェインの作成に失敗");
            return false;
        }
    }
   
    {
       
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain_));
     
        tempSwapChain->Release();
        if (FAILED(hr)) {
            assert(false && "スワップチェインのアップグレードに失敗");
            return false;
        }
    }
    return true;
}

[[nodiscard]] IDXGISwapChain3* SwapChain::get() const noexcept {
    if (!swapChain_) {
        assert(false && "スワップチェインが未作成です");
        return nullptr;
    }
    return swapChain_;
}

[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& SwapChain::getDesc() const noexcept {
    if (!swapChain_) {
        assert(false && "スワップチェインが未作成です");
    }
    return swapChainDesc_;
}