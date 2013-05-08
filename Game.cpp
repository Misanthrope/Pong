
#include "DirectX.h"
using namespace std;

const string APPTITLE = "PONG";
const int SCREENW = 1400;
const int SCREENH = 900;
DWORD dwLastTick = GetTickCount();
SPRITE bar1,bar2,ball;
LPDIRECT3DTEXTURE9 img = NULL;
D3DXVECTOR2 size; 
LPD3DXFONT font = NULL;
int p1score, p2score = 0;

bool Game_Init(HWND window)
{
    //initialize Direct3D
    Direct3D_Init(window, SCREENW, SCREENH, false);

    //initialize DirectInput
    DirectInput_Init(window);
	font = MakeFont("Arial", 40);


	//load non-transparent image
	img = LoadTexture("sprites.png");
	if(!img) return false;
	size = GetBitMapSize("sprites.png");

	bar1.x = 20;
	bar1.y = (SCREENH - size.y) / 2;
	bar1.width = size.x / 3;
	bar1.height = size.y;
	bar1.columns = 3;
	bar1.startframe = 0;
	bar1.endframe = 1;

	

	bar2.x = (SCREENW - ((size.x / 3)+ 20));
	bar2.y = (SCREENH - size.y) / 2;
	bar2.width = size.x / 3;
	bar2.height = size.y;
	bar2.columns = 3;
	bar2.startframe = 1;
	bar2.endframe = 2;

	ball.x = (SCREENW - (size.x / 3)) / 2;
	ball.y = (SCREENH - (size.x /3 )) / 2;
	ball.width = size. x / 3;
	ball.height = size.x / 3;
	ball.columns = 3;
	ball.startframe = 2;
	ball.endframe = 3;
	Initialize_Ball(ball.velx);
	Initialize_Ball(ball.vely);


	return true;


	
}

void Game_Run(HWND window)
{
    //make sure the Direct3D device is valid
    if (!d3ddev) return;

    //update input devices
    DirectInput_Update();

    //clear the scene
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

	if(Key_Down(DIK_W))
	{
		bar1.y -= 1.0f;
		if(bar1.y < 0) bar1.y = 0;
	}

	if(Key_Down(DIK_S))
	{
		bar1.y += 1.0f;
		if(bar1.y > SCREENH - bar1.height) bar1.y = SCREENH - bar1.height;

	}

	if(Key_Down(DIK_UP))
	{
		bar2.y -= 1.0f;
		if(bar2.y < 0) bar2.y = 0;
	}

	if(Key_Down(DIK_DOWN))
	{
		bar2.y += 1.0f;
		if(bar2.y > SCREENH - bar2.height) bar2.y = SCREENH -bar2.height;
	}


	ball.x += ball.velx;
	if(ball.x < 0) 
	{
		p1score += 1;
		ball.x = (SCREENW - (size.x / 3)) / 2;
		ball.y = (SCREENH - (size.x /3 )) / 2;
		
		Initialize_Ball(ball.velx);
		Initialize_Ball(ball.vely);
	}
	if(ball.x > SCREENW - ball.width)
	{
		p2score += 1;
		ball.x = (SCREENW - (size.x / 3)) / 2;
		ball.y = (SCREENH - (size.x /3 )) / 2;
		
		Initialize_Ball(ball.velx);
		Initialize_Ball(ball.vely);
	}

	ball.y += ball.vely;
	if (ball.y > SCREENH - ball.height || ball.y < 0)
		ball.vely *= -1;

	if(Collision(ball,bar1))
	{
		rebound(ball,bar1);
	}
	
	if(Collision(ball,bar2))
	{
		rebound(ball,bar2);
	}


    //start rendering
    if (d3ddev->BeginScene())
    {
        //stard drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		RECT rect = {(SCREENW / 2), 10, ((SCREENW / 2) + 150), 200};
		D3DCOLOR white = D3DCOLOR_XRGB(255,255,255);
		string score = "SCORE\n ";
		string p1 = std::to_string(p1score);
		string p2 = std::to_string(p2score);
		string text = score + p1 + "   -   " + p2;
		//draw
		font->DrawText(spriteobj, text.c_str(), text. length(), &rect, DT_WORDBREAK, white);
		Sprite_Draw_Frame(img, bar1.x, bar1.y, bar1.startframe, bar1.width,bar1.height, bar1.columns);
		Sprite_Draw_Frame(img, bar2.x, bar2.y, bar2.startframe, bar2.width, bar2.height, bar2.columns);
		Sprite_Draw_Frame(img, ball.x, ball.y, ball.startframe, ball.width, ball.height,ball.columns);

		//stop rendering
		spriteobj->End();
        d3ddev->EndScene();
        d3ddev->Present(NULL, NULL, NULL, NULL);
    }

    //F1 key ends program
    if (Key_Down(DIK_F1))
        gameover = true;

    //controller Back button also ends
    if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
        gameover = true;

}

void Game_End()
{
    //free memory and shut down
	font->Release();
	img->Release();
    DirectInput_Shutdown();
    Direct3D_Shutdown();
}