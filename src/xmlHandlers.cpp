#include "xmlHandlers.hpp"

namespace xmlhandler {

using std::cerr;
using std::endl;

std::vector<std::shared_ptr<Tag>> getTags(const std::string& xmlFilePath) {
    std::ifstream xmlFile(xmlFilePath);
    std::vector<std::shared_ptr<Tag>> tags;
    std::vector<std::shared_ptr<Tag>> tagStack;  // Stack to track nesting
    std::shared_ptr<Tag> currentTag = nullptr;
    std::string line;

    if (!xmlFile.is_open()) {
        cerr << "Failed to open file!" << endl;
        return tags;
    }

    // xml processing
    while (std::getline(xmlFile, line)) {
        if (line.find("<?") != std::string::npos)
            continue;
        // Trim whitespace
        line.erase(line.begin(), std::find_if_not(line.begin(), line.end(), isspace));
        line.erase(std::find_if_not(line.rbegin(), line.rend(), isspace).base(), line.end());
        // Handle opening tag
        if (line.find('<') == 0 && line.find('>') != std::string::npos &&
            line.find('/') == std::string::npos) {
            // Extract tag name
            std::string tagName = line.substr(1, line.find('>') - 1);
            std::size_t attributeStart = tagName.find(' ');
            if (attributeStart != std::string::npos) {
                tagName = tagName.substr(0, attributeStart);
            }
            // Create new tag and add attributes
            std::shared_ptr<Tag> newTag = std::make_shared<Tag>();
            newTag->name = tagName;
            // Parse attributes if found
            if (line.find('=') != std::string::npos) {
                std::stringstream ss(
                    line.substr(line.find(' ') + 1, line.find('>') - line.find(' ') - 1));
                std::string attribute;
                while (std::getline(ss, attribute, ' ')) {
                    std::size_t equalSign = attribute.find('=');
                    if (equalSign != std::string::npos) {
                        std::string attributeName = attribute.substr(0, equalSign);
                        std::string attributeValue = attribute.substr(equalSign + 1);
                        // Remove quotes
                        attributeValue.erase(0, 1);
                        attributeValue.erase(attributeValue.size() - 1, 1);
                        newTag->attributes[attributeName] = attributeValue;
                    }
                }
            }
            // Add to parent's children
            if (currentTag != nullptr)
                currentTag->children.push_back(newTag);

            // Push to stack and set current tag
            tagStack.push_back(newTag);
            if (currentTag == nullptr)
                tags.push_back(newTag);
            currentTag = tagStack.back();
        }
        // Handle closing tag
        else if (line.find("</") == 0 && line.find('>') != std::string::npos) {
            // Pop from stack
            tagStack.pop_back();
            currentTag = (tagStack.empty() ? nullptr : tagStack.back());
        }
        // Handle one-line tag
        else if ((line.find('<') == 0 && line.find("/>") != std::string::npos) ||
                 (std::count(line.begin(), line.end(), '<') == 2)) {
            // Extract tag name and attributes
            std::string tagName = line.substr(1, line.find('>') - 1);
            std::size_t attributeStart = tagName.find(' ');
            if (attributeStart != std::string::npos) {
                tagName = tagName.substr(0, attributeStart);
            }

            std::shared_ptr<Tag> newTag = std::make_shared<Tag>();
            newTag->name = tagName;

            // Parse attributes if found
            if (line.find('=') != std::string::npos) {
                size_t attrEnd = line.find("/>");
                if (attrEnd == std::string::npos)
                    attrEnd = line.find(">");
                std::stringstream ss(line.substr(line.find(' ') + 1, attrEnd - line.find(' ') - 1));
                std::string attribute;
                while (std::getline(ss, attribute, ' ')) {
                    std::size_t equalSign = attribute.find('=');
                    if (equalSign != std::string::npos) {
                        std::string attributeName = attribute.substr(0, equalSign);
                        std::string attributeValue = attribute.substr(equalSign + 1);
                        // Remove quotes
                        attributeValue.erase(0, 1);
                        attributeValue.erase(attributeValue.size() - 1, 1);
                        newTag->attributes[attributeName] = attributeValue;
                    }
                }
            }
            if (line.find("</") != std::string::npos)
                newTag->content +=
                    line.substr(line.find('>') + 1, line.find("</") - line.find('>') - 1);
            // Add to parent's children
            if (currentTag != nullptr)
                currentTag->children.push_back(newTag);
            if (currentTag == nullptr)
                tags.push_back(newTag);
        }
        // Handle content
        else if (currentTag != nullptr) {
            currentTag->content += line;
        }
    }

    xmlFile.close();
    return tags;
}

std::vector<std::string> splitXML(const std::string& xmlFilePath, int nestingLevel,
                                  const std::string& savePath, bool sepFolders) {
    std::vector<std::shared_ptr<Tag>> tags = getTags(xmlFilePath);
    if (tags.empty())
        return std::vector<std::string>();
    // Create directories
    std::vector<std::string> outputFilePaths;
    std::string xmlFileName = xmlFilePath.substr(
        xmlFilePath.rfind('/') + 1, xmlFilePath.rfind('.') - xmlFilePath.rfind('/') - 1);
    std::string pathToSave;
    if (!pathToSave.empty() && pathToSave.back() != '/')
        pathToSave.push_back('/');
    std::string pathToTags = pathToSave + "TempFiles/";
    bool tfDirCreated = (mkdir(pathToTags.c_str(), myconst::CODE_RWE) == 0);
    bool stDirCreated = true;
    if (tfDirCreated || errno == EEXIST) {
        pathToTags += "SplittedTags/";
        stDirCreated = (mkdir(pathToTags.c_str(), myconst::CODE_RWE) == 0);
    }
    pathToTags += xmlFileName + '/';
    bool docDirCreated = (mkdir(pathToTags.c_str(), myconst::CODE_RWE) == 0);
    if (errno != EEXIST && !docDirCreated)
        if (stDirCreated)
            pathToTags = "TempFiles/SplittedTags/";
        else if (tfDirCreated)
            pathToTags = "TempFiles/";
        else
            pathToTags = "";
    // Tags processing
    std::stringstream tagContent;
    std::queue<std::pair<std::shared_ptr<xmlhandler::Tag>, int>> queue;
    
    for (const auto& tag : tags) {
        queue.push({tag, 0});
    }
    while (!queue.empty()) {
        auto currentTag = queue.front().first;
        auto currentLevel = queue.front().second;
        queue.pop();

        if (currentLevel == nestingLevel) {
            traverseTag(currentTag, tagContent);
            if (!tagContent.str().empty()) {
                // Create new file
                std::string newFileName = pathToTags + currentTag->name;
                std::ofstream newFile(newFileName);
                newFile << tagContent.str();
                newFile.close();
                outputFilePaths.push_back(newFileName); // add new file to result
                tagContent.str("");  // Clear content for next tag
            }
        } else if (currentLevel < nestingLevel) {
            // Добавляем детей в очередь
            for (const auto& child : currentTag->children) {
                queue.push({child, currentLevel + 1});
            }
        }
    }

    return outputFilePaths;
}

void traverseTag(const std::shared_ptr<Tag> tag, std::stringstream& ss, int startLevel) {
    static int nestingLevel = 0;
    ++nestingLevel;
    if (nestingLevel < startLevel) {
        for (auto& ch : tag->children)
            traverseTag(ch, ss, startLevel);
        return;
    }
    ss << std::string(nestingLevel, '\t') << "<" << tag->name;
    for (const auto& attr : tag->attributes) {
        ss << " " << attr.first << "=\"" << attr.second << "\"";
    }
    if (!tag->children.empty()) {
        for (auto& ch : tag->children)
            std::cout << " ch " << ch->name << std::endl;
        ss << ">\n";
        for (const auto child : tag->children) {
            traverseTag(child, ss, startLevel);
        }

        ss << std::string(nestingLevel, '\t') << "</" << tag->name << ">\n" << std::endl;
    } else {
        ss << ">" << tag->content << "</" << tag->name << ">\n" << std::endl;
    }
    --nestingLevel;
}

bool file_filter(std::string const& f) {
    if (f.empty())
        return false;

    size_t sep = f.rfind('/');
    if (sep != std::string::npos)
        sep = 0;
    else
        ++sep;

    // return false if the first character of the filename is a .
    if (f[sep] == '.')
        return false;
    return true;
}

}  // namespace xmlhandler