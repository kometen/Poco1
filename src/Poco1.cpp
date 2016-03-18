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
            pqxx::connection B("dbname=weather user=claus hostaddr=127.0.0.1 port=5432");
            if (!B.is_open()) {
                cerr << "Unable to connect to database" << B.dbname() << endl;
                return EXIT_FAILURE;
            }

            // Get a list of current locations.
            std::string query = "select site_id,measurementsitename,coordinate,coordinate[0] as longitude,coordinate[1] as latitude from locations";
            pqxx::nontransaction N(B);
            pqxx::result R(N.exec(query));

            // lum, location unordered_map, ls location string.
            unordered_map<std::string, std::string> lum {};
            std::string ls {};
            // Add site_id to unordered_map and format columns in json-format.
            // Omit ending curly brace, since more data may be appended.
            for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
                ls  = "{\"site_id\":\"" + c[0].as<std::string>();
                ls += "\",\"measurementSiteName\":\"" + c[1].as<std::string>();
                ls += "\",\"coordinate\":\"" + c[2].as<std::string>();
                ls += "\",\"latitude\":\"" + c[4].as<std::string>();
                ls += "\",\"longitude\":\"" + c[3].as<std::string>() + "\"";
                lum.emplace(c[0].as<std::string>(), ls);
            }

            for (auto& i : lum) {
                cout << i.second << endl;
            }

            B.disconnect();

            // Weather data collected.
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

            pqxx::connection C("dbname=weather user=claus hostaddr=127.0.0.1 port=5432");
            std::string json_data {};
            std::string date {};
            std::string prepared_table = "prep";
            auto um = handler.readings();
            for (auto& i : um) {
                pqxx::work W(C);
                // Set to default values.
                query = "insert into readings_json(site_id,measurementTimeDefault,doc) values ($1,$2,$3)";
                json_data = "";
                date = "";
                json_data = lum.at(i.first);

                for (auto& j : i.second) {
                    auto v = j.measuredValue();
                    json_data += ",\"" + v.first + "\":\"" + v.second + "\"";
                    // When we read the date remember it for storing in the database.
                    if (v.first == "measurementTimeDefault") {
                        date = v.second;
                    }
                }
                json_data += "}";
                C.prepare(prepared_table, query);
                try {
                    W.prepared(prepared_table)(i.first)(date)(json_data).exec();
                    W.commit();
                } catch (const pqxx::sql_error& e) {
                    cerr << "unable to insert, error: " << e.what() << endl;
                }
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
