#pragma once
#pragma once
#include "enemy.h"

class Boss : public Enemy {

	float boss_life = 5.0f;
	bool chanceDir = true;
	Vector2D direction1 = Vector2D(-1.0f, 0.0f);

public:
	std::vector<Bullet*> B_bullets;
	void BossUpdateBullets();
	float size = 0.0f;
	void update() override;
	void draw() override;
	void init() override;
	float getBossLife() { return boss_life; }
	Disk getCollisionHull() const override;
	//drain life of boss
	void drainLife(float amount) {
		boss_life = std::max<float>(0.0f, boss_life - amount);
		if (boss_life == 0.0f)
		{
			setActive(false);

		}
	}
	Boss(const class Game& mygame);
	~Boss();

};