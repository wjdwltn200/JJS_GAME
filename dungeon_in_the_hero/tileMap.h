#pragma once

#define TILE_SCALE 1.0f
#define TILE_SIZE (32.0f * TILE_SCALE) 
#define TILE_SETNON 4

#define TILE_ALPHA_VALUE 155
#define TILE_SHAKING_COUNT 20
#define TILE_DES_DALEY 10

#define POPUP_COUNT 1
#define TILE_POPUP_Y_SIZE 64.0f

#define TILE_NON_BLOCK 5
#define TILE_NON_BLOCK_SET_Y 1

#define TILE_LV_0 3
#define TILE_LV_1 4
#define TILE_LV_2 6
#define TILE_LV_3 9
#define TILE_LV_4 12
#define TILE_LV_5 15

#define TILE_OVERLORD_X 15
#define TILE_OVERLORD_Y 4

class uiPopup;
class uiManager;
class enemyManager;
class heroManager;

class tileMap
{
private:
	enum tagTileType
	{
		START, TOP, BLOCK, RAND, BLOCK_NON, END_NUM
	};
	enum eTileEnemy
	{
		NonEnemy ,Slime, Bug, Lizardman, Lili, Mamon ,Demon, Lady
	};

	SYNTHESIZE(TileInfo, m_tileset[2048], tile);
	SYNTHESIZE(int, m_tileSizeMaxX, tileMaxValueX);
	SYNTHESIZE(int, m_tileSizeMaxY, tileMaxValueY);
	SYNTHESIZE(bool, m_isTileClick, IsTileClivk);
	SYNTHESIZE(int, m_tileSizeX, TileSizeX);
	SYNTHESIZE(int, m_tileSizeY, TileSizeY);
	char szText[256];

	int m_tileDesDaley;
	PlayerInfo * m_pPlayer;
	enemyManager * m_pEnemyMag;
	heroManager * m_pHeroMag;
	uiManager * m_pUiMag;
	uiPopup * m_pTilePopup;
	tagItemData m_tItemInfo;

public:
	HRESULT init(int tileX, int tileY,
		PlayerInfo * playerData, uiManager * uiMagData,
		enemyManager * pEnemyMag, heroManager * pHeroMag);
	void release();
	void update();
	void render(HDC hdc);

	void keyInput();
	void tileSetTxt(int tileType, int tileNum);
	tagItemData dropItemSet(int itemType);
	void monsSetDrop(float posX, float posY, int setTileNum, int tileX, int tileY);
	void tileImgSet(int tileX, int tileY, HDC hdc, bool isShaking = false);
	void tierSet(int tileNum);

	void tileDesSys(int tileValue, int tileX, int tileY);

	void tileDesSe();
	void tileItemGet(int tileValue);

	TileInfo * getTileSetPoint() { return m_tileset; }

	tileMap();
	~tileMap();
};

