#ifndef AUXILIARY_FUNCITONS_HPP
#define AUXILIARY_FUNCITONS_HPP

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <libtorrent/create_torrent.hpp>
#include <libtorrent/magnet_uri.hpp>

namespace aux {
// 0777 : Read, write, and execute permissions for the owner, group, and others.
#define CODE_RWE 0777

// Split XML file into tags according to nestLevel
std::vector<std::string> splitXML(const std::string& xmlFilePath, int nestLevel = 2,
                                  std::string pathToSave = "", bool sepFolders = true);

// allows do not include files and folders whose
// name starts with a "."
bool file_filter(std::string const& f);

// docName will be added to collection field
std::string createTorrentFile(const std::string& fullPathToTag, const std::string& docName);

}  // namespace aux

#endif  //! AUXILIARY_FUNCITONS_HPP