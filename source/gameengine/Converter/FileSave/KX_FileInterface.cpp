#ifdef WITH_FBT

#include "KX_FileInterface.h"
#include "fbtTables.h"
#include "fbtStreams.h"
#include <iostream>

const FBTuint32 GLOB = FBT_ID('G', 'L', 'O', 'B');
static char BgeFile_Header[8] = {'B', 'G', 'E', 'C', 'O', 'N', 'V', '\0'};

struct fbtIdDB
{
        const FBTuint16		m_code;
		Bgedna::fbtList		KX_FileInterface::*m_ptr;
};

fbtIdDB fbtData[] =
{
		//all attributes of class here!
		{ SCENE, &KX_FileInterface::m_scene},
		{ FRAME_SETTINGS, &KX_FileInterface::m_frameSettings},
		{ RAS_RECT, &KX_FileInterface::m_rasRect},
		{ BUCKET_MANAGER, &KX_FileInterface::m_bucketManager},
		{ MATERIAL_BUCKET, &KX_FileInterface::m_materialBucket},
		{ WORLD_INFO, &KX_FileInterface::m_worldInfo},
		{ IPOLY_MATERIAL, &KX_FileInterface::m_iPolyMaterial},
        { 0, 0 }
};




extern unsigned char bfBgeFBT[];
extern int bfBgeLen;


KX_FileInterface::KX_FileInterface()
	:   fbtFile(BgeFile_Header), m_stripList(0)
{
        m_aluhid = "BGECONVs"; //a stripped blend file
		
		m_memory = new fbtBinTables();
		if (initializeTables(m_memory)!= FS_OK) std::cout << "Error initializing tables.\n";

		m_fg = new Bgedna::FileGlobal();
		/*declare the KX_ConvertClassStruct here. KX_ConvertClassStruct will know that this
		class declared it, and will save all the converted data into the attributes of this class.
		Note1: right now i have to undestand how the containing lists work. Ask to xat if you find him on IRC.*/
		m_convertClass = new KX_ConvertClassStruct(this);
}



KX_FileInterface::~KX_FileInterface()
{
	if(m_fg)
	{
		delete m_fg;
		m_fg = 0;
	}

	if(m_convertClass)
		delete(m_convertClass);
}



int KX_FileInterface::initializeTables(fbtBinTables* tables)
{
        return tables->read(bfBgeFBT, bfBgeLen, false) ? FS_OK : FS_FAILED;
}



int KX_FileInterface::notifyData(void* p, const Chunk& id)
{
        if (id.m_code == GLOB)
        {
                m_fg = (Bgedna::FileGlobal*)p;
                return FS_OK;
        }

        if ((id.m_code <= 0xFFFF))
        {
                int i = 0;
                while (fbtData[i].m_code != 0)
                {
                        if (fbtData[i].m_code == id.m_code)
                        {
                                (this->*fbtData[i].m_ptr).push_back(p);
                                break;
                        }
                        ++i;
                }
        }
        return FS_OK;
}


int KX_FileInterface::writeData(fbtStream* stream)
{
        fbtBinTables::OffsM::Pointer md = m_memory->m_offs.ptr();


        for (MemoryChunk* node = (MemoryChunk*)m_chunks.first; node; node = node->m_next)
        {
                if (node->m_newTypeId > m_memory->m_strcNr)
                        continue;
                if (!node->m_newBlock)
                        continue;

                void* wd = node->m_newBlock;

                Chunk ch;
                ch.m_code   = node->m_chunk.m_code;
                ch.m_nr     = node->m_chunk.m_nr;
                ch.m_len    = node->m_chunk.m_len;
                ch.m_typeid = node->m_newTypeId;
                ch.m_old    = (FBTsize)wd;

                stream->write(&ch, sizeof(Chunk));
                stream->write(wd, ch.m_len);
        }

        return FS_OK;
}



bool KX_FileInterface::skip(const FBTuint32& id)
{

        if (!m_stripList)
                return false;

        int i = 0;
        while (m_stripList[i] != 0)
        {
                if (m_stripList[i++] == id)
                        return true;
        }

        return false;
}


void*   KX_FileInterface::getFBT(void)
{
        return (void*)bfBgeFBT;
}

FBTsize KX_FileInterface::getFBTlength(void)
{
        return bfBgeLen;
}

int KX_FileInterface::save(const char *path, const int mode)
{
        m_version = m_fileVersion;
        return reflect(path, mode);
}
#endif