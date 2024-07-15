#ifndef AUXILIARY_FUNCITONS_HPP
#define AUXILIARY_FUNCITONS_HPP

#include <errno.h>
#include <sys/stat.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace aux {
// 0777 : Read, write, and execute permissions for the owner, group, and others.
#define CODE_RWE 0777

std::vector<std::string> splitXML(const std::string& xmlFilePath, int nestLevel = 2,
                                  std::string pathToSave = "", bool sepFolders = true);
bool file_filter(std::string const& f);

}  // namespace aux

#endif  //! AUXILIARY_FUNCITONS_HPP