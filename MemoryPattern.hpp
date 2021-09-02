// MemoryPattern.hpp
// George Whitfield gwhitfie@andrew.cmu.edu
// September 2, 2021

#include <vector>
#include <random>
#include "DrawHelper.hpp"

// MemoryPattern is a class that stores information about 
// the current pattern being shown to the player. The game will
// create a new MemoryPattern at the beginning of each
// memory sequence, which randomly generates a sequence of 
// UP, DOWN, LEFT, and RIGHT directions. 
//
// ----- drawing -----
// The MemoryGameMode calls MemoryPattern.update every frame. 
// MemoryPattern.update stores information about 
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
        MemoryPattern(int len);
        ~MemoryPattern();

        bool isDoneDrawing() { return _t >= _drawDuration; };
        bool isDrawing() { return _t > 0.0f && _t < _drawDuration; };
        
        void beginDrawing();

        void update(float elapsed_time);

    private: 
        // ----- drawing -----

        // t is the timestep for drawing each of the tiles
        float _t;

        // the time in seconds it takes to draw the entire sequence from 
        // start to finish
        float _drawDuration;

};