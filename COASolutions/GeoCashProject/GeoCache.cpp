/******************************************************************************

GeoCache Hunt Project (GeoCache.cpp)

This is skeleton code provided as a project development guideline only.  You
are not required to follow this coding structure.  You are free to implement
your project however you wish.

List Team Members Here:
        Team 2
1. Anthony RamosLebron
2. Jessica John
3. Hedley Cayasso
4. Matthew Phillips

NOTES:

You only have 32k of program space and 2k of data space.  You must
use your program and data space wisely and sparingly.  You must also be
very conscious to properly configure the digital pin usage of the boards,
else weird things will happen.

The Arduino GCC sprintf() does not support printing floats or doubles.  You should
consider using sprintf(), dtostrf(), strtok() and strtod() for message string
parsing and converting between floats and strings.

The GPS provides latitude and longitude in degrees minutes format (DDDMM.MMMM).
You will need convert it to Decimal Degrees format (DDD.DDDD).  The switch on the
GPS Shield must be set to the "Soft Serial" position, else you will not receive
any GPS messages.

*******************************************************************************

Following is the GPS Shield "GPRMC" Message Structure.  This message is received
once a second.  You must parse the message to obtain the parameters required for
the GeoCache project.  GPS provides coordinates in Degrees Minutes (DDDMM.MMMM).
The coordinates in the following GPRMC sample message, after converting to Decimal
Degrees format(DDD.DDDDDD) is latitude(23.118757) and longitude(120.274060).  By
the way, this coordinate is GlobaTop Technology in Tiawan, who designed and
manufactured the GPS Chip.

"$GPRMC,064951.000,A,2307.1256,N,12016.4438,E,0.03,165.48,260406,3.05,W,A*2C/r/n"

$GPRMC,         // GPRMC Message
064951.000,     // utc time hhmmss.sss
A,              // status A=data valid or V=data not valid
2307.1256,      // Latitude 2307.1256 (degrees minutes format dddmm.mmmm)
N,              // N/S Indicator N=north or S=south
12016.4438,     // Longitude 12016.4438 (degrees minutes format dddmm.mmmm)
E,              // E/W Indicator E=east or W=west
0.03,           // Speed over ground knots
165.48,         // Course over ground (decimal degrees format ddd.dd)
260406,         // date ddmmyy
3.05,           // Magnetic variation (decimal degrees format ddd.dd)
W,              // E=east or W=west
A               // Mode A=Autonomous D=differential E=Estimated
*2C             // checksum
/r/n            // return and newline

******************************************************************************/
#define HHMMSSSS 1
#define VALIDATION 2
#define LATITUDE 3
#define NS 4
#define LONGITUDE 5
#define EW 6
#define SPEED 7
#define COURSEGROUND 8
#define PIe 3.141592653



// Required
#include "Arduino.h"
#include <string.h>
#include <math.h>
//#include "SecureDigital.h"


String GpsData[15];


float Latitude;
char N_S;
float Longitude;
char E_W;
float CourseGround;


uint8_t target = 0;
double heading = 20;
double distance = 50;

#define DEG_TO_RAD 0.0174532
#define EARTH_RADIUS_IN_METERS 6372797.560856
#define EARTH_RADIUS_IN_MILES 3959

bool CheckingInput(int number)
{
    for (int x = 0; x < 1000; x++)
    {
        if(digitalRead(number))
        return false;
    }
    return true;
}


void DMtoDDLongitude(String degreeMinutes, float& saveTo);
void DMtoDDLatitude(String degreeMinutes, float& saveTo);


struct Position
{
    //Position(double lat, double lon) : _lat(lat), _lon(lon) {}
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
    double tmp = cos(from.lat() * DEG_TO_RAD) * cos(to.lat() * DEG_TO_RAD);
    return 2.0f * asin(sqrt(lat_h + tmp * lon_h));
}

double GreatCircleBearing(const Position& from, const Position& to)
{
<<<<<<< HEAD
	double lat1 = (from.lat()) * DEG_TO_RAD;
	double lat2 = (to.lat()) * DEG_TO_RAD;
	double lon1 = (from.lon()) * DEG_TO_RAD;
	double lon2 = (to.lon()) * DEG_TO_RAD;

	double temp =  atan2(sin(lon2 - lon1) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2 - lon1)) * 180/PIe;
	if (temp < 0)
	{
		temp += 360;
	}
	if (temp > 359)
	{
		temp -= 360;
	}

	return temp * RAD_TO_DEG;
=======
    double lat1 = from.lat() * DEG_TO_RAD;
    double lat2 = to.lat() * DEG_TO_RAD;
    double lon1 = from.lon() * DEG_TO_RAD;
    double lon2 = to.lon() * DEG_TO_RAD;

    double temp =  atan2(sin(lon2 - lon1) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2 - lon1)) * 180/PIe;

    if(temp < 0)
    temp += 360;
    if(temp > 359)
    temp -= 360;

    return temp;// * RAD_TO_DEG;
>>>>>>> e5de0421eb2989e43240441d0c3a8a1ae780a156
}

double distance_in_meters(const Position& from, const Position& to)
{
    return EARTH_RADIUS_IN_METERS * haversine(from, to);
}

//double distance_in_miles(const Position& from, const Position& to)
//{
//return EARTH_RADIUS_IN_MILES * haversine(from, to);
//}

/*
Configuration settings.

These defines makes it easy to enable/disable certain capabilities
during the development and debugging cycle of this project.  There
may not be sufficient room in the PROGRAM or DATA memory to enable
all these libraries at the same time.  You are only permitted to
have NEO_ON, GPS_ON and SDC_ON during the actual GeoCache Treasure
Hunt.
*/
#define NEO_ON 1		// NeoPixelShield
#define TRM_ON 0		// SerialTerminal4
#define ONE_ON 0		// 1Sheeld
#define SDC_ON 1		// SecureDigital
#define GPS_ON 1		// GPSShield (off = simulated)

// define pin usage
#define NEO_TX	6		// NEO transmit
#define GPS_TX	7		// GPS transmit
#define GPS_RX	8		// GPS receive

// GPS message buffer
#define GPS_RX_BUFSIZ	128
char cstr[GPS_RX_BUFSIZ];

#if GPS_ON
#include "SoftwareSerial.h"
SoftwareSerial gps(GPS_RX, GPS_TX);
#endif

// library includes
#if NEO_ON
#include "NeoPixel.h"
Adafruit_NeoPixel strip = Adafruit_NeoPixel(40, NEO_TX, NEO_GRB + NEO_KHZ800);
#endif

#if ONE_ON
#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_SHIELD
#include "OneSheeld.h"
#endif

#if SDC_ON
#include "SecureDigital.h"

//SDClass myFile;
int fileNumber = 0;
//String fileName = "myFile";
char fileName[16] = "myFile";
File m_file;

void WriteToSDCard(Position pos, float dist_km)
{
    //String str1, str2, str3;
    ////sprintf(str1, "%d", pos.lat());
    ////sprintf(str2, "%d", pos.lon());
    ////sprintf(str3, "%d", dist_km);
    //str1 = String(pos.lat());
    //str2 = String(pos.lon());
    //str3 = String(dist_km);
    
    if(m_file)
    {
        m_file.print(pos.lon(), 6);
        m_file.print(", ");
        m_file.print(pos.lat(), 6);
        m_file.print(", ");
        m_file.println(dist_km, 6);
        
        m_file.flush();
        //m_file.close();
    }
}

bool checkTheFile()
{
<<<<<<< HEAD
	/*while(myFile.exists(&fileName[0]))
	{
		fileName[7]++;
		if(fileName >= 10)
		{
			fileName[7] = 0;
			fileName[6]++;
		}
	}

	myFile.begin();
	m_file = myFile.open(fileName, O_APPEND | O_WRITE);*/

	myFile.begin();

	for(int i = 0; i < fileNumber; i++)
	{
		sprintf(fileName, "myFile%20d", fileNumber);
			
		if(!(myFile.exists(&fileName[0])))
		{
			m_file = myFile.open(fileName, FILE_WRITE);	
			break;
		}
	}
	
	return true;
=======
    /*while(myFile.exists(&fileName[0]))
    {
    fileName[7]++;
    if(fileName >= 10)
    {
    fileName[7] = 0;
    fileName[6]++;
    }
    }

    myFile.begin();
    m_file = myFile.open(fileName, O_APPEND | O_WRITE);*/

    
    if (SD.begin())
    {
        
        for(int i = 0; i < 100; i++)
        {
            sprintf(fileName, "myFile%d.txt", i);
            
            if(!(SD.exists(fileName)))
            {
                m_file = SD.open(fileName, FILE_WRITE);
                break;
            }
            
        }
    }
    
    return true;
>>>>>>> e5de0421eb2989e43240441d0c3a8a1ae780a156
}

#endif

/*
Following is a Decimal Degrees formatted waypoint for the large tree
in the parking lot just outside the front entrance of FS3B-116.
*/
#define GEOLAT0 28.594532
#define GEOLON0 -81.304437

#if GPS_ON
/*
These are GPS command messages (only a few are used).
*/
#define PMTK_AWAKE "$PMTK010,002*2D"
#define PMTK_STANDBY "$PMTK161,0*28"
#define PMTK_Q_RELEASE "$PMTK605*31"
#define PMTK_ENABLE_WAAS "$PMTK301,2*2E"
#define PMTK_ENABLE_SBAS "$PMTK313,1*2E"
#define PMTK_CMD_HOT_START "$PMTK101*32"
#define PMTK_CMD_WARM_START "$PMTK102*31"
#define PMTK_CMD_COLD_START "$PMTK103*30"
#define PMTK_CMD_FULL_COLD_START "$PMTK104*37"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"
#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C"
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C"
#define PMTK_API_SET_FIX_CTL_5HZ  "$PMTK300,200,0,0,0,0*2F"
#define PMTK_SET_NMEA_OUTPUT_RMC "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
#define PMTK_SET_NMEA_OUTPUT_GGA "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

#endif // GPS_ON

#if NEO_ON
/*
Sets target number, heading and distance on NeoPixel Display
*/
void setNeoPixel(uint8_t target, float heading, float distance)
{
    //Clearing LEDs
    for (int i = 0; i < 40; i++)
    {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        //strip.show();
    }
    
    // add code here0
    int NUMLEDs = 6;
    
    if(distance < 100)
    {
        NUMLEDs = distance / 16.66;
    }
    
    uint32_t color = 0;
    if(target == 0)
    color = strip.Color(255,0,0);
    
    else if(target == 1)
    color = strip.Color(0,255,0);
    
    else if(target == 2)
    color = strip.Color(0,0,255);
    
    else if(target == 3)
    color = strip.Color(255,0,255);
    
    //More LEDs in the middle as you get farther less when you get closer
    for (int i = 17; i < 17 + NUMLEDs; i++)
    {
        strip.setPixelColor(i, color);
        strip.show();
    }
    
    
    //Which Direction Simple Arrows
    if((heading > 330) || (heading > 0 && heading < 30))
    {
        strip.setPixelColor(9, strip.Color(255, 255, 255));
        strip.setPixelColor(16, strip.Color(255, 255, 255));
        strip.setPixelColor(25, strip.Color(255, 255, 255));
    }
    
    else if(heading > 30 &&  heading < 60)
    {
        strip.setPixelColor(8, strip.Color(255, 255, 255));
        strip.setPixelColor(0, strip.Color(255, 255, 255));
        strip.setPixelColor(1, strip.Color(255, 255, 255));
    }
    
    else if(heading > 60 && heading < 120)
    {
        strip.setPixelColor(4, strip.Color(255, 255, 255));
        strip.setPixelColor(11, strip.Color(255, 255, 255));
        strip.setPixelColor(13, strip.Color(255, 255, 255));
    }
    
    else if(heading > 120 && heading < 150)
    {
        strip.setPixelColor(6, strip.Color(255, 255, 255));
        strip.setPixelColor(7, strip.Color(255, 255, 255));
        strip.setPixelColor(15, strip.Color(255, 255, 255));
    }
    
    else if(heading > 150 && heading < 210)
    {
        strip.setPixelColor(14, strip.Color(255, 255, 255));
        strip.setPixelColor(23, strip.Color(255, 255, 255));
        strip.setPixelColor(30, strip.Color(255, 255, 255));
    }
    
    else if(heading > 210 && heading < 240)
    {
        strip.setPixelColor(31, strip.Color(255, 255, 255));
        strip.setPixelColor(38, strip.Color(255, 255, 255));
        strip.setPixelColor(39, strip.Color(255, 255, 255));
    }
    
    else if(heading > 240 && heading < 300)
    {
        strip.setPixelColor(36, strip.Color(255, 255, 255));
        strip.setPixelColor(29, strip.Color(255, 255, 255));
        strip.setPixelColor(27, strip.Color(255, 255, 255));
    }
    
    else if(heading > 300 && heading < 330)
    {
        strip.setPixelColor(24, strip.Color(255, 255, 255));
        strip.setPixelColor(32, strip.Color(255, 255, 255));
        strip.setPixelColor(33, strip.Color(255, 255, 255));
    }
    
    strip.show();
}

#endif	// NEO_ON

#if GPS_ON
/*
Get valid GPS message. This function returns ONLY once a second.

void getGPSMessage(void)

Side affects:
Message is placed in global "cstr" string buffer.

Input:
none

Return:
none

*/
void getGPSMessage(void)
{
    uint8_t x=0, y=0, isum=0;

    memset(cstr, 0, sizeof(cstr));
    
    // get name string
    while (true)
    {
        if (gps.peek() != -1)
        {
            cstr[x] = gps.read();
            
            // if multiple inline messages, then restart
            if ((x != 0) && (cstr[x] == '$'))
            {
                x = 0;
                cstr[x] = '$';
            }
            
            // if complete message
            if ((cstr[0] == '$') && (cstr[x++] == '\n'))
            {
                // nul terminate string before /r/n
                cstr[x-2] = 0;

                // if checksum not found
                if (cstr[x-5] != '*')
                {
                    x = 0;
                    continue;
                }
                
                // convert hex checksum to binary
                isum = strtol(&cstr[x-4], NULL, 16);
                
                // reverse checksum
                for (y=1; y < (x-5); y++) isum ^= cstr[y];
                
                // if invalid checksum
                if (isum != 0)
                {
                    x = 0;
                    continue;
                }
                
                // else valid message
                break;
            }
        }
    }
}

#else
/*
Get simulated GPS message once a second.

This is the same message and coordinates as described at the top of this
file.  You could edit these coordinates to point to the tree out front (GEOLAT0,
GEOLON0) to test your distance and direction calculations.  Just note that the
tree coordinates are in Decimal Degrees format, and the message coordinates are
in Degrees Minutes format.

void getGPSMessage(void)

Side affects:
Static GPRMC message is placed in global "cstr" string buffer.

Input:
none

Return:
none

*/
void getGPSMessage(void)
{
    static unsigned long gpsTime = 0;
    
    // simulate waiting for message
    while (gpsTime > millis()) delay(100);
    
    // do this once a second
    gpsTime = millis() + 1000;
    
    memcpy(cstr, "$GPRMC,064951.000,A,2307.1256,N,12016.4438,E,0.03,165.48,260406,3.05,W,A*2C", sizeof(cstr));

    return;
}

#endif	// GPS_ON

bool ParseGPSStringData()
{
    int Traveling = 0;
    char data[GPS_RX_BUFSIZ];
    
    memcpy(data, cstr, sizeof(cstr));
    
    char* token = strtok(data, ",");
    
    while(token != NULL)
    {
        GpsData[Traveling] = token;
        token = strtok(NULL, ",");
        Traveling++;
    }
    
    if (GpsData[VALIDATION].c_str()[0] == 'V')
    return false;
    
    DMtoDDLongitude(GpsData[LONGITUDE], Longitude);
    DMtoDDLatitude(GpsData[LATITUDE], Latitude);
    N_S = GpsData[NS].c_str()[0];
    E_W = GpsData[EW].c_str()[0];
    if (N_S == 'S')
    Latitude *= -1;
    if (E_W == 'W')
    Longitude *= -1;
    CourseGround = GpsData[COURSEGROUND].toFloat();
    
    return true;
}



/*
Main Program Entry

int main(void)

Input:
none

Return:
false

*/

Position prevPos;

int main(void)
{
    init();

    // variables
    Position OurPosition;
    Position Destination[4];
    Destination[0].lon(-81.30205);
    Destination[0].lat(28.59671);
    Destination[1].lon(-81.30407);
    Destination[1].lat(28.59208);
    Destination[2].lon(-81.30538);
    Destination[2].lat(28.5962);
    Destination[3].lon(-81.30357);
    Destination[3].lat(28.5956);
    
    
    // init target button
    pinMode(2, INPUT_PULLUP);
    

    #if TRM_ON
    Serial.begin(115200);
    #endif
    
    #if ONE_ON
    // init OneShield Shield
    OneSheeld.begin();
    #endif
    
    #if NEO_ON
    // init NeoPixel Shield
    strip.begin();
    strip.setBrightness(20);
    strip.show();
    #endif

    #if SDC_ON
    /*
    Initialize the SecureDigitalCard and open a numbered sequenced file
    name "MyMapNN.txt" for storing your coordinates, where NN is the
    sequential number of the file.  The filename can not be more than 8
    chars in length (excluding the ".txt").
    */
    checkTheFile();
    #endif
    
    // enable GPS sending GPRMC message
    #if GPS_ON
    gps.begin(9600);
    gps.println(PMTK_SET_NMEA_UPDATE_1HZ);
    gps.println(PMTK_API_SET_FIX_CTL_1HZ);
    gps.println(PMTK_SET_NMEA_OUTPUT_RMC);
    #endif
    
    bool firstTime = false;
    target = 0;
    
    while (true)
    {
        // if button pressed, set new target
        if(CheckingInput(2))
        {
            if(firstTime == false)
            {
                target++;
                if(target > 3)
                target = 0;
                firstTime = true;
            }
        }
        else
            firstTime = false;
        
        //save previous position for bearing calculation
        // returns with message once a second
        getGPSMessage();
        
        // if GPRMC message (3rd letter = R)
        while (cstr[3] == 'R')
        {

            // parse message parameters
            if(Latitude)
                prevPos.lat(Latitude);
            if(Longitude)
                prevPos.lon(Longitude);
                
            ParseGPSStringData();
            
            // calculated destination heading
            OurPosition.lat(Latitude);
            OurPosition.lon(Longitude);
            heading = GreatCircleBearing(OurPosition, Destination[target]);
            
            
            //calculate relative bearing
            heading = heading - CourseGround;
            
            if(heading < 0)
            heading += 360;
            if(heading > 359)
            heading -= 360;
            
            // calculated destination distance
            distance = distance_in_meters(OurPosition, Destination[target]);
            
            #if SDC_ON
            // write current position to SecureDigital then flush
            WriteToSDCard(OurPosition, distance);
            #endif

            break;
            
        }
        
        // set NeoPixel target display
        #if NEO_ON
        setNeoPixel(target, heading, distance);
        #endif

        #if TRM_ON
        // print debug information to Serial Terminal
        Serial.println(cstr);
        Serial.println(heading);
        Serial.println(distance);
        Serial.println(Longitude);
        Serial.println(Latitude);
        #endif
        
        #if ONE_ON
        // print debug information to OneSheeld Terminal
        if (serialEventRun) serialEventRun();
        Terminal.println(cstr);
        Terminal.println(heading);
        Terminal.println(distance);
        Terminal.println(Longitude);
        Terminal.println(Latitude);
        #endif
    }
    
    return(false);
}

void DMtoDDLongitude(String degreeMinutes, float& saveTo)
{
    String degrees, minutes;
    for (unsigned int i = 0; i < degreeMinutes.length(); i++)
    {
        if (i  < 3)
        degrees += degreeMinutes[i];
        else
        minutes += degreeMinutes[i];
    }
    //float deg = degrees.toFloat();
    //float min = minutes.toFloat();

    saveTo = degrees.toFloat() + minutes.toFloat() / 60;
}

void DMtoDDLatitude(String degreeMinutes, float& saveTo)
{
    String degrees, minutes;
    for (unsigned int i = 0; i < degreeMinutes.length(); i++)
    {
        if (i  < 2)
        degrees += degreeMinutes[i];
        else
        minutes += degreeMinutes[i];
    }
    //float deg = degrees.toFloat();
    //float min = minutes.toFloat();

    saveTo = degrees.toFloat() + minutes.toFloat() / 60;
}

