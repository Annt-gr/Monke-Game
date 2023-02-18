#pragma once
#include "gameobject.h"
#include "config.h"
#include "bullet.h"
#include "util.h"
#include "Vector.h"

class Player :public GameObject, public Collidable
{
	Vector2D direction = Vector2D(0.0f, -1.0f);
	float pos_x = CANVAS_WIDTH / 2, pos_y=CANVAS_HEIGT / 2;
	float ori = 0.0f;
	float speed = 5.0f;
	float height = 1.0f;
	float life=1.0f;
	
	
public:
	std::vector<Bullet*> bullets;
	Player(const class Game& mygame);
	void update() override;
	void updateBullets();
	void draw()override;
	void init()override;
	float getRemainingLife() const { return life; }
	float getPosY() { return pos_y; }
	float getPosX() { return pos_x; }
	float getOri() { return ori; }
	Disk getCollisionHull() const override;
	//drain life of player and play hit effect
	void drainLife(float amount) { life = std::max<float>(0.0f, life - amount);
	graphics::playSound(std::string(ASSET_PATH) + "Hit.mp3", 0.1f, false);

	}
};