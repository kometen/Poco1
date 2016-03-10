/*
 * ParseSAX.cpp
 *
 *  Created on: 08/03/2016
 *      Author: claus
 */

#include "MyHandler.hpp"

MyHandler::MyHandler() {}
MyHandler::~MyHandler() {}

void MyHandler::setDocumentLocator(const Poco::XML::Locator* loc) {
}

void MyHandler::startDocument() {
}

void MyHandler::endDocument() {
}

void MyHandler::startElement(const Poco::XML::XMLString& namespaceURI, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname, const Poco::XML::Attributes& attributes) {
    int x {0};
    std::cout << "qname: " << qname << std::endl;
    std::cout << "getValue(): " << attributes.getValue(qname) << std::endl;
    std::cout << "getLength(): " << attributes.getLength() << std::endl;
    if (attributes.getLength()) {
        x = attributes.getLength();
        for (int i = 0; i < x; ++i) {
            std::cout << "getQName(): " << attributes.getQName(i) << ", getValue(): " << attributes.getValue(i) << std::endl;
        }
    }
}

void MyHandler::endElement(const Poco::XML::XMLString& allocator,
        const Poco::XML::XMLString& allocator1,
        const Poco::XML::XMLString& allocator2) {
}

void MyHandler::characters(const Poco::XML::XMLChar ch[], int start, int length) {
    std::cout << std::string(ch + start, length) << std::endl;
}

void MyHandler::ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int length) {
}

void MyHandler::processingInstruction(const Poco::XML::XMLString& allocator, const Poco::XML::XMLString& allocator1) {
}

void MyHandler::startPrefixMapping(const Poco::XML::XMLString& allocator, const Poco::XML::XMLString& allocator1) {
}

void MyHandler::endPrefixMapping(const Poco::XML::XMLString& allocator) {
}

void MyHandler::skippedEntity(const Poco::XML::XMLString& allocator) {
}
