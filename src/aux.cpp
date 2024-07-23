#include "aux.hpp"

namespace aux {

bool load_file(std::string const& filename, std::vector<char>& v, int limit) {
    std::fstream f(filename, std::ios_base::in | std::ios_base::binary);
    f.seekg(0, std::ios_base::end);
    auto const s = f.tellg();
    if (s > limit || s < 0)
        return false;
    f.seekg(0, std::ios_base::beg);
    v.resize(static_cast<std::size_t>(s));
    if (s == std::fstream::pos_type(0))
        return !f.fail();
    f.read(v.data(), int(v.size()));
    return !f.fail();
}

bool save_file(std::string const& filename, const char *data) {
    std::fstream f(filename, std::ios_base::trunc | std::ios_base::out |
                                 std::ios_base::binary);
    f.write(data, sizeof(data));
    return !f.fail();
}

bool save_file(std::string const& filename, std::vector<char> const& v) {
    return save_file(filename, v.data());
}

bool save_file(std::string const& filename, std::string const& str) {
    return save_file(filename, str.data());
}

std::vector<std::pair<std::string, int>> readDHT(std::string const& filename) {
    std::vector<char> readData;
    std::vector<std::pair<std::string, int>> nodes;
    if (filename.empty() || !load_file(filename, readData)) {
        return nodes;
    }
    std::string currentIP;
    std::string currentPort;
    bool isPort = false;
    for (int i = 0; i < readData.size(); i++) {
        char ch = readData[i];
        if (ch == ':') {
            isPort = true;
            continue;
        }
        if (isPort) {
            currentPort += ch;
        } else {
            currentIP += ch;
        }
        if (ch == '\n' || (readData.size() - 1) == i) {
            isPort = false;
            nodes.push_back(std::make_pair(currentIP, std::stoi(currentPort)));
            currentIP.clear();
            currentPort.clear();
            continue;
        }
    }
    return nodes;
}

std::string makeStringDHTbootstrap(
    const std::vector<std::pair<std::string, int>>& nodes) {
    std::string stringNodes;
    for (auto& node : nodes)
        stringNodes += node.first + ':' + std::to_string(node.second) + ',';
    if (!stringNodes.empty())
        stringNodes.pop_back();
    return stringNodes;
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


}  // namespace aux