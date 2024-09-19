#include "xmlHandlers.hpp"

namespace xmlhandler {

using std::cerr;
using std::endl;

std::vector<std::shared_ptr<Tag>> getTags(const std::string& xmlContent) {
    std::vector<std::shared_ptr<Tag>> tags;
    std::vector<std::shared_ptr<Tag>> tagStack;  // Stack to track nesting
    std::shared_ptr<Tag> currentTag = nullptr;
    std::string line;
    std::istringstream content(xmlContent);
    // xml processing
    while (std::getline(content, line)) {
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
    return std::move(tags);
}

std::vector<std::string> splitXML(const std::string& xmlContent, int nestingLevel) {
    std::vector<std::shared_ptr<Tag>> tags = getTags(xmlContent);
    if (tags.empty())
        return std::vector<std::string>();

    // Tags processing
    std::stringstream tagContent;
    std::queue<std::pair<std::shared_ptr<xmlhandler::Tag>, int>> queue;
    std::vector<std::string> splitXmls;
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
                splitXmls.push_back(tagContent.str());
                tagContent.str("");  // Clear content for next tag
            }
        } else if (currentLevel < nestingLevel) {
            // add child tags to queue
            for (const auto& child : currentTag->children) {
                queue.push({child, currentLevel + 1});
            }
        }
    }

    return std::move(splitXmls);
}

std::vector<std::string> splitDocument(const std::string& xmlDocPath, const std::string savePath,
                                       bool separatedFolders) {
    // read file
    std::ifstream docFile(xmlDocPath);
    if (!docFile.is_open()) {
        std::cerr << "Error: could not open document " << xmlDocPath << '!' << std::endl;
        return std::vector<std::string>();
    }
    std::string docContent;
    std::string line;
    while (std::getline(docFile, line))
        docContent += line + '\n';
    std::vector<std::string> splitXmls = splitXML(docContent, 2);
    if (splitXmls.empty())
        return std::vector<std::string>();

    // Create directories
    std::string pathToSave = savePath;
    if (pathToSave.back() != '/')
        pathToSave += '/';
    // add document filename to directory name
    if (savePath == myconst::DEFAULT_TAGS_PATH) {
        int startName = xmlDocPath.rfind('/') + 1;
        int nameSize = xmlDocPath.rfind('.') - startName - 1;
        pathToSave += xmlDocPath.substr(startName, nameSize);
    }
    if (!aux::makeDirectory(pathToSave))
        return std::vector<std::string>();

    // Create Files
    std::vector<std::string> filePaths;
    for (auto& xml : splitXmls) {
        int startName = xml.find_first_of('<') + 1;
        int nameSize = xml.find_first_of("> ") - startName;

        std::string newFilename = pathToSave + xml.substr(startName, nameSize) + ".xml";
        std::cout << newFilename << std::endl;
        aux::save_file(newFilename, xml);
        filePaths.push_back(newFilename);
    }

    return std::move(filePaths);
}

std::vector<std::string> createPartXmls(const jinja2::ValuesList& parts, jinja2::Template& tmpl,
                                        const std::string& pathToDocTags) {
    if (!aux::makeDirectory(pathToDocTags))
        return std::vector<std::string>();

    if (parts.empty())
        return std::vector<std::string>();

    std::string pathToSave = pathToDocTags;
    if (pathToDocTags.back() != '/')
        pathToSave += '/';

    std::vector<std::string> filePaths;
    for (auto& part : parts) {
        jinja2::ValuesMap temp;
        temp["part"] = part;
        std::string result = tmpl.RenderAsString(temp).value();
        std::string filename = pathToSave + "part-" + part.asMap().at("hash").asString() + ".xml";
        aux::save_file(filename, result);
        filePaths.push_back(filename);
    }
    return std::move(filePaths);
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

}  // namespace xmlhandler