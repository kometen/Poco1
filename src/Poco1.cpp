#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include <pqxx/pqxx>

#include "MyHandler.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <username> <password>" << endl;
        return EXIT_FAILURE;
    }
    try {
        try {
            pqxx::connection C("dbname=weather user=claus hostaddr=127.0.0.1 port=5432");
            if (!C.is_open()) {
                cerr << "Unable to connect to database" << C.dbname() << endl;
                return EXIT_FAILURE;
            }

            const string url = "https://www.vegvesen.no/ws/no/vegvesen/veg/trafikkpublikasjon/vaer/1/GetMeasuredWeatherData/";

            std::string username = argv[1];
            std::string password = argv[2];
            std::string credentials = username + ":" + password;

            curlpp::Cleanup cleaner;
            curlpp::Easy request;

            request.setOpt(new curlpp::options::Url(url));
            request.setOpt(new curlpp::options::UserPwd(credentials));

            ostringstream out;
            out << request;

            MyHandler handler {};
            Poco::XML::SAXParser parser {};
            parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, false);
            parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
            parser.setContentHandler(&handler);

            try {
                parser.parseString(out.str());
            } catch (Poco::Exception& e) {
                cerr << e.displayText() << endl;
            }

            pqxx::work W(C);
            std::string json_data {};
            std::string query {};
            auto um = handler.readings();
            for (auto& i : um) {
//                query = "insert into readings_json(site_id,measurementTimeDefault,doc) values"
                json_data = "";
                json_data = "{\"site_id\":\"" + i.first + "\"";
                for (auto& j : i.second) {
                    auto v = j.measuredValue();
                    json_data += ",\"" + v.first + "\":\"" + v.second + "\"";
                }
                json_data += "}";
                cout << json_data << endl;
            }

            C.disconnect();
            return EXIT_SUCCESS;
        } catch (const exception& e) {
            cerr << e.what() << endl;
            return EXIT_FAILURE;
        }

    } catch (curlpp::LogicError& e) {
        cout << e.what() << endl;
    } catch (curlpp::RuntimeError& e) {
        cout << e.what() << endl;
    }

    return EXIT_FAILURE;
}
