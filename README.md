# libderpyzza
a small personal "standard" library with convenience functions and data types.

included modules:
  (note that this library is not a header-only library)
  - derp.h: general datatypes and functions required by all the other modules
  - dbuf.h: general string and buffer manipulation functions. contains functionality for dynamically sized buffers ( std::vector ), dynamically sized strings ( std::string ), and some handy file I/O functions
  - dcolour.h: colour format functions ( hex to normalized rgb, that sorta stuff )
  - ddebug.h: handy debug and logging functionality

planned features:
  - command line arg parsing
  - simple json/yaml/ini parsers
  - terminal ansi code helper functions
  - simple image loaders
  - uTF-8 strings + associated support functions
  - threads
  
# license
This software is dual-licensed under the MIT License and The Unlicense.
You may choose whichever license you prefer.

See LICENSE.MIT and LICENSE.UNLICENSE for details.
