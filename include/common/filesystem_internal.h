/*
  This file associated with Hayato Labs project.
  
  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef NFCXX_FILESYSTEM_HPP
#define NFCXX_FILESYSTEM_HPP

#include "filesystem_public.h"

class CFilesystem : public IFileSystem {
protected:
   bool m_bInitilized = false;
   std::vector<Path_t> m_SearchPaths;

public:
   CFilesystem() = default;

   bool     Initilize();
   bool     IsInitilized() { return m_bInitilized; }
   void     Shutdown();

   /* =========================================================== */
   /* инструменты                                                 */

   char      GetSlash();
   Path_t    FixSlashes(const Path_t filepath, char slash = 0);
   Path_t    RemoveTrailingSlash(const Path_t rawpath, char slash = 0);
   bool      Exists(const Path_t &filepath);
   Path_t    GetExecutablePath();
   Path_t    GetThisModulePath();

   /* =========================================================== */
   /* работа с папками                                            */

   Path_t    GetWorkingDirectory();
   Path_t    DirectoryUserDocuments();
   bool      SetWorkingDirectory(const Path_t dirPath);
   Path_t    DirectoryTemporary();
   bool      DirectoryCreate(const Path_t &dirPath);
   bool      DirectoryRemove(const Path_t &dirPath, bool deleteSubdirectories);
   bool      PathIsDirectory(const Path_t dirPath);
   Path_t    PathStripFilename(const Path_t filepath, char slash);
   Path_t    PathStripDirectory(const Path_t filepath, char slash);

   /* =========================================================== */
   /* работа с папками (URL)                                      */

   bool      URLProtocolAssign(const Path_t realPath, const Path_t protoName);
   bool      URLProtocolCancel(const Path_t protoName);
   Path_t    URLProtocolParse(const Path_t urlPath);

   /* =========================================================== */
   /* работа с игровой файловой системой (Anabazz)                */

   void      GameSetModeDirectory(const Path_t dirPath);
   Path_t    GameGetModeDirectory();
   void      GameAddModeSearchPath(const Path_t dirPath);
   Path_t    GameSearchModeFile(const Path_t filePath);
   void      GameSetCoreDirectory(const Path_t dirPath);
   Path_t    GameGetCoreDirectory();
   std::vector<Path_t>  GameGetModeSearchPaths();
   Path_t    GameGetCacheDirectory();

   /* =========================================================== */
   /* работа с файлами                                            */

   Path_t    PathGetExtension(const Path_t filepath);
   Path_t    PathFix(const Path_t filepath);
   Path_t    PathStripExtension(const Path_t filepath);
   bool      PathIsAbsolute(const Path_t &filepath);
   bool      FileIsValid(const File_t file);
   File_t    FileOpen(const Path_t filePath, fileMode_t fileMode);
   void      FileClose(const File_t file);
   void      FilePrint(const File_t file, const char* pszFormat, ...);
   bool      FileReadLine(const File_t file, std::string &refsLine);
   void      CloseAllFiles();
};

#endif NFCXX_FILESYSTEM_HPP