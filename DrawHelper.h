// George Whitfield
// gwhitfie@andrew.cmu.edu
// September 2, 2021

#ifndef GLM
#define GLM
#include <glm/glm.hpp>
#endif

// The vertex class was copied from the NEST framework
// draw functions will work on vectors of vertices, defined as follows:
struct Vertex {
    Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
        Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
    glm::vec3 Position;
    glm::u8vec4 Color;
    glm::vec2 TexCoord;
};

// HEX_TO_U8VEC4 was copied from the NEST framework
// some nice colors from the course web page:
#define HEX_TO_U8VEC4( HX ) (glm::u8vec4( (HX >> 24) & 0xff, (HX >> 16) & 0xff, (HX >> 8) & 0xff, (HX) & 0xff ))
const glm::u8vec4 bg_color = HEX_TO_U8VEC4(0x000000ff);
const glm::u8vec4 fg_color = HEX_TO_U8VEC4(0x4361eeff);

// inline helper functions for drawing shapes. The triangles are being counter clockwise.
// draw_rectangle copied from NEST framework
inline void draw_rectangle (std::vector<Vertex> &verts, glm::vec2 const &center, glm::vec2 const &radius, glm::u8vec4 const &color) {
    verts.emplace_back(glm::vec3(center.x-radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    verts.emplace_back(glm::vec3(center.x+radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    verts.emplace_back(glm::vec3(center.x+radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));

    verts.emplace_back(glm::vec3(center.x-radius.x, center.y-radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    verts.emplace_back(glm::vec3(center.x+radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    verts.emplace_back(glm::vec3(center.x-radius.x, center.y+radius.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
};

inline void draw_quadrilateral (std::vector<Vertex> &verts, glm::vec2 const &top_left, glm::vec2 const &top_right, glm::vec2 const &bot_left, glm::vec2 const &bot_right, glm::u8vec4 const &color) {
    // the body of this function was copied largely from Professor McCann's start code for 'draw_rectangle's
    verts.emplace_back(glm::vec3(top_left.x, top_left.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    verts.emplace_back(glm::vec3(top_right.x, top_right.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    verts.emplace_back(glm::vec3(bot_right.x, bot_right.y, 0.0f), color, glm::vec2(0.5f, 0.5f));

    verts.emplace_back(glm::vec3(top_left.x, top_left.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    verts.emplace_back(glm::vec3(bot_right.x, bot_right.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
    verts.emplace_back(glm::vec3(bot_left.x, bot_left.y, 0.0f), color, glm::vec2(0.5f, 0.5f));
};