#ifndef DB_CONSTANTS_HPP
#define DB_CONSTANTS_HPP

#include <string>
#include <vector>
// template keys
namespace tmplkey {
using std::string;
using std::vector;
const vector<string> DOCINFO = {"header", "sender",    "recepient", "documentlist",
                          "author", "addressee", "parts"};
const vector<string> HEADER = {"messageid", "time"};
const vector<string> CONTACT = {"organization", "shortname", "fullname", "post", "index", "econtact"};
const vector<string> DOCUMENTLIST = {"documentid", "number", "date", "consists", "annotation", "rubric"};
const vector<string> PART = {"name", "type", "hashsum", "fields"};
const vector<string> FIELD = {"name", "value"};

}  // namespace tmplkey

#endif  //! DB_CONSTANTS_HPP