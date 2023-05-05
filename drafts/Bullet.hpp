#pragma once

#include "Animation.hpp"
#include "Enemy.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

class Bullet : public Enemy {
	bool is_friend;
	// bullet has been shooted from Main Charecter -> true else -> false
	bool check_collision;
	bool flipped;

	sf::Sprite sprite;

	sf::Texture texture;

	Animation walk_animation;

public:
	Bullet(Enemy& entity);
	Bullet(Mario& entity);
	bool get_dead(const bool i_deletion) const;

	void die(const unsigned char i_death_type);
	void draw(const unsigned i_view_x, sf::RenderWindow& i_window);
	void update(const unsigned i_view_x, const std::vector<std::shared_ptr<Enemy>>& i_enemies, const MapManager& i_map_manager, Mario& i_mario);
};