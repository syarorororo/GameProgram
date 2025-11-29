/*#include <Windows.h>

int WINAPI WinMain(
    HINSTANCE hInstance,      // アプリケーションの識別番号
    HINSTANCE hPrevInstance,  // 基本使わなくていい
    LPSTR lpCmdLine,          // コマンドライン引数（起動時のオプション）
    int nCmdShow              // ウィンドウの表示方法（最大化、最小化など）
)
{
    HWND hwnd;
    WNDCLASS winc;
    winc.style = CS_HREDRAW | CS_VREDRAW;
    winc.lpfnWndProc = DefWindowProc;
    winc.cbClsExtra = winc.cbWndExtra = 0;
    winc.hInstance = hInstance;
    winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winc.hCursor = LoadCursor(NULL, IDC_ARROW);
    winc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winc.lpszMenuName = NULL;
    winc.lpszClassName = TEXT("KITTY");

    if (!RegisterClass(&winc)) return 0;
    hwnd = CreateWindow(TEXT("KITTY"), TEXT("Kitty no your lap"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,NULL, NULL, hInstance, NULL);
    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, SW_SHOW);
    MSG msg{};  // メッセージを格納する構造体
    while (GetMessage(&msg, NULL, 0, 0))  // メッセージが来るまで待機
    {
        TranslateMessage(&msg);  // キーボードメッセージを使いやすい形に変換
        DispatchMessage(&msg);   // 適切なウィンドウプロシージャに送信
    }
    // ここにメインの処理を書く
    // 1. ウィンドウクラス登録
    // 2. ウィンドウ作成
    // 3. メッセージループ
    return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:  // ×ボタンが押された時
        PostQuitMessage(0);  // 「アプリを終了します」とWindowsに通知
        return 0;
    case WM_PAINT:    // ウィンドウの再描画が必要な時
        // 画面を更新する処理をここに書く
        return 0;
    case WM_KEYDOWN:  // キーが押された時
        // キー入力の処理をここに書く
        return 0;
    }
    // 自分で処理しないメッセージはWindowsに任せる
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}*/
#include "window.h"
namespace {
    
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_DESTROY:  // ウィンドウが閉じられたとき
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}  

[[nodiscard]] HRESULT Window::create(HINSTANCE instance, int width, int height, std::string_view name) noexcept {
    
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = name.data();
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
   
    RegisterClass(&wc);
   
    handle_ = CreateWindow(wc.lpszClassName, wc.lpszClassName,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, instance, nullptr);
    if (!handle_) {
        return E_FAIL;
    }

    ShowWindow(handle_, SW_SHOW);

    UpdateWindow(handle_);

    witdh_ = width;
    height_ = height;
  
    return S_OK;
}

[[nodiscard]] bool Window::messageLoop() const noexcept {
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false; 
        }
       
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

[[nodiscard]] HWND Window::handle() const noexcept {
    return handle_;
}

[[nodiscard]] std::pair<int, int> Window::size() const noexcept {
    return { witdh_, height_ };
}
