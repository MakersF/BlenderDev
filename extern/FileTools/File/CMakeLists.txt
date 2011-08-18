#------------------------------------------------------------------------------
#   This file is part of FBT (File Binary Tables).
#   http://gamekit.googlecode.com/
#
#   Copyright (c) 2010 Charlie C & Erwin Coumans.
#
#------------------------------------------------------------------------------
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#------------------------------------------------------------------------------
#include(FlexBison)

set(File_SRC
    fbtFile.cpp
    fbtTables.cpp    
    fbtTypes.cpp
    fbtStreams.cpp
	fbtBuilder.cpp
)

set(File_HDR
    fbtFile.h
    fbtTables.h
    fbtBuilder.h
    fbtTypes.h
    fbtConfig.h
    fbtStreams.h
    fbtPlatformHeaders.h
)

set(fbtScanner Generated/fbtScanner.inl)

#if (FBT_USE_ZLIB)
#    include_directories(${FBT_ZLIB_INCLUDE})
#    link_libraries(${FBT_ZLIB_LIBRARY})
#endif()

#this is the original line, i added a few other things i see used in other cmakelists
add_library(extern_fbtfile ${File_SRC} ${File_HDR} ${fbtScanner})
set_property(GLOBAL APPEND PROPERTY BLENDER_LINK_LIBS extern_fbtfile)
set(SOURCE_FILES ${File_SRC} ${File_HDR})
blender_source_group("${SOURCE_FILES}")

#i think this should set everything fine and should be the preferred solution..
#set(SOURCE_FILES ${File_SRC})
#list(APPEND SOURCE_FILES ${File_HDR})
#list(APPEND SOURCE_FILES ${fbtScanner})
#set(INC)
#set(INC_SYS)
#blender_add_lib(extern_fbtfile ${SOURCE_FILES} INC INC_SYS)
