#ifndef DB_FIELDS_HPP
#define DB_FIELDS_HPP

#include <odb/core.hxx>
#include <string>
#include <vector>

#pragma db object table("fields")
class Field {
public:
    Field() {
    }
    Field(unsigned long partid, std::string name, std::string value)
        : m_partid(partid), m_name(name), m_value(value) {
    }
    void setPartId(unsigned long partId) {
        m_partid = partId;
    }
    unsigned long getFieldId() const {
        return m_fieldid;
    }
    unsigned long getPartId() const {
        return m_partid;
    }
    std::string getName() const {
        return m_name;
    }
    std::string getValue() const {
        return m_value;
    }
    std::vector<std::string> getData() {
        std::vector<std::string> v;
        v.push_back(m_name);
        v.push_back(m_value);
        return v;
    }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long m_fieldid;

    unsigned long m_partid;
    std::string m_name;
    std::string m_value;
};

#endif  //! DB_FIELDS_HPP