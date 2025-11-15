#pragma once
#include<winrt/base.h>
#include<dxgi1_6.h>
	//↓これ必須
#pragma comment(lib,"dxgi.lib")
#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")

class DXGI
{
public:
	DXGI(){}
	~DXGI(){}

    [[nodiscard]] bool setDisplayAdapter() noexcept;

    //---------------------------------------------------------------------------------
    /**
     * @brief	dxgi ファクトリーを取得する
     * @return	dxgi ファクトリーのポインタ
     */
    [[nodiscard]] IDXGIFactory4* factory() const noexcept;

    //---------------------------------------------------------------------------------
    /**
     * @brief	ディスプレイアダプターを取得する
     * @return	ディスプレイアダプターのポインタ
     */
    [[nodiscard]] IDXGIAdapter1* displayAdapter() const noexcept;

private:
    IDXGIFactory4* dxgiFactory_{};  /// DXGIを作成するファクトリー
    IDXGIAdapter1* dxgiAdapter_{};  /// ディスプレイモード取得用アダプタ
};

