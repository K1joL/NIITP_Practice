#include "databaseHandler.hpp"

namespace dbHandler {

jinja2::ValuesMap getValuesMapFromDocument(unsigned long docInfoId, const std::string &dbUser,
                                           const std::string &dbPass, const std::string &dbName) {
    std::shared_ptr<database> db(new odb::pgsql::database(dbUser, dbPass, dbName));
    jinja2::ValuesMap mDocinfo;
    jinja2::ValuesMap mHeader;
    jinja2::ValuesMap mSender;
    jinja2::ValuesMap mRecepient;
    jinja2::ValuesList mDocumentlist;
    jinja2::ValuesMap data;
    // select docinfo
    std::shared_ptr<DocInfo> docinfo(selectDocInfo(db, docInfoId));
    // select document
    std::vector<std::shared_ptr<Document>> docs(selectDocuments(db, docInfoId));
    // select contacts
    std::shared_ptr<Contact> sender(selectContact(db, docinfo->getSenderId()));
    std::shared_ptr<Contact> recepient(selectContact(db, docinfo->getRecepientId()));

    // select parts
    std::vector<std::shared_ptr<Part>> parts(selectParts(db, docInfoId));
    // fill the maps
    // documentList
    for (auto &doc : docs) {
        jinja2::ValuesMap mAuthor;
        jinja2::ValuesMap mAddressee;
        jinja2::ValuesList mParts;
        jinja2::ValuesMap tempDocument;
        std::shared_ptr<Contact> author(selectContact(db, doc->getId()));
        std::vector<std::shared_ptr<Contact>> addressee(selectAddressees(db, doc->getId()));
        std::vector<std::string> docVect = doc->getData();
        std::vector<std::string> authorVect = author->getData();
        for (int i = 0; i < tmplkey::CONTACT.size(); ++i)
            mAuthor[tmplkey::CONTACT[i]] = authorVect[i];

        std::vector<std::string> addresseeVect = addressee[0]->getData();
        for (int i = 0; i < tmplkey::CONTACT.size(); ++i)
            mAddressee[tmplkey::CONTACT[i]] = addresseeVect[i];

        for (auto &part : parts) {
            jinja2::ValuesMap tempPart;
            std::vector<std::string> partData = part->getData();
            for (int i = 0; i < tmplkey::PART.size() - 1; ++i)
                tempPart[tmplkey::PART[i]] = partData[i];
            mParts.emplace_back(tempPart);
        }
        // filling document with previous parts
        int i;
        for (i = 1; i < tmplkey::DOCUMENT.size() - 3; ++i)
            tempDocument[tmplkey::DOCUMENT[i]] = docVect[i];
        tempDocument[tmplkey::DOCUMENT[i++]] = std::move(mAuthor);
        tempDocument[tmplkey::DOCUMENT[i++]] = std::move(mAddressee);
        tempDocument[tmplkey::DOCUMENT[i++]] = std::move(mParts);
        mDocumentlist.push_back(std::move(tempDocument));
    }
    // filling docinfo
    // fill "messageid",    "time",   "messagetype"
    std::vector<std::string> docInfoVect = docinfo->getData();
    for (int i = 0; i < 3; ++i) {
        mDocinfo[tmplkey::DOCINFO[i]] = docInfoVect[i];
    }
    std::vector<std::string> senderVect = sender->getData();
    for (int i = 0; i < tmplkey::CONTACT.size(); ++i) {
        mSender[tmplkey::CONTACT[i]] = senderVect[i];
    }
    std::vector<std::string> recepientVect = recepient->getData();
    for (int i = 0; i < tmplkey::CONTACT.size(); ++i) {
        mRecepient[tmplkey::CONTACT[i]] = recepientVect[i];
    }
    mDocinfo[tmplkey::DOCINFO[3]] = std::move(mSender);
    mDocinfo[tmplkey::DOCINFO[4]] = std::move(mRecepient);
    mDocinfo[tmplkey::DOCINFO[5]] = std::move(mDocumentlist);

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

std::vector<std::shared_ptr<Document>> selectDocuments(const std::shared_ptr<database> db,
                                                       unsigned long docInfoId) {
    typedef odb::query<Document> query;
    typedef odb::result<Document> result;
    std::vector<std::shared_ptr<Document>> vect;
    transaction t(db->begin());
    result resDocs = db->query<Document>(query::docinfoid == docInfoId);
    for (result::iterator it = resDocs.begin(); it != resDocs.end(); ++it)
        vect.push_back(std::make_shared<Document>(*it));
    t.commit();
    return vect;
}

std::shared_ptr<DocInfo> selectDocInfo(const std::shared_ptr<database> db, unsigned long docInfoId) {
    typedef odb::query<DocInfo> query;
    typedef odb::result<DocInfo> result;
    transaction t(db->begin());
    std::shared_ptr<DocInfo> docinfo = db->query_one<DocInfo>(query::docinfoid == docInfoId);
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

std::shared_ptr<Contact> selectContact(const std::shared_ptr<database> db, const std::string &fullname,
                                       const std::string &post, const std::string &index) {
    typedef odb::query<Contact> query;
    typedef odb::result<Contact> result;
    transaction t(db->begin());
    std::shared_ptr<Contact> contact =
        db->query_one<Contact>(query::fullname == fullname && query::post == post && query::index == index);
    t.commit();

    return contact;
}

std::vector<std::shared_ptr<Contact>> selectAddressees(const std::shared_ptr<database> db,
                                                       unsigned long documentId) {
    typedef odb::query<Addressee> query;
    typedef odb::result<Addressee> result;
    std::vector<std::shared_ptr<Contact>> addressees;
    transaction t(db->begin());
    std::vector<unsigned long> contactIds;
    result resAddressees = db->query<Addressee>(query::documentid == documentId);
    for (result::iterator it = resAddressees.begin(); it != resAddressees.end(); ++it)
        contactIds.push_back(it->getContactId());
    t.commit();
    for (auto &id : contactIds)
        addressees.push_back(selectContact(db, id));
    return std::move(addressees);
}

std::vector<std::shared_ptr<Part>> selectParts(const std::shared_ptr<database> db, unsigned long documentId) {
    typedef odb::query<Part> query;
    typedef odb::result<Part> result;
    std::vector<std::shared_ptr<Part>> parts;
    transaction t(db->begin());
    result resParts = db->query<Part>(query::documentid == documentId);
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

std::shared_ptr<Field> selectField(const std::shared_ptr<database> db, const std::string &name,
                                   const std::string &value, unsigned long partId) {
    typedef odb::query<Field> query;
    std::shared_ptr<Field> field;
    query q;
    bool isQueryAppended = false;
    if (name != "") {
        q.append("name = \'" + name + '\'');
        isQueryAppended = true;
    }
    if (value != "") {
        if (isQueryAppended)
            q.append(" AND ");
        q.append("value = \'" + value + '\'');
        isQueryAppended = true;
    }
    if (partId != ulong(-1)) {
        if (isQueryAppended)
            q.append(" AND ");
        q.append("partid = \'" + partId + '\'');
    }
    transaction t(db->begin());
    field = db->query_one<Field>(q);
    t.commit();
    return std::move(field);
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

/*  TO DO:
    Remove using of partsList vector
    Maybe make new function for this purpose
*/
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
        if (doc == nullptr) {
            std::cerr << "Document with number " << docNumber << " not found!" << std::endl;
            return mParts;
        }
        // select docinfo
        docinfo = selectDocInfo(db, doc->getId());
        if (docinfo == nullptr) {
            std::cerr << "DocInfo with document number " << doc->getId() << " not found!" << std::endl;
            return mParts;
        }
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
        for (auto &field : selectFields(db, dbPart->getId())) {
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

unsigned long checkExistence(const std::shared_ptr<database> db, const jinja2::ValuesMap &tagPart) {
    if (tagPart.empty())
        return -1;

    // if tag is Contact
    auto it = tagPart.find(tmplkey::CONTACT[2]);
    if (it != tagPart.end()) {
        auto ptrContact = selectContact(db, tagPart.at(tmplkey::CONTACT[2]).asString(),
                                        tagPart.at(tmplkey::CONTACT[3]).asString(),
                                        tagPart.at(tmplkey::CONTACT[4]).asString());
        if (ptrContact != nullptr)
            return ptrContact->getId();
    }
    // if tag is Document
    it = tagPart.find(tmplkey::DOCUMENT[1]);
    if (it != tagPart.end()) {
        auto ptrDocument = selectDocument(db, it->second.asString());
        if (ptrDocument != nullptr)
            return ptrDocument->getId();
    }
    // if tag is Additionalfield
    it = tagPart.find(tmplkey::PART[2]);
    if (it != tagPart.end()) {
        auto ptrPart = selectPart(db, it->second.asString());
        if (ptrPart != nullptr)
            return ptrPart->getId();
    }
    // if tag is Value
    it = tagPart.find(tmplkey::FIELD[0]);
    if (it != tagPart.end()) {
        auto it2 = tagPart.find(tmplkey::FIELD[1]);
        auto ptrField =
            selectField(db, it->second.asString(), it2 != tagPart.end() ? it2->second.asString() : "");
        if (ptrField != nullptr)
            return ptrField->getId();
    }

    return -1;
}

unsigned long checkExistence(const jinja2::ValuesMap &tagPart, const std::string &dbUser,
                             const std::string &dbPass, const std::string &dbName) {
    std::shared_ptr<database> db(new odb::pgsql::database(dbUser, dbPass, dbName));
    return checkExistence(db, tagPart);
}

bool updateContact(const std::shared_ptr<database> db, const jinja2::ValuesMap &contactMap,
                   unsigned long contactId) {
    if (contactId != ulong(-1))
        return false;
    std::shared_ptr<Contact> ptrContact;
    {
        transaction t(db->begin());
        ptrContact = db->load<Contact>(contactId);
        t.commit();
    }
    if (ptrContact == nullptr) {
        std::cerr << "Contact with id " << contactId << " doesn`t exist!" << std::endl;
        return false;
    }
    for (int i = 0; i < tmplkey::CONTACT.size(); ++i) {
        auto it = contactMap.find(tmplkey::CONTACT[i]);
        if (it == contactMap.end()) {
            std::cerr << tmplkey::CONTACT[i] << " tag not found in docMap! It will be skipped!" << std::endl;
            continue;
        }
        std::function<void(const jinja2::Value &)> functionWrapper = nullptr;
        switch (i) {
            case 0:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setOrganization(stoi(x.asString()));
                };
                break;
            case 1:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setShortName(x.asString());
                };
                break;
            case 2:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setFullName(x.asString());
                };
                break;
            case 3:
                functionWrapper = [ptrContact](const jinja2::Value &x) { ptrContact->setPost(x.asString()); };
                break;
            case 4:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setIndex(x.asString());
                };
                break;
            case 5:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setEcontact(x.asString());
                };
                break;
            default:
                std::cerr << "Unhandled contact key: " << i << std::endl;
                break;
        }
        if (functionWrapper != nullptr)
            functionWrapper(it->second);
    }
    unsigned long id = ulong(-1);
    {
        transaction t(db->begin());
        id = db->persist(ptrContact);
        t.commit();
    }
    return id != ulong(-1);
}

bool updateContact(const jinja2::ValuesMap &contactMap, const std::string &dbUser, const std::string &dbPass,
                   const std::string &dbName) {
    std::shared_ptr<database> db(new odb::pgsql::database(dbUser, dbPass, dbName));
    return updateContact(db, contactMap);
}

bool updatePart(const std::shared_ptr<database> db, const jinja2::ValuesMap &tagPartMap,
                unsigned long partId) {
    if (partId == ulong(-1))
        return false;
    std::shared_ptr<Part> ptrPart;
    {
        transaction t(db->begin());
        ptrPart = db->load<Part>(partId);
        t.commit();
    }
    if (ptrPart == nullptr) {
        std::cerr << "Part with id " << partId << " doesn`t exist!" << std::endl;
        return false;
    }
    auto FULL_PART_KEYS = tmplkey::PART;
    FULL_PART_KEYS.push_back("docinfoid");
    std::vector<std::shared_ptr<Field>> fields;
    for (int i = 0; i < FULL_PART_KEYS.size(); ++i) {
        auto it = tagPartMap.find(FULL_PART_KEYS[i]);
        if (it == tagPartMap.end()) {
            std::cerr << FULL_PART_KEYS[i] << " tag not found in docMap! It will be skipped!" << std::endl;
            continue;
        }
        std::function<void(const jinja2::Value &)> functionWrapper = nullptr;
        switch (i) {
            case 0:
                functionWrapper = [ptrPart](const jinja2::Value &x) { ptrPart->setName(x.asString()); };
                break;
            case 1:
                functionWrapper = [ptrPart](const jinja2::Value &x) { ptrPart->setType(x.asString()); };
                break;
            case 2:
                functionWrapper = [ptrPart](const jinja2::Value &x) { ptrPart->setHash(x.asString()); };
                break;
            case 3:
                functionWrapper = [partId, &fields, &db, ptrPart](const jinja2::Value &x) {
                    for (auto &field : x.asList()) {
                        auto mapField = field.asMap();
                        if (checkExistence(db, field.asMap()) == ulong(-1))
                            fields.push_back(
                                std::make_shared<Field>(partId, mapField.at(tmplkey::FIELD[0]).asString(),
                                                        mapField.at(tmplkey::FIELD[1]).asString()));
                    }
                };
                break;
            case 4:
                functionWrapper = [ptrPart](const jinja2::Value &x) {
                    ptrPart->setDocumentId(stoi(x.asString()));
                };
            default:
                std::cerr << "Unhandled part key: " << i << std::endl;
                break;
        }
        if (functionWrapper != nullptr)
            functionWrapper(it->second);
    }
    unsigned long id = -1;
    {
        transaction t(db->begin());
        id = db->persist(ptrPart);
        for (auto &field : fields) {
            ulong fieldId = -1;
            fieldId = db->persist(field);
            if (fieldId == ulong(-1))
                std::cerr << "Field " << field->getName() << " was not persisted!" << std::endl;
        }
        t.commit();
    }
    return id != ulong(-1);
}

bool updatePart(const jinja2::ValuesMap &tagPartMap, unsigned long partId, const std::string &dbUser,
                const std::string &dbPass, const std::string &dbName) {
    std::shared_ptr<database> db(new odb::pgsql::database(dbUser, dbPass, dbName));
    return updatePart(db, tagPartMap);
}

bool updateDocument(const std::shared_ptr<database> db, const jinja2::ValuesMap &docMap,
                    unsigned long docId) {
    if (docId != ulong(-1))
        return false;
    bool result = false;
    std::shared_ptr<Document> ptrDoc;
    {
        transaction t(db->begin());
        ptrDoc = db->load<Document>(docId);
        t.commit();
    }
    if (ptrDoc == nullptr)
        return result;
    result = true;
    for (int i = 1; i < tmplkey::DOCUMENT.size(); ++i) {
        auto it = docMap.find(tmplkey::DOCUMENT[i]);
        if (it == docMap.end()) {
            std::cerr << tmplkey::DOCUMENT[i] << " tag not found in docMap! It will be skipped!" << std::endl;
            continue;
        }
        std::function<void(const jinja2::Value &)> functionWrapper = nullptr;
        switch (i) {
            case 1:
                functionWrapper = [ptrDoc](const jinja2::Value &x) { ptrDoc->setNumber(x.asString()); };
                break;
            case 2:
                functionWrapper = [ptrDoc](const jinja2::Value &x) {
                    std::string result = x.asString();
                    result.erase(std::remove(result.begin(), result.end(), '-'), result.end());
                    ptrDoc->setDate(boost::gregorian::date_from_iso_string(result));
                };
                break;
            case 3:
                functionWrapper = [ptrDoc](const jinja2::Value &x) { ptrDoc->setConsists(x.asString()); };
                break;
            case 4:
                functionWrapper = [ptrDoc](const jinja2::Value &x) { ptrDoc->setAnnotation(x.asString()); };
                break;
            case 5:
                functionWrapper = [ptrDoc](const jinja2::Value &x) { ptrDoc->setRubric(x.asString()); };
                break;
            case 6:
                functionWrapper = [ptrDoc, &db](const jinja2::Value &x) {
                    unsigned long id = checkExistence(db, x.asMap());
                    if (id != ulong(-1))
                        if (!updateContact(db, x.asMap(), id))
                            std::cerr << "Failed to update contact! " << std::endl;
                        else
                            id = insertContact(db, x.asMap());
                    if (id != ulong(-1))
                        ptrDoc->setAuthorId(id);
                    else
                        std::cerr << "Failed to insert contact with document id: " << ptrDoc->getId()
                                  << std::endl;
                };
                break;
            case 7:
                functionWrapper = [ptrDoc, &db](const jinja2::Value &x) {
                    unsigned long id = checkExistence(db, x.asMap());
                    if (id != ulong(-1))
                        if (!updateContact(db, x.asMap(), id))
                            std::cerr << "Failed to update contact! " << std::endl;
                        else {
                            unsigned long contactId = insertContact(db, x.asMap());
                            if (contactId != ulong(-1))
                                id = insertAddressee(db, contactId, ptrDoc->getId());
                            else
                                std::cerr << "Failed to insert contact with document id: " << ptrDoc->getId()
                                          << std::endl;
                            if (id == ulong(-1))
                                std::cerr << "Failed to insert addressee!" << std::endl;
                        }
                };
                break;
            case 8:
                functionWrapper = [ptrDoc, &db](const jinja2::Value &x) {
                    for (auto &part : x.asList()) {
                        unsigned long partId = checkExistence(db, part.asMap());
                        if (partId != ulong(-1)) {
                            if (!updatePart(db, part.asMap(), partId))
                                std::cerr << "Failed to update part! " << std::endl;
                        } else {
                            unsigned long id = insertPart(db, part.asMap(), ptrDoc->getId());
                            if (id != ulong(-1))
                                std::cerr << "Failed to insert part with document id: " << ptrDoc->getId()
                                          << std::endl;
                        }
                    }
                };
                break;
            default:
                std::cerr << "Unhandled document key: " << i << std::endl;
                break;
        }
        if (functionWrapper != nullptr)
            functionWrapper(it->second);
    }
    {
        transaction t(db->begin());
        db->persist(ptrDoc);
        t.commit();
    }
    return result;
}
unsigned long insertContact(const std::shared_ptr<database> db, const jinja2::ValuesMap &contactMap) {
    std::shared_ptr<Contact> ptrContact = std::make_shared<Contact>();
    bool isFilled = false;
    for (int i = 0; i < tmplkey::CONTACT.size(); ++i) {
        auto it = contactMap.find(tmplkey::CONTACT[i]);
        if (it == contactMap.end()) {
            std::cerr << tmplkey::CONTACT[i] << " tag not found in docMap! It will be skipped!" << std::endl;
            continue;
        }
        std::function<void(const jinja2::Value &)> functionWrapper = nullptr;
        switch (i) {
            case 0:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setOrganization(stoi(x.asString()));
                };
                break;
            case 1:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setShortName(x.asString());
                };
                break;
            case 2:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setFullName(x.asString());
                };
                isFilled = true;
                break;
            case 3:
                functionWrapper = [ptrContact](const jinja2::Value &x) { ptrContact->setPost(x.asString()); };
                break;
            case 4:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setIndex(x.asString());
                };
                break;
            case 5:
                functionWrapper = [ptrContact](const jinja2::Value &x) {
                    ptrContact->setEcontact(x.asString());
                };
                isFilled = true;
                break;
            default:
                std::cerr << "Unhandled contact key: " << i << std::endl;
                break;
        }
        if (functionWrapper != nullptr)
            functionWrapper(it->second);
    }
    if (!isFilled)
        return -1;
    unsigned long id = -1;
    {
        transaction t(db->begin());
        id = db->persist(ptrContact);
        t.commit();
    }
    return id;
}

unsigned long insertAddressee(const std::shared_ptr<database> db, unsigned long contactId,
                              unsigned long docId) {
    if (docId == ulong(-1))
        return -1;
    if (contactId == ulong(-1))
        return -1;
    unsigned long id = -1;
    std::shared_ptr<Addressee> addr = std::make_shared<Addressee>(docId, contactId);
    transaction t(db->begin());
    id = db->persist(addr);
    t.commit();
    return id;
}

unsigned long insertPart(const std::shared_ptr<database> db, const jinja2::ValuesMap &tagPartMap,
                         unsigned long docId) {
    std::vector<std::shared_ptr<Field>> fields;
    std::shared_ptr<Part> ptrPart = std::make_shared<Part>();
    bool isFilled = false;
    std::vector<std::string> FULL_PART_KEYS = tmplkey::PART;
    if (docId == ulong(-1)) {
        FULL_PART_KEYS.push_back("docinfoid");
    } else
        ptrPart->setDocumentId(docId);

    for (int i = 0; i < FULL_PART_KEYS.size(); ++i) {
        auto it = tagPartMap.find(FULL_PART_KEYS[i]);
        if (it == tagPartMap.end()) {
            std::cerr << FULL_PART_KEYS[i] << " tag not found in docMap! It will be skipped!" << std::endl;
            continue;
        }
        std::function<void(const jinja2::Value &)> functionWrapper = nullptr;
        switch (i) {
            case 0:
                functionWrapper = [&ptrPart](const jinja2::Value &x) { ptrPart->setName(x.asString()); };
                break;
            case 1:
                functionWrapper = [&ptrPart](const jinja2::Value &x) { ptrPart->setType(x.asString()); };
                break;
            case 2:
                functionWrapper = [&ptrPart](const jinja2::Value &x) { ptrPart->setHash(x.asString()); };
                isFilled = true;
                break;
            case 3:
                functionWrapper = [&fields, &db, &ptrPart](const jinja2::Value &x) {
                    for (auto &field : x.asList()) {
                        auto mapField = field.asMap();
                        // set 0 to id because we dont know exactly what id a part will get
                        fields.push_back(std::make_shared<Field>(0, mapField.at(tmplkey::FIELD[0]).asString(),
                                                                 mapField.at(tmplkey::FIELD[1]).asString()));
                    }
                };
                break;
            case 4:
                functionWrapper = [&ptrPart](const jinja2::Value &x) {
                    ptrPart->setDocumentId(stoi(x.asString()));
                };
            default:
                std::cerr << "Unhandled part key: " << i << std::endl;
                break;
        }
        if (functionWrapper != nullptr)
            functionWrapper(it->second);
    }
    if (!isFilled)
        return -1;
    unsigned long id = -1;

    {
        transaction t(db->begin());
        id = db->persist(ptrPart);
        for (auto &field : fields) {
            // set id
            field->setPartId(id);
            ulong fieldId = -1;
            fieldId = db->persist(field);
            if (fieldId == ulong(-1))
                std::cerr << "Field " << field->getName() << " was not persisted!" << std::endl;
        }
        t.commit();
    }
    return id;
}

}  // namespace dbHandler