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
    Part(unsigned long documentId, std::string name, std::string type, std::string hash)
        : m_documentid(documentId), m_name(name), m_type(type), m_hash(hash) {
    }
    void setDocumentId(unsigned long documentId) {
        m_documentid = documentId;
    }
    
    void setName(std::string name) {
        m_name = std::move(name);
    }
    
    void setType(std::string type) {
        m_type = std::move(type);
    }
    
    void setHash(std::string hash) {
        m_hash = std::move(hash);
    }

    unsigned long getId() const {
        return m_partid;
    }
    unsigned long getDocumentId() const {
        return m_documentid;
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

    unsigned long m_documentid;
    std::string m_name;
    std::string m_type;
    std::string m_hash;
};

#endif  //! DB_PARTS_HPP