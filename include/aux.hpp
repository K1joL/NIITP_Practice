#ifndef AUXILIARY_HPP
#define AUXILIARY_HPP

#include <libtorrent/session.hpp>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>

namespace aux {

// returns True if loading is successful
bool load_file(std::string const& filename, std::vector<char>& v, int limit = 8000000);

// returns True if saving is successful
bool save_file(std::string const& filename, const char *data);
bool save_file(std::string const& filename, std::vector<char> const& v);
bool save_file(std::string const& filename, std::string const& str);

std::vector<std::pair<std::string, int>> readDHT(std::string const& filename);
std::string makeStringDHTbootstrap(const std::vector<std::pair<std::string, int>>& nodes);

// TO DO: 
// xml file formatting functions
// std::string removeExtraWhiteSpaces(const std::string& line);
// void removeExtraNewlines(const std::string& fileName);

// allows do not include files and folders whose
// name starts with a "."
bool file_filter(std::string const& f);

}  // namespace aux

#endif  //! AUXILIARY_HPP