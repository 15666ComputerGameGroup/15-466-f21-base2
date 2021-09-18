//
//  RaceMode.cpp
//  
//
//  Created by owen ou on 2021/9/18.
//

#include "RaceMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

// shader
GLuint hexapod_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > hexapod_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("hexapod.pnct"));
	hexapod_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

// asset loading
Load< Scene > hexapod_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("hexapod.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = hexapod_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = hexapod_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

	});
});

RaceMode::RaceMode() : scene(*hexapod_scene) (){
	//TODO: implement this
}

RaceMode::~RaceMode(){
	
}

bool RaceMode::handle_event(SDL_Event const &, glm::uvec2 const &window_size){
	//TODO: implement this
	
	// arrow key controls directions
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		}
	}
	
	return false;
}

void RaceMode::update(float elapsed){
	//TODO: implement this
	
	// camera move with player vehicle, no z axis motion
	// player vehicle always on center
	{
		// forward and reverse
		if (down.pressed && !up.pressed) PlayerSpeed =-1.0f;
		if (!down.pressed && up.pressed) PlayerSpeed = 1.0f;
		// left right should be rotation
		float degree = 0.0f;				// rotation in degree
		if (left.pressed && !right.pressed) degree =-1.0f;
		if (!left.pressed && right.pressed) degree = 1.0f;
		
		// compute movement
		
		PlayerSpeed = (PlayerSpeed <= PlayerMaxSpeed) ? PlayerSpeed : PlayerMaxSpeed;			// speed cap
		
		// update direction
		//https://stackoverflow.com/questions/8844585/glm-rotate-usage-in-opengl
		player_dir = glm::rotate(player_dir, glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate around z axis// TODO: check usage
		player_dir = glm::normalize(player_dir)		// make sure still unit vector
		
		glm::vec2 move = glm::vec2(0.0f);			// translation for both player and camera
		move = player_dir * PlayerSpeed * elapsed	// TODO: check size
		
		// camera transform matrix
		glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 right = frame[0];
		//glm::vec3 up = frame[1];
		glm::vec3 forward = -frame[2];
		
		// move camera
		camera->transform->position += move.x * right + move.y * forward;
		camera->transform->rotation = glm::normalize(
			camera->transform->rotation
			* glm::angleAxis(-player_dir.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::angleAxis(player_dir.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
		);		// TODO: check whether correct
		
		// move player
		player_at.x += move.x;
		player_at.y += move.y;
		
	}
	
}

void RaceMode::draw(glm::uvec2 const &drawable_size){
	//TODO: implement this
	

}
