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
    DocInfo(std::string messageId, boost::gregorian::date time, std::string messageType,
            unsigned long senderId, unsigned long recepientId)
        : m_messageid(messageId),
          m_time(time),
          m_messagetype(messageType),
          m_senderid(senderId),
          m_recepientid(recepientId) {
    }
    void setMessageId(const std::string& messageId) {
        m_messageid = messageId;
    }
    void setTime(const boost::gregorian::date& time) {
        m_time = time;
    }
    void setMessageType(const std::string& messageType) {
        m_messagetype = messageType;
    }
    void setSenderId(unsigned long senderId) {
        m_senderid = senderId;
    }
    void setRecipientId(unsigned long recepientId) {
        m_recepientid = recepientId;
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
    std::string getMessageId() const {
        return m_messageid;
    }
    std::string getMessageType() const {
        return m_messagetype;
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
        return v;
    }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long m_docinfoid;

    std::string m_messageid;
    boost::gregorian::date m_time;
    std::string m_messagetype;
    unsigned long m_senderid;
    unsigned long m_recepientid;
};

#endif  //! DB_DOCINFOS_HPP