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

void MemoryPattern::update(float elapsed_time)
{
    // step 1) calculate which tile is supposed to be activated
    // int tile = (unsigned)std::floor(elapsed_time / _timePerTile);
    // Direction dir = pattern[tile];

    // step 2) left 
    
    // step 3) top

    // step 4) right

    // step 5) bottom 

}

void MemoryPattern::beginDrawing()
{
    // 1) calculate the amount of time that is needed to draw the entire pattern
    _drawDuration = pattern.size() * _timePerTile;
    _t = 0;
}
