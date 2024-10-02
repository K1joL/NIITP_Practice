#ifndef DATABASE_HANDLER_HPP
#define DATABASE_HANDLER_HPP
// Third-party dependencies
#include <jinja2cpp/template.h>

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

// ODB
#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>
// DB TYPES (Tables)
#include "dbTypes.hpp"
// Intra-project dependencies
#include "dbConstants.hpp"

/*  TO DO:
    Make many-to-many bindings for parts
*/

namespace dbHandler {
using namespace odb::core;

jinja2::ValuesMap getValuesMapFromDocument(unsigned long docInfoId, const std::string &dbUser = "postgres",
                                           const std::string &dbPass = "postgres",
                                           const std::string &dbName = "transferdb");
std::shared_ptr<Document> selectDocument(const std::shared_ptr<database> db, const std::string &docNumber);
std::vector<std::shared_ptr<Document>> selectDocuments(const std::shared_ptr<database> db,
                                                       unsigned long docInfoId);
std::shared_ptr<DocInfo> selectDocInfo(const std::shared_ptr<database> db, unsigned long docInfoId);
std::shared_ptr<Contact> selectContact(const std::shared_ptr<database> db, unsigned long contactId);
std::shared_ptr<Contact> selectContact(const std::shared_ptr<database> db, const std::string &fullname,
                                       const std::string &post, const std::string &index);
std::vector<std::shared_ptr<Contact>> selectAddressees(const std::shared_ptr<database> db,
                                                       unsigned long documentId);
std::vector<std::shared_ptr<Part>> selectParts(const std::shared_ptr<database> db, unsigned long documentId);
std::shared_ptr<Part> selectPart(const std::shared_ptr<database> db, const std::string &hash);
std::vector<std::shared_ptr<Field>> selectFields(const std::shared_ptr<database> db, unsigned long partId);
std::shared_ptr<Field> selectField(const std::shared_ptr<database> db, const std::string &name = "",
                                                const std::string &value ="", unsigned long partId = ulong(-1));

jinja2::ValuesList getPartsFromDocument(const std::string &docNumber,
                                        const jinja2::ValuesList &partsVect = jinja2::ValuesList(),
                                        const std::string &dbUser = "postgres",
                                        const std::string &dbPass = "postgres",
                                        const std::string &dbName = "transferdb");
// Return id of tagPart or ulong(-1) if not found
unsigned long checkExistence(const std::shared_ptr<database> db, const jinja2::ValuesMap &tagPart);
unsigned long checkExistence(const jinja2::ValuesMap &tagPart, const std::string &dbUser = "postgres",
                             const std::string &dbPass = "postgres",
                             const std::string &dbName = "transferdb");

bool updateContact(const std::shared_ptr<database> db, const jinja2::ValuesMap &contactMap,
                   unsigned long contactId = ulong(-1));
bool updateContact(const jinja2::ValuesMap &contactMap, const std::string &dbUser = "postgres",
                   const std::string &dbPass = "postgres", const std::string &dbName = "transferdb");

bool updatePart(const std::shared_ptr<database> db, const jinja2::ValuesMap &tagPartMap,
                unsigned long partId = ulong(-1));
bool updatePart(const jinja2::ValuesMap &tagPartMap, unsigned long partId = ulong(-1),
                const std::string &dbUser = "postgres", const std::string &dbPass = "postgres",
                const std::string &dbName = "transferdb");

bool updateDocument(const std::shared_ptr<database> db, const jinja2::ValuesMap &docMap,
                    unsigned long docId = ulong(-1));

unsigned long insertContact(const std::shared_ptr<database> db, const jinja2::ValuesMap &contactMap);
unsigned long insertAddressee(const std::shared_ptr<database> db, unsigned long contactId = ulong(-1),
                              unsigned long docId = ulong(-1));
unsigned long insertPart(const std::shared_ptr<database> db, const jinja2::ValuesMap &tagPartMap,
                         unsigned long docId = ulong(-1));
}  // namespace dbHandler
#endif  // !DATABASE_HANDLER_HPP