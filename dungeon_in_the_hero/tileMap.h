#pragma once

#define TILE_SIZE 32.0f
#define TILE_SETNON 4

#define TILE_ALPHA_VALUE 155
#define TILE_SHAKING_COUNT 20
#define TILE_DES_DALEY 10

#define TILE_POPUP_COUNT 120
#define TILE_POPUP_Y_SIZE 64.0f

#define TILE_NON_BLOCK 5
#define TILE_NON_BLOCK_SET_Y 1


class uiPopup;
class uiManager;

class tileMap
{
private:
	enum tagTileType
	{
		START, TOP, BLOCK, RAND, BLOCK_NON, END_NUM
	};

	SYNTHESIZE(TileInfo, m_tileset[2048], tile);
	SYNTHESIZE(int, m_tileSizeMaxX, tileMaxValueX);
	SYNTHESIZE(int, m_tileSizeMaxY, tileMaxValueY);
	SYNTHESIZE(bool, m_isTileClick, IsTileClivk);

	char szText[256];

	int m_tileSizeX;
	int m_tileSizeY;

	bool m_isTilePopup;
	int m_tileDesDaley;
	PlayerInfo * m_pPlayer;
	uiManager * m_pUiMag;
	uiPopup * m_pTilePopup;
	tagItemData m_tItemInfo;

public:
	HRESULT init(int tileX, int tileY, PlayerInfo * playerData, uiManager * uiMagData);
	void release();
	void update();
	void render(HDC hdc);

	void keyInput();
	void tileSetTxt(int tileType);
	tagItemData dropItemSet(int itemType);

	tileMap();
	~tileMap();
};

