set(INC 
	.
	../../../../extern/FileTools
	../../Ketsji
	../../Rasterizer
	../../Expressions
	../../Converter
	../../SceneGraph
	../../../../intern/container
	../../../../intern/moto/include
	../../../../intern/string

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