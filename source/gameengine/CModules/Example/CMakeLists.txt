setup_libdirs()

set(INC	
	.
	../../BlenderRoutines
	../../Converter
	../../Expressions
	../../GameLogic
	../../Ketsji
	../../Ketsji/KXNetwork
	../../Network
	../../Network/LoopBackNetwork
	../../Physics/common
	../../Rasterizer
	../../Rasterizer/RAS_OpenGLRasterizer
	../../SceneGraph
	../../../blender
	../../../blender/blenfont
	../../../blender/blenkernel
	../../../blender/blenlib
	../../../blender/blenloader
	../../../blender/blenpluginapi
	../../../blender/gpu
	../../../blender/imbuf
	../../../blender/makesdna
	../../../blender/python
	../../../blender/python/generic
	../../../blender/python/mathutils
	../../../../intern/container
	../../../../intern/guardedalloc
	../../../../intern/moto/include
	../../../../intern/string
)

set(INC_SYS
	
	)

set(SRC
	"SHARED"
	BGE_TestModule.cpp 
	
	BGE_TestModule.h 
	
)

if(WITH_IMAGE_OPENJPEG)
	list(APPEND INC_SYS
		${OPENJPEG_INCLUDE_DIRS}
	)
	add_definitions(-DWITH_OPENJPEG)
endif()

blender_add_lib(BGE_TestModule "${SRC}" "${INC}" "${INC_SYS}")
setup_libdirs()

if(WITH_CODEC_QUICKTIME)
	add_definitions(-DWITH_QUICKTIME)
endif()

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dna.c
	COMMAND ${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}/makesdna ${CMAKE_CURRENT_BINARY_DIR}/dna.c ${CMAKE_SOURCE_DIR}/source/blender/makesdna/
	DEPENDS ${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}/makesdna
)

if(WIN32 AND NOT UNIX)
	string(SUBSTRING ${BLENDER_VERSION} 0 1 bver1)
	string(SUBSTRING ${BLENDER_VERSION} 2 1 bver2)
	string(SUBSTRING ${BLENDER_VERSION} 3 1 bver3)
	if(MINGW)
		add_definitions(
			-DWINDRES
			-DBLEN_VER_RC_STR_M=${BLENDER_VERSION}
			-DBLEN_VER_RC_1=${bver1}
			-DBLEN_VER_RC_2=${bver2}
			-DBLEN_VER_RC_3=${bver3}
			-DBLEN_VER_RC_4=0
		)
	else()
		add_definitions(
			-DBLEN_VER_RC_STR=${BLENDER_VERSION}
			-DBLEN_VER_RC_1=${bver1}
			-DBLEN_VER_RC_2=${bver2}
			-DBLEN_VER_RC_3=${bver3}
			-DBLEN_VER_RC_4=0
		)
	endif()
else()
	
endif()

get_property(BLENDER_LINK_LIBS GLOBAL PROPERTY BLENDER_LINK_LIBS)

list(APPEND BLENDER_LINK_LIBS
	ge_player_common
	ge_player_ghost
	blenkernel_blc
)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	list(APPEND BLENDER_LINK_LIBS extern_binreloc)
endif()

# if(UNIX)
	# Sort libraries
	set(BLENDER_SORTED_LIBS
		ge_player_ghost 
		ge_player_common 
		bf_intern_string 
		bf_intern_ghost
		bf_rna
		bf_blenkernel
		bf_blenloader 
		bf_blenpluginapi 
		ge_blen_routines
		bf_editor_datafiles
		ge_converter 
		ge_logic_ketsji 
		ge_phys_bullet 
		ge_phys_dummy
		ge_phys_common 
		ge_logic 
		ge_rasterizer 
		ge_oglrasterizer 
		ge_logic_expressions 
		ge_scenegraph
		bf_ikplugin
		bf_intern_itasc
		bf_intern_ik
		bf_intern_smoke
		bf_modifiers
		bf_intern_moto 
		bf_nodes
		bf_gpu
		bf_imbuf
		bf_avi 
		ge_logic_network 
		ge_logic_ngnetwork 
		ge_logic_loopbacknetwork 
		extern_bullet 
		bf_intern_guardedalloc 
		bf_intern_memutil 
		bf_python_ext
		bf_python_mathutils
		bf_blenlib 
		bf_imbuf_cineon
		bf_imbuf_openexr 
		extern_openjpeg
		extern_redcode
		bf_imbuf_dds
		bf_dna
		ge_videotex
		bf_blenfont
		bf_intern_audaspace
		blenkernel_blc
		extern_binreloc
		extern_minilzo
		bf_intern_ghost # duplicate for linking
		bf_blenkernel # duplicate for linking
		bf_intern_mikktspace
	)

	if(WITH_MOD_CLOTH_ELTOPO)
		list(APPEND BLENDER_SORTED_LIBS extern_eltopo)
	endif()

	if(WITH_BUILTIN_GLEW)
		list(APPEND BLENDER_SORTED_LIBS extern_glew)
	endif()

	if(WITH_LZMA)
		list(APPEND BLENDER_SORTED_LIBS extern_lzma)
	endif()

	if(WITH_MOD_DECIMATE)
		list(APPEND BLENDER_SORTED_LIBS bf_intern_decimate)
	endif()

	if(WITH_MOD_BOOLEAN)
		list(APPEND BLENDER_SORTED_LIBS bf_intern_bsp)
		list(APPEND BLENDER_SORTED_LIBS bf_intern_bop)
		list(APPEND BLENDER_SORTED_LIBS bf_intern_moto)
	endif()

	if(WITH_CODEC_QUICKTIME)
		list(APPEND BLENDER_SORTED_LIBS bf_quicktime)
	endif()

	if(WITH_CXX_GUARDEDALLOC)
		list(APPEND BLENDER_SORTED_LIBS bf_intern_guardedalloc_cpp)
	endif()
	

	foreach(SORTLIB ${BLENDER_SORTED_LIBS})
		set(REMLIB ${SORTLIB})
		foreach(SEARCHLIB ${BLENDER_LINK_LIBS})
			if(${SEARCHLIB} STREQUAL ${SORTLIB})
				set(REMLIB "")
			endif()
		endforeach()
		if(REMLIB)
			#message(STATUS "Removing library ${REMLIB} from blenderplayer linking because: not configured")
			list(APPEND REM_MSG ${REMLIB})
			list(REMOVE_ITEM BLENDER_SORTED_LIBS ${REMLIB})
		endif()
	endforeach()
	if(REM_MSG)
		list(SORT REM_MSG)
		message(STATUS "Modules Skipping: (${REM_MSG})")
	endif()
	target_link_libraries(BGE_TestModule ${BLENDER_SORTED_LIBS})
	add_dependencies(BGE_TestModule blender)

	unset(SEARCHLIB)
	unset(SORTLIB)
	unset(REMLIB)
	unset(REM_MSG)

# else()
# 	target_link_libraries(blenderplayer ${BLENDER_LINK_LIBS})
# endif()

setup_liblinks(BGE_TestModule)
