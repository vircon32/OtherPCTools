// *****************************************************************************
    // include project headers
    #include "Globals.hpp"
    
    // declare used namespaces
    using namespace std;
// *****************************************************************************


// =============================================================================
//      GLOBAL VARIABLES
// =============================================================================


// actual working objects
vector< int32_t > EncodedFrames;
PNGImage CurrentFrame;

// storing data for statistics
int HighestFrameWords = 0;
int TotalFrames = 0;
