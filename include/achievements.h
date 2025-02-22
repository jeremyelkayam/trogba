
#pragma once


namespace trog {
    struct achievement
    {
        //Value can represent either a 64-bit integer 
        // or a sequence of 64 boolean values. If number is true 
        // it's an int, if false it's bools

        // If value represents 64 bools they MUST be right-to-left
        // And threshold MUST be a power of two corresponding
        // to the number of bools, e.g. 8 = four bools (2^4),
        // or in binary 1111
        long value, threshold;
        bool number;
    };
}