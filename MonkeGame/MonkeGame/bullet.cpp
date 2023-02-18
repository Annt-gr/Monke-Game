#include "bullet.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"
#include <math.h>
#include "player.h"

//move bullet using vector.h
void Bullet::update()
{
	pos_x += direction.x * speed * graphics::getDeltaTime() / 10.0f;
	pos_y += direction.y * speed * graphics::getDeltaTime() / 10.0f;

	if (pos_x < 0 || pos_x > CANVAS_WIDTH || pos_y < 0 || pos_y > CANVAS_HEIGT)
		setActive(false);
}

	
//draw bullet
void Bullet::draw()
{
	
	brush.texture = std::string(ASSET_PATH) + "bullet.png";
	brush.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y, size, size, brush);


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

void Bullet::init()
{


}
//bullet constarctor
Bullet::Bullet(const Game& mygame, float x, float y,float sizee, Vector2D dir)
	:GameObject(mygame)
{
	direction = dir;
	pos_x = x + 35.5f * dir.x;
	pos_y = y + 35.5f * dir.y;
	size = sizee;
}

Bullet::~Bullet()
{

}
//bullet collision
Disk Bullet::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size -10;
	return disk;
}
