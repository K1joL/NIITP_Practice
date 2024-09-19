#ifndef MY_CONSTANTS_HPP
#define MY_CONSTANTS_HPP

namespace myconst{

// 0777 : Read, write, and execute permissions for the owner, group, and others.
constexpr int CODE_RWE = 0777;

constexpr const char* DEFAULT_TAGS_PATH = "TempFiles/SplitTags/";
constexpr const char* DEFAULT_TORRENT_FILES_PATH = "TempFiles/Torrents/";
constexpr const char* DEFAULT_DOWNLOADS_PATH = "TempFiles/Downloads/";
constexpr const char* DEFAULT_DOCUMENTS_PATH = "TempFiles/Documents/";
}

#endif //!MY_CONSTANTS_HPP