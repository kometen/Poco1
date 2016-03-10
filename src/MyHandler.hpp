/*
 * ParseSAX.h
 *
 *  Created on: 08/03/2016
 *      Author: claus
 */

#ifndef MYHANDLER_HPP_
#define MYHANDLER_HPP_

#include <iostream>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/ContentHandler.h>
#include <Poco/SAX/SAXParser.h>

class MyHandler: public Poco::XML::ContentHandler {
public:
    MyHandler();
    virtual ~MyHandler();

    // ContentHandler overrides, begin.
    void setDocumentLocator(const Poco::XML::Locator* loc);
    void startDocument();
    void endDocument();
    void startElement(
            const Poco::XML::XMLString&,
            const Poco::XML::XMLString&,
            const Poco::XML::XMLString&,
            const Poco::XML::Attributes&);
    void endElement(
            const Poco::XML::XMLString&,
            const Poco::XML::XMLString&,
            const Poco::XML::XMLString&);
    void characters(const Poco::XML::XMLChar ch[], int, int);
    void ignorableWhitespace(const Poco::XML::XMLChar ch[], int, int);
    void processingInstruction(const Poco::XML::XMLString&, const Poco::XML::XMLString&);
    void startPrefixMapping(const Poco::XML::XMLString&, const Poco::XML::XMLString&);
    void endPrefixMapping(const Poco::XML::XMLString&);
    void skippedEntity(const Poco::XML::XMLString&);
    // ContentHandler overrides, end

};

#endif /* MYHANDLER_HPP_ */