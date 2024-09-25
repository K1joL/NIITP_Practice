#ifndef DB_CONTACTS_HPP
#define DB_CONTACTS_HPP

#include <odb/core.hxx>
#include <string>
#include <vector>

#pragma db object table("contacts")
class Contact {
public:
    Contact() {
    }
    Contact(int organization, std::string shortName, std::string fullName, std::string post,
            std::string index, std::string econtact)
        : m_organization(organization),
          m_shortname(shortName),
          m_fullname(fullName),
          m_post(post),
          m_index(index),
          m_econtact(econtact) {
    }
    unsigned long getId() const {
        return m_contactid;
    }
    int getOrganization() const {
        return m_organization;
    }
    std::string getShortName() const {
        return m_shortname;
    }
    std::string getFullName() const {
        return m_fullname;
    }
    std::string getPost() const {
        return m_post;
    }
    std::string getIndex() const {
        return m_index;
    }
    std::string getEcontact() const {
        return m_econtact;
    }

    std::vector<std::string> getData() const {
        std::vector<std::string> v;
        v.push_back(std::to_string(m_organization));
        v.push_back(m_shortname);
        v.push_back(m_fullname);
        v.push_back(m_post);
        v.push_back(m_index);
        v.push_back(m_econtact);
        return v;
    }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long m_contactid;

    int m_organization;
    std::string m_shortname;
    std::string m_fullname;
    std::string m_post;
    std::string m_index;
    std::string m_econtact;
};

#endif  //! DB_CONTACTS_HPP