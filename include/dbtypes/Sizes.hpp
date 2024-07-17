#ifndef DB_SIZES_HPP
#define DB_SIZES_HPP

#include <odb/core.hxx>
#include <string>
#include <vector>
class Sizes {
public:
    Sizes() {}
    Sizes(int id, const std::string& type, const std::vector<int>& bytes) : id(id), type(type), bytes(bytes) {}

private:
    friend class odb::access;
    int id;
    std::string type;
    std::vector<int> bytes;
};

#pragma db object(Sizes)
#pragma db member(Sizes::id) id

#endif  //! DB_SIZES_HPP