#ifndef AUXILIARY_HPP
#define AUXILIARY_HPP

#include <libtorrent/session.hpp>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>

#include "constants.hpp"

namespace aux {

// returns True if loading is successful
bool load_file(std::string const& filename, std::vector<char>& v, int limit = 8000000);

// returns True if saving is successful
bool save_file(std::string const& filename, const char *data);
bool save_file(std::string const& filename, std::vector<char> const& v);
bool save_file(std::string const& filename, std::string const& str);
bool saveFileWithDirectory(std::string const& filePath, std::string const& str);
bool saveFileWithDirectory(std::string const& filePath, std::vector<char> const& v);
bool saveFileWithDirectory(std::string const& filePath, const char *data);

std::vector<std::pair<std::string, int>> readDHT(std::string const& filename);
std::string makeStringDHTbootstrap(const std::vector<std::pair<std::string, int>>& nodes);
bool makeDirectory(const std::string savePath = "");
// allows do not include files and folders whose
// name starts with a "."
bool file_filter(std::string const& f);
std::string toLowerStr(const std::string &str);
}  // namespace aux

#endif  //! AUXILIARY_HPP