//
//  RaceMode.hpp
//  
//
//  Created by owen ou on 2021/9/18.
//
#include "Mode.hpp"

#include "Scene.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct RaceMode : Mode {
	RaceMode();
	virtual ~RaceMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;
	
	//player state:
	constexpr float PlayerMaxSpeed = 30.0f;				// player max velocity // TODO: may need to adjust
	float PlayerSpeed = 0.0f;							// player current velocity
	glm::vec3 player_dir = glm::vec3(0.0f, 1.0f, 0.0f);	// player current face direction, (0,1,0))
	glm::vec3 player_at = glm::vec3(0.0f);	// player initial location
	glm::vec3 player_radius = glm::vec3(1.0f, 1.0f, 1.0f);	// size of player's vehicle // TODO: need to figure out number later
	
	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;
	
	//camera:
	Scene::Camera *camera = nullptr;		// TODO: initial camera position should be different to player position
	
};
