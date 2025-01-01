============================================================
               Black & White frame encoder
============================================================

This is a PC program I made to encode the video frames for
the Bad Apple tech demo. But it could also be used to
encode any other video scene as long as it is monochrome.

It will process a whole folder of same-sized PNG images and
encode them into a single output file. It is limited to
monochrome images, which is perfect for the Bad Apple video
animation which is all silhouettes.


USED ENCODING ALGORITHM
------------------------------------------------------------
The encoding will process all input pixels in this way:

- Load and process each individual image frame
  - Process pixels in each Y coordinate independently
    - Detect color changes to decompose lines in segments
    
Lines will be considered as a series of black segments
drawn over a fully white background.

The encoded video is simply a sequence of 32-bit words, in
which each word has the following 4 possible meanings:

Case 1:
  Word 0xFFFFEEEE signals the end of the video.

Case 2:
  Word 0xFFFFFFFF signals the end of current frame.

Case 3:
  When the word's higher 16 bits are 0xFFFF, this signals
  the start of a new horizontal line. The Y coordinate for
  the new line is given by the word's lower 16 bits.

Case 4:
  Any other value represents a black horizontal segment to
  be drawn at the current line: 
  - The 16 higher bits contain the segment starting X
  - The 16 lower bits contain the segment ending X

This PC program only implements the encoding. Frames will
later be decoded and drawn by Vircon32 programs.


LICENSE
------------------------------------------------------------

This program is free and open source. It is offered
under the 3-Clause BSD License, which full text is the
following:

Copyright 2024 Carra.
All rights reserved.

Redistribution and use in source and binary forms, with or
without modification, are permitted provided that the
following conditions are met:

1. Redistributions of source code must retain the above
copyright notice, this list of conditions and the following
disclaimer.

2. Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following
disclaimer in the documentation and/or other materials
provided with the distribution.

3. Neither the name of the copyright holder nor the names of
its contributors may be used to endorse or promote products
derived from this software without specific prior written
permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
