#pragma once
#include "player.h"
#include "boss.h"


class Game
{	
	typedef enum {STATUS_START, STATUS_PLAYING, STATUS_DIED,STATUS_BOSS,STATUS_CREDITS} status_t;
	
	Player* player= nullptr;
	Boss* boss = nullptr;
	bool player_initialized = false;
	bool boss_initialized = false;
	bool debug_mode = false;
	float score = 0;
	std::vector<Enemy*> enemies;
	
	void spawnEnemies();
	void updateEnemies();
	void checkActiveEnemies();
	bool checkCollision();
	bool checkCollisionBoss();
	void checkPlayer();
	status_t status = STATUS_START;
	void reset();
	void updateStartScreen();
	void updateLevelScreen();
	void drawStartScreen();
	void drawLevelScreen();
	void updateBossScreen();
	void drawBossScreen();
	void updateDiedScreen();
	void drawDiedScreen();
	void updateCreditsScreen();
	void drawCreditsScreen();
public:
	void setDebugMode(bool d) { debug_mode = d; }
	bool getDebugMode()const { return debug_mode; }
	void update();
	void draw();
	void init();
	


	Game();

	~Game();


};

