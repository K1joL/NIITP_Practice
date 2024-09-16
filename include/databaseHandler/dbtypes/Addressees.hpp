#ifndef DB_ADDRESSEES_HPP
#define DB_ADDRESSEES_HPP

#include <odb/core.hxx>

#pragma db object table("addressees")
class Addressee{
public:
    Addressee(){}
    Addressee(unsigned long docInfoId, unsigned long contactId)
    : m_docinfoid(docInfoId),
    m_contactid(contactId)
    {}

    unsigned long getContactId()const {return m_contactid;}

private:
    friend class odb::access;

    #pragma db id auto
    unsigned long m_addresseeid;

    unsigned long m_docinfoid;
    unsigned long m_contactid;
};

#endif  //! DB_ADDRESSEES_HPP