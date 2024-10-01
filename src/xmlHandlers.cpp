#include "xmlHandlers.hpp"

namespace xmlhandler {

using std::cerr;
using std::endl;

void parseAttributes(std::stringstream& ss, std::shared_ptr<Tag>& tag) {
    std::string attribute;
    while (std::getline(ss, attribute, ' ')) {
        std::size_t equalSign = attribute.find('=');
        if (equalSign != std::string::npos) {
            std::string attributeName = attribute.substr(0, equalSign);
            // Check for quotes
            if (attribute[equalSign + 1] == '\"' || attribute[equalSign + 1] == '\'') {
                // Quoted value
                std::size_t endQuote = attribute.find(attribute[equalSign + 1], equalSign + 2);
                if (endQuote != std::string::npos) {
                    std::string attributeValue = attribute.substr(equalSign + 2, endQuote - equalSign - 2);
                    tag->attributes[attributeName] = attributeValue;
                } else {
                    std::stringstream ssCopy;
                    ssCopy << ss.str().substr(ss.str().find(' ') + 1);
                    std::string endAttribute;
                    std::getline(ssCopy, endAttribute, ' ');
                    if (endAttribute.find(attribute[equalSign + 1]) != std::string::npos) {
                        std::string attributeValue =
                            attribute.substr(equalSign + 2, endQuote - equalSign - 2);
                        std::string endAttributeValue =
                            endAttribute.substr(0, endAttribute.find(attribute[equalSign + 1]));
                        tag->attributes[attributeName] = attributeValue + ' ' + endAttributeValue;
                        std::getline(ss, attribute, ' ');
                    }
                }
            } else {
                // Unquoted value (single word)
                std::string attributeValue = attribute.substr(equalSign + 1);
                tag->attributes[attributeName] = attributeValue;
            }
        }
    }
}

std::vector<std::shared_ptr<Tag>> getTags(const std::string& xmlContent) {
    if (xmlContent.empty()) {
        std::cerr << "Content is empty!" << std::endl;
        return std::vector<std::shared_ptr<Tag>>();
    }
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
                std::stringstream ss(line.substr(line.find(' ') + 1, line.find('>') - line.find(' ') - 1));
                parseAttributes(ss, newTag);
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
                parseAttributes(ss, newTag);
            }
            if (line.find("</") != std::string::npos)
                newTag->content += line.substr(line.find('>') + 1, line.find("</") - line.find('>') - 1);
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
            std::string result = tagContent.str();
            // remove end of line symbol(s)
            if (result.size() >= 2 && result.substr(result.size() - 2) == "\r\n")
                result.erase(result.size() - 2, result.size() - 1);
            else if (result.size() >= 1 && result.back() == '\n')
                result.pop_back();
            if (!result.empty())
                splitXmls.push_back(result);
            tagContent.str("");  // Clear content for next tag
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
                                       bool separatedFolders, int nestingLevel) {
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
    std::vector<std::string> splitXmls = splitXML(docContent, nestingLevel);
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

        std::string newFilename = xml.substr(startName, nameSize);
        if (newFilename.find("AdditionalField") != std::string::npos) {
            std::string hashsumTag = "Hashsum";
            if (xml.find(hashsumTag) != std::string::npos) {
                startName = xml.find(hashsumTag) + hashsumTag.size() + 2;
                nameSize = xml.find("</", startName) - startName;
                newFilename = "Part-" + xml.substr(startName, nameSize);
            }
        } else if (newFilename.find("Document") != std::string::npos) {
            std::string Number = "Number";
            if (xml.find(Number) != std::string::npos) {
                startName = xml.find(Number) + Number.size() + 1;
                nameSize = xml.find("</", startName) - startName;
                newFilename = "Document-" + xml.substr(startName, nameSize);
            }
        } else if (xml.find("Contact") == std::string::npos)
            continue;
        newFilename = pathToSave + newFilename + ".xml";
        aux::saveFileWithDirectory(newFilename, xml);
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
        std::string filename = pathToSave;
        if (!part.asMap().at(tmplkey::PART[3]).asList().empty())
            filename += "Full";
        filename += "Part-" + part.asMap().at(tmplkey::PART[2]).asString() + ".xml";
        aux::saveFileWithDirectory(filename, result);
        filePaths.push_back(filename);
    }
    return std::move(filePaths);
}

jinja2::ValuesMap getTagPartsFromFiles(const std::vector<std::string>& tagPartsPaths) {
    if (tagPartsPaths.empty()) {
        std::cerr << "The files path vector is empty!" << std::endl;
        return jinja2::ValuesMap();
    }
    jinja2::ValuesMap partsMap;
    for (auto& tagPartPath : tagPartsPaths) {
        std::ifstream partFile(tagPartPath);
        if (!partFile.is_open()) {
            std::cerr << "Unable to open file " + tagPartPath << std::endl;
            continue;
        }
        std::string partContent;
        std::string line;
        while (std::getline(partFile, line))
            partContent += line + '\n';
        // Separate Document info from tags2
        if (partContent.find("Document") != std::string::npos) {
            partContent = partContent.substr(0, partContent.find("<Author>"));
            partContent += "<Document>";
        }
        std::vector<std::shared_ptr<Tag>> tags = getTags(partContent);
        if (tags.empty()) {
            std::cerr << "Tag vector is empty in this file: " + tagPartPath << std::endl;
            continue;
        }
        jinja2::ValuesMap tagMap;
        std::string tagName;
        if (partContent.find("Document") != std::string::npos) {
            tagName = "documents";
            if (partsMap[tagName].isEmpty())
                partsMap[tagName] = jinja2::ValuesList();
        } else if (partContent.find("AdditionalField") != std::string::npos) {
            // if found part
            // Prepare maps for correct filling
            tagName = "parts";
            if (partsMap[tagName].isEmpty())
                partsMap[tagName] = jinja2::ValuesList();
            tagMap[tmplkey::PART[3]] = jinja2::ValuesList();
        } else {
            tagName = aux::toLowerStr(tagPartPath.substr(
                tagPartPath.rfind('/') + 1, tagPartPath.rfind('.') - tagPartPath.rfind('/') - 1));
        }
        traverseTag(tags[0], tagMap, 0);
        if (!tagMap.empty()) {
            if (tagName != "parts" && tagName != "documents")
                partsMap.emplace(tagName, tagMap);
            else {
                partsMap.at(tagName).asList().push_back(tagMap);
            }
        }
    }

    return std::move(partsMap);
}

void traverseTag(const std::shared_ptr<Tag> tag, std::stringstream& ss, int startLevel) {
    static int nestingLevel = 0;
    ++nestingLevel;
    if (nestingLevel < startLevel) {
        for (auto& ch : tag->children)
            traverseTag(ch, ss, startLevel);
        return;
    }
    ss << std::string(nestingLevel - startLevel - 1, '\t') << "<" << tag->name;
    for (const auto& attr : tag->attributes) {
        ss << " " << attr.first << "=\"" << attr.second << "\"";
    }
    if (!tag->children.empty()) {
        ss << ">\n";
        for (const auto child : tag->children) {
            traverseTag(child, ss, startLevel);
        }

        ss << std::string(nestingLevel - startLevel - 1, '\t') << "</" << tag->name << ">" << std::endl;
    } else {
        ss << ">" << tag->content << "</" << tag->name << ">" << std::endl;
    }
    --nestingLevel;
}

void traverseTag(const std::shared_ptr<Tag> tag, jinja2::ValuesMap& tagMap, int startLevel) {
    static int nestingLevel = 0;
    ++nestingLevel;
    if (nestingLevel < startLevel) {
        for (auto& ch : tag->children)
            traverseTag(ch, tagMap, startLevel);
        return;
    }
    jinja2::ValuesMap fieldMap;
    for (const auto& attr : tag->attributes) {
        if (tag->name == "Value")
            fieldMap.emplace(aux::toLowerStr(attr.first), attr.second);
        else if (tag->name != "Document" && tag->name != "AdditionalField")
            tagMap.emplace(aux::toLowerStr(tag->name), attr.second);
        else
            tagMap.emplace(aux::toLowerStr(attr.first), attr.second);
    }
    if (!tag->children.empty()) {
        for (const auto child : tag->children) {
            traverseTag(child, tagMap, startLevel);
        }
    } else {
        if (!tag->content.empty())
            if (tag->name == "Value" && tag->attributes.at("Name") == "Hashsum")
                tagMap[aux::toLowerStr(tag->attributes.at("Name"))] = tag->content;
            else if (tag->name == "Value") {
                fieldMap[tmplkey::FIELD[0]] = tag->attributes.at("Name");
                fieldMap[tmplkey::FIELD[1]] = tag->content;
                tagMap[tmplkey::PART[3]].asList().push_back(fieldMap);
            } else
                tagMap.emplace(aux::toLowerStr(tag->name), tag->content);
    }
    --nestingLevel;
}

}  // namespace xmlhandler