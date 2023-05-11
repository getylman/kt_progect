#include <array>
#include <cmath>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Animation.hpp"
#include "Global.hpp"
#include "MapManager.hpp"
#include "PowerUp.hpp"
#include "Hero.hpp"
#include "Enemy.hpp"
#include "OrdStar.hpp"

OrdStar::OrdStar(const bool i_underground, const float i_x, const float i_y) :
	Enemy(i_x, i_y),
	no_collision_dying(0),
	underground(i_underground),
	death_timer(ORDSTAR_DEATH_DURATION),
	walk_animation(CELL_SIZE, "Resources/Images/GoombaWalk.png", ORDSTAR_WALK_ANIMATION_SPEED)
{
	horizontal_speed = -ORDSTAR_SPEED;

	if (0 == underground)
	{
		texture.loadFromFile("Resources/Images/GoombaDeath0.png");
	}
	else
	{
		texture.loadFromFile("Resources/Images/UndergroundGoombaDeath0.png");

		walk_animation.set_texture_location("Resources/Images/UndergroundGoombaWalk.png");
	}
}

bool OrdStar::get_dead(const bool i_deletion) const
{
	if (1 == i_deletion)
	{
		return dead;
	}
	else
	{
		return dead || no_collision_dying || ORDSTAR_DEATH_DURATION > death_timer;
	}
}

void OrdStar::die(const unsigned char i_death_type)
{
	switch (i_death_type)
	{
	case 0:
	{
		//Instant death. Setting dead to 1 will immediately delete the object. NILL KIGGERS
		dead = 1;

		break;
	}
	case 1:
	{
		//Star is dead by Naby-Eu
		if (0 == no_collision_dying)
		{
			death_timer--;
		}

		break;
	}
	case 2:
	{
		if (ORDSTAR_DEATH_DURATION == death_timer)
		{
			//Star is dead by weapon
			no_collision_dying = 1;

			vertical_speed = 0.5f * HERO_JUMP_SPEED;

			if (0 == underground)
			{
				texture.loadFromFile("Resources/Images/GoombaDeath1.png");
			}
			else
			{
				texture.loadFromFile("Resources/Images/UndergroundGoombaDeath1.png");
			}
		}
	}
	}
}

void OrdStar::draw(const unsigned i_view_x, sf::RenderWindow& i_window)
{
	//Making sure we don't draw Star outside the view.
	if (-CELL_SIZE < round(y) && round(x) > static_cast<int>(i_view_x) - CELL_SIZE && round(x) < SCREEN_WIDTH + i_view_x && round(y) < SCREEN_HEIGHT)
	{
		if (1 == no_collision_dying || ORDSTAR_DEATH_DURATION > death_timer)
		{
			sprite.setPosition(round(x), round(y));
			sprite.setTexture(texture);

			i_window.draw(sprite);
		}
		else
		{
			walk_animation.set_position(round(x), round(y));
			walk_animation.draw(i_window);
		}
	}
}

void OrdStar::update(const unsigned i_view_x, const std::vector<std::shared_ptr<Enemy>>& i_enemies, const MapManager& i_map_manager, Hero& i_mario)
{
	//Making sure we don't update Star outside the view.
	if (-CELL_SIZE < y && x >= static_cast<int>(i_view_x) - CELL_SIZE - ENTITY_UPDATE_AREA && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + i_view_x && y < SCREEN_HEIGHT)
	{
		std::vector<unsigned char> collision;

		sf::FloatRect hit_box = get_hit_box();

		vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

		hit_box.top += vertical_speed;

		collision = i_map_manager.map_collision({ Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall }, hit_box);

		if (0 == no_collision_dying)
		{
			if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
				{
					return 0 == i_value;
				}))
			{
				if (0 > vertical_speed)
				{
					y = CELL_SIZE * (1 + floor((vertical_speed + y) / CELL_SIZE));
				}
				else
				{
					y = CELL_SIZE * (ceil((vertical_speed + y) / CELL_SIZE) - 1);
				}

				vertical_speed = 0;
			}
			else
			{
				bool changed = 0;

				//Here we're making sure that when Star falls on another enemy, they don't intersect.
				if (0 == get_dead(0))
				{
					for (unsigned short a = 0; a < i_enemies.size(); a++)
					{
						if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == hit_box.intersects(i_enemies[a]->get_hit_box()))
						{
							changed = 1;

							if (0 > vertical_speed)
							{
								y = i_enemies[a]->get_hit_box().height + i_enemies[a]->get_hit_box().top;
							}
							else
							{
								y = i_enemies[a]->get_hit_box().top - CELL_SIZE;
							}

							vertical_speed = 0;

							break;
						}
					}
				}

				if (0 == changed)
				{
					y += vertical_speed;
				}
			}

			if (0 == get_dead(0))
			{
				hit_box = get_hit_box();
				hit_box.left += horizontal_speed;

				collision = i_map_manager.map_collision({ Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall }, hit_box);

				if (0 == std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
					{
						return 0 == i_value;
					}))
				{
					if (0 < horizontal_speed)
					{
						x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
					}
					else
					{
						x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
					}

					horizontal_speed *= -1;
				}
				else
				{
					bool changed = 0;

					//Changing direction when colliding with another enemy.
					for (unsigned short a = 0; a < i_enemies.size(); a++)
					{
						if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == hit_box.intersects(i_enemies[a]->get_hit_box()))
						{
							changed = 1;

							horizontal_speed *= -1;

							break;
						}
					}

					if (0 == changed)
					{
						x += horizontal_speed;
					}
				}

				if (0 == i_mario.get_dead() && 1 == get_hit_box().intersects(i_mario.get_hit_box()))
				{
					if (0 < i_mario.get_vertical_speed())
					{
						die(1);

						i_mario.set_vertical_speed(0.5f * HERO_JUMP_SPEED);
					}
					else
					{
						i_mario.die(0);
					}
				}

				if (i_mario.get_dead() == 0) {
					for (Bullet& bullet : i_mario.get_bullets_vector()) {
						if (get_hit_box().intersects(bullet.get_hit_box())) {
							die(2);
							bullet.die(1);
						}
					}
				}

				walk_animation.update();
			}
			else if (ORDSTAR_DEATH_DURATION > death_timer)
			{
				if (0 < death_timer)
				{
					death_timer--;
				}
				else
				{
					die(0);
				}
			}
		}
		else
		{
			y += vertical_speed;
		}
	}

	if (SCREEN_HEIGHT <= y)
	{
		die(0);
	}
}
