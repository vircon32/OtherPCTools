// *****************************************************************************
    // start include guard
    #ifndef GLOBALS_HPP
    #define GLOBALS_HPP
    
    // include project headers
    #include "PNGImage.hpp"
    
    // include C/C++ headers
    #include <vector>       // [ C++ STL ] Vectors
    #include <stdint.h>     // [ ANSI C ] Standard integers
// *****************************************************************************


// =============================================================================
//      GENERAL DEFINITIONS
// =============================================================================


// frame dimensions, in pixels
#define FRAME_WIDTH   480
#define FRAME_HEIGHT  360

// constants for the encoder
#define BASE_COLOR      (int32_t)0xFFFFFFFF   // background = full-opacity white
#define NEW_LINE_CODE   (int32_t)0xFFFF0000   // lines are started with { 0xFFFF, <line_y> }
#define END_FRAME_CODE  (int32_t)0xFFFFFFFF   // this code indicates current frame has ended
#define END_VIDEO_CODE  (int32_t)0xFFFFEEEE   // this code indicates there are no more frames


// =============================================================================
//      GLOBAL VARIABLES
// =============================================================================


// actual working objects
extern std::vector< int32_t > EncodedFrames;
extern PNGImage CurrentFrame;

// storing data for statistics
extern int HighestFrameWords;
extern int TotalFrames;


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
