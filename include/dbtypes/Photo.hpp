#ifndef DB_PHOTO_HPP
#define DB_PHOTO_HPP

#include <odb/core.hxx>
#include <boost/date_time.hpp>
#include <vector>

class Photo {
public:
    Photo() {}
    Photo(int photo_id, int message_id, long long id, long long access_hash, const std::vector<char>& file_reference, int sizes_id,
          const boost::gregorian::date& date, int dc_id, const std::string& type, int w, int h,
          const std::vector<long long>& sizes, int replies, int replies_pts, bool comments_, long long channel_id, long long max_id,
          long long read_max_id, long long size)
        : photo_id(photo_id), message_id(message_id), id(id), access_hash(access_hash), 
        file_reference(file_reference), sizes_id(sizes_id), date(date), dc_id(dc_id), 
        type(type), w(w), h(h), sizes(sizes), replies(replies), replies_pts(replies_pts), 
        comments_(comments_), channel_id(channel_id), max_id(max_id), read_max_id(read_max_id), 
        size(size) {}

private:
    friend class odb::access;
    int photo_id;
    int message_id;
    long long id;
    long long access_hash;
    std::vector<char> file_reference;
    int sizes_id;
    boost::gregorian::date date;
    int dc_id;
    std::string type;
    int w;
    int h;
    std::vector<long long> sizes;
    int replies;
    int replies_pts;
    bool comments_;
    long long channel_id;
    long long max_id;
    long long read_max_id;
    long long size;
};

#pragma db object(Photo)
#pragma db member(Photo::id) id


#endif  //! DB_PHOTO_HPP