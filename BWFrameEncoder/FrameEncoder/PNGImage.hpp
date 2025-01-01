// *****************************************************************************
    // start include guard
    #ifndef PNGIMAGE_HPP
    #define PNGIMAGE_HPP
    
    // include C/C++ headers
    #include <string>       // [ C++ STL ] Strings
    #include <stdint.h>     // [ ANSI C ] Standard integers
// *****************************************************************************


// =============================================================================
//      PNG IMAGE CLASS
// =============================================================================


class PNGImage
{
    public:
    
        // dimensions in pixels
        int Width, Height;
        
        // pixel data
        uint8_t** RowPixels;
        
    public:
        
        // instance handling
        PNGImage();
       ~PNGImage();
        
        // file I/O
        void LoadFromFile( const std::string& PNGFilePath );
        void SaveToFile( const std::string& PNGFilePath ) const;
};


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
