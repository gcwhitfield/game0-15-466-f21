#include "ColorTextureProgram.hpp"

#include "Mode.hpp"
#include "GL.hpp"

#ifndef GLM
#define GLM
#include <glm/glm.hpp>
#endif

#include <vector>
#include <deque>
#include "MemoryPattern.hpp"

#ifndef DRAW_HELPER
#define DRAW_HELPER
#include "DrawHelper.hpp"
#endif

/*
 * MemoryGameMode is a game mode that implements a single-player game of Pong.
 */

struct MemoryGameMode : Mode {
	MemoryGameMode();
	virtual ~MemoryGameMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----
	glm::vec2 court_radius = glm::vec2(7.0f, 5.0f);
	MemoryPattern pattern;

	// ----- draw -----
	static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "MemoryGameMode::Vertex should be packed");

	//vertices will be accumulated into this list and then uploaded+drawn at the end of the 'draw' function:
	std::vector< Vertex > vertices;

	//----- opengl assets / helpers ------

	//Shader program that draws transformed, vertices tinted with vertex colors:
	ColorTextureProgram color_texture_program;

	//Buffer used to hold vertex data during drawing:
	GLuint vertex_buffer = 0;

	//Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
	GLuint vertex_buffer_for_color_texture_program = 0;

	//Solid white texture:
	GLuint white_tex = 0;

	//matrix that maps from clip coordinates to court-space coordinates:
	glm::mat3x2 clip_to_court = glm::mat3x2(1.0f);
	// computed in draw() as the inverse of OBJECT_TO_CLIP
	// (stored here so that the mouse handling code can use it to position the paddle)

};
