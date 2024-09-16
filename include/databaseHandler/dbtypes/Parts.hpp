#ifndef DB_PARTS_HPP
#define DB_PARTS_HPP

#include <odb/core.hxx>
#include <string>
#include <vector>

#pragma db object table("parts")
class Part {
public:
    Part() {
    }
    Part(unsigned long docInfoId, std::string name, std::string type, std::string hash)
        : m_docinfoid(docInfoId), m_name(name), m_type(type), m_hash(hash) {
    }

    unsigned long getPartId() const {
        return m_partid;
    }
    unsigned long getDocInfoId() const {
        return m_docinfoid;
    }
    std::string getName() const {
        return m_name;
    }
    std::string getType() const {
        return m_type;
    }
    std::string getHash() const {
        return m_hash;
    }
    std::vector<std::string> getData() {
        std::vector<std::string> v;
        v.push_back(m_name);
        v.push_back(m_type);
        v.push_back(m_hash);
        return v;
    }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long m_partid;

    unsigned long m_docinfoid;
    std::string m_name;
    std::string m_type;
    std::string m_hash;
};

#endif  //! DB_PARTS_HPP