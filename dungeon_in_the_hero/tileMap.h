#pragma once

#define TILE_SIZE 32
#define TILE_SETNON 4

#define TILE_ALPHA_VALUE 155
#define TILE_SHAKING_COUNT 20

class tileMap
{
private:
	SYNTHESIZE(TileInfo, m_tileset[2048], tile);
	SYNTHESIZE(int, m_tileSizeMaxX, tileMaxValueX);
	SYNTHESIZE(int, m_tileSizeMaxY, tileMaxValueY);

	char szText[256];

	int m_tileSizeX;
	int m_tileSizeY;

	bool m_isTileClick;

	PlayerInfo * m_pPlayer;

public:
	HRESULT init(int tileX, int tileY, PlayerInfo * playerData);
	void release();
	void update();
	void render(HDC hdc);

	void keyInput();
	tileMap();
	~tileMap();
};

