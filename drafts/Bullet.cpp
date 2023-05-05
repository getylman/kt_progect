#include <array>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Animation.hpp"
#include "Global.hpp"
#include "MapManager.hpp"
#include "Mushroom.hpp"
#include "Mario.hpp"
#include "Enemy.hpp"
#include "Koopa.hpp"
#include "Goomba.hpp"
#include "Bullet.hpp"

Bullet::Bullet(Mario& entity) : Enemy(entity.x, entity.y), is_friend(false), check_collision(true), flipped(entity.flipped) {
	vertical_speed = ((flipped) ? (-1) : 1) * BULLET_SPEED;
}