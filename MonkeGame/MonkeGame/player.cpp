#include "player.h"
#include "graphics.h"
#include "game.h"
#include <math.h>

Player::Player(const Game & mygame)
	:GameObject(mygame)
{
}
//move player using vector.h and chance oriantation usin a or d
// move with w or s
void Player::update()
{
	if (graphics::getKeyState(graphics::SCANCODE_A))
	{
		//pos_x -= speed * graphics::getDeltaTime() / 10.0f;
		ori += 2.0f * graphics::getDeltaTime() / 10.0f;
		if (ori > 360)
		{
			ori = 0;
		}
	}

	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		ori -= 2.0f * graphics::getDeltaTime() / 10.0f;
		if (ori < 0)
		{
			ori = 360;
		}
		
		//pos_x += speed * graphics::getDeltaTime() / 10.0f;
	}

	//Update direction vector based on orientation
	direction.fromAngle(ori);

	if (graphics::getKeyState(graphics::SCANCODE_W))
	{
		//pos_x -= sin(ori* M_PI /180)* speed * graphics::getDeltaTime()/10.0f;
		//pos_y -= cos(ori* M_PI / 180 )* speed * graphics::getDeltaTime() / 10.0f;

		pos_x += direction.x * speed * graphics::getDeltaTime() / 10.0f;
		pos_y += direction.y * speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_S))
	{
		//pos_x += sin(ori* M_PI / 180) * speed * graphics::getDeltaTime() / 10.0f;
		//pos_y += cos(ori* M_PI / 180) * speed * graphics::getDeltaTime() / 10.0f;
		//pos_y += speed * graphics::getDeltaTime() / 10.0f;
		pos_x -= direction.x * speed * graphics::getDeltaTime() / 10.0f;
		pos_y -= direction.y * speed * graphics::getDeltaTime() / 10.0f;

	}

	
	
	if (pos_x < 0)pos_x = 2;
	if (pos_x > CANVAS_WIDTH) pos_x = CANVAS_WIDTH;

	if (pos_y < 0)pos_y = CANVAS_HEIGT;
	if (pos_y > CANVAS_HEIGT) pos_y = 0;


	//Check for bullet request
	if (graphics::getKeyState(graphics::SCANCODE_SPACE) && bullets.size() < MAX_BULLET_COUNT) {
		//Generate Bullet
		Bullet* b = new Bullet(game, pos_x, pos_y,20.0f, direction);
		bullets.push_back(b);
		graphics::playSound(std::string(ASSET_PATH) + "Pew.mp3", 0.2f, false);
		//std::cout << "Generating Bullet " << std::endl;
	}
	//std::cout << life << std::endl;

	//Update Bullets
	updateBullets();

}

void Player::updateBullets() {
	//Update Bullet Positions
	
	bool activeBullets[MAX_BULLET_COUNT];

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->update();
		activeBullets[i] = bullets[i]->getActive();
	}

	//Delete inactive bullets
	for (int i = bullets.size() - 1; i >= 0; i--) {
		if (!activeBullets[i]) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}

}
//draw player chance depending on remaining life
//draw and chance player life
void Player::draw()
{
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime()/10);

	graphics::Brush br;
	//br.fill_opacity = 0.5f;
	//br.texture = std::string(ASSET_PATH) + "monkeyS.png";
	if (life > 0.5f) {
		br.outline_opacity = 0.0f;
		graphics::setOrientation(ori);
		//	graphics::drawRect(pos_x-10*height, pos_y+30*2*height, 80, 80, br);
		br.fill_opacity = 1.0f;

		br.texture = std::string(ASSET_PATH) + "spacemonkey_fly2.png";
		graphics::drawRect(pos_x, pos_y, 60, 60, br);
	}
	if (life <= 0.5f && life >0.1f)
	{
		br.outline_opacity = 0.0f;
		graphics::setOrientation(ori);
		//	graphics::drawRect(pos_x-10*height, pos_y+30*2*height, 80, 80, br);
		br.fill_opacity = 1.0f;

		br.texture = std::string(ASSET_PATH) + "spacemonkey_dead01.png";
		graphics::drawRect(pos_x, pos_y, 60, 60, br);
	}
	if (life<0.1f) {

		br.outline_opacity = 0.0f;
		graphics::setOrientation(ori);
		//	graphics::drawRect(pos_x-10*height, pos_y+30*2*height, 80, 80, br);
		br.fill_opacity = 1.0f;

		br.texture = std::string(ASSET_PATH) + "spacemonkey_dead02.png";
		graphics::drawRect(pos_x, pos_y, 60, 60, br);
	}
	
	graphics::resetPose();
	
	float player_life = getRemainingLife();
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
	graphics::drawRect( 100 - ((1.0f - player_life) * 120 / 2), 30.0f, player_life * 120, 20, br);
	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	br.outline_width = 3.0f;
	graphics::drawRect( 100, 30.0f, 120, 20, br);
	
	if (game.getDebugMode())
	{
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 0.3f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;

		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx,hull.cy,hull.radius,br);
	}


	//Draw Players bullets
	
	for (Bullet* b : bullets)
		b->draw();

}

void Player::init()
{
}
//create player collision
Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = 35.0f;
	return disk;
}
