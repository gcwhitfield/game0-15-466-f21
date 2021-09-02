// George Whitfield
// gwhitfie@andrew.cmu.edu
// September 2, 2021

#include "MemoryPattern.hpp"

MemoryPattern::MemoryPattern(int len)
{
    static std::mt19937 rand; 
    pattern = std::vector<Direction>(len);
    for (int i = 0; i < len; i++)
    {
        // the following line was inspired from enum tutorial on static casting
        // https://riptutorial.com/cplusplus/example/18751/enum-conversions
        pattern[i] = static_cast<Direction>(rand() % NUM_DIRECTIONS);
    }
}

void MemoryPattern::update(float elapsed_time)
{

}

void MemoryPattern::beginDrawing()
{
    // 1) calculate the amount of time that is needed to draw the entire pattern
}
