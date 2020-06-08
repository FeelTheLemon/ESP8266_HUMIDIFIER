#ifndef __WEBFS_H__
#define __WEBFS_H__
#include <Arduino.h>

namespace WEB
{
	bool handleFileRead(String path);
	void handleFileUpload();

	void handleFolderDelete();
	void handleFileDelete();

	void handleFolderCreate();
	void handleFileCreate();

	void handleFileList();
	void handleFileListOld();
} // namespace WEB

#endif //__WiFiC_H__
