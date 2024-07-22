#ifndef TORRENT_HANDLER_HPP
#define TORRENT_HANDLER_HPP

#include <libtorrent/create_torrent.hpp>
#include <libtorrent/file_storage.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <string>

#include "constants.hpp"
#include "xmlHandlers.hpp"

namespace torhandler {

// docName will be added to collection field
std::string createTorrentFile(const std::string& fullPathToTag, const std::string& docName);

}  // namespace torhandler

#endif  //! TORRENT_HANDLER_HPP