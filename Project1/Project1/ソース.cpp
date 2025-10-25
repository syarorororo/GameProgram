#include <Windows.h>

int WINAPI WinMain(
    HINSTANCE hInstance,      // �A�v���P�[�V�����̎��ʔԍ�
    HINSTANCE hPrevInstance,  // ��{�g��Ȃ��Ă���
    LPSTR lpCmdLine,          // �R�}���h���C�������i�N�����̃I�v�V�����j
    int nCmdShow              // �E�B���h�E�̕\�����@�i�ő剻�A�ŏ����Ȃǁj
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
    MSG msg{};  // ���b�Z�[�W���i�[����\����
    while (GetMessage(&msg, NULL, 0, 0))  // ���b�Z�[�W������܂őҋ@
    {
        TranslateMessage(&msg);  // �L�[�{�[�h���b�Z�[�W���g���₷���`�ɕϊ�
        DispatchMessage(&msg);   // �K�؂ȃE�B���h�E�v���V�[�W���ɑ��M
    }
    // �����Ƀ��C���̏���������
    // 1. �E�B���h�E�N���X�o�^
    // 2. �E�B���h�E�쐬
    // 3. ���b�Z�[�W���[�v
    return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:  // �~�{�^���������ꂽ��
        PostQuitMessage(0);  // �u�A�v�����I�����܂��v��Windows�ɒʒm
        return 0;
    case WM_PAINT:    // �E�B���h�E�̍ĕ`�悪�K�v�Ȏ�
        // ��ʂ��X�V���鏈���������ɏ���
        return 0;
    case WM_KEYDOWN:  // �L�[�������ꂽ��
        // �L�[���͂̏����������ɏ���
        return 0;
    }
    // �����ŏ������Ȃ����b�Z�[�W��Windows�ɔC����
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
