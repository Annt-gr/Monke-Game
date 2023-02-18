#include "boss.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"
#include <iostream>


Boss::Boss(const Game& mygame): Enemy(mygame)
{
	init();
}

Boss::~Boss()
{

}
//move boss and update boss bullets to shot
void Boss::update()
{
	if (chanceDir)
	{
		pos_y -= speed * graphics::getDeltaTime();
		if (pos_y < 20.0f)
			chanceDir = false;
	}
	else {
		pos_y += speed * graphics::getDeltaTime();
		if (pos_y > 500.0f)
			chanceDir = true;
	}
	if (B_bullets.size() < 1) {
		Bullet* b_b = new Bullet(game, pos_x, pos_y,40, direction1);
		B_bullets.push_back(b_b);

	}

	BossUpdateBullets();
	//std::cout << "Boss Position : " << pos_x << " " << pos_y << std::endl;
	//pos_y -= speed * graphics::getDeltaTime();
	//rotation += 0.05f * graphics::getDeltaTime();
	//rotation = fmodf(rotation, 360);
}
//draw boss and boss life
void Boss::draw()
{
	graphics::Brush br;

	brush.texture = std::string(ASSET_PATH) + "TheBoss.png";
	
	brush.fill_opacity = 1.0f;
	graphics::drawRect(pos_x, pos_y, size, size, brush);
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
	float player_life = getBossLife();
	br.texture = "";
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 0.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 0.0f;
	br.fill_secondary_color[0] = 0.2f;
	br.fill_secondary_color[1] = 1.0f * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[2] = 0.2f * (1.0f - player_life) + player_life * 0.2f;
	br.gradient = true;
	br.gradient_dir_u = 2.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH/2 - 100 - ((1.0f - player_life) * 120 / 2), 30.0f, player_life * 120, 20, br);
	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	br.outline_width = 3.0f;
	graphics::drawRect(CANVAS_WIDTH/2+140, 30.0f, 600, 20, br);

	for (Bullet* b_b : B_bullets)
		b_b->draw();

}
//boss bullets pew pew
void Boss::BossUpdateBullets() {
	//Update Bullet Positions

	bool BossActiveBullets[1];

	for (int i = 0; i < B_bullets.size(); i++) {
		B_bullets[i]->update();
		BossActiveBullets[i] = B_bullets[i]->getActive();
	}

	//Delete inactive bullets
	for (int i = B_bullets.size() - 1; i >= 0; i--) {
		if (!BossActiveBullets[i]) {
			delete B_bullets[i];
			B_bullets.erase(B_bullets.begin() + i);
		}
	}

}
void Boss::init()
{
	speed = 0.1f;
	size = 200.0f;
	pos_x = CANVAS_WIDTH - 100;
	pos_y = CANVAS_HEIGT / 2.0f;
	//rotation = 360 * rand0to1();
	brush.outline_opacity = 0.0f;
}
//boss collision
Disk Boss::getCollisionHull() const
{

	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.40f;
	return disk;
}

