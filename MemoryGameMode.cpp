#include "MemoryGameMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>

MemoryGameMode::MemoryGameMode() {
	
	{ // set up the game mode
		_START_CALLED = false;
	}

	// ----- set up game state -----
	pattern = MemoryPattern(difficulty);
	{
		curr_state = INIT;
		next_state = INIT;
		difficulty = 1;
	}

	//----- allocate OpenGL resources -----
	{ //vertex buffer:
		glGenBuffers(1, &vertex_buffer);
		//for now, buffer will be un-filled.

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}

	{ //vertex array mapping buffer for color_texture_program:
		//ask OpenGL to fill vertex_buffer_for_color_texture_program with the name of an unused vertex array object:
		glGenVertexArrays(1, &vertex_buffer_for_color_texture_program);

		//set vertex_buffer_for_color_texture_program as the current vertex array object:
		glBindVertexArray(vertex_buffer_for_color_texture_program);

		//set vertex_buffer as the source of glVertexAttribPointer() commands:
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

		//set up the vertex array object to describe arrays of MemoryGameMode::Vertex:
		glVertexAttribPointer(
			color_texture_program.Position_vec4, //attribute
			3, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(Vertex), //stride
			(GLbyte *)0 + 0 //offset
		);
		glEnableVertexAttribArray(color_texture_program.Position_vec4);
		//[Note that it is okay to bind a vec3 input to a vec4 attribute -- the w component will be filled with 1.0 automatically]

		glVertexAttribPointer(
			color_texture_program.Color_vec4, //attribute
			4, //size
			GL_UNSIGNED_BYTE, //type
			GL_TRUE, //normalized
			sizeof(Vertex), //stride
			(GLbyte *)0 + 4*3 //offset
		);
		glEnableVertexAttribArray(color_texture_program.Color_vec4);

		glVertexAttribPointer(
			color_texture_program.TexCoord_vec2, //attribute
			2, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(Vertex), //stride
			(GLbyte *)0 + 4*3 + 4*1 //offset
		);
		glEnableVertexAttribArray(color_texture_program.TexCoord_vec2);

		//done referring to vertex_buffer, so unbind it:
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//done setting up vertex array object, so unbind it:
		glBindVertexArray(0);

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}

	{ //solid white texture:
		//ask OpenGL to fill white_tex with the name of an unused texture object:
		glGenTextures(1, &white_tex);

		//bind that texture object as a GL_TEXTURE_2D-type texture:
		glBindTexture(GL_TEXTURE_2D, white_tex);

		//upload a 1x1 image of solid white to the texture:
		glm::uvec2 size = glm::uvec2(1,1);
		std::vector< glm::u8vec4 > data(size.x*size.y, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

		//set filtering and wrapping parameters:
		//(it's a bit silly to mipmap a 1x1 texture, but I'm doing it because you may want to use this code to load different sizes of texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//since texture uses a mipmap and we haven't uploaded one, instruct opengl to make one for us:
		glGenerateMipmap(GL_TEXTURE_2D);

		//Okay, texture uploaded, can unbind it:
		glBindTexture(GL_TEXTURE_2D, 0);

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}
}

MemoryGameMode::~MemoryGameMode() {

	//----- free OpenGL resources -----
	glDeleteBuffers(1, &vertex_buffer);
	vertex_buffer = 0;

	glDeleteVertexArrays(1, &vertex_buffer_for_color_texture_program);
	vertex_buffer_for_color_texture_program = 0;

	glDeleteTextures(1, &white_tex);
	white_tex = 0;

	// ----- free game state ----- 

}

bool MemoryGameMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	switch (curr_state)
	{
		case INIT:
			if (evt.key.keysym.sym == SDLK_SPACE)
			{
				pattern.beginDrawing();
				next_state = PATTERN_DELIVERY;
			}
			break;
		case PATTERN_DELIVERY:
			break;
		case PATTERN_RECALL:
			{
				auto check_input = [this](MemoryPattern::Direction d)
				{
					if (pattern.pattern[recall_tile_index] != d)
					{
						std::cout << "incorrect :(" << std::endl;
						next_state = PATTERN_RECALL;
						difficulty = 1;
					} else if (recall_tile_index >= pattern.pattern.size())
					{
						std::cout << "correct!!" << std::endl;
						next_state = PATTERN_DELIVERY;
					}
					return;
				};
				MemoryPattern::Direction dir = pattern.pattern[recall_tile_index];
				if (evt.key.type == SDL_KEYDOWN)
				{
					if (evt.key.keysym.sym == SDLK_UP || evt.key.keysym.sym == SDLK_w)
					{
						check_input(dir);
					} else if (evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_s)
					{
						check_input(dir);
					} else if (evt.key.keysym.sym == SDLK_LEFT || evt.key.keysym.sym == SDLK_a)
					{
						check_input(dir);
					} else if (evt.key.keysym.sym == SDLK_RIGHT || evt.key.keysym.sym == SDLK_d)
					{
						check_input(dir);
					}
					recall_tile_index ++;
				}

			}
			break;
		case FINISH:
			break;
		case NONE:
			break;
	}

	return false;
}

void MemoryGameMode::start()
{
	curr_state = INIT;
}

void MemoryGameMode::update(float elapsed) {


	if (!_START_CALLED)
	{
		start();
		_START_CALLED = true;
	}

	// next state logic
	switch (curr_state)
	{
		case INIT:
			// INIT will transition into PATTERN_DELIVERY when the spacebar is 
			// pressed
			break;
		case PATTERN_DELIVERY:
			if (pattern.isDoneDrawing())
			{
				std::cout << "is done drawing" << std::endl;
				next_state = PATTERN_RECALL;
				recall_tile_index = 0;
			} else {
				pattern.update(elapsed);
			}
			break;
		case PATTERN_RECALL: // next state logic defined in handle_input	
			break;
		case FINISH:
			break;
		case NONE:
			break;
	}

	static std::mt19937 mt; //mersenne twister pseudo-random number generator
	//std::cout << curr_state << std::endl; 
}

void MemoryGameMode::draw(glm::uvec2 const &drawable_size) {

	vertices.clear();

	//other useful drawing constants:
	const float wall_radius = 0.05f;
	// const float shadow_offset = 0.07f;
	const float padding = 0.14f; //padding between outside of walls and edge of window


	// ----- draw items based on the current game state -----
	switch (curr_state)
	{
		case INIT:
			draw_init();
			break;
		case PATTERN_RECALL:
			draw_pattern_recall();
			break;
		case PATTERN_DELIVERY:
			draw_pattern_delivery();
			break;
		case FINISH:
			draw_finish();
			break;
		case NONE:
			std::cerr << "Game has entered NONE state, exiting... " << std::endl;
			exit(0);
			break;
	}

	curr_state = next_state;
	// walls:
	draw_rectangle(vertices, glm::vec2(-court_radius.x-wall_radius, 0.0f), glm::vec2(wall_radius, court_radius.y + 2.0f * wall_radius), fg_color);
	draw_rectangle(vertices, glm::vec2( court_radius.x+wall_radius, 0.0f), glm::vec2(wall_radius, court_radius.y + 2.0f * wall_radius), fg_color);
	draw_rectangle(vertices, glm::vec2( 0.0f,-court_radius.y-wall_radius), glm::vec2(court_radius.x, wall_radius), fg_color);
	draw_rectangle(vertices, glm::vec2( 0.0f, court_radius.y+wall_radius), glm::vec2(court_radius.x, wall_radius), fg_color);

	// draw_rectangle(&vertices, ball, ball_radius, fg_color);

	//scores:
	glm::vec2 score_radius = glm::vec2(0.1f, 0.1f);

	//------ compute court-to-window transform ------

	//compute area that should be visible:
	glm::vec2 scene_min = glm::vec2(
		-court_radius.x - 2.0f * wall_radius - padding,
		-court_radius.y - 2.0f * wall_radius - padding
	);
	glm::vec2 scene_max = glm::vec2(
		court_radius.x + 2.0f * wall_radius + padding,
		court_radius.y + 2.0f * wall_radius + 3.0f * score_radius.y + padding
	);

	//compute window aspect ratio:
	float aspect = drawable_size.x / float(drawable_size.y);
	//we'll scale the x coordinate by 1.0 / aspect to make sure things stay square.

	//compute scale factor for court given that...
	float scale = std::min(
		(2.0f * aspect) / (scene_max.x - scene_min.x), //... x must fit in [-aspect,aspect] ...
		(2.0f) / (scene_max.y - scene_min.y) //... y must fit in [-1,1].
	);

	glm::vec2 center = 0.5f * (scene_max + scene_min);

	//build matrix that scales and translates appropriately:
	glm::mat4 court_to_clip = glm::mat4(
		glm::vec4(scale / aspect, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, scale, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(-center.x * (scale / aspect), -center.y * scale, 0.0f, 1.0f)
	);
	//NOTE: glm matrices are specified in *Column-Major* order,
	// so each line above is specifying a *column* of the matrix(!)

	//also build the matrix that takes clip coordinates to court coordinates (used for mouse handling):
	clip_to_court = glm::mat3x2(
		glm::vec2(aspect / scale, 0.0f),
		glm::vec2(0.0f, 1.0f / scale),
		glm::vec2(center.x, center.y)
	);
	// ---- actual drawing ----

	// clear the color buffer:
	glClearColor(bg_color.r / 255.0f, bg_color.g / 255.0f, bg_color.b / 255.0f, bg_color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// don't use the depth test:
	glDisable(GL_DEPTH_TEST);

	// upload vertices to vertex_buffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); // set vertex_buffer as current
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STREAM_DRAW); // upload vertices array
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// set color_texture_program as current program:
	glUseProgram(color_texture_program.program);

	// upload OBJECT_TO_CLIP to the proper uniform location:
	glUniformMatrix4fv(color_texture_program.OBJECT_TO_CLIP_mat4, 1, GL_FALSE, glm::value_ptr(court_to_clip));

	// use the mapping vertex_buffer_for_color_texture_program to fetch vertex data:
	glBindVertexArray(vertex_buffer_for_color_texture_program);

	// bind the solid white texture to location zero so things will be drawn just with their colors:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, white_tex);

	// run the OpenGL pipeline:
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(vertices.size()));

	// unbind the solid white texture:
	glBindTexture(GL_TEXTURE_2D, 0);

	// reset vertex array to none:
	glBindVertexArray(0);

	// reset current program to none:
	glUseProgram(0);
	

	GL_ERRORS(); // PARANOIA: print errors just in case we did something wrong.

}

void MemoryGameMode::draw_init()
{
	// draw a blue plus-sign on the screen
	glm::u8vec4 blue = HEX_TO_U8VEC4(0x00ffffff);
	draw_rectangle(vertices, glm::vec2(0, 0), glm::vec2(court_radius.x / 16, court_radius.y / 2), blue);
	draw_rectangle(vertices, glm::vec2(0, 0), glm::vec2(court_radius.x / 2, court_radius.x / 16), blue);
}

void MemoryGameMode::draw_pattern_delivery()
{
	pattern.draw(court_radius);

	{ // Add vertex data from MemoryPattern
		for (auto v = pattern.vertices.begin(); v < pattern.vertices.end(); v++)
		{
			vertices.emplace_back(*v);
		}
	}
}

void MemoryGameMode::draw_pattern_recall()
{
	
}

void MemoryGameMode::draw_finish()
{
	// draw a green plus-sign on the screen
}