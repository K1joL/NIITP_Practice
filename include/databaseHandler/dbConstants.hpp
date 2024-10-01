#ifndef DB_CONSTANTS_HPP
#define DB_CONSTANTS_HPP

#include <string>
#include <vector>
// template keys
namespace tmplkey {
using std::string;
using std::vector;
const vector<string> DOCINFO = {"messageid", "time", "messagetype", "sender", "recepient", "documentlist"};
const vector<string> CONTACT = {"organization", "shortname", "fullname", "post", "index", "econtact"};
const vector<string> DOCUMENT = {"docinfoid",  "documentid", "number", "date",      "consists",
                                 "annotation", "rubric",     "author", "addressee", "parts"};
const vector<string> PART = {"name", "type", "hashsum", "fields"};
const vector<string> FIELD = {"name", "value"};

}  // namespace tmplkey

#endif  //! DB_CONSTANTS_HPP