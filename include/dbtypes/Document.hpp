#ifndef DB_DOCUMENT_HPP
#define DB_DOCUMENT_HPP

#include <boost/date_time.hpp>
#include <odb/core.hxx>
#include <string>
#include <vector>

// Document Class
class Document {
public:
    Document() {}
    Document(int document_id, int message_id, long long id, long long access_hash, const std::vector<char>& file_reference,
             const boost::gregorian::date& date, const std::string& mime_type, long long size, int dc_id, int duration,
             int w, int h, bool round_message, bool supports_streaming)
        : document_id(document_id), message_id(message_id), id(id), 
        access_hash(access_hash), file_reference(file_reference), 
        date(date), mime_type(mime_type), size(size), dc_id(dc_id), 
        duration(duration), w(w), h(h), round_message(round_message), 
        supports_streaming(supports_streaming) {}

private:
    friend class odb::access;
    int document_id;
    int message_id;
    long long id;
    long long access_hash;
    std::vector<char> file_reference;
    boost::gregorian::date date;
    std::string mime_type;
    long long size;
    int dc_id;
    int duration;
    int w;
    int h;
    bool round_message;
    bool supports_streaming;
};

#pragma db object(Document)
#pragma db member(Document::id) id

#endif  //! DB_DOCUMENT_HPP