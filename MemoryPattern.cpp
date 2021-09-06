// George Whitfield
// gwhitfie@andrew.cmu.edu
// September 2, 2021

#include "MemoryPattern.hpp"

MemoryPattern::MemoryPattern(int len)
{
    static std::mt19937 rand; 
    pattern = std::vector<Direction>(len);
    // vertices = std::vector<Vertex>();
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
    _t += elapsed_time;
    if (_t > _drawDuration)
        _t = 0;
}

void MemoryPattern::draw(glm::vec2 const &drawable_size)
{
    vertices.clear();
    // step 1) calculate which tile is supposed to be activated
    int tile = (unsigned)std::floor(_t / _timePerTile);
    Direction dir = pattern[tile];

    // step 2) draw the tiles
    glm::u8vec4 top_col    = dir == UP ? _top_tile_color : _deactivated_tile_color;
    glm::u8vec4 left_col   = dir == LEFT ? _left_tile_color : _deactivated_tile_color;
    glm::u8vec4 right_col  = dir == RIGHT ?_right_tile_color : _deactivated_tile_color;
    glm::u8vec4 bottom_col = dir == DOWN ? _bottom_tile_color : _deactivated_tile_color;

    glm::vec2 top_tile_location = glm::vec2(0, drawable_size.y / 4.0f);
    glm::vec2 bottom_tile_location = glm::vec2(0, -drawable_size.y / 4.0f);
    glm::vec2 left_tile_location = glm::vec2(-drawable_size.x / 4.0f, 0);
    glm::vec2 right_tile_location = glm::vec2(drawable_size.x / 4.0f, 0);

    (void)left_col;
    draw_rectangle(vertices, left_tile_location, glm::vec2(0.3f, 0.4f), left_col);
    draw_rectangle(vertices, right_tile_location, glm::vec2(0.3f, 0.4f), right_col);
    draw_rectangle(vertices, top_tile_location, glm::vec2(0.3f, 0.4f), top_col);
    draw_rectangle(vertices, bottom_tile_location, glm::vec2(0.3f, 0.4f), bottom_col);


}

void MemoryPattern::beginDrawing()
{
    // 1) calculate the amount of time that is needed to draw the entire pattern
    _drawDuration = pattern.size() * _timePerTile;
    _t = 0.0f;
}
