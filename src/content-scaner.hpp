#pragma once
#include <dirent.h>
#include <string>
#include <deque>
#include <unordered_map>

namespace altium
{
	class ContentScaner
	{
	public:
		typedef std::deque<std::string> deqGroup;
		typedef std::deque<deqGroup> deqDuplicates;

		struct FileInfo
		{
			int64_t size;
			int32_t begin_hash;
			int32_t full_hash;
			std::string path;

			FileInfo(std::string path);
		};

		typedef std::shared_ptr<FileInfo> ptrFileInfo;

		struct FilterSizeKey
		{
			int32_t size;
			int32_t begin_hash;

			FilterSizeKey(ptrFileInfo file_info);
		};

		typedef std::deque<ptrFileInfo> deqFiles;
		typedef std::unordered_map<SizeFilterKey, ptrFileInfo> mapFilterSize;

	public:
		ContentScaner(std::string path);

	public:
		void Scan();

	public:
		deqDuplicates duplicates() const;

	private:
		void EnumerateFiles();
		void FilterBySize();

	private:
		std::string path_;
		deqDuplicates duplicates_;
		deqFiles files_;
		mapFilterSize filter_size_;

	};
}