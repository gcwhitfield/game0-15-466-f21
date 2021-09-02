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

//some nice colors from the course web page:
#define HEX_TO_U8VEC4( HX ) (glm::u8vec4( (HX >> 24) & 0xff, (HX >> 16) & 0xff, (HX >> 8) & 0xff, (HX) & 0xff ))
const glm::u8vec4 bg_color = HEX_TO_U8VEC4(0x193b59ff);
const glm::u8vec4 fg_color = HEX_TO_U8VEC4(0xf2d2b6ff);