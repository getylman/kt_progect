#pragma once

class Bullet {
	bool is_friend;
	// bullet has been shooted from Main Charecter -> true else -> false
	bool check_collision = 0;
	bool collision_dying = 0;
	bool flipped;
	bool dead;

	float x;
  float y;
  float vertical_speed;

	unsigned short death_timer = BULLET_DEATH_DURATION;

	sf::Sprite sprite;

	sf::Texture texture;

	Animation bullet_fly_animation;

public:
  Bullet(const float i_x, const float i_y, bool flip);

	bool get_dead() const;

	void die(const unsigned char i_death_type);
	void draw(const unsigned i_view_x, sf::RenderWindow& i_window);
  void update(const unsigned i_view_x, const MapManager& i_map_manager);

	sf::FloatRect get_hit_box() const;
};