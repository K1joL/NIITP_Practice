#ifndef XML_HANDLERS_HPP
#define XML_HANDLERS_HPP

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

#include "constants.hpp"

namespace xmlhandler {

struct Tag {
    std::string name;
    std::map<std::string, std::string> attributes;
    std::string content;
    std::vector<std::shared_ptr<Tag>> children;
};

// Split XML file into tags according to nestLevel
std::vector<std::shared_ptr<Tag>> getTags(const std::string& xmlFilePath);
std::vector<std::string> splitXML(const std::string& xmlFilePath, int nestingLevel,
                                  const std::string& savePath = "", bool sepFolders = true);
void traverseTag(const std::shared_ptr<Tag> tag, std::stringstream& ss, int startLevel = 0);

// allows do not include files and folders whose
// name starts with a "."
bool file_filter(std::string const& f);

}  // namespace xmlhandler

#endif  //! XML_HANDLERS_HPP