// MemoryPattern.hpp
// George Whitfield gwhitfie@andrew.cmu.edu
// August 31, 2021

#include <vector>
#include <random>

class MemoryPattern {

    public:
        
        static const int NUM_DIRECTIONS = 4;

        enum Direction
        {
            UP = 0,
            DOWN = 1,
            LEFT = 2,
            RIGHT = 3
        };

        std::vector<Direction> pattern;

        // randomly generates a new pattern of size 'len'
        MemoryPattern(int len)
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

};