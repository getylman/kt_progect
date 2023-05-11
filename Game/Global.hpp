#pragma once

constexpr float BRICK_PARTICLE_SPEED = 4;
//A coin jumping out of the question block.
constexpr float COIN_JUMP_SPEED = -6;
constexpr float ORDSTAR_SPEED = 1;
constexpr float GRAVITY = 0.25f;
constexpr float NEUTRON_SPEED = 4;
constexpr float SHOOTINGSTAR_SPEED = 1;
constexpr float HERO_ACCELERATION = 0.25f;
constexpr float HERO_JUMP_SPEED = -4;
constexpr float HERO_WALK_SPEED = 2;
constexpr float MAX_VERTICAL_SPEED = 8;
constexpr float POWERUP_SPEED = 1;
constexpr float BULLET_SPEED = 3;

constexpr unsigned char CELL_SIZE = 16;
//We use this constant when Hero is growing or when he's invincible.
constexpr unsigned char HERO_BLINKING = 4;
//Lemme explain:
//1) Hero gets hit by an enemy.
//2) Hero looks at you like "Oh no! Me lost! Mama mia!" and pauses for this amount.
//3) Hero bounces off the screen.
constexpr unsigned char HERO_DEATH_DURATION = 32;
constexpr unsigned char HERO_POWERUPPING_DURATION = 64;
constexpr unsigned char HERO_INVINCIBILITY_DURATION = 128;
//The longer we press the jump button, the higher Hero jumps.
constexpr unsigned char HERO_JUMP_TIMER = 8;
constexpr unsigned char SCREEN_RESIZE = 2;
constexpr unsigned char SHOOTING_TIME_DELAY = 32;

constexpr unsigned short COIN_ANIMATION_SPEED = 4;
//Since we deactivate everything outside the view, we need to have a safe area around the view where objects start updating.
constexpr unsigned short ENTITY_UPDATE_AREA = 64;
constexpr unsigned short ORDSTAR_DEATH_DURATION = 32;
constexpr unsigned short BULLET_DEATH_DURATION = 16; 
//Why do I have so many animation speed constants? I use them only once.
constexpr unsigned short ORDSTAR_WALK_ANIMATION_SPEED = 32;
constexpr unsigned short SHOOTINGSTAR_REINCARNETING_SPEED = 4;
constexpr unsigned short SHOOTINGSTAR_REINCARNATION_DURATION = 512;
constexpr unsigned short SHOOTINGSTAR_WALK_ANIMATION_SPEED = 32;
constexpr unsigned short HERO_WALK_ANIMATION_SPEED = 4;
constexpr unsigned short BULLET_FLY_ANIMATION_SPEED = 2;
constexpr unsigned short QUESTION_BLOCK_ANIMATION_SPEED = 8;
constexpr unsigned short SCREEN_HEIGHT = 240;
constexpr unsigned short SCREEN_WIDTH = 320;

constexpr std::chrono::microseconds FRAME_DURATION(16667);

enum Cell
{
	ActivatedQuestionBlock,
	Brick,
	Coin,
	Empty,
	Pipe,
	QuestionBlock,
	Wall
};

//We use this for brick particles.
//Damn, I should've called it "Particle"! I'm so dumb!
struct Object
{
	float horizontal_speed;
	float vertical_speed;
	float x;
	float y;

	Object(const float i_x, const float i_y, const float i_horizontal_speed = 0, const float i_vertical_speed = 0) :
		horizontal_speed(i_horizontal_speed),
		vertical_speed(i_vertical_speed),
		x(i_x),
		y(i_y)
	{

	}
};

typedef std::vector<std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> Map;