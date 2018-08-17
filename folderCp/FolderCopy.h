#ifndef _FOLDER_COPY_H_
#define _FOLDER_COPY_H_

#include <string>

class FolderCopy
{
public:
	FolderCopy(const std::string& source, const std::string& dest);

	void startCopy();
        void createDestDir();
	bool isDir(const std::string& path);
	void copyFile(const std::string& source, const std::string& dest);
	void copyDir(const std::string& source, const std::string& dest);

private:
	std::string m_source;
	std::string m_dest;
};

#endif
