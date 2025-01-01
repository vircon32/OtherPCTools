// *****************************************************************************
    // start include guard
    #ifndef ENCODE_HPP
    #define ENCODE_HPP
    
    // include project headers
    #include "PNGImage.hpp"
    #include "Globals.hpp"
    
    // include C/C++ headers
    #include <list>         // [ C++ STL ] Lists
    #include <stdint.h>     // [ ANSI C ] Standard integers
// *****************************************************************************


// =============================================================================
//      FRAME ENCODING FUNCTIONS
// =============================================================================


// we encode each "instruction" to draw a frame in a single
// 32-bit word by using its 2 halves as separate 16-bit integers
int32_t EncodeSegment( int StartX, int EndX );
int32_t EncodeNewLine( int LineY );

// functions to encode whole lines and frames
std::list< int32_t > EncodeLineSegments( int32_t* Pixels );
void EncodeFrame( const PNGImage& Image );


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
