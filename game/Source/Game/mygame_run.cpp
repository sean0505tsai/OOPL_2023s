#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

const int CHARACTER_WIDTH = 72;
const int CHARACTER_HEIGHT = 72;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	int x = character.getX();
	int y = character.getYactual();
	int velocityY = character.getVelocityY();			// 正往下, 負往上
	int velocityX = character.getVelocityX();
	// int width = characterResource[character.getResourceShow()].GetWidth();
	// int height = characterResource[character.getResourceShow()].GetHeight();

	if (isCheating) {
		/*
		character.setTopCollision(0);
		character.setBottomCollision(0);
		character.setLeftCollision(0);
		character.setRightCollision(0);
		*/
	}
	else {
		
		//character.setBottomCollision(gameMap.bottomCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 5));
		
		if (velocityY > 0) {	// Y速度向下
			character.setBottomCollision(gameMap.bottomCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, velocityY));
			character.setTopCollision(gameMap.topCollision(x, y, CHARACTER_WIDTH, 1));
		}
		else {					// Y速度向上
			character.setBottomCollision(gameMap.bottomCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1));
			character.setTopCollision(gameMap.topCollision(x, y, CHARACTER_WIDTH, -1*velocityY));
		}
		
		if (velocityX > 0) {
			character.setRightCollision(gameMap.rightCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, velocityX));
			character.setLeftCollision(gameMap.leftCollision(x, y, CHARACTER_HEIGHT, 1));
		}
		else {
			character.setRightCollision(gameMap.rightCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1));
			character.setLeftCollision(gameMap.leftCollision(x, y, CHARACTER_HEIGHT, (-1*velocityX)));
		}
		
	}
	

	if (isPause == false) {			// 暫停狀態不運算物理 所有物件不動
		character.onMove();
	}

}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	isPause = false;
	isCheating = false;

	// character init
	character.init();

	// map init


	// load map resources
	for (int i = 0; i < 43; i++) {
		std::stringstream ss;
		ss << i;
		// std::string filename = "resources/map_720/map" + ss.str() + ".bmp";
		mapResource[i].LoadBitmapByString({ "resources/map_720/map" + ss.str() + ".bmp" });
		mapResource[i].SetTopLeft(0, 0);
	}


	// load character resources and set default value
	// 0: 預設向右圖
	characterResource[0].LoadBitmapByString({ "resources/character/defaultRight.bmp" }, RGB(0, 255, 0));
	characterResource[0].SetTopLeft(character.getX(), character.getYshow());

	// 1: 預設向左圖
	characterResource[1].LoadBitmapByString({ "resources/character/defaultLeft.bmp" }, RGB(0, 255, 0));
	characterResource[1].SetTopLeft(character.getX(), character.getYshow());

	// 2: 向右走動畫
	characterResource[2].LoadBitmapByString({ "resources/character/moving_right/move_right_1.bmp", 
												"resources/character/moving_right/move_right_2.bmp", 
												"resources/character/moving_right/move_right_3.bmp", 
												"resources/character/moving_right/move_right_2.bmp" }, 
																					RGB(0, 255, 0));
	characterResource[2].SetTopLeft(character.getX(), character.getYshow());
	characterResource[2].SetAnimation(animationSpeed, false);

	// 3: 向左走動畫
	characterResource[3].LoadBitmapByString({ "resources/character/moving_left/move_left_1.bmp", 
												"resources/character/moving_left/move_left_2.bmp", 
												"resources/character/moving_left/move_left_3.bmp", 
												"resources/character/moving_left/move_left_2.bmp" }, 
																					RGB(0, 255, 0));
	characterResource[3].SetTopLeft(character.getX(), character.getYshow());
	characterResource[3].SetAnimation(animationSpeed, false);

	// 4: 跳躍蓄力
	characterResource[4].LoadBitmapByString({ "resources/character/charge.bmp" }, RGB(0, 255, 0));
	characterResource[4].SetTopLeft(character.getX(), character.getYshow());

	// 5: 向右跳躍圖
	characterResource[5].LoadBitmapByString({ "resources/character/rise_right.bmp" }, RGB(0, 255, 0));
	characterResource[5].SetTopLeft(character.getX(), character.getYshow());

	// 6: 向右跳躍圖
	characterResource[6].LoadBitmapByString({ "resources/character/rise_left.bmp" }, RGB(0, 255, 0));
	characterResource[6].SetTopLeft(character.getX(), character.getYshow());

	// 7: 向右落下圖
	characterResource[7].LoadBitmapByString({ "resources/character/falling_right.bmp" }, RGB(0, 255, 0));
	characterResource[7].SetTopLeft(character.getX(), character.getYshow());

	// 8: 向左落下圖
	characterResource[8].LoadBitmapByString({ "resources/character/falling_left.bmp" }, RGB(0, 255, 0));
	characterResource[8].SetTopLeft(character.getX(), character.getYshow());

	// 9: 落下途中受撞擊_右
	characterResource[9].LoadBitmapByString({ "resources/character/after_hit_right.bmp" }, RGB(0, 255, 0));
	characterResource[9].SetTopLeft(character.getX(), character.getYshow());

	// 10: 落下途中受撞擊_左
	characterResource[10].LoadBitmapByString({ "resources/character/after_hit_left.bmp" }, RGB(0, 255, 0));
	characterResource[10].SetTopLeft(character.getX(), character.getYshow());

	// 11: 從高處落地_右
	characterResource[11].LoadBitmapByString({ "resources/character/fall_right.bmp" }, RGB(0, 255, 0));
	characterResource[11].SetTopLeft(character.getX(), character.getYshow());

	// 11: 從高處落地_左
	characterResource[12].LoadBitmapByString({ "resources/character/fall_left.bmp" }, RGB(0, 255, 0));
	characterResource[12].SetTopLeft(character.getX(), character.getYshow());

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE) {
		// pause menu
		pause();
	}

	if (!isPause) {					// 暫停狀態不接受 keyboard input

		if (nChar == VK_LEFT) {
			// 角色往左移動
			character.setMoveLeft(true);
		}

		if (nChar == VK_RIGHT) {
			// 角色往右移動
			character.setMoveRight(true);
		}

		if (nChar == VK_SPACE) {
			// 跳躍蓄力
			character.jumpCharge(true);
		}

		if (isCheating) {

			if (nChar == VK_UP) {
				character.setMoveUp(true);		// cheat mode
			}

			if (nChar == VK_DOWN) {
				character.setMoveDown(true);	// cheat mode
			}

		}
		else {

			if (nChar == VK_UP) {
				character.setMoveUp(true);		// cheat mode
			}

			if (nChar == VK_DOWN) {
				character.setMoveDown(true);	// cheat mode
			}
		}

		if (nChar == 0x4A) {		// J
			character.previousResource();		// 上一張角色圖
		}

		if (nChar == 0x4B) {		// K
			character.nextResource();		// 下一張角色圖
		}

		if (nChar == 0x57) {			// W
			cheat();
			if (nChar == 0x41) {		// A
				if (nChar == 0x53) {	// S
					if (nChar == 0x44) {// D

					}
				}
			}
		}
	}

}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT) {
		// 角色往左移動
		character.setMoveLeft(false);
	}

	if (nChar == VK_RIGHT) {
		// 角色往右移動
		character.setMoveRight(false);
	}

	if (nChar == VK_SPACE) {
		// 跳躍蓄力
		character.jumpCharge(false);
	}

	// 作弊/開發模式
	if (isCheating) {
		if (nChar == VK_UP) {
			character.setMoveUp(false);		// dev mode
		}

		if (nChar == VK_DOWN) {
			character.setMoveDown(false);	// dev mode
		}
	}
	else {
		if (nChar == VK_UP) {
			character.setMoveUp(false);		// dev mode
		}

		if (nChar == VK_DOWN) {
			character.setMoveDown(false);	// dev mode
		}
	}
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	character.onShow();
	gameMap.setMap(character.getYactual());
	mapResource[gameMap.getCurrentMap()].ShowBitmap();
	characterResource[character.getResourceShow()].SetTopLeft(character.getX(), character.getYshow());
	characterResource[character.getResourceShow()].ShowBitmap(1.5);

	if (isPause) {
		drawPauseText();
	}

	int x = character.getX();
	int y = character.getYactual();
	// int width = characterResource[character.getResourceShow()].GetWidth();
	// int height = characterResource[character.getResourceShow()].GetHeight();

	// dev mode
	// drawText("Resource:" + std::to_string(character.getResourceShow()), 10, 30);
	
	drawText("X:" + std::to_string(character.getX()), 10, 50);
	drawText("Yactual:" + std::to_string(character.getYactual()), 10, 70);
	drawText("Yshow: " + std::to_string(character.getYshow()), 10, 90);
	drawText("top collision: " + std::to_string(gameMap.topCollision(x, y, CHARACTER_WIDTH, 5)), 10, 110);
	drawText("bottom collision: " + std::to_string(gameMap.bottomCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 5)), 10, 130);
	drawText("left collision: " + std::to_string(gameMap.leftCollision(x, y, CHARACTER_HEIGHT, 5)), 10, 150);
	drawText("right collision: " + std::to_string(gameMap.rightCollision(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 5)), 10, 170);
	//drawText("top left target Y:" + std::to_string(((((y - 720) - 1) / 16) + 1934)), 10, 190);
	//drawText("top left target X:" + std::to_string((x - 1) / 16), 10, 210);
	drawText("velocityY: " + std::to_string(character.getVelocityY()), 10, 230);
	drawText("velocityX: " + std::to_string(character.getVelocityX()), 10, 250);
	drawText("InitialVelocity: " + std::to_string(character.getInitialVelocity()), 10, 270);
	drawText("Direction: " + std::to_string(character.getDirection()), 10, 290);
	//drawText("isCharging: " + std::to_string(character.getCharging()), 10, 310);
	//drawText("isMovingLeft: " + std::to_string(character.getMovingLeft()), 10, 330);
	//drawText("isMovingRight: " + std::to_string(character.getMovingRight()), 10, 350);
	
	if (isCheating) {
		drawText("Cheat Activated", 10, 690); 
	}

}

void CGameStateRun::pause()
{
	if (!isPause) {
		isPause = true;			// 設為暫停狀態
	}
	else {
		isPause = false;		// 解除暫停狀態
	}
}

void CGameStateRun::cheat() {
	if (!isCheating) {
		isCheating = true;
	}
	else {
		isCheating = false;
	}
}

void CGameStateRun::drawPauseText() {

	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 20, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 10, 10, "PAUSE");
	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::drawText(string text, int x, int y) {

	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 20, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, x, y, text);
	CDDraw::ReleaseBackCDC();
}