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
#define TILE_LV_6 20


#define TILE_OVERLORD_X 15
#define TILE_OVERLORD_Y 4

#define TILE_DES_SET_COUNT 10

#define HERO_START_DALEY 120
#define BaseMoveSpeed 1.0f
#define BaseHp 100
#define BaseMana 100
#define BaseAtkPoint 5
#define BaseDefPoint 0


class uiPopup;
class uiManager;
class enemyManager;
class heroManager;
class overlord;
class hero;

class tileMap
{
private:
	enum tagTileType
	{
		START, GROUND, TOP, BLOCK, RAND, BLOCK_NON, END_NUM
	};
	enum eTileEnemy
	{
		NonEnemy ,Slime, Bug, Lizardman, Lili, Mamon ,Demon, Lady
	};
	enum eTileLv
	{
		tGround, tWall, tBrick, tFloor, tNoEnemy, tSlime, tBug, tLizardman, tLili, tMamon, tDemon, tLady
	};
	enum eHeroClass
	{
		hero_0, Warriors, Wizard, Alchemist, Theif, Bard
	};


	SYNTHESIZE(TileInfo, m_tileset[2048], tile);
	SYNTHESIZE(int, m_tileSizeMaxX, tileMaxValueX);
	SYNTHESIZE(int, m_tileSizeMaxY, tileMaxValueY);
	SYNTHESIZE(bool, m_isTileClick, IsTileClivk);
	SYNTHESIZE(int, m_tileSizeX, TileSizeX);
	SYNTHESIZE(int, m_tileSizeY, TileSizeY);
	char szText[256];

	int m_tileDesDaley;
	int m_tileDesCurr;
	int m_tileDesCount;
	PlayerInfo * m_pPlayer;
	enemyManager * m_pEnemyMag;
	heroManager * m_pHeroMag;
	uiManager * m_pUiMag;
	uiPopup * m_pTilePopup;
	overlord * m_pOverlord;
	tagItemData m_tItemInfo;

	list<tagHeroData*>	m_vecHeroList;
	list<tagHeroData*>::iterator m_iterHeroList;
	bool m_isHeroStart;
	int m_HeroStartDaley = HERO_START_DALEY;

	SYNTHESIZE(int, m_gameState, GameState);

public:
	HRESULT init(int tileX, int tileY,
		PlayerInfo * playerData, uiManager * uiMagData,
		enemyManager * pEnemyMag, heroManager * pHeroMag,
		overlord * pOverlord);
	void release();
	void update();
	void render(HDC hdc);

	void keyInput();
	void heroStartSys();
	void heroListStart();
	tagHeroData * heroSet(int heroClass);
	void tileSetTxt(int tileType, int tileNum);
	tagItemData dropItemSet(int itemType);
	void monsSetDrop(float posX, float posY, int setTileNum, int tileX, int tileY);
	void tileImgSet(int tileX, int tileY, HDC hdc, bool isShaking = false);
	void tierSet(int tileX, int tileY);

	void tileDesSys(int tileValue, int tileX, int tileY);
	void tileDesSe();
	void tileItemGet(int tileValue);
	void tileDesEneChag();

	void enemyArrNullptrList(int tileMapValue, tagEnemyData * enemyData);
	void enemyArrInList(int tileMapValue, tagEnemyData * enemyData);
	bool enemyArrIsList(int tileMapValue, tagEnemyData * enemyData);
	tagEnemyData * enemyArrOutList(int tileMapValue);
	void enemyBackMove(int tileMapValue, tagHeroData * pHero);
	bool enemyFoodChain(int tileMapValue, tagEnemyData * pEnemyDate);

	void HeroArrNullptrList(int tileMapValue, tagHeroData * HeroData);
	void HeroArrInList(int tileMapValue, tagHeroData * HeroData);
	bool HeroArrIsList(int tileMapValue, tagHeroData * HeroData = nullptr);
	tagHeroData * HeroArrOutList(int tileMapValue);

	void HeroSkillAtk(int tileMapValue, tagHeroData * pHeroData);
	bool tileCheck(int tileX, int tileY);
	bool DemonTileCheck(int tileX, int tileY);
	bool LadyTileCheck(int tileX, int tileY);

	void heroStart(int tileValue);
	void heroSetting(tagHeroData * heroData);
	TileInfo * getTileSetPoint() { return m_tileset; }

	tileMap();
	~tileMap();
};

