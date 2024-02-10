/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef NFCXX_MEMZONE_HPP
#define NFCXX_MEMZONE_HPP

typedef int memorytag_t;

#define TAG_NONE 		0  
#define TAG_GENERAL 	1 // malloc, free
#define TAG_NEW  		2 // new, delete
#define TAG_THREAD  	3

/*
===========================================================================

   memoryblock_t

===========================================================================
*/

struct memoryblock_t {

	size_t 			m_nSize; // размер блока
	int 			m_Idx; // номер блока
	int 			m_ZoneIdx; // номер зоны
	bool 			m_bFreed; // свободен ли блок?
	memorytag_t 	m_iTag; // тэг блока
	struct memoryblock_t *m_pNextblock, *m_pPrevblock;

	// для дампа
	const char* 	m_szFnname;
	const char* 	m_szFilename;
	int 			m_iLine;
	size_t 			m_nAllocatiosize;
};

struct memoryzone_t {

	size_t 	m_nSize; // размер зоны
	size_t 	m_nUsed; // использовано памяти
	size_t 	m_nBlockOriginalSize; // Оригинальный размер зоны
	int 	m_Idx; // Номер зоны

	memoryblock_t 	blockList;
	memoryblock_t*	roverblock;

	void 	Clear();
	void 	LogHeap();
	void*	Malloc(size_t nSize, memorytag_t iTag, const char* fnname, const char* pszFilename, int iLine);
	void*	Malloc(size_t nSize, memorytag_t iTag);
	void 	Free(void *ptr);
};

#endif