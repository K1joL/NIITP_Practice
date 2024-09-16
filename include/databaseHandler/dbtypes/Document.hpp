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
    Document(std::string number, boost::gregorian::date date, std::string consists,
             std::string annotation, std::string rubric)
        : m_number(number),
          m_date(date),
          m_consists(consists),
          m_annotation(annotation),
          m_rubric(rubric) {
    }

    unsigned long getId() const {
        return m_documentid;
    }

    std::string getNumber() const {
        return m_number;
    }
    void setNumber(const std::string &number) {
        m_number = number;
    }

    boost::gregorian::date getDate() const {
        return m_date;
    }
    void setDate(const boost::gregorian::date &date) {
        m_date = date;
    }
    void setDate(unsigned year, unsigned month, unsigned day) {
        m_date = boost::gregorian::date(year, month, day);
    }

    std::string getConsists() const {
        return m_consists;
    }
    void setConsists(const std::string &consists) {
        m_consists = consists;
    }

    std::string getAnnotation() const {
        return m_annotation;
    }
    void setAnnotation(const std::string &annotation) {
        m_annotation = annotation;
    }

    std::string getRubric() const {
        return m_rubric;
    }
    void setRubric(const std::string &rubric) {
        m_rubric = rubric;
    }

    std::vector<std::string> getData() const{
        std::vector<std::string> v;
        v.push_back(std::to_string(m_documentid));
        v.push_back(m_number);
        v.push_back(boost::gregorian::to_iso_extended_string(m_date));
        v.push_back(m_consists);
        v.push_back(m_annotation);
        v.push_back(m_rubric);
        return v;
    }
private:
    friend class odb::access;

#pragma db id auto
    unsigned long m_documentid;

    std::string m_number;
    boost::gregorian::date m_date;
    std::string m_consists;
    std::string m_annotation;
    std::string m_rubric;
};

#endif  //! DB_DOCUMENT_HPP