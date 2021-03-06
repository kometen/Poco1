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
    // We have reached the actual weather data in the xml string.
    if (qname == "siteMeasurements") {
        weather_data = true;
    }
    // Set counter to zero. When counter reaches three the current qname value is used as key.
    if (qname == "basicData" && weather_data) {
        counter = 0;
        key = "";
    }
    if (weather_data) {
        ++counter;
    }
    if (counter == 3) {
        key = qname;
    }

    // Get id of site.
    if (qname == "measurementSiteReference") {
        if (attributes.getLength()) {
            x = attributes.getLength();
            for (int i = 0; i < x; ++i) {
                if (attributes.getQName(i) == "id") {
                    _site_id = attributes.getValue(i);
                }
            }
        }
    }

}

void MyHandler::endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname) {
    if (qname == "basicData" && weather_data) {
        counter = 0;
    }
    if (qname == "siteMeasurements") {
        // We reached the end of one site, emplace the vector in the unordered_map.
        _readings.emplace(_site_id, _reading);
        // And empty the _reading vector.
        _reading.clear();
    }
}

void MyHandler::characters(const Poco::XML::XMLChar ch[], int start, int length) {
    if (weather_data) {
//        std::cout << _id << ", " << key << ", " << std::string(ch + start, length) << std::endl;
        Reading r {key, std::string(ch + start, length)};
        _reading.emplace_back(r);
    }
}

void MyHandler::ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int length) {
}

void MyHandler::processingInstruction(const Poco::XML::XMLString& target, const Poco::XML::XMLString& data) {
}

void MyHandler::startPrefixMapping(const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri) {
}

void MyHandler::endPrefixMapping(const Poco::XML::XMLString& prefix) {
}

void MyHandler::skippedEntity(const Poco::XML::XMLString& name) {
}

std::unordered_map<std::string, std::vector<Reading>> MyHandler::readings() {
    return _readings;
}
