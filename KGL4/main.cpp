#include <iostream>
#include <Windows.h>
#include <math.h>
#include <conio.h>

using namespace std;

int SCREEN_WIDTH = 900;
int SCREEN_HEIGHT = 600;

const int N = 8; //для параллелепипеда - восемь строк координат, восемь точек
const int M = 4; //три координаты в трехмерном пространстве + одна однородная
const float DX = 5;
const float DY = 5;
const float ALPHA = 0.087;
const float S1 = 1.01;
const float S2 = 0.99;
const int Color = 0xff4444; //lightcyan

float parallelepiped[N][M] = {
	{100, 400, 100, 1}, {100, 200, 100, 1},
	{400, 200, 100, 1}, {400, 400, 100, 1},
	{100, 400, 300, 1}, {100, 200, 300, 1},
	{400, 200, 300, 1}, {400, 400, 300, 1}
};

////////////////////////////////////////////////////////////////////////////////////////////////

void multing(float lin[N][M], float matrix[M][M]);
void moving(float lin[N][M], float dx, float dy);
void scaling(float lin[N][M], float S);
void rotating_z(float lin[N][M], float angle);
void rotating_x(float lin[N][M], float angle);
void rotating_y(float lin[N][M], float angle);
void draw(float fig1[N][M]);


void multing(float lin[N][M], float matrix[M][M]) {
	float res[N][M] = { 0, 0, 0, 0 };
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			for (int k = 0; k < M; k++)
				res[i][j] += (lin[i][k] * matrix[k][j]);
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++)
			lin[i][j] = res[i][j];
	}
}

void moving(float lin[N][M], float dx, float dy) {
	float dz = 5; //dz = dx = dy
	float m_move[M][M] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{dx, dy, dz, 1} }; //матрица перемещения
	multing(lin, m_move);
}

void scaling(float lin[N][M], float S) {
	float xc = 0.0, yc = 0.0, zc = 0.0;
	for (int i = 0; i < N; i++)
		xc += lin[i][0];
	xc = xc / N;
	for (int i = 0; i < N; i++)
		yc += lin[i][1];
	yc = yc / N;
	for (int i = 0; i < N; i++)
		zc += lin[i][2];
	zc = zc / N;

	float m_scale[M][M] = {
		{S, 0, 0, 0},
		{0, S, 0, 0},
		{0, 0, S, 0},
		{xc * (1 - S), yc * (1 - S), zc * (1 - S), 1} }; //матрица масштабирования
	multing(lin, m_scale);
}

void rotating_z(float lin[N][M], float angle) {
	float xc = 0.0, yc = 0.0;
	for (int i = 0; i < N; i++)
		xc += lin[i][0];
	xc = xc / N;
	for (int i = 0; i < N; i++)
		yc += lin[i][1];
	yc = yc / N;

	float m_rotate[M][M] = {
		{cos(angle), sin(angle), 0, 0},
		{-sin(angle), cos(angle), 0, 0},
		{0, 0, 1, 0},
		{xc * (1 - cos(angle)) + yc * sin(angle), yc * (1 - cos(angle)) - xc * sin(angle), 0, 1} };
	multing(lin, m_rotate);
}

void rotating_x(float lin[N][M], float angle) {
	float yc = 0.0, zc = 0.0;
	for (int i = 0; i < N; i++)
		yc += lin[i][1];
	yc = yc / N;
	for (int i = 0; i < N; i++)
		zc += lin[i][2];
	zc = zc / N;

	float m_rotate[M][M] = {
		{1, 0, 0, 0},
		{0, cos(angle), sin(angle), 0},
		{0, -sin(angle), cos(angle), 0},
		{0, yc * (1 - cos(angle)) + zc * sin(angle), zc * (1 - cos(angle)) - yc * sin(angle), 1} };
	multing(lin, m_rotate);
}

void rotating_y(float lin[N][M], float angle) {
	float xc = 0.0, zc = 0.0;
	for (int i = 0; i < N; i++)
		xc += lin[i][0];
	xc = xc / N;
	for (int i = 0; i < N; i++)
		zc += lin[i][2];
	zc = zc / N;

	float m_rotate[M][M] = {
		{cos(angle), 0, -sin(angle), 0},
		{0, 1, 0, 0},
		{sin(angle), 0, cos(angle), 0},
		{xc * (1 - cos(angle)) - zc * sin(angle), 0, zc * (1 - cos(angle)) + xc * sin(angle), 1} };
	multing(lin, m_rotate);
}

void draw(HDC hdc, float fig1[N][M]) {
	HPEN hPen;
	hPen = CreatePen(PS_DASHDOT, 2, Color);
	SelectObject(hdc, hPen);
	MoveToEx(hdc, fig1[0][0], fig1[0][1], NULL);
	LineTo(hdc, fig1[1][0], fig1[1][1]);
	LineTo(hdc, fig1[2][0], fig1[2][1]);
	LineTo(hdc, fig1[3][0], fig1[3][1]);
	LineTo(hdc, fig1[0][0], fig1[0][1]);

	MoveToEx(hdc, fig1[4][0], fig1[4][1], NULL);
	LineTo(hdc, fig1[5][0], fig1[5][1]);
	LineTo(hdc, fig1[6][0], fig1[6][1]);
	LineTo(hdc, fig1[7][0], fig1[7][1]);
	LineTo(hdc, fig1[4][0], fig1[4][1]);

	MoveToEx(hdc, fig1[0][0], fig1[0][1], NULL); LineTo(hdc, fig1[4][0], fig1[4][1]);
	MoveToEx(hdc, fig1[1][0], fig1[1][1], NULL); LineTo(hdc, fig1[5][0], fig1[5][1]);
	MoveToEx(hdc, fig1[2][0], fig1[2][1], NULL); LineTo(hdc, fig1[6][0], fig1[6][1]);
	MoveToEx(hdc, fig1[3][0], fig1[3][1], NULL); LineTo(hdc, fig1[7][0], fig1[7][1]);
}

void control(int key) {
	if (key == int('W')) moving(parallelepiped, 0, -DY);
	if (key == int('A')) moving(parallelepiped, -DX, 0);
	if (key == int('S')) moving(parallelepiped, 0, DY);
	if (key == int('D')) moving(parallelepiped, DX, 0);
	if (key == 101) rotating_x(parallelepiped, ALPHA);
	if (key == 104) rotating_x(parallelepiped, -ALPHA);
	if (key == 100) rotating_y(parallelepiped, ALPHA);
	if (key == 102) rotating_y(parallelepiped, -ALPHA);
	if (key == 105) rotating_z(parallelepiped, ALPHA);
	if (key == 103) rotating_z(parallelepiped, -ALPHA);
	if (key == int('E')) scaling(parallelepiped, S1);
	if (key == int('Q')) scaling(parallelepiped, S2);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps; RECT Rect; HDC hdc, hCmpDC; HBITMAP hBmp;

	switch (messg) {

	case WM_PAINT: {
		GetClientRect(hWnd, &Rect);
		hdc = BeginPaint(hWnd, &ps);

		// Создание теневого контекста для двойной буферизации
		hCmpDC = CreateCompatibleDC(hdc);
		hBmp = CreateCompatibleBitmap(hdc, Rect.right - Rect.left, Rect.bottom - Rect.top);
		SelectObject(hCmpDC, hBmp);

		// Закраска фоновым цветом
		LOGBRUSH br;
		br.lbStyle = BS_SOLID;
		br.lbColor = 0xFFFFFF;
		HBRUSH brush;
		brush = CreateBrushIndirect(&br);
		FillRect(hCmpDC, &Rect, brush);
		DeleteObject(brush);

		// Отрисовка
		draw(hCmpDC, parallelepiped);

		// Копируем изображение из теневого контекста на экран
		SetStretchBltMode(hdc, COLORONCOLOR);
		BitBlt(hdc, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, hCmpDC, 0, 0, SRCCOPY);

		// Удаляем ненужные системные объекты
		DeleteDC(hCmpDC);
		DeleteObject(hBmp);
		hCmpDC = NULL;

		EndPaint(hWnd, &ps);
	} break;

	case WM_ERASEBKGND:
		return 1;
		break;

	case WM_KEYDOWN:
		control(int(wParam));
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hWnd, messg, wParam, lParam));
	}
	return (0);
}
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	HWND hWnd;
	MSG lpMsg;
	WNDCLASS WndClass;

	// Заполняем структуру класса окна
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"CG_WAPI_Template";

	// Регистрируем класс окна
	if (!RegisterClass(&WndClass)) {
		MessageBox(NULL, L"Не могу зарегистрировать класс окна!", L"Ошибка", MB_OK);
		return 0;
	}

	// Создаем основное окно приложения
	hWnd = CreateWindow(
		L"CG_WAPI_Template", // Имя класса 
		L"Компьютерная графика лабораторная работа №4", // Текст заголовка
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // Стиль окна 
		CW_USEDEFAULT, CW_USEDEFAULT, // Позиция левого верхнего угла 
		SCREEN_WIDTH, SCREEN_HEIGHT, // Ширина и высота окна 
		(HWND)NULL, // Указатель на родительское окно NULL 
		(HMENU)NULL, // Используется меню класса окна 
		(HINSTANCE)hInstance, // Указатель на текущее приложение
		NULL); // Передается в качестве lParam в событие WM_CREATE


	// Если окно не удалось создать
	if (!hWnd) {
		MessageBox(NULL, L"Не удается создать главное окно!", L"Ошибка", MB_OK);
		return 0;
	}

	// Показываем окно
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Выполняем цикл обработки сообщений до закрытия приложения
	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}
	return (int)lpMsg.wParam;
}