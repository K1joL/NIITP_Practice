#ifndef AUXILIARY_HPP
#define AUXILIARY_HPP

#include <libtorrent/session.hpp>

namespace aux{

using  node = std::pair<std::string, int>;

void add_dht_routers(lt::session &ses, const std::vector<const node&> &nodes);
void add_dht_nodes(lt::session &ses, const std::vector<const node&> &nodes);
bool load_file(std::string const& filename, std::vector<char>& v, int limit = 8000000);
int save_file(std::string const& filename, std::vector<char> const& v);
}

#endif //!AUXILIARY_HPP