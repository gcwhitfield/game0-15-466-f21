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

	// inline helper functions for drawing shapes. The triangles are being counter clockwise.
	void draw_rectangle (glm::vec2 const &center, glm::vec2 const &radius, glm::u8vec4 const &color) {
		vertices.emplace_back(glm::vec3(center.x-radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
		vertices.emplace_back(glm::vec3(center.x+radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
		vertices.emplace_back(glm::vec3(center.x+radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));

		vertices.emplace_back(glm::vec3(center.x-radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
		vertices.emplace_back(glm::vec3(center.x+radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
		vertices.emplace_back(glm::vec3(center.x-radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
	};

    void draw_quadrilateral (glm::vec2 const &top_left, glm::vec2 const &top_right, glm::vec2 const &bot_left, glm::vec2 const &bot_right, glm::u8vec4 const &color) {
        // the body of this function was copied largely from Professor McCann's start code for 'draw_rectangle's
		vertices.emplace_back(glm::vec3(top_left.x, top_left.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
		vertices.emplace_back(glm::vec3(top_right.x, top_right.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
		vertices.emplace_back(glm::vec3(bot_right.x, bot_right.y, 0.0f), color, glm::vec2(0.5f, 0.5f));

		vertices.emplace_back(glm::vec3(top_left.x, top_left.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
		vertices.emplace_back(glm::vec3(bot_right.x, bot_right.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
		vertices.emplace_back(glm::vec3(bot_left.x, bot_left.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    };

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
