/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef HAYATOLABS_FILESYSTEM_PUBLIC_HPP
#define HAYATOLABS_FILESYSTEM_PUBLIC_HPP

#include "public.h"

enum fileMode_t {
   FILEMODE_READONLY,
   FILEMODE_WRITE_APPEND,
   FILEMODE_WRITE_TRUNCATE,
};

#ifndef MAX_UNICODE_PATH
#define MAX_UNICODE_PATH 32767
#endif

#ifndef MAX_UNICODE_PATH_IN_UTF8
#define MAX_UNICODE_PATH_IN_UTF8 (MAX_UNICODE_PATH * 4)
#endif

/*
===========================================================================

   IFileSystem - кроссплатформенный интерфейс для работы с
   файловой системой.
   Интегрирована поддержка .pak файлов, поддержка игровой файловой системы

===========================================================================
*/
class IFileSystem {
public:

   virtual bool      Initilize() = 0;
   virtual bool      IsInitilized() = 0;

   virtual void      Shutdown() = 0;

   /* =========================================================== */
   /* инструменты                                                 */

   virtual char      GetSlash() = 0;
   virtual Path_t    FixSlashes(const Path_t filepath, char slash = 0) = 0;
   virtual Path_t    RemoveTrailingSlash(const Path_t rawpath, char slash = 0) = 0;
   virtual bool      Exists(const Path_t &filepath) = 0;
   virtual Path_t    GetExecutablePath() = 0;
   virtual Path_t    GetThisModulePath() = 0;

   /* =========================================================== */
   /* работа с папками                                            */

   virtual Path_t    GetWorkingDirectory() = 0;
   virtual bool      SetWorkingDirectory(const Path_t dirPath) = 0;

   virtual Path_t    DirectoryUserDocuments() = 0;
   virtual Path_t    DirectoryTemporary() = 0;
   virtual bool      DirectoryCreate(const Path_t &dirPath) = 0;
   virtual bool      DirectoryRemove(const Path_t &dirPath, bool deleteSubdirectories) = 0;
   virtual bool      PathIsDirectory(const Path_t dirPath) = 0;
   virtual Path_t    PathStripFilename(const Path_t filepath, char slash = 0) = 0;
   virtual Path_t    PathStripDirectory(const Path_t filepath, char slash = 0) = 0;

   /* =========================================================== */
   /* работа с папками (URL)                                      */

   virtual bool      URLProtocolAssign(const Path_t realPath, const Path_t protoName) = 0;
   virtual bool      URLProtocolCancel(const Path_t protoName) = 0;
   virtual Path_t    URLProtocolParse(const Path_t urlPath) = 0;

   /* =========================================================== */
   /* работа с игровой файловой системой (Anabazz)                */

   virtual void      GameSetModeDirectory(const Path_t dirPath) = 0;
   virtual Path_t    GameGetModeDirectory() = 0;
   virtual void      GameAddModeSearchPath(const Path_t dirPath) = 0;
   virtual Path_t    GameSearchModeFile(const Path_t filePath) = 0;
   virtual void      GameSetCoreDirectory(const Path_t dirPath) = 0;
   virtual Path_t    GameGetCoreDirectory() = 0;
   virtual std::vector<Path_t> GameGetModeSearchPaths() = 0;
   virtual Path_t    GameGetCacheDirectory() = 0;

   /* =========================================================== */
   /* работа с файлами                                            */

   virtual Path_t    PathGetExtension(const Path_t filepath) = 0;
   virtual Path_t    PathFix(const Path_t filepath) = 0;
   virtual Path_t    PathStripExtension(const Path_t filepath) = 0;
   virtual bool      PathIsAbsolute(const Path_t &filepath) = 0;
   virtual bool      FileIsValid(const File_t file) = 0;
   virtual File_t    FileOpen(const Path_t filePath, fileMode_t fileMode) = 0;
   virtual void      FileClose(const File_t file) = 0;
   virtual void      FilePrint(const File_t file, const char* pszFormat, ...) = 0;
   virtual bool      FileReadLine(const File_t file, std::string &refsLine) = 0;
   virtual void      CloseAllFiles() = 0;
};

extern IFileSystem* filesystem();

#endif /* HAYATOLABS_FILESYSTEM_PUBLIC_HPP */
