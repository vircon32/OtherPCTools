// *****************************************************************************
    // include project headers
    #include "Encode.hpp"
    #include "Globals.hpp"
    
    // include C/C++ headers
    #include <iostream>     // [ C++ STL ] I/O Streams
    #include <stdexcept>    // [ C++ STL ] Exceptions
    #include <algorithm>    // [ C++ STL ] Algorithms
    
    // declare used namespaces
    using namespace std;
// *****************************************************************************


// =============================================================================
//      FRAME ENCODING FUNCTIONS
// =============================================================================


int32_t EncodeSegment( int StartX, int EndX )
{
    return (StartX << 16) | EndX;
}

// -----------------------------------------------------------------------------

int32_t EncodeNewLine( int LineY )
{
    return NEW_LINE_CODE | LineY;
}

// -----------------------------------------------------------------------------

list< int32_t > EncodeLineSegments( int32_t* Pixels )
{
    list< int32_t > LineSegments;
    bool SegmentWasStarted = false;
    int SegmentStartX = 0;
    
    for( int CurrentX = 0; CurrentX < FRAME_WIDTH; CurrentX++ )
    {
        // get current color
        bool IsBaseColor = (Pixels[ CurrentX ] == BASE_COLOR);
        
        // open segments when first non-base pixel is found
        if( !SegmentWasStarted )
        {
            if( !IsBaseColor )
            {
                SegmentStartX = CurrentX;
                SegmentWasStarted = true;
            }
        }
        
        // close segments when next base pixel is found,
        else if( IsBaseColor )
        {
            // careful: last non-base pixel was the previous one
            LineSegments.push_back( EncodeSegment( SegmentStartX, CurrentX - 1 ) );
            SegmentWasStarted = false;
        }
        
        // close any started segments when end of line is found
        if( SegmentWasStarted && CurrentX == (FRAME_WIDTH-1) )
        {
            LineSegments.push_back( EncodeSegment( SegmentStartX, CurrentX ) );
            SegmentWasStarted = false;
        }
    }
    
    return LineSegments;
}

void EncodeFrame( const PNGImage& Image )
{
    int PreviousSize = EncodedFrames.size();
    
    // keep decoding all horizontal lines
    for( int CurrentLine = 0; CurrentLine < FRAME_HEIGHT; CurrentLine++ )
    {
        list< int32_t > LineSegments = EncodeLineSegments( (int32_t*)Image.RowPixels[ CurrentLine ] );
        
        // add the line if it is not empty
        if( LineSegments.size() > 0 )
        {
            // preface segments with line declaration
            EncodedFrames.push_back( EncodeNewLine( CurrentLine ) );
            
            // copy the data for line segments
            EncodedFrames.insert( EncodedFrames.end(), LineSegments.begin(), LineSegments.end() );
        }
    }
    
    // finally signal the end of this frame
    EncodedFrames.push_back( END_FRAME_CODE );
    
    // save frame stats
    TotalFrames++;
    int FrameWords = EncodedFrames.size() - PreviousSize;
    
    if( FrameWords > HighestFrameWords )
      HighestFrameWords = FrameWords;
}
