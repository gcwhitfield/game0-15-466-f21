// George Whitfield
// gwhitfie@andrew.cmu.edu
// September 2, 2021

#include "MemoryPattern.hpp"

MemoryPattern::MemoryPattern(int len)
{
    static std::mt19937 rand; 
    pattern = std::vector<Direction>(len);
    vertices = std::vector<Vertex>();
    for (int i = 0; i < len; i++)
    {
        // the following line was inspired from enum tutorial on static casting
        // https://riptutorial.com/cplusplus/example/18751/enum-conversions
        pattern[i] = static_cast<Direction>(rand() % NUM_DIRECTIONS);
    }
}

MemoryPattern::~MemoryPattern()
{

}

void MemoryPattern::update(float elapsed_time)
{
    // step 1) calculate which tile is supposed to be activated
    int tile = (unsigned)std::floor(elapsed_time / _timePerTile);
    Direction dir = pattern[tile];

    // step 2) draw the tiles
    glm::u8vec4 top    = dir == UP ? _top_tile_color : _deactivated_tile_color;
    glm::u8vec4 left   = dir == LEFT ? _left_tile_color : _deactivated_tile_color;
    glm::u8vec4 right  = dir == RIGHT ?_right_tile_color : _deactivated_tile_color;
    glm::u8vec4 bottom = dir == DOWN ? _bottom_tile_color : _deactivated_tile_color;
    (void)top;
    (void)left;
    (void)right;
    (void)bottom;
    // left 
    draw_rectangle(vertices, glm::vec2(50, 50), glm::vec2(20, 20), left);
    // top

    // right

    // bottom 

}

void MemoryPattern::beginDrawing()
{
    // 1) calculate the amount of time that is needed to draw the entire pattern
    _drawDuration = pattern.size() * _timePerTile;
    _t = 0;
}
