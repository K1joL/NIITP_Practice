#ifndef DB_DOCINFOS_HPP
#define DB_DOCINFOS_HPP

#include <boost/date_time.hpp>
#include <odb/core.hxx>
#include <string>
#include <vector>

#pragma db object table("docinfos")
class DocInfo {
public:
    DocInfo() {
    }
    DocInfo(std::string messageId, boost::gregorian::date time, unsigned long senderId,
            unsigned long recepientId, unsigned long authorId, unsigned documentId)
        : m_messageid(messageId),
          m_time(time),
          m_senderid(senderId),
          m_recepientid(recepientId),
          m_authorid(authorId),
          m_documentid(documentId) {
    }

    unsigned long getId() const {
        return m_docinfoid;
    }
    unsigned long getSenderId() const {
        return m_senderid;
    }
    unsigned long getRecepientId() const {
        return m_recepientid;
    }
    unsigned long getAuthorId() const {
        return m_authorid;
    }
    unsigned long getDocumentId() const {
        return m_documentid;
    }
    std::string getMessageId() const {
        return m_messageid;
    }
    boost::gregorian::date getTime() const {
        return m_time;
    }

    std::vector<std::string> getData() const {
        std::vector<std::string> v;
        v.push_back(m_messageid);
        v.push_back(boost::gregorian::to_iso_extended_string(m_time));
        v.push_back(std::to_string(m_senderid));
        v.push_back(std::to_string(m_recepientid));
        v.push_back(std::to_string(m_authorid));
        v.push_back(std::to_string(m_documentid));
        return v;
    }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long m_docinfoid;

    std::string m_messageid;
    boost::gregorian::date m_time;
    unsigned long m_senderid;
    unsigned long m_recepientid;
    unsigned long m_authorid;
    unsigned long m_documentid;
};

#endif  //! DB_DOCINFOS_HPP