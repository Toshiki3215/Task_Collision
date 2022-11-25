#include "DxLib.h"
#include "Vector2.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "LE2B_19_ハトウトシキ: タイトル";

// ウィンドウ横幅
const int WIN_WIDTH = 600;

// ウィンドウ縦幅
const int WIN_HEIGHT = 400;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み


	// ゲームループで使う変数の宣言
	Vector2 lineA(100, 300);
	Vector2 lineB(500, 300);

	Vector2 circle(300, 100);
	int r = 50;

	int cr = GetColor(255, 255, 255);

	Vector2 lineV = lineB - lineA;
	Vector2 lineCircleVA = circle - lineA;
	Vector2 lineCircleVB = circle - lineB;

	Vector2 lineVnorm = lineV;

	bool trigger1 = false;
	bool trigger2 = false;

	float lineLengthA = lineCircleVA.length();
	float lineLengthB = lineCircleVB.length();

	int n = lineVnorm.x * lineCircleVA.y - lineCircleVA.x * lineVnorm.y;

	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		if (keys[KEY_INPUT_W] == 1 || keys[KEY_INPUT_S] == 1)
		{
			if (keys[KEY_INPUT_W] == 1)
			{
				circle.y -= 10;
			}
			else if (keys[KEY_INPUT_S] == 1)
			{
				circle.y += 10;
			}
		}

		if (keys[KEY_INPUT_A] == 1 || keys[KEY_INPUT_D] == 1)
		{
			if (keys[KEY_INPUT_A] == 1)
			{
				circle.x -= 10;
			}
			else if (keys[KEY_INPUT_D] == 1)
			{
				circle.x += 10;
			}
		}

		lineCircleVA = circle - lineA;
		lineCircleVB = circle - lineB;

		lineLengthA = lineCircleVA.length();
		lineLengthB = lineCircleVB.length();

		lineVnorm.normalize();

		n = lineVnorm.x * lineCircleVA.y - lineCircleVA.x * lineVnorm.y;

		if (circle.y <= lineA.y)
		{
			n *= -1;
		}

		if (n <= r)
		{
			float dot01 = lineCircleVA.x * lineV.x + lineCircleVA.y * lineV.y;

			float dot02 = lineCircleVB.x * lineV.x + lineCircleVB.y * lineV.y;

			if (dot01 * dot02 <= 0.0f)
			{
				trigger1 = TRUE;
			}
			else
			{
				trigger1 = false;
			}
		}
		else
		{
			trigger1 = false;
		}

		if (lineLengthA < r || lineLengthB < r)
		{
			trigger2 = TRUE;
		}
		else
		{
			trigger2 = false;
		}

		if (trigger1 == TRUE|| trigger2 == TRUE)
		{
			cr = GetColor(255, 0, 0);
		}
		else
		{
			cr = GetColor(255, 255, 255);
		}

		//ベクトルなし(斜め時非対応)
		/*if (circle.x + r >= lineA.x && circle.x - r <= lineB.x)
		{
			if (lineA.y <= circle.y + r && lineA.y >= circle.y - r)
			{
				cr = GetColor(255, 0, 0);
			}
			else
			{
				cr = GetColor(255, 255, 255);
			}
		}
		else
		{
			cr = GetColor(255, 255, 255);
		}*/

		// 描画処理
		DrawLine(lineA.x, lineA.y, lineB.x, lineB.y, cr, TRUE);

		DrawCircle(circle.x, circle.y, r, cr, TRUE);

		DrawFormatString(0, 10, cr, "%d", n, TRUE);

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}
