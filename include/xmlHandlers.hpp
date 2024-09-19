#ifndef XML_HANDLERS_HPP
#define XML_HANDLERS_HPP

#include <errno.h>
#include <jinja2cpp/template.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "aux.hpp"

namespace xmlhandler {

struct Tag {
    std::string name;
    std::map<std::string, std::string> attributes;
    std::string content;
    std::vector<std::shared_ptr<Tag>> children;
};

// Split XML file into tags according to nestLevel
std::vector<std::shared_ptr<Tag>> getTags(const std::string& xmlContent);
void traverseTag(const std::shared_ptr<Tag> tag, std::stringstream& ss, int startLevel = 0);
std::vector<std::string> splitXML(const std::string& xmlContent, int nestingLevel);
std::vector<std::string> splitDocument(const std::string& xmlDocPath,
                                       const std::string savePath = myconst::DEFAULT_TAGS_PATH,
                                       bool separatedFolders = true, int nestingLevel = 2);
std::vector<std::string> createPartXmls(
    const jinja2::ValuesList& parts, jinja2::Template& tmpl,
    const std::string& pathToDocTags = myconst::DEFAULT_TAGS_PATH);

}  // namespace xmlhandler

#endif  //! XML_HANDLERS_HPP