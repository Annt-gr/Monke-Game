#include "enemy.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"
#include <iostream>

//update enemy poss and delete if out of window
void Enemy::update()
{
	pos_x -= speed * graphics::getDeltaTime();
	rotation += 0.05f * graphics::getDeltaTime();
	rotation = fmodf(rotation, 360);
	if (pos_x < -size) {
		active = false;
	}

	//std::cout << "Enemy Position " << pos_x << ", " << pos_y << std::endl;
}
//draw enemy chance when life <0.5
void Enemy::draw()
{
	if (life >= 0.5f) {
		graphics::setOrientation(rotation);
		brush.texture = std::string(ASSET_PATH) + "nut.png";
		//brush.fill_opacity = 0.5f;
		//graphics::drawRect(pos_x-10, pos_y+30, size, size, brush);
		//brush.texture = std::string(ASSET_PATH) + "banana.png";
		brush.fill_opacity = 1.0f;
		graphics::drawRect(pos_x, pos_y, size, size, brush);
	}
	else {
		graphics::setOrientation(rotation);
		brush.texture = std::string(ASSET_PATH) + "nut_cracked.png";
		//brush.fill_opacity = 0.5f;
		//graphics::drawRect(pos_x-10, pos_y+30, size, size, brush);
		//brush.texture = std::string(ASSET_PATH) + "banana.png";
		brush.fill_opacity = 1.0f;
		graphics::drawRect(pos_x, pos_y, size, size, brush);

	}
	graphics::resetPose();

	if (game.getDebugMode())
	{
		graphics::Brush br;
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 1.3f;
		br.fill_color[1] = 0.3f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;

		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);

	}
}
//enemy starting var
void Enemy::init()
{
	speed = 0.1f;
	size = 50 + 100 * rand0to1();
	pos_x = CANVAS_WIDTH + 1.1f*size;
	pos_y = CANVAS_HEIGT*rand0to1();
	rotation = 360*rand0to1();
	brush.outline_opacity = 0.0f;
}


//calling init usic constractor
Enemy::Enemy(const Game& mygame) :GameObject(mygame)
{
	init();
}

Enemy::~Enemy()
{
}
//create collision enemy
Disk Enemy::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.40f;
	return disk;
}

