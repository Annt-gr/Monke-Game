#pragma once
#include "gameobject.h"
#include "graphics.h"
#include "config.h"
#include "bullet.h"


class Enemy: public GameObject, public Collidable
{
	
public :
	float life = 1.0f;
	float size=0.0f;
	float pos_x,  pos_y;
	float speed;
	float rotation;
	graphics::Brush brush;
	bool active = true;
public :
	void update() override;
	void draw() override;
	void init() override;
	bool isActive() { return active; }
	void setActive(bool s) {
		active = s;
	}
	Enemy(const class Game& mygame);
	~Enemy();
	Disk getCollisionHull() const override;
	//drain life of enemy
	void drainLife(float amount) {
		life = std::max<float>(0.0f, life - amount);
		if (life == 0.0f)
		{
			setActive(false);
			
		}

	}


};
