#ifndef DATABASE_HANDLER_HPP
#define DATABASE_HANDLER_HPP
// Third-party dependencies
#include <jinja2cpp/template.h>

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

namespace dbHandler {
using namespace odb::core;

int test();
jinja2::ValuesMap getValuesMapFromDocument(const std::string &docNumber,
 const std::string &dbUser = "postgres",
                        const std::string &dbPass = "postgres",
                        const std::string &dbName = "transferdb");
std::shared_ptr<Document> selectDocument(const std::shared_ptr<database> db,
                                         const std::string &docNumber);
std::shared_ptr<DocInfo> selectDocInfo(const std::shared_ptr<database> db,
                                       unsigned long documentId);
std::shared_ptr<Contact> selectContact(const std::shared_ptr<database> db, unsigned long contactId);
std::vector<std::shared_ptr<Contact>> selectAddressees(const std::shared_ptr<database> db,
                                                       unsigned long docInfoId);
std::vector<std::shared_ptr<Part>> selectParts(const std::shared_ptr<database> db,
                                               unsigned long docInfoId);
std::vector<std::shared_ptr<Field>> selectFields(const std::shared_ptr<database> db,
                                                 unsigned long partId);
}  // namespace dbHandler
#endif  // !DATABASE_HANDLER_HPP