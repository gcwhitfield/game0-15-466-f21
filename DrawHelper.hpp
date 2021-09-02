// George Whitfield
// gwhitfie@andrew.cmu.edu
// September 2, 2021

#include <glm/glm.hpp>

// The vertex class was copied from the NEST framework
// draw functions will work on vectors of vertices, defined as follows:
struct Vertex {
    Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
        Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
    glm::vec3 Position;
    glm::u8vec4 Color;
    glm::vec2 TexCoord;
};