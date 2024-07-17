#ifndef XML_HANDLERS_HPP
#define XML_HANDLERS_HPP

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "constants.hpp"

namespace xmlhandler {

// Split XML file into tags according to nestLevel
std::vector<std::string> splitXML(const std::string& xmlFilePath, int nestLevel = 2,
                                  std::string pathToSave = "", bool sepFolders = true);

// allows do not include files and folders whose
// name starts with a "."
bool file_filter(std::string const& f);


}  // namespace aux

#endif  //! XML_HANDLERS_HPP