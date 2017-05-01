

#define PI 3.14159265358979323846264338327950288419716939937510

static const double DEG_TO_RAD = 0.017453292519943295769236907684886;
static const double EARTH_RADIUS_IN_METERS = 6372797.560856;
static const double EARTH_RADIUS_IN_MILES = 3959;


struct Position
{
	Position(double lat, double lon) : _lat(lat), _lon(lon) {}
	void lat(double lat) { _lat = lat; }
	double lat()const { return _lat; }
	void lon(double lon) { _lon = lon; }
	double lon()const { return _lon; }
private:
	double _lat, _lon;
};

double haversine(const Position& from, const Position& to)
{
	double lat_arc = (from.lat() - to.lat()) * DEG_TO_RAD;
	double lon_arc = (from.lon() - to.lon()) * DEG_TO_RAD;
	double lat_h = sin(lat_arc * 0.5f);
	lat_h *= lat_h;
	double lon_h = sin(lon_arc * 0.5f);
	lon_h *= lon_h;
	double tmp = cos(from.lat()*DEG_TO_RAD) * cos(to.lat() * DEG_TO_RAD);
	return 2.0f * asin(sqrt(lat_h + tmp * lon_h));
}

double GreatCircleBearing(const Position& from, const Position& to)
{
	double lat1 = (from.lat()) * DEG_TO_RAD;
	double lat2 = (to.lat()) * DEG_TO_RAD;
	double lon1 = (from.lon()) * DEG_TO_RAD;
	double lon2 = (to.lon()) * DEG_TO_RAD;

	double temp =  atan2(sin(lon2 - lon1) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2 - lon1)) * 180/PI;
	if (temp < 0)
	{
		temp += 360;
	}
	if (temp > 359)
	{
		temp -= 360;
	}

	return temp;
}

double distance_in_meters(const Position& from, const Position& to)
{
	return EARTH_RADIUS_IN_METERS * haversine(from, to);
}

double distance_in_miles(const Position& from, const Position& to)
{
	return EARTH_RADIUS_IN_MILES * haversine(from, to);
}

//int main()
//{
	//double meters = distance_in_meters(Position(45.5371781, -122.6500385), Position(51.5141667, 0.0936111));
	//double miles = distance_in_miles(Position(45.5371781, -122.6500385), Position(51.5141667, 0.0936111));
//
	//double temp = GreatCircleBearing(Position(45.5371781, -122.6500385), Position(51.5141667, 0.0936111));
	//Serial.print("Distance in meters is: " );
	//Serial.println(meters);
	//Serial.print("\nDistance in miles is: ");
	//Serial.println(miles);
	//Serial.print("\nBearing is: " );
	//Serial.println(temp);
//}
