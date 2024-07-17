#include "aux.hpp"
namespace aux {

using std::cerr;
using std::endl;

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
    bool tfDirCreated = (mkdir(pathToTags.c_str(), CODE_RWE) == 0);
    bool stDirCreated = true;
    if (tfDirCreated || errno == EEXIST) {
        pathToTags += "SplittedTags/";
        stDirCreated = (mkdir(pathToTags.c_str(), CODE_RWE) == 0);
    }
    pathToTags += xmlFileName + '/';
    bool docDirCreated = (mkdir(pathToTags.c_str(), CODE_RWE) == 0);
    if (errno != EEXIST && !docDirCreated)
        if (stDirCreated)
            pathToTags = "TempFiles/SplittedTags/";
        else if (tfDirCreated)
            pathToTags = "TempFiles/";
        else
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

std::string createTorrentFile(const std::string& fullPathToTag, const std::string& docName) {
    std::string creator_str = "libtorrent";
    std::string comment_str;

    // Create directory if needed
    std::string outfile = fullPathToTag;
    size_t index = fullPathToTag.find("SplittedTags");
    if (index != std::string::npos)
        outfile = outfile.replace(index, sizeof("SplittedTags") - 1, "Torrents");
    mkdir((fullPathToTag.substr(0, index) + "Torrents").c_str(), CODE_RWE) == 0;
    bool dirCreated = (mkdir((fullPathToTag.substr(0, index) + "Torrents/" + docName).c_str(), CODE_RWE) == 0);
    if (errno != EEXIST && !dirCreated)
        outfile = fullPathToTag.substr(0, fullPathToTag.rfind('.'));
    outfile = outfile.substr(0, outfile.rfind('.')) + ".torrent";

    // Prepare
    lt::create_flags_t flags{};
    lt::file_storage fileStor;
    lt::add_files(fileStor, fullPathToTag, aux::file_filter, flags);
    if (fileStor.num_files() == 0) {
        return "";
    }
    // fill the torrent with info
    lt::create_torrent t(fileStor, 0, flags);
    t.add_collection(docName);
    // t.add_tracker("udp://tracker.opentrackr.org:1337/announce");
    auto const num = t.num_pieces();
    lt::set_piece_hashes(t, fullPathToTag.substr(0, fullPathToTag.rfind('/')));
    t.set_creator(creator_str.c_str());
    if (!comment_str.empty()) {
        t.set_comment(comment_str.c_str());
    }

    // Create the torrent and save the file
    std::vector<char> torrent;
    lt::bencode(back_inserter(torrent), t.generate());
    if (!outfile.empty()) {
        std::fstream out;
        out.exceptions(std::ifstream::failbit);
        out.open(outfile.c_str(), std::ios_base::out | std::ios_base::binary);
        out.write(torrent.data(), int(torrent.size()));
        out.close();
    }

    return outfile;
}

}  // namespace aux