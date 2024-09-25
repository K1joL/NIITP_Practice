#include "databaseHandler.hpp"

namespace dbHandler {

jinja2::ValuesMap getValuesMapFromDocument(const std::string &docNumber, const std::string &dbUser,
                                           const std::string &dbPass, const std::string &dbName) {
    std::shared_ptr<database> db(new odb::pgsql::database(dbUser, dbPass, dbName));
    jinja2::ValuesMap mDocinfo;
    jinja2::ValuesMap mHeader;
    jinja2::ValuesMap mSender;
    jinja2::ValuesMap mRecepient;
    jinja2::ValuesMap mDocumentlist;
    jinja2::ValuesMap mAuthor;
    jinja2::ValuesMap mAddressee;
    jinja2::ValuesList mParts;
    jinja2::ValuesMap data;
    // select document
    std::shared_ptr<Document> doc(selectDocument(db, docNumber));
    // select docinfo
    std::shared_ptr<DocInfo> docinfo(selectDocInfo(db, doc->getId()));
    // select contacts
    std::shared_ptr<Contact> sender(selectContact(db, docinfo->getSenderId()));
    std::shared_ptr<Contact> recepient(selectContact(db, docinfo->getRecepientId()));
    std::shared_ptr<Contact> author(selectContact(db, docinfo->getAuthorId()));
    std::vector<std::shared_ptr<Contact>> addressee = selectAddressees(db, docinfo->getId());
    // select parts
    std::vector<std::shared_ptr<Part>> parts(selectParts(db, docinfo->getId()));
    // fill the maps
    // header
    std::vector<std::string> headerVect = docinfo->getData();
    for (int i = 0; i < tmplkey::HEADER.size(); ++i) {
        mHeader[tmplkey::HEADER[i]] = headerVect[i];
    }
    std::vector<std::string> senderVect = sender->getData();
    for (int i = 0; i < tmplkey::CONTACT.size(); ++i) {
        mSender[tmplkey::CONTACT[i]] = senderVect[i];
    }
    std::vector<std::string> recepientVect = recepient->getData();
    for (int i = 0; i < tmplkey::CONTACT.size(); ++i) {
        mRecepient[tmplkey::CONTACT[i]] = recepientVect[i];
    }
    // documentList
    std::vector<std::string> docVect = doc->getData();
    for (int i = 0; i < tmplkey::DOCUMENTLIST.size(); ++i) {
        mDocumentlist[tmplkey::DOCUMENTLIST[i]] = docVect[i];
    }
    std::vector<std::string> authorVect = author->getData();
    for (int i = 0; i < tmplkey::CONTACT.size(); ++i) {
        mAuthor[tmplkey::CONTACT[i]] = authorVect[i];
    }
    std::vector<std::string> addresseeVect = addressee[0]->getData();
    for (int i = 0; i < tmplkey::CONTACT.size(); ++i) {
        mAddressee[tmplkey::CONTACT[i]] = addresseeVect[i];
    }
    for (auto &part : parts) {
        jinja2::ValuesMap tempPart;
        std::vector<std::string> partData = part->getData();
        for (int i = 0; i < tmplkey::PART.size() - 1; ++i)
            tempPart[tmplkey::PART[i]] = partData[i];
        mParts.emplace_back(tempPart);
    }

    mDocinfo["header"] = mHeader;
    mDocinfo["sender"] = mSender;
    mDocinfo["recepient"] = mRecepient;
    mDocinfo["documentlist"] = mDocumentlist;
    mDocinfo["author"] = mAuthor;
    mDocinfo["addressee"] = mAddressee;
    mDocinfo["parts"] = mParts;

    return std::move(mDocinfo);
}

std::shared_ptr<Document> selectDocument(const std::shared_ptr<database> db, const std::string &docNumber) {
    typedef odb::query<Document> query;
    typedef odb::result<Document> result;
    transaction t(db->begin());
    std::shared_ptr<Document> doc = db->query_one<Document>(query::number == docNumber);
    t.commit();
    return doc;
}

std::shared_ptr<DocInfo> selectDocInfo(const std::shared_ptr<database> db, unsigned long documentId) {
    typedef odb::query<DocInfo> query;
    typedef odb::result<DocInfo> result;
    transaction t(db->begin());
    std::shared_ptr<DocInfo> docinfo = db->query_one<DocInfo>(query::documentid == documentId);
    t.commit();
    return docinfo;
}

std::shared_ptr<Contact> selectContact(const std::shared_ptr<database> db, unsigned long contactId) {
    typedef odb::query<Contact> query;
    typedef odb::result<Contact> result;
    transaction t(db->begin());
    std::shared_ptr<Contact> contact = db->query_one<Contact>(query::contactid == contactId);
    t.commit();

    return contact;
}

std::shared_ptr<Contact> selectContact(const std::shared_ptr<database> db, unsigned long organization,
                                       const std::string &shortname, const std::string &fullname,
                                       const std::string &post, const std::string &index,
                                       const std::string &econtact) {
    typedef odb::query<Contact> query;
    typedef odb::result<Contact> result;
    transaction t(db->begin());
    std::shared_ptr<Contact> contact = db->query_one<Contact>(
        query::organization == organization && query::shortname == shortname && query::fullname == fullname &&
        query::post == post && query::index == index && query::econtact == econtact);
    t.commit();

    return contact;
}

std::vector<std::shared_ptr<Contact>> selectAddressees(const std::shared_ptr<database> db,
                                                       unsigned long docInfoId) {
    typedef odb::query<Addressee> query;
    typedef odb::result<Addressee> result;
    std::vector<std::shared_ptr<Contact>> addressees;
    transaction t(db->begin());
    std::vector<unsigned long> contactIds;
    result resAddressees = db->query<Addressee>(query::docinfoid == docInfoId);
    for (result::iterator it = resAddressees.begin(); it != resAddressees.end(); ++it)
        contactIds.push_back(it->getContactId());
    t.commit();
    for (auto &id : contactIds)
        addressees.push_back(selectContact(db, id));
    return std::move(addressees);
}

std::vector<std::shared_ptr<Part>> selectParts(const std::shared_ptr<database> db, unsigned long docInfoId) {
    typedef odb::query<Part> query;
    typedef odb::result<Part> result;
    std::vector<std::shared_ptr<Part>> parts;
    transaction t(db->begin());
    result resParts = db->query<Part>(query::docinfoid == docInfoId);
    if (!resParts.empty())
        for (result::iterator it = resParts.begin(); it != resParts.end(); ++it) {
            // transform iterator to Part class
            parts.push_back(std::make_shared<Part>(*it));
        }
    t.commit();
    return std::move(parts);
}

std::vector<std::shared_ptr<Field>> selectFields(const std::shared_ptr<database> db, unsigned long partId) {
    typedef odb::query<Field> query;
    typedef odb::result<Field> result;
    std::vector<std::shared_ptr<Field>> fields;
    transaction t(db->begin());
    result resFields = db->query<Field>(query::partid == partId);
    if (!resFields.empty())
        for (result::iterator it = resFields.begin(); it != resFields.end(); ++it) {
            // transform iterator to Part class
            fields.push_back(std::make_shared<Field>(*it));
        }
    t.commit();
    return std::move(fields);
}

std::shared_ptr<Part> selectPart(const std::shared_ptr<database> db, const std::string &hash) {
    typedef odb::query<Part> query;
    typedef odb::result<Part> result;
    std::shared_ptr<Part> part;
    transaction t(db->begin());
    part = db->query_one<Part>(query::hash == hash);
    t.commit();
    return part;
}

jinja2::ValuesList getPartsFromDocument(const std::string &docNumber, const jinja2::ValuesList &partsList,
                                        const std::string &dbUser, const std::string &dbPass,
                                        const std::string &dbName) {
    std::shared_ptr<database> db(new odb::pgsql::database(dbUser, dbPass, dbName));
    std::shared_ptr<Document> doc;
    std::shared_ptr<DocInfo> docinfo;
    jinja2::ValuesList mParts;
    if (partsList.empty()) {
        // select document
        doc = selectDocument(db, docNumber);
        // select docinfo
        docinfo = selectDocInfo(db, doc->getId());
        // select parts
        for (auto &part : selectParts(db, docinfo->getId())) {
            jinja2::ValuesMap tempPart;
            std::vector<std::string> partData = part->getData();
            for (int i = 0; i < tmplkey::PART.size() - 1; ++i)
                tempPart[tmplkey::PART[i]] = partData[i];
            mParts.emplace_back(std::move(tempPart));
        }
    } else
        mParts = partsList;

    for (auto &part : mParts) {
        jinja2::ValuesList fields;
        std::shared_ptr<Part> dbPart = selectPart(db, part.asMap().at(tmplkey::PART[2]).asString());
        // select fields
        for (auto &field : selectFields(db, dbPart->getPartId())) {
            // filling the map
            jinja2::ValuesMap mField;
            for (int i = 0; i < tmplkey::FIELD.size(); ++i)
                mField[tmplkey::FIELD[i]] = field->getData()[i];
            fields.push_back(std::move(mField));
        }
        part.asMap().emplace(tmplkey::PART.back(), fields);
    }

    return mParts;
}

unsigned long checkExistence(const std::shared_ptr<database> db, const jinja2::ValuesMap &part) {
    if (part.empty())
        return -1;

    // if part is contact
    auto it = part.find(tmplkey::CONTACT[0]);
    if (it != part.end()) {
        auto ptrContact = selectContact(
            db, stoi(part.at(tmplkey::CONTACT[0]).asString()), part.at(tmplkey::CONTACT[1]).asString(),
            part.at(tmplkey::CONTACT[2]).asString(), part.at(tmplkey::CONTACT[3]).asString(),
            part.at(tmplkey::CONTACT[4]).asString(), part.at(tmplkey::CONTACT[5]).asString());
        if (ptrContact != nullptr)
            return ptrContact->getId();
    }
    // if part is Document
    it = part.find(tmplkey::DOCUMENTLIST[1]);
    if (it != part.end()) {
        auto ptrDocument = selectDocument(db, it->second.asString());
        if (ptrDocument != nullptr)
            return ptrDocument->getId();
    }
    // if part is Additionalfield
    it = part.find(tmplkey::PART[2]);
    if (it != part.end()) {
        auto ptrPart = selectPart(db, it->second.asString());
        if (ptrPart != nullptr)
            return ptrPart->getPartId();
    }

    return -1;
}

unsigned long checkExistence(const jinja2::ValuesMap &part, const std::string &dbUser,
                             const std::string &dbPass, const std::string &dbName) {
    std::shared_ptr<database> db(new odb::pgsql::database(dbUser, dbPass, dbName));
    return checkExistence(db, part);
}

}  // namespace dbHandler