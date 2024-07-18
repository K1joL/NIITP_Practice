#include "aux.hpp"

namespace aux {

void add_dht_routers(lt::session& ses, const std::vector<const node>& nodes) {
    for (auto& node : nodes) {
        ses.add_dht_router(node);
    }
}

void add_dht_nodes(lt::session& ses, const std::vector<const node>& nodes) {
    for (auto& node : nodes) {
        ses.add_dht_node(node);
    }
}

bool load_file(std::string const& filename, std::vector<char>& v, int limit) {
    std::fstream f(filename, std::ios_base::in | std::ios_base::binary);
    f.seekg(0, std::ios_base::end);
    auto const s = f.tellg();
    if (s > limit || s < 0) return false;
    f.seekg(0, std::ios_base::beg);
    v.resize(static_cast<std::size_t>(s));
    if (s == std::fstream::pos_type(0)) return !f.fail();
    f.read(v.data(), int(v.size()));
    return !f.fail();
}

int save_file(std::string const& filename, std::vector<char> const& v) {
    std::fstream f(filename, std::ios_base::trunc | std::ios_base::out | std::ios_base::binary);
    f.write(v.data(), int(v.size()));
    return !f.fail();
}

}  // namespace aux