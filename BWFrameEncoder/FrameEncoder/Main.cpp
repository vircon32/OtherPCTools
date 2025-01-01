// *****************************************************************************
    // include project headers
    #include "PNGImage.hpp"
    #include "Encode.hpp"
    #include "Globals.hpp"
    
    // include libpng headers
    #include <png.h>
    
    // include C/C++ headers
    #include <filesystem>   // [ C++ STL ] File system
    #include <iostream>     // [ C++ STL ] I/O Streams
    #include <fstream>      // [ C++ STL ] File streams
    #include <stdexcept>    // [ C++ STL ] Exceptions
    #include <algorithm>    // [ C++ STL ] Algorithms
    #include <cstring>      // [ ANSI C ] Strings
    
    // declare used namespaces
    using namespace std;
// *****************************************************************************


// =============================================================================
//      AUXILIARY FUNCTIONS
// =============================================================================


void PrintUsage()
{
    cout << "USAGE: encodebwframe [options] inputfolder outputfile" << endl;
    cout << "InputFolder: Path to a folder containing all PNG frames to encode" << endl;
    cout << "OutputFile: Path for the resulting encoded data file" << endl;
    cout << "Options:" << endl;
    cout << "  --help       Displays this information" << endl;
    cout << "  --version    Displays program version" << endl;
    cout << endl;
    cout << "This tool expects frame files to be named in order" << endl;
    cout << "(for instance: frame001.png, frame002.png, etc.)" << endl;
}

// -----------------------------------------------------------------------------

void PrintVersion()
{
    cout << "encodebwframe v24.12.30" << endl;
    cout << "Tool to encode Bad Apple video frames into a Vircon32 data file" << endl;
}


// =============================================================================
//      MAIN FUNCTION
// =============================================================================


int main( int NumberOfArguments, char* Arguments[] )
{
    try
    {
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Process command line arguments
        
        // variables to capture input parameters
        string InputFolder, OutputFile;
        
        // process arguments
        for( int i = 1; i < NumberOfArguments; i++ )
        {
            if( Arguments[i] == string("--help") )
            {
                PrintUsage();
                return 0;
            }
            
            if( Arguments[i] == string("--version") )
            {
                PrintVersion();
                return 0;
            }
            // discard any other parameters starting with '-'
            if( Arguments[i][0] == '-' )
              throw runtime_error( string("unrecognized command line option '") + Arguments[i] + "'" );
            
            // first non-option parameter is taken as the input folder
            if( InputFolder.empty() )
            {
                InputFolder = Arguments[i];
            }
           
            // second non-option parameter is taken as the output file
            else if( OutputFile.empty() )
            {
                OutputFile = Arguments[i];
            }
            
            // only a single input file is supported!
            else
              throw runtime_error( "too many input files, only 1 is supported" );
        }
        
        // check if an input folder was given
        if( InputFolder.empty() )
          throw runtime_error( "no input folder" );
        
        // check that it exists and is a folder
        filesystem::path InputPath( InputFolder );
        
        if( !filesystem::exists( InputPath ) )
          throw runtime_error( "input folder does not exist" );
        
        if( !filesystem::is_directory( InputPath ) )
          throw runtime_error( "input path is a file, expected a folder" );
        
        // check if an output file was given and is not a folder
        if( OutputFile.empty() )
          throw runtime_error( "no output file" );
        
        if( filesystem::is_directory( OutputFile ) )
          throw runtime_error( "input path must be a file, found a folder" );
      
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Scan folder encoding every PNG image in order
        
        for( auto const& DirEntry : std::filesystem::directory_iterator{ InputPath } )
        {
            // discard symlinks, non-files, etc.
            if( DirEntry.is_directory() || DirEntry.is_symlink() || !DirEntry.is_regular_file() )
              continue;
            
            // load the PNG image
            string Extension = DirEntry.path().extension().string();
            
            if( Extension == ".png" || Extension == ".PNG" )
            {
                PNGImage FrameImage;
                FrameImage.LoadFromFile( DirEntry.path().string() );
                
                // check dimensions
                if( FrameImage.Width != FRAME_WIDTH || FrameImage.Height != FRAME_HEIGHT )
                  throw runtime_error( "Frame " + to_string( EncodedFrames.size() ) + " has not the expected size" );
                
                // encode the current frame and add it to the data sequence
                EncodeFrame( FrameImage );
            }
        }
        
        // after all frames add the end video code
        EncodedFrames.push_back( END_VIDEO_CODE );
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Save all encoded frames to the output file
        
        // open output file as binary
        ofstream Output;
        Output.open( OutputFile, ios::binary );
        
        if( !Output.good() )
          throw runtime_error( "cannot open the output file" );
        
        // save the whole encoded sequence
        size_t SizeInBytes = EncodedFrames.size() * sizeof(int32_t);
        Output.write( (char*)EncodedFrames.data(), SizeInBytes );
      
        // close the file
        Output.close();
    }
    
    catch( const exception& e )
    {
        cerr << "encodebwframe: error: " << e.what() << endl;
        return 1;
    }
    
    // report success
    cout << "all frames encoded successfully" << endl;
    
    // report stats
    cout << "* frames encoded: " << to_string( TotalFrames ) << endl;
    cout << "* average frame words: " << to_string( (int)EncodedFrames.size() / TotalFrames ) << endl;
    cout << "* maximum frame words: " << to_string( HighestFrameWords ) << endl;
    
    return 0;
}
