set(INC 
	.
	../../../../extern/FileTools
)

set(INC_SYS

)

set(SRC 
	KX_FileInterface.cpp 
	KX_FBT_CustomTypes.cpp
	
	KX_FileInterface.h 
	KX_BGEdna.h
	KX_FBT_CustomTypes.h
)

blender_add_lib(ge_convertedsaveloader "${SRC}" "${INC}" "${INC_SYS}")