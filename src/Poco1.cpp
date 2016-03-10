#include <iostream>

#include "MyHandler.hpp"

using namespace std;

int main() {
	auto s = "<siteMeasurements><measurementSiteReference targetClass=\"MeasurementSiteRecord\" version=\"7\" id=\"150\"/><measurementTimeDefault>2016-02-18T20:30:00.000+01:00</measurementTimeDefault><measuredValue index=\"201\"><measuredValue><basicData xsi:type=\"HumidityInformation\"><humidity><relativeHumidity><percentage>74.6</percentage></relativeHumidity></humidity></basicData></measuredValue></measuredValue><measuredValue index=\"2501\"><measuredValue><basicData xsi:type=\"PrecipitationInformation\"><precipitationDetail><precipitationIntensity><millimetresPerHourIntensity>0.0</millimetresPerHourIntensity></precipitationIntensity></precipitationDetail></basicData></measuredValue></measuredValue><measuredValue index=\"801\"><measuredValue><basicData xsi:type=\"RoadSurfaceConditionInformation\"><roadSurfaceConditionMeasurements><roadSurfaceTemperature><temperature>-5.5</temperature></roadSurfaceTemperature></roadSurfaceConditionMeasurements></basicData></measuredValue></measuredValue><measuredValue index=\"901\"><measuredValue><basicData xsi:type=\"WindInformation\"><wind><windSpeed><speed>11.52</speed></windSpeed></wind></basicData></measuredValue></measuredValue><measuredValue index=\"1001\"><measuredValue><basicData xsi:type=\"WindInformation\"><wind><windDirectionBearing><directionBearing>343</directionBearing></windDirectionBearing></wind></basicData></measuredValue></measuredValue><measuredValue index=\"101\"><measuredValue><basicData xsi:type=\"TemperatureInformation\"><temperature><airTemperature><temperature>-6.3</temperature></airTemperature></temperature></basicData></measuredValue></measuredValue><measuredValue index=\"301\"><measuredValue><basicData xsi:type=\"TemperatureInformation\"><temperature><dewPointTemperature><temperature>-10.1</temperature></dewPointTemperature></temperature></basicData></measuredValue></measuredValue><measuredValue index=\"1401\"><measuredValue><basicData xsi:type=\"VisibilityInformation\"><visibility><minimumVisibilityDistance><integerMetreDistance>20000</integerMetreDistance></minimumVisibilityDistance></visibility></basicData></measuredValue></measuredValue></siteMeasurements>";
	MyHandler handler {};
	Poco::XML::SAXParser parser {};
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, false);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(&handler);

	try {
        parser.parseString(s);
    } catch (Poco::Exception& e) {
        cerr << e.displayText() << endl;
    }
	return 0;
}
