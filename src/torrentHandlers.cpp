#include "torrentHandlers.hpp"

namespace torhandler {
    
std::string createTorrentFile(const std::string& fullPathToTag, const std::string& docName) {
    std::string creator_str = "libtorrent";
    std::string comment_str;

    // Create directory if needed
    std::string outfile = fullPathToTag;
    size_t index = fullPathToTag.find("SplittedTags");
    if (index != std::string::npos)
        outfile = outfile.replace(index, sizeof("SplittedTags") - 1, "Torrents");
    mkdir((fullPathToTag.substr(0, index) + "Torrents").c_str(), myconst::CODE_RWE) == 0;
    bool dirCreated = (mkdir((fullPathToTag.substr(0, index) + "Torrents/" + docName).c_str(), myconst::CODE_RWE) == 0);
    if (errno != EEXIST && !dirCreated)
        outfile = fullPathToTag.substr(0, fullPathToTag.rfind('.'));
    outfile = outfile.substr(0, outfile.rfind('.')) + ".torrent";

    // Prepare
    lt::create_flags_t flags{};
    lt::file_storage fileStor;
    lt::add_files(fileStor, fullPathToTag, xmlhandler::file_filter, flags);
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
}  // namespace torhandler