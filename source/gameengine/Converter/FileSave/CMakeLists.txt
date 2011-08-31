set(INC 
	.
	../../../../extern/FileTools
	../../Ketsji
	../../Rasterizer
	../../Expressions
	../../../../intern/string
	../../Converter
	../../SceneGraph
)

set(INC_SYS

)

set(SRC 
	KX_FileInterface.cpp 
	KX_ConvertClassStruct.cpp
	
	KX_FileInterface.h 
	KX_BGEdna.h
	KX_ConvertClassStruct.h
)

add_definitions(-DWITH_FBT)

blender_add_lib(ge_convertedsaveloader "${SRC}" "${INC}" "${INC_SYS}")