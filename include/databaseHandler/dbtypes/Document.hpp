#ifndef DB_DOCUMENT_HPP
#define DB_DOCUMENT_HPP

#include <boost/date_time.hpp>
#include <odb/core.hxx>
#include <string>
#include <vector>

// Document Class
#pragma db object table("documents")
class Document {
public:
    Document() {
    }
    Document(unsigned long docInfoId, std::string number, boost::gregorian::date date, std::string consists,
             std::string annotation, std::string rubric, unsigned long authorId)
        : m_docinfoid(docInfoId),
          m_number(number),
          m_date(date),
          m_consists(consists),
          m_annotation(annotation),
          m_rubric(rubric),
          m_authorid(authorId) {
    }
    void setDocinfoId(unsigned long docInfoId) {
        m_docinfoid = docInfoId;
    }

    unsigned long getDocinfoId() const {
        return m_docinfoid;
    }

    void setNumber(const std::string& number) {
        m_number = number;
    }

    std::string getNumber() const {
        return m_number;
    }

    void setDate(const boost::gregorian::date& date) {
        m_date = date;
    }

    boost::gregorian::date getDate() const {
        return m_date;
    }

    void setConsists(const std::string& consists) {
        m_consists = consists;
    }

    std::string getConsists() const {
        return m_consists;
    }

    void setAnnotation(const std::string& annotation) {
        m_annotation = annotation;
    }

    std::string getAnnotation() const {
        return m_annotation;
    }

    void setRubric(const std::string& rubric) {
        m_rubric = rubric;
    }

    std::string getRubric() const {
        return m_rubric;
    }

    void setAuthorId(unsigned long authorId) {
        m_authorid = authorId;
    }

    unsigned long getAuthorId() const {
        return m_authorid;
    }

    unsigned long getId() const {
        return m_documentid;
    }

    std::vector<std::string> getData() const {
        std::vector<std::string> v;
        v.push_back(std::to_string(m_docinfoid));
        v.push_back(std::to_string(m_documentid));
        v.push_back(m_number);
        v.push_back(boost::gregorian::to_iso_extended_string(m_date));
        v.push_back(m_consists);
        v.push_back(m_annotation);
        v.push_back(m_rubric);
        v.push_back(std::to_string(m_authorid));
        return v;
    }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long m_documentid;

    unsigned long m_docinfoid;
    std::string m_number;
    boost::gregorian::date m_date;
    std::string m_consists;
    std::string m_annotation;
    std::string m_rubric;
    unsigned long m_authorid;
};

#endif  //! DB_DOCUMENT_HPP