# libderpyzza
a small personal "standard" static library with convenience functions and data types.
the library is designed to simply be bundled in as a single static library.

included modules:
  - dstr: length-encoded strings
  - dbuf: dynamic array/buffers
  - dlog: logging and assertion utilities
  - dmem: custom allocator support
  - darena: arena allocators
  - dfile: file loading and reading functions

planned features:
  - command line arg parsing
  - simple json/yaml/ini parsers
  - uTF-8 strings + associated support functions
  - threads?
  
# license
This software is dual-licensed under the MIT License and The Unlicense.
You may choose whichever license you prefer.

See LICENSE.MIT and LICENSE.UNLICENSE for details.
