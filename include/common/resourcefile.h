/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef NFCXX_RESOURCEFILE_HPP
#define NFCXX_RESOURCEFILE_HPP

#include "public.h"

enum VariableType_t {
	VARTYPE_INT32,
	VARTYPE_INT64,
	VARTYPE_UINT16,

	VARTYPE_BOOLEAN,
	VARTYPE_STRING,

	VARTYPE_INVALID
};

struct ScriptDeclaredVariable_t {
	char 			m_strValue[64];
	bool 			m_bValue;
	int32 			m_iValue;
	VariableType_t 	m_iValueType = VARTYPE_INVALID;

	template <typename T>
	T ConvertValue();

	ScriptDeclaredVariable_t(const char* sVariable, VariableType_t iValueType);
	ScriptDeclaredVariable_t() = default;
};

struct ScriptVariablePair_t {
	char 						m_strValue[64];
	ScriptDeclaredVariable_t 	m_sdvVar;
	ScriptVariablePair_t(const char*, const char*, VariableType_t);
};

struct ScriptBlock_t {
	std::string 						m_pszNametag; 	// Название блока
	std::vector<ScriptVariablePair_t> 	m_Pairs; 		// Переменные
	ScriptDeclaredVariable_t*			Search(std::string pszNametag, VariableType_t iValueType = VARTYPE_STRING, std::string strDefaultValue = "");
	ScriptBlock_t() = default;
};

/*
===========================================================================

   CResourceFile

===========================================================================
*/
class CResourceFile {
public:
	bool 								m_bValidated;
	std::vector<ScriptVariablePair_t> 	m_GlobalPairs; 	// Глобальные переменные
	std::vector<ScriptBlock_t> 			m_Blocks; 		// Блоки данных

	bool 						CreateFromFile(std::string filepath);
	void 						Destroy();
	ScriptBlock_t*				SearchBlock(std::string strBlockNametag);
	ScriptDeclaredVariable_t* 	Search(std::string strBlockNametag, std::string strVariableNametag, VariableType_t iValueType = VARTYPE_STRING, std::string strDefaultValue = "");
	ScriptDeclaredVariable_t* 	Search(std::string strVariableNametag, VariableType_t iValueType = VARTYPE_STRING, std::string strDefaultValue = "");

	CResourceFile() = default;
private:
	ScriptVariablePair_t 		ParseVariable(std::string strToParse);
};

#endif