/*#include "DXGI.h"
#include<cassert>
using namespace winrt;

IDXGIFactory4* CreateDXGIFactory()
{
	IDXGIFactory4* factory;
	UINT createFactoryFlags = 0;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	HRESULT hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create DXGI Factory\n");
		return nullptr;
	}

	return factory;
}

IDXGIAdapter* GetHardwareAdapter(IDXGIFactory4* factory)
{
	IDXGIAdapter1* adapter;

	for (UINT adapterIndex = 0; ; ++adapterIndex)
	{
		adapter = nullptr;
		if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(adapterIndex, &adapter))
		{
			break;
		}
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
		{
			adapter->Release();
			continue;
		}
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)));
		{
			return adapter;
		}
		adapter->Release();
	}
	return nullptr;
}
ID3D12Device* CreateD3D12Device(IDXGIAdapter1* adapter)
{
	ID3D12Device* device;

	// デバイス作成を試行
	HRESULT hr = D3D12CreateDevice(
		adapter,                    // 使用するアダプター
		D3D_FEATURE_LEVEL_11_0,     // 最小機能レベル
		IID_PPV_ARGS(&device)       // 作成されるデバイス
	);

	if (FAILED(hr))
	{
		// フォールバック：ソフトウェアアダプターを試行
		hr = D3D12CreateDevice(
			nullptr,                // 規定アダプター
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&device)
		);

		if (FAILED(hr))
		{
			OutputDebugString("Failed to create D3D12 Device\n");
			return nullptr;
		}

		OutputDebugString("Using software adapter (WARP)\n");
	}

	return device;
}
ID3D12CommandQueue* CreateCommandQueue(ID3D12Device* device)
{
	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;    // 直接実行型
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;  // 通常優先度
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;    // 特別フラグなし
	queueDesc.NodeMask = 0;                             // 単一GPU使用

	ID3D12CommandQueue* commandQueue;
	HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Command Queue\n");
		return nullptr;
	}

	// デバッグ用の名前設定（任意だが推奨）
	commandQueue->SetName(L"Main Command Queue");

	return commandQueue;
}
IDXGISwapChain3* CreateSwapChain(IDXGIFactory4* factory, ID3D12CommandQueue* commandQueue, HWND hwnd)
{
	// スワップチェーンの詳細設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;                      // ダブルバッファリング
	swapChainDesc.Width = 1280;                         // 画面幅
	swapChainDesc.Height = 720;                         // 画面高
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ピクセルフォーマット
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // レンダーターゲット用
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // 高速切替
	swapChainDesc.SampleDesc.Count = 1;                 // マルチサンプリングなし

	IDXGISwapChain1* swapChain1;
	HRESULT hr = factory->CreateSwapChainForHwnd(
		commandQueue,       // コマンドキュー
		hwnd,              // ターゲットウィンドウ
		&swapChainDesc,    // 設定
		nullptr,           // フルスクリーン設定
		nullptr,           // 出力制限
		&swapChain1        // 作成されるスワップチェーン
	);

	if (FAILED(hr))
	{
		OutputDebugString("Failed to create Swap Chain\n");
		return nullptr;
	}

	// より高機能なインターフェースにキャスト
	IDXGISwapChain3* swapChain;
	hr = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
	swapChain1->Release();

	if (FAILED(hr))
	{
		OutputDebugString("Failed to cast to SwapChain3\n");
		return nullptr;
	}

	return swapChain;
}*/
#include "DXGI.h"
#include <cassert>
#pragma comment(lib, "dxgi.lib")
//---------------------------------------------------------------------------------
/**
 * @brief    デストラクタ
 */
DXGI::~DXGI() {
	// DXGIファクトリーの解放
	if (dxgiFactory_) {
		dxgiFactory_->Release();
		dxgiFactory_ = nullptr;
	}
	// アダプタの解放
	if (dxgiAdapter_) {
		dxgiAdapter_->Release();
		dxgiAdapter_ = nullptr;
	}
}
//---------------------------------------------------------------------------------
/**
 * @brief	アダプタの設定
 * @return	情報が正しく取得できた場合は true
 */
[[nodiscard]] bool DXGI::setDisplayAdapter() noexcept {
#if _DEBUG
	// デバッグレイヤーをオンに
	// これを行う事で、DirectXのエラー内容をより詳細に知ることができる
	ID3D12Debug* debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
		debug->EnableDebugLayer();
	}
#endif
	// DXGIファクトリーの作成
	{
		UINT createFactoryFlags = 0;
#if _DEBUG
		createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
		const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory_));
		if (FAILED(hr)) {
			assert(false && "DXGIファクトリーの作成に失敗");
			return false;
		}
	}
	// アダプタの取得
	{
		// アダプタを列挙
		auto           select = 0;
		IDXGIAdapter1* dxgiAdapter{};
		// 適切なアダプタを選ぶ
		while (dxgiFactory_->EnumAdapters1(select, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {
			DXGI_ADAPTER_DESC1 desc{};
			dxgiAdapter->GetDesc1(&desc);
			select++;
			// ソフトウェアアダプタは除外
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
				dxgiAdapter->Release();
				continue;
			}
			// Direct3D12 が動かない場合も除外
			if (FAILED(D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
				dxgiAdapter->Release();
				continue;
			}
			dxgiAdapter_ = dxgiAdapter;
			break;
		}
		if (!dxgiAdapter_) {
			assert(false && "アダプタの取得に失敗");
			return false;
		}
	}
	return true;
}
//---------------------------------------------------------------------------------
/**
 * @brief	dxgi ファクトリーを取得する
 * @return	dxgi ファクトリーのポインタ
 */
[[nodiscard]] IDXGIFactory4* DXGI::factory() const noexcept {
	if (!dxgiFactory_) {
		assert(false && "DXGIファクトリーが未作成です");
	}
	return dxgiFactory_;
}
//---------------------------------------------------------------------------------
/**
 * @brief	ディスプレイアダプターを取得する
 * @return	ディスプレイアダプターのポインタ
 */
[[nodiscard]] IDXGIAdapter1* DXGI::displayAdapter() const noexcept {
	if (!dxgiAdapter_) {
		assert(false && "ディスプレイアダプターが未作成です");
	}
	return dxgiAdapter_;
}