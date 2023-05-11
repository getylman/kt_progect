#include <array>
#include <chrono>
#include <cmath>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Animation.hpp"
#include "Global.hpp"
#include "MapManager.hpp"
#include "PowerUp.hpp"
#include "Hero.hpp"
#include "Enemy.hpp"
#include "OrdStar.hpp"
#include "ShootingStar.hpp"
#include "ConvertSketch.hpp"

int main()
{
	unsigned char current_level = 0;

	unsigned short level_finish = 0;

	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;

	//Vector to save and use "stars"
	std::vector<std::shared_ptr<Enemy>> enemies;

	sf::Color background_color = sf::Color(0, 219, 255);

	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Daytime Starfall", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));

	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	MapManager map_manager;

	Hero main_hero;

	convert_sketch(current_level, level_finish, enemies, background_color, map_manager, main_hero);

	previous_time = std::chrono::steady_clock::now();

	while (1 == window.isOpen())
	{
		std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

		lag += delta_time;

		previous_time += delta_time;

		while (FRAME_DURATION <= lag)
		{
			unsigned view_x;

			lag -= FRAME_DURATION;

			while (1 == window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
				{
					window.close();

					break;
				}
				case sf::Event::KeyPressed:
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Enter:
					{
						enemies.clear();

						main_hero.reset();

						convert_sketch(current_level, level_finish, enemies, background_color, map_manager, main_hero);
					}
					}
				}
				}
			}

			//Next level path
			if (CELL_SIZE * level_finish <= main_hero.get_x())
			{
				current_level++;

				enemies.clear();

				main_hero.reset();

				convert_sketch(current_level, level_finish, enemies, background_color, map_manager, main_hero);
			}

			//Keeping hero in the centre of screen
			view_x = std::clamp<int>(round(main_hero.get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map_manager.get_map_width() - SCREEN_WIDTH);

			map_manager.update();

			main_hero.update(view_x, map_manager);

			for (unsigned short a = 0; a < enemies.size(); a++)
			{
				enemies[a]->update(view_x, enemies, map_manager, main_hero);
			}

			for (unsigned short a = 0; a < enemies.size(); a++)
			{
				if (1 == enemies[a]->get_dead(1))
				{
					//Getting rid of dead stars
					enemies.erase(a + enemies.begin());

					a--;
				}
			}

			if (FRAME_DURATION > lag)
			{
				view.reset(sf::FloatRect(view_x, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

				window.setView(view);
				window.clear(background_color);

				//Changing the level depending on background color
				map_manager.draw_map(1, sf::Color(0, 0, 85) == background_color, view_x, window);

				main_hero.draw_mushrooms(view_x, window);

				main_hero.draw_bullet(view_x, window);

				map_manager.draw_map(0, sf::Color(0, 0, 85) == background_color, view_x, window);

				for (unsigned short a = 0; a < enemies.size(); a++)
				{
					enemies[a]->draw(view_x, window);
				}

				main_hero.draw(window);

				window.display();
			}
		}
	}
}
