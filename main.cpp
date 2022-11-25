#include "DxLib.h"
#include "Vector2.h"

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "LE2B_19_�n�g�E�g�V�L: �^�C�g��";

// �E�B���h�E����
const int WIN_WIDTH = 600;

// �E�B���h�E�c��
const int WIN_HEIGHT = 400;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���


	// �Q�[�����[�v�Ŏg���ϐ��̐錾
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

	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����
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

		//�x�N�g���Ȃ�(�΂ߎ���Ή�)
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

		// �`�揈��
		DrawLine(lineA.x, lineA.y, lineB.x, lineB.y, cr, TRUE);

		DrawCircle(circle.x, circle.y, r, cr, TRUE);

		DrawFormatString(0, 10, cr, "%d", n, TRUE);

		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}
