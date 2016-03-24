#include "content-scaner.hpp"
#include "include/logger.hpp"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;
using namespace altium;

ContentScaner::FileInfo::FileInfo(std::string path)
	: path(path)
	, size(0)
	, begin_hash(0)
	, full_hash(0)
{

}

ContentScaner::FilterSizeKey::FilterSizeKey(ptrFileInfo file_info)
	: size(file_info->size)
	, begin_hash(file_info->begin_hash)
{

}

ContentScaner::ContentScaner(std::string path)
{
	path_ = path;
}

void ContentScaner::Scan()
{
	EnumerateFiles();
	FilterBySize();
}

void ContentScaner::EnumerateFiles()
{
	DIR *dirp = opendir(path_.data());
	if (dirp == nullptr)
	{
		logger().err("scaner", "path not found", curr_errno_msg());
		return;
	}

	struct dirent *de = readdir(dirp);
	while (de != nullptr) 
	{
		if (de->d_ino != 0)
			files_.push_back(make_shared<FileInfo>(de->d_name));
		de = readdir(dirp);
	}
	closedir(dirp);
}

void ContentScaner::FilterBySize()
{
	struct stat statbuf;
	for (auto file_info : files_)
	{
		if (stat(file_info->path.data(), &statbuf) == -1) 
		{
			logger().wrn("scaner", "get file siz failed", file_info->path, curr_errno_msg());
			continue;
		}

		file_info->size = statbuf.st_size;

		FilterSizeKey key(file_info);
		auto it = filter_size_.find(key);
		if (it == filter_size_.end())
			it = filter_size_.insert(key, deqFiles()).second;
		deqFiles group = it->second;
		group.push_back(file_info);
	}
}
