#ifndef AUXILIARY_HPP
#define AUXILIARY_HPP

#include <libtorrent/session.hpp>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
namespace aux {

bool load_file(std::string const& filename, std::vector<char>& v, int limit = 8000000);
bool save_file(std::string const& filename, std::vector<char> const& v);
std::vector<std::pair<std::string, int>> readDHT(std::string const& filename);
std::string makeStringDHTbootstrap(const std::vector<std::pair<std::string, int>>& nodes);

}  // namespace aux

#endif  //! AUXILIARY_HPP