#include <array>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Animation.hpp"
#include "Global.hpp"
#include "MapManager.hpp"
#include "Bullet.hpp"

Bullet::Bullet(const float i_x, const float i_y, bool flip)
    : x(i_x),
      y(i_y),
      dead(false),
      is_friend(true),
      check_collision(true),
      flipped(!flip),
				bullet_fly_animation(CELL_SIZE, "Resources/Images/BulletFly.png", BULLET_FLY_ANIMATION_SPEED) {
	vertical_speed = 0;
	texture.loadFromFile("Resources/Images/BulletDeath.png");
}

bool Bullet::get_dead() const {
	return dead || collision_dying;
}

void Bullet::die(const unsigned char i_death_type) {
	switch (i_death_type) {
		case 0: {
			//Instant death bcs of collision
			dead = 1;
			break;
		}
		case 1: {
			//Dying from entity collision.
			collision_dying = 1;
		}
	}
}

void Bullet::draw(const unsigned i_view_x, sf::RenderWindow& i_window) {
	//Making sure we don't draw bullet outside the view.
	if (-CELL_SIZE < round(y) && round(x) > static_cast<int>(i_view_x) - CELL_SIZE && round(x) < SCREEN_WIDTH + i_view_x && round(y) < SCREEN_HEIGHT) {
		if (1 == collision_dying || BULLET_DEATH_DURATION > death_timer) {
			sprite.setPosition(round(x), round(y));
			sprite.setTexture(texture);
			i_window.draw(sprite);
		} else {
      bullet_fly_animation.set_flipped(flipped);
			bullet_fly_animation.set_position(round(x), round(y));
			bullet_fly_animation.draw(i_window);
		}
	}
}

void Bullet::update(const unsigned i_view_x, const MapManager& i_map_manager) {
	//Mushrooms behave the same way Goombas do so go check out the OrdStar class.
	char horizontal_direction = !flipped == false ? 1 : -1;

	if (0 == dead)
	{
		if (-CELL_SIZE < y && x >= static_cast<int>(i_view_x) - CELL_SIZE - ENTITY_UPDATE_AREA && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + i_view_x && y < SCREEN_HEIGHT)
		{
			std::vector<unsigned char> collision;

			sf::FloatRect horizontal_hit_box;

			vertical_speed = 0;

			collision = i_map_manager.map_collision({ Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall }, sf::FloatRect(x, vertical_speed + y, CELL_SIZE, CELL_SIZE));

			if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
				{
					return 0 == i_value;
				}))
			{
				vertical_speed = 0;
			} else {
				y += vertical_speed;
			}

			horizontal_hit_box = { BULLET_SPEED * horizontal_direction + x, y, CELL_SIZE, CELL_SIZE };

			collision = i_map_manager.map_collision({ Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall }, horizontal_hit_box);

			if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value) {
				return 0 == i_value;
			})) {
				if (0 < horizontal_direction) {
					x = CELL_SIZE * (floor((BULLET_SPEED * horizontal_direction + x) / CELL_SIZE) - 1);
				} else {
					x = CELL_SIZE * (1 + floor((BULLET_SPEED * horizontal_direction + x) / CELL_SIZE));
				}
        die(0);
				horizontal_direction *= -1;
				flipped = true;
			} else {
				x += POWERUP_SPEED * horizontal_direction;
			}
			bullet_fly_animation.update(); 
		} else if (BULLET_DEATH_DURATION > death_timer) {
			if (0 < death_timer) {
				death_timer--;
			} else {
				die(0);
			}
		}

		if (SCREEN_HEIGHT <= y)
		{
			dead = 1;
		}
	}
}

sf::FloatRect Bullet::get_hit_box() const {
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}