#include "webfs.h"
#include "web.h"

namespace WEB
{
		
	//format bytes
	String formatBytes(size_t bytes)
	{
		if (bytes < 1024) {
			return String(bytes) + "B";
		} else if (bytes < (1024 * 1024)) {
			return String(bytes / 1024.0) + "KB";
		} else if (bytes < (1024 * 1024 * 1024)) {
			return String(bytes / 1024.0 / 1024.0) + "MB";
		} else {
			return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
		}
	}

	String getContentType(String filename)
	{
		if (server.hasArg("download")) {
			return "application/octet-stream";
		} else if (filename.endsWith(".htm")) {
			return "text/html";
		} else if (filename.endsWith(".html")) {
			return "text/html";
		} else if (filename.endsWith(".css")) {
			return "text/css";
		} else if (filename.endsWith(".js")) {
			return "application/javascript";
		} else if (filename.endsWith(".png")) {
			return "image/png";
		} else if (filename.endsWith(".gif")) {
			return "image/gif";
		} else if (filename.endsWith(".jpg")) {
			return "image/jpeg";
		} else if (filename.endsWith(".ico")) {
			return "image/x-icon";
		} else if (filename.endsWith(".svg")) {
			return "image/svg+xml";
		} else if (filename.endsWith(".xml")) {
			return "text/xml";
		} else if (filename.endsWith(".pdf")) {
			return "application/x-pdf";
		} else if (filename.endsWith(".zip")) {
			return "application/x-zip";
		} else if (filename.endsWith(".gz")) {
			return "application/x-gzip";
		} else if (filename.endsWith(".config")) {
			return "text/plain";
		}
		return "text/plain";
	}

	bool handleFileRead(String path)
	{
		//DBG_OUTPUT_PORT.println("handleFileRead: " + path);
		if (path.endsWith("/")) {
			path += "index.htm";
		}
		String contentType = getContentType(path);
		String pathWithGz = path + ".gz";
		if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
			if (SPIFFS.exists(pathWithGz)) {
				path += ".gz";
			}
			File file = SPIFFS.open(path, "r");
			server.streamFile(file, contentType);
			file.close();
			return true;
		}
		return false;
	}

	void handleFileUpload()
	{
		if (server.uri() != "/edit") {
			return;
		}
		HTTPUpload& upload = server.upload();
		if (upload.status == UPLOAD_FILE_START) {
			String filename = upload.filename;
			if (!filename.startsWith("/")) {
				filename = "/" + filename;
			}
			//DBG_OUTPUT_PORT.print("handleFileUpload Name: "); //DBG_OUTPUT_PORT.println(filename);
			fsUploadFile = SPIFFS.open(filename, "w");
			filename = String();
		} else if (upload.status == UPLOAD_FILE_WRITE) {
			////DBG_OUTPUT_PORT.print("handleFileUpload Data: "); //DBG_OUTPUT_PORT.println(upload.currentSize);
			if (fsUploadFile) {
				fsUploadFile.write(upload.buf, upload.currentSize);
			}
		} else if (upload.status == UPLOAD_FILE_END) {
			if (fsUploadFile) {
				fsUploadFile.close();
			}
			//DBG_OUTPUT_PORT.print("handleFileUpload Size: "); //DBG_OUTPUT_PORT.println(upload.totalSize);
		}
	}

	void handleFileDelete()
	{
		if (server.args() == 0) {
			return server.send(500, "text/plain", "BAD ARGS");
		}
		String path = server.arg(0);
		//DBG_OUTPUT_PORT.println("handleFileDelete: " + path);
		if (path == "/") {
			return server.send(500, "text/plain", "BAD PATH");
		}
		if (!SPIFFS.exists(path)) {
			return server.send(404, "text/plain", "FileNotFound");
		}
		SPIFFS.remove(path);
		server.send(200, "text/plain", "");
		path = String();
	}

	void handleFileCreate()
	{
		if (server.args() == 0) {
			return server.send(500, "text/plain", "BAD ARGS");
		}
		String path = server.arg(0);
		//DBG_OUTPUT_PORT.println("handleFileCreate: " + path);
		if (path == "/") {
			return server.send(500, "text/plain", "BAD PATH");
		}
		if (SPIFFS.exists(path)) {
			return server.send(500, "text/plain", "FILE EXISTS");
		}
		File file = SPIFFS.open(path, "w");
		if (file) {
			file.close();
		} else {
			return server.send(500, "text/plain", "CREATE FAILED");
		}
		server.send(200, "text/plain", "");
		path = String();
	}

	void handleFolderCreate()
	{
		if (server.args() == 0) {
			return server.send(500, "text/plain", "BAD ARGS");
		}
		String path = server.arg(0);
		//DBG_OUTPUT_PORT.println("handleFolderCreate: " + path);
		if (path == "/") {
			return server.send(500, "text/plain", "BAD PATH");
		}
		if (SPIFFS.exists(path)) {
			return server.send(500, "text/plain", "FOLDER EXISTS");
		}

		bool dir = SPIFFS.mkdir(path);
	
		//File file = SPIFFS.open(path, "w");
		if (!dir) {
			return server.send(500, "text/plain", "CREATE FAILED");
		}

		server.send(200, "text/plain", "");
		path = String();
	}

	void handleFileListOld()
	{
		if (!server.hasArg("dir")) {
			server.send(500, "text/plain", "BAD ARGS");
			return;
		}

		String path = server.arg("dir");
		//DBG_OUTPUT_PORT.println("handleFileList: " + path);
		Dir dir = SPIFFS.openDir(path);
		path = String();

		String output = "[\n";
		while (dir.next()) {
			File entry = dir.openFile("r");
			if (output != "[\n") {
				output += ',';
				output += '\n';
			}
			bool isDir = dir.isDirectory();
			output += "{\"type\":\"";
			output += (isDir) ? "dir" : "file";
			output += "\",\"name\":\"";
			output += String(entry.name()).substring(1);
			output += "\"}";
			entry.close();
		}

		output += "\n]";
		server.send(200, "text/json", output);
	}

	void strRepeat(String *in, String str, int count)
	{
		for (int i = 0; i < count; i++)
		{
			in->concat(str);
		}
	}

	void handleDirEntry(String * output, String curpath, String path, int dirLevel)
	{
		if (!output->substring(output->length()-2).equals("[\n")) {
			output->concat(",\n");
		} 
		

		int slash = max(path.substring(curpath.length()).indexOf('/'), 0);
/*
		strRepeat(output, "	", dirLevel);
		output->concat("-- curpath:");
		output->concat(curpath);
		output->concat(" path:");
		output->concat(path);
		output->concat(" slash:");
		output->concat(slash);
		output->concat("\n");
*/
		if (slash > 0 || (path == "/" && path.length() == 1))
		{
			slash += curpath.length();
			strRepeat(output, "	", dirLevel);
			output->concat("{\n");

			strRepeat(output, "	", dirLevel + 1);
			output->concat("\"name\": \"");
			output->concat(path.substring(1, slash));
			output->concat("/\",\n");

			strRepeat(output, "	", dirLevel + 1);
			output->concat("\"files\": [\n");

			Dir dir = SPIFFS.openDir(path.substring(0, slash));

			while (dir.next()) {
				File entry = dir.openFile("r");

				int lastSlash = max(path.lastIndexOf('/') + 1, 1);
				handleDirEntry(output, path.substring(0, lastSlash), String(entry.name()), dirLevel + 2);

				entry.close();
			}

			//handleDirEntry(output, path.substring(slash - 1), dirLevel + 2);
			output->concat("\n");
			strRepeat(output, "	", dirLevel + 1);
			output->concat("]\n");

			strRepeat(output, "	", dirLevel);
			output->concat("}");
		}else
		{
			strRepeat(output, "	", dirLevel);
			output->concat("\"");
			output->concat(path.substring(curpath.length()));
			output->concat("\"");
		}
		
	}

	void handleFileList()
	{
		if (!server.hasArg("dir")) {
			server.send(500, "text/plain", "BAD ARGS");
			return;
		}

		String path = server.arg("dir");
		//DBG_OUTPUT_PORT.println("handleFileList: " + path);
		Dir dir = SPIFFS.openDir(path);
		//path = String();

		String output = "[\n";
		
		handleDirEntry(&output, "/", path, 1);
/*
		while (dir.next()) {
			File entry = dir.openFile("r");
			
			entry.close();
		}
*/
		output += "\n]";
		server.send(200, "text/json", output);
	}
} // namespace WEB

/* 
	File entry = dir.openFile("r");
	if (output != "[\n") {
		output += ',';
		output += '\n';
	}
	bool isDir = dir.isDirectory();
	output += "{\"type\":\"";
	output += (isDir) ? "dir" : "file";
	output += "\",\"name\":\"";
	output += String(entry.name()).substring(1);
	output += "\"}";
	entry.close();
*/