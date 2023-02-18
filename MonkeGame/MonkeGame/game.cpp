#include "game.h"
#include "graphics.h"
#include "config.h"
#include <iostream>


using namespace std;

//creating new enemies and push in vector
void Game::spawnEnemies()
{
	while (enemies.size() < MAX_ENEMY_COUNT) {
		Enemy* e = new Enemy(*this);
	

#if DEBUG
		cout << "Adding new Enemy" << endl;
#endif
		
		enemies.push_back(e);
	}
}

//update for all the enemies
void Game::updateEnemies() {
	for (Enemy* e : enemies) {
		e->update();
	}

	
}
//check and delete enemies (if active=false then delete)
void Game::checkActiveEnemies() {
	
	bool activeEnemyStatus[MAX_ENEMY_COUNT];
	
	for (int i = 0; i < enemies.size(); i++) {
		activeEnemyStatus[i] = enemies[i]->isActive();
	}

	for (int i = enemies.size() - 1; i >= 0; i--) {
		if (!activeEnemyStatus[i]) {
			delete enemies[i];
			enemies.erase(enemies.begin() + i);
		}
	}

}

//check collision with player and enemies , bullets with enemies
// if enemy -> collision_player  then player life --
// if bullet hit enemy then enemy life--
bool Game::checkCollision()
{
	//TODO: check multiple collisions with the multiple enemies
	
	if (!player)
	{
		return false;
	}


	//Step A: Check PLayer collision with Enemies

	Disk d1 = player->getCollisionHull();
	
	for (Enemy* e : enemies) {
		Disk d2 = e->getCollisionHull();
		float dx = d1.cx - d2.cx;
		float dy = d1.cy - d2.cy;
		if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		{
			e->setActive(false);
			player->drainLife(0.1f);
			return true;
		}
	}

	//Step B: Check Player's bullets collision with Enemies

	for (Bullet* b : player->bullets) {
		d1 = b->getCollisionHull();
		for (Enemy* e : enemies) {
			Disk d2 = e->getCollisionHull();
			float dx = d1.cx - d2.cx;
			float dy = d1.cy - d2.cy;
			if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
			{
				b->setActive(false);
				e->drainLife(0.1f);
				graphics::playSound(std::string(ASSET_PATH) + "Klonk.mp3", 0.1f, false);
				score += 123;
				//TODO: add enemy drain life
				return true;
			}
		}
	}

	return false;
}

//check collision boss , if boss collision player (player will die )
//if bullet-> boss touch player -> player life --
//if bullet -> player touch boss -> boss life--
bool Game::checkCollisionBoss()
{

	if (!player)
	{
		return false;
	}
	if (!boss)
	{
		return false;
	}


	Disk d1 = player->getCollisionHull();
	Disk d2 = boss->getCollisionHull();
	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;
	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
	{
		
		player->drainLife(1.0f);
		return true;
	}

	for (Bullet* b : player->bullets) {
		d1 = b->getCollisionHull();
			float dx = d1.cx - d2.cx;
			float dy = d1.cy - d2.cy;
			if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
			{
				boss->drainLife(0.01f);
				b->setActive(false);
				graphics::playSound(std::string(ASSET_PATH) + "Bambo.mp3", 0.1f, false);
				score += 123;
				return true;
			}
		}

	d1 = player->getCollisionHull();
	for (Bullet* b_b : boss->B_bullets) {
		d2 = b_b->getCollisionHull();
		float dx = d1.cx - d2.cx;
		float dy = d1.cy - d2.cy;
		if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		{
			player->drainLife(0.1f);
			b_b->setActive(false);
			graphics::playSound(std::string(ASSET_PATH) + "Hit.mp3", 0.1f, false);
			return true;
		}
	
	
	}


	return false;
}
void Game::checkPlayer()
{
	if (player->getRemainingLife() == 0.0f) {
		delete player;
		player = nullptr;
		status = STATUS_DIED;
		graphics::playMusic(std::string(ASSET_PATH) + "YOU_DIED.mp3", 0.5f, false);
	}
}
//reset game delete already existing reasources
void Game::reset()
{
	if (player) {
		delete player;
		player=nullptr;
		player_initialized = false;
	}
	score = 0.0f;
	if (boss) {
		boss_initialized = false;
		delete boss;
		boss = nullptr;
	}
	if (!boss) {
		boss_initialized = false;
	}
	graphics::setFont(std::string(ASSET_PATH) + "space.ttf");
	graphics::playMusic(std::string(ASSET_PATH) + "BackgroundMusic.mp3", 0.5f, false);

	for (Enemy* e : enemies) {
		e->setActive(false);
	}
	checkActiveEnemies();
}
//call reset
void Game::updateStartScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_RETURN)){
		status = STATUS_PLAYING;

	}

}
//call functions for update
void Game::updateLevelScreen()
{
	//std::cout << "FPS: " << 1000.0f / graphics::getDeltaTime() << endl;

	if (!player_initialized || player==nullptr)
	{
		player = new Player(*this);
		player_initialized = true;
	}
	else {
		player->update();
	}
	
	//Update Enemies
	updateEnemies();

	//Check and remove inactive enemies
	checkActiveEnemies();

	//Collision Check
	checkCollision();

	//Spawn new enemies
	spawnEnemies();
	//delete player 
	checkPlayer();
	//chance to boss stage
	if (score > 25000) {

		for (Enemy* e : enemies) {
			e->setActive(false);
		}
		checkActiveEnemies();

		graphics::playMusic(std::string(ASSET_PATH) + "Dummy.mp3", 0.5f, false, 1000);

		status = STATUS_BOSS;
	}

}
void Game::drawStartScreen()
{
	graphics::Brush br;
	br.texture =std::string(ASSET_PATH) + "keywords.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 4, CANVAS_HEIGT / 2, 300, 250, br);
	br.texture = std::string(ASSET_PATH) + "startscreen.png";
	graphics::drawRect(CANVAS_WIDTH -300, CANVAS_HEIGT / 2, 250, 250, br);
	char info[40];
	sprintf_s(info, "Press ENTER to start");
	graphics::drawText( CANVAS_WIDTH/2, CANVAS_HEIGT-100,30, info, br);
	sprintf_s(info, "Space-Monke-Game");
	graphics::drawText(275, 100, 40, info, br);
	sprintf_s(info, "Press 'A' - 'D' TO ROTATE" );
	graphics::drawText(CANVAS_WIDTH/4-200, CANVAS_HEIGT/2+150, 25, info, br);
	sprintf_s(info, "Press 'W' - 'S' TO MOVE");
	graphics::drawText(CANVAS_WIDTH/4-200, CANVAS_HEIGT/2 +200, 25, info, br);
}
void Game::drawLevelScreen()
{
	//draw background
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "BackgroundImg_s.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGT / 2, CANVAS_WIDTH, CANVAS_HEIGT, br);

	//draw player 
	if (player) {
		player->draw();
	}
	
	//Draw enemies
	for (Enemy* e : enemies) {
		e->draw();
	}

	
	

	//ui/info layer
	//#ifdef GAME_DEBUG

	if (player && debug_mode) {
		char info[40];
		sprintf_s(info, "(%6.0f,%6.2f,%6.2f)", player->getPosX(), player->getPosY(), player->getOri());
		graphics::drawText(50, 50, 40, info, br);

	}
	//show score up left corner
	char score_c[40];
	sprintf_s(score_c,"Score:%6.0f/25000",  score);
	graphics::drawText(40.0f, 20.0f , 30, score_c, br);

	
	

}
void Game::updateBossScreen()
{
	//create boss if not exist
	if ( !boss_initialized) {
		boss = new Boss(*this);
		boss_initialized = true;
		boss->setActive(true);
	}
	//update player
	if (player) {
		player->update();

	}
	//update boss
	if (boss) {
		boss->update();
	}

	//debuging
	//std::cout << boss->getBossLife() << endl;
	//std::cout << boss->isActive() << endl;
	
checkCollisionBoss();
//delete boss if dead and chance stage credits
	if (!boss->isActive()) {
		delete boss;
		boss = nullptr;
		graphics::playMusic(std::string(ASSET_PATH) + "SpongeBob.mp3", 0.5f, false, 1000);

		status = STATUS_CREDITS;
	}

	//delete player if dead 
	checkPlayer();
}
//draw boss - player - background- score
void Game::drawBossScreen()
{
	graphics::Brush br;

	br.texture = std::string(ASSET_PATH) + "BackgroundImg_s.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGT / 2, CANVAS_WIDTH, CANVAS_HEIGT, br);



	


	char score_c[40];
	sprintf_s(score_c, "Score:%6.0f", score);
	graphics::drawText(40.0f, 20.0f, 30, score_c, br);

	if (boss)
		boss->draw();
	if (player) {
		player->draw();
	}
}
//reset button
void Game::updateDiedScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_R)) {
		status = STATUS_START;	
		reset();

	}


}

void Game::drawDiedScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "Gameover_s.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGT / 2, CANVAS_WIDTH, CANVAS_HEIGT, br);
}

void Game::updateCreditsScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_R)) {
		reset();

		status = STATUS_START;
	}

}
//spongbob attack best credits ever
void Game::drawCreditsScreen()
{
	graphics::Brush br;
	graphics::setFont(std::string(ASSET_PATH) + "credits.otf");

	char info[40];
	sprintf_s(info, "Victory!");
	graphics::drawText(400, 80, 50, info, br);
	sprintf_s(info, "Thanks For Playing!");
	graphics::drawText(350, 150, 30, info, br);
	sprintf_s(info, "Created  By Annt & Laladin!");
	graphics::drawText(300, 200, 30, info, br);
	br.texture = std::string(ASSET_PATH) + "credits_s.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGT / 2, CANVAS_WIDTH, CANVAS_HEIGT, br);
}

//call update depending on status
void Game::update()
{
	
	
	switch (status) {
	case STATUS_START:
		updateStartScreen();
		break;
	case STATUS_PLAYING:
		updateLevelScreen();
		break;
	case STATUS_DIED:
		updateDiedScreen();

		break;
	case STATUS_BOSS:
		updateBossScreen();

		break;
	case STATUS_CREDITS:
		updateCreditsScreen();

		break;
	default:
		cout << "Not implemented status" << endl;

	}
}
//call draw depending on status
void Game::draw()
{

	switch (status) {
		case STATUS_START:
			drawStartScreen();
			break;
		case STATUS_PLAYING:
			drawLevelScreen();
			break;
		case STATUS_DIED:
			drawDiedScreen();
			break;
		case STATUS_BOSS:
			drawBossScreen();
			break;
		case STATUS_CREDITS:
			drawCreditsScreen();
			break;
		default:
			cout << "Not implemented status" << endl;
			
	}

}
//set font and music at start
void Game::init()
{
	graphics::playMusic(std::string(ASSET_PATH) + "BackgroundMusic.mp3", 0.5f, true, 4000);

	graphics::setFont(std::string(ASSET_PATH) + "space.ttf");
}

//create vector enemy 
Game::Game()
{
	enemies = vector<Enemy*>();

}
Game::~Game()
{
	if (player)
		delete player;
	
	//Delete enemies
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i] != nullptr)
			delete enemies[i];
	}
	
	enemies.clear();
	
}
