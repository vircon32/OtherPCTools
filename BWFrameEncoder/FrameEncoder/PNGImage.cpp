// *****************************************************************************
    // include project headers
    #include "PNGImage.hpp"
    
    // include libpng headers
    #include <png.h>
    
    // include C/C++ headers
    #include <iostream>     // [ C++ STL ] I/O Streams
    #include <stdexcept>    // [ C++ STL ] Exceptions
    #include <cstring>      // [ ANSI C ] Strings
    
    // declare used namespaces
    using namespace std;
// *****************************************************************************


// =============================================================================
//      PNG IMAGE CLASS
// =============================================================================


PNGImage::PNGImage()
{
    Width = Height = 0;
    RowPixels = nullptr;
}

// -----------------------------------------------------------------------------

PNGImage::~PNGImage()
{
    // free image pixels if any
    for( int y = 0; y < Height; y++ )
      free( RowPixels[ y ] );
    
    free( RowPixels );
    RowPixels = nullptr;
    
    Width = Height = 0;
}

// -----------------------------------------------------------------------------

void PNGImage::LoadFromFile( const string& PNGFilePath )
{
    // open input file
    FILE *PNGFile = fopen( PNGFilePath.c_str(), "rb" );
    
    if( !PNGFile )
      throw runtime_error( string("cannot open input file \"") + PNGFilePath + "\"" );
    
    // allocate structures to hold PNG information
    png_structp PNGHandler = png_create_read_struct( PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr );
    
    if(!PNGHandler)
      throw runtime_error( "cannot allocate PNG handler structure" );

    png_infop PNGInfo = png_create_info_struct( PNGHandler );
    
    if(!PNGInfo)
      throw runtime_error( "cannot allocate PNG information structure" );
    
    // define a callback function expected by libpng for error handling
    if( setjmp( png_jmpbuf( PNGHandler ) ) )
      throw runtime_error( "cannot define an error callback for PNG loading functions" );
    
    // read basic information into the structures we created
    png_init_io( PNGHandler, PNGFile );
    png_read_info( PNGHandler, PNGInfo );
    
    // extract their basic fields using the premade functions
    Width  = png_get_image_width ( PNGHandler, PNGInfo );
    Height = png_get_image_height( PNGHandler, PNGInfo );
    
    uint8_t ColorType = png_get_color_type( PNGHandler, PNGInfo );
    uint8_t BitDepth  = png_get_bit_depth ( PNGHandler, PNGInfo );
    
    // convert any color type and depth into 8-bit RGB
    if( BitDepth == 16 )
      png_set_strip_16( PNGHandler );
    
    if( ColorType == PNG_COLOR_TYPE_PALETTE )
      png_set_palette_to_rgb( PNGHandler );

    // grayscale images might have less than 8 bit color depth
    if( ColorType == PNG_COLOR_TYPE_GRAY && BitDepth < 8 )
      png_set_expand_gray_1_2_4_to_8( PNGHandler );
    
    if( png_get_valid( PNGHandler, PNGInfo, PNG_INFO_tRNS ))
      png_set_tRNS_to_alpha( PNGHandler );
    
    // if there is no alpha channel, create it as totally opaque
    if( ColorType == PNG_COLOR_TYPE_RGB
    ||  ColorType == PNG_COLOR_TYPE_GRAY
    ||  ColorType == PNG_COLOR_TYPE_PALETTE )
      png_set_filler( PNGHandler, 0xFF, PNG_FILLER_AFTER );
    
    // for grayscale images, convert grays to RGB color
    if( ColorType == PNG_COLOR_TYPE_GRAY
    ||  ColorType == PNG_COLOR_TYPE_GRAY_ALPHA )
      png_set_gray_to_rgb( PNGHandler );
    
    png_read_update_info( PNGHandler, PNGInfo );
    
    // save pointers for each row in the image
    RowPixels = (uint8_t**)malloc( sizeof(uint8_t*) * Height );
    
    for( int y = 0; y < Height; y++ )
      RowPixels[ y ] = (uint8_t*)malloc( png_get_rowbytes(PNGHandler, PNGInfo) );
    
    // now read the actual pixel data for each row
    png_read_image( PNGHandler, RowPixels );

    // clean-up
    fclose( PNGFile );
    png_destroy_read_struct( &PNGHandler, &PNGInfo, nullptr );
}

// -----------------------------------------------------------------------------

void PNGImage::SaveToFile( const string& PNGFilePath ) const
{
    // open output file
    FILE *PNGFile = fopen( PNGFilePath.c_str(), "wb" );
    
    if( !PNGFile )
      throw runtime_error( "Cannot open output file for writing" );

    png_structp PNGHandler = png_create_write_struct( PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr );
    
    if( !PNGHandler )
      throw runtime_error( "Cannot create a PNG handler" );

    png_infop PNGInfo = png_create_info_struct( PNGHandler );
    
    if( !PNGInfo )
      throw runtime_error( "Cannot create a PNG info structure" );

    // define a callback function expected by libpng for error handling
    if( setjmp( png_jmpbuf(PNGHandler) ) )
      throw runtime_error( "Cannot set callback function for libpng" );
      
    // begin writing
    png_init_io( PNGHandler, PNGFile );

    // define output as 8bit depth in RGBA format
    png_set_IHDR
    (
        PNGHandler,
        PNGInfo,
        Width, Height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    
    // write basic image info
    png_write_info( PNGHandler, PNGInfo );
    
    // write the actual pixel data for all rows
    if( !RowPixels )
      throw runtime_error( "The row pointers are not allocated" );
    
    png_write_image( PNGHandler, RowPixels );
    
    // end writing
    png_write_end( PNGHandler, nullptr );
    
    // clean-up
    fclose( PNGFile );
    png_destroy_write_struct( &PNGHandler, &PNGInfo );
}
