#pragma once
#include "gameobject.h"
#include "graphics.h"
#include "Vector.h"


class Bullet :public GameObject, public Collidable
{
	Vector2D direction;
	float pos_x = 0.0f; 
	float pos_y = 0.0f;
	float speed = 8.0f;
	float size;
	//float rotation;
	graphics::Brush brush;
	bool active = true;
public:
	void update() override;
	void draw() override;
	void init() override;
	
	//bool isActive() { return active; }
	Bullet(const class Game& mygame, float x, float y,float size, Vector2D init_dir);
	~Bullet();
	Disk getCollisionHull() const override;
	bool getActive() { return active; }
	void setActive(bool s) { active = s; }



};