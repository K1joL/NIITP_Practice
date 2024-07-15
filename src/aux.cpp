#include "aux.hpp"
namespace aux {

using std::cerr;
using std::endl;

// Split XML file into tags according to nestLevel
std::vector<std::string> splitXML(const std::string& xmlFilePath, int nestLevel,
                                  std::string pathToSave, bool sepFolders) {
    std::vector<std::string> outputFilePaths;
    std::ifstream xmlFile(xmlFilePath);

    if (!xmlFile.is_open()) {
        cerr << "Failed to open file!" << endl;
        return outputFilePaths;
    }
    int currentDeepLevel = 0;
    std::string line, tagName;
    std::stringstream tagContent;
    bool createNewFile = false;
    bool insideTag = false;
    std::string xmlFileName = xmlFilePath.substr(xmlFilePath.rfind('/') + 1,
                                                 xmlFilePath.rfind('.') - xmlFilePath.rfind('/') - 1);
    // Create directories 
    if (!pathToSave.empty() && pathToSave.back() != '/')
        pathToSave.push_back('/');
    std::string pathToTags = pathToSave + "TempFiles/";
    bool dirCreated = mkdir(pathToTags.c_str(), CODE_RWE) == 0;
    if (!dirCreated && errno == EEXIST ){
        pathToTags = pathToTags + "SplittedTags/";
        mkdir((pathToTags).c_str(), CODE_RWE) == 0;
    }
    pathToTags += xmlFileName + '/';
    dirCreated = mkdir(pathToTags.c_str(), CODE_RWE) == 0;
    if (errno != EEXIST && dirCreated)
        pathToTags = "";

    while (std::getline(xmlFile, line)) {
        // std::cout << currentDeepLevel << " " << tagContent.str() << endl;
        if (line.find("<") != std::string::npos && currentDeepLevel != nestLevel) {
            // Start of a tag
            size_t start = line.find("<");
            size_t end = line.find(">");

            if (start != std::string::npos && line[start + 1] == '?')
                continue;

            if (start != std::string::npos && end != std::string::npos && currentDeepLevel < nestLevel) {
                bool oneline = (std::count(line.begin(), line.end(), '<') == 2);
                tagName = line.substr(start + 1, end - start - 1);
                if (oneline) {
                    size_t startValue = line.find('>');
                    size_t endValue = line.find_last_of('<');
                    tagContent << line.substr(startValue + 1, endValue - startValue - 1) << "\n";
                    tagName = '/' + tagName;
                }
                // Check if it's a closing tag
                if (tagName[0] == '/') {
                    insideTag = false;
                    createNewFile = true;
                } else {
                    insideTag = true;
                    currentDeepLevel++;
                }
            }
        } else if (insideTag) {
            // Append content to the current tag
            if (line.find("</" + tagName + '>') != std::string::npos) {
                tagName = '/' + tagName;
                createNewFile = true;
            } else
                tagContent << line << "\n";
        }
        if (createNewFile) {
            currentDeepLevel--;
            insideTag = false;
            createNewFile = false;
            if (!tagContent.str().empty()) {
                // Create new file
                std::string newFileName = pathToTags + tagName.substr(1) + ".xml";
                std::ofstream newFile(newFileName);
                newFile << "<" << tagName.substr(1) << ">" << "\n"
                        << tagContent.str() << "</" << tagName.substr(1) << ">";
                newFile.close();

                outputFilePaths.push_back(newFileName);

                tagContent.str("");  // Clear content for next tag
                continue;
            }
        }
    }

    xmlFile.close();
    return outputFilePaths;
}

// allows do not include files and folders whose
// name starts with a "."
bool file_filter(std::string const& f) {
    if (f.empty()) return false;

    size_t sep = f.rfind('/');
    if (sep != std::string::npos)
        sep = 0;
    else
        ++sep;

    // return false if the first character of the filename is a .
    if (f[sep] == '.') return false;
    return true;
}

}  // namespace aux