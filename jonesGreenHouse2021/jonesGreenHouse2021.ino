#include <Process.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <EEPROM.h>
#include <FileIO.h>
#include <Bridge.h>
#include <HttpClient.h>

#define DHTTYPE DHT22
#define resistance 10  //the value of the pull-down resistor
#include "EmonLib.h"

char buf[50];String comdata = "";int numdata[7] ={ 0}, j = 0, mark = 0;

float hysteresis = 0.25, upperTem = 0.00,
      bat_V, arr_V,chg_cur,
      insideTemp, outsideTemp, insideHumidity, outsideHumidity, moisture,
      realPower1, realPower2, realPower3, realPower4;
      
long unsigned int lastresults;
int yesterday, total_days, date = 1, Solarmonth = 1, Solaryear, Solarday, Solarhours, Solarminutes, Solarseconds, lastSecond = -99, lastcheck = -99, emergency_email_sent = 0, loop_cycles_down,
    co2pump1,co2pump1State,charge_percentage,EPO_state,
    //53 burnt
                                            sunMH = 9,sunMHState,          
    heater = 49,heaterState,                inletFan = 47,inletFanState,    
    outletFan = 45,outletFanState,          geoFan = 43,geoFanState,
    valve1 = 41,valve1State,                valve2 = 39,valve2State,
    linetohouse = 12,linetohouseState=0,wellpumpMain = 11, wellpumpMainState = 0;
     
String  event_msg, CO2, ServerTime, RTC_dt, WiFiStatus, rainyday, cloudymoment, three_day_rain, make_it_rain, charging_state, datetime, dt, ding, ting, PowerUseDataString, PowerGenDataString, ClimateDataString, SunDataString, RainDataString, WindDataString;
const int insideDHT = A1;const int outsideDHT = A0;//Humiture sensors
DHT insidedht(insideDHT, DHTTYPE);
DHT outsidedht(outsideDHT, DHTTYPE);
ThreeWire myWire(4,5,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
EnergyMonitor emon1, emon2, emon3, emon4;

void setup() {
    pinMode(53, OUTPUT);digitalWrite(53, HIGH);pinMode(51, OUTPUT);digitalWrite(51, HIGH);
    pinMode(49, OUTPUT);digitalWrite(49, HIGH);pinMode(47, OUTPUT);digitalWrite(47, HIGH);
    pinMode(45, OUTPUT);digitalWrite(45, HIGH);pinMode(43, OUTPUT);digitalWrite(43, HIGH);
    pinMode(41, OUTPUT);digitalWrite(41, HIGH);pinMode(39, OUTPUT);digitalWrite(39, HIGH);
    pinMode(12, OUTPUT);digitalWrite(12, HIGH);pinMode(11, OUTPUT);digitalWrite(11, HIGH);
    pinMode(10, OUTPUT);digitalWrite(10, HIGH);pinMode(9, OUTPUT);digitalWrite(9, HIGH);

    FileSystem.begin();Bridge.begin();insidedht.begin();outsidedht.begin();
    getServerTime();paveRouteToTristar();DataLog();updateBridge();//  enMonInit();     
    Bridge.put("Light", "Bloom");
    digitalWrite(linetohouseState, HIGH);Bridge.put("linetohouseState", String(0));//Low is LED OFF and Relay ON for line to house
    digitalWrite(wellpumpMain, HIGH);Bridge.put("wellpumpMain", String(0));//High is LED ON and Relay OFF for well pump relay
    Bridge.put("RainyDay", String(0));Bridge.put("CloudyMoment", String(0));
    Bridge.put("MakeItRain", String(0));Bridge.put("ThreeDayRain", String(1));
    yesterday = total_days - 1;Bridge.put("Yesterday", String(yesterday));
    loop_cycles_down = 99;
    getBridgeVariables();
    PowerCheck();
    TheMainProgram();
    DataLog();
    event_msg += "Just Booted Up and ran setup...";
    event_log(event_msg);
    updateBridge();
}
// Brahma Muhurta - 1hr 40 min before sunrise is the most advantageous time to practice anything or to get things done, especially spiritual practices.
void loop() {
    event_msg = "Fresh Lap ...";
    commsCheck();
    PowerCheck();    
    // if comms are up, update RTC; when comms are down, operate on RTC
    if(WiFiStatus == "U") {
        dt = getServerTime();
        getBridgeVariables();
//      dailyTasks();
        TheMainProgram();
        delay(10000);
    }else{
               /*  turn on local light so you can look out the window 
                 *  and see the program is running and the connection is down maybe color coded
                  *   run TheMainProgram with RealTimeClock  
                 *   periodically try to reboot and reconnect
                  *
                  * eventually, have an offline program that runs and checks for WiFi availability 
                  * every loop main functioning of offline program is to continue filing with time 
                  * from onboard RTC continue with essential functioning like power management and 
                  * environment control. periodically check for internet connection and then send 
                  * updated files to main server (which it is its own main server as of now but you 
                  * should/could/will send it to the house to compile data on a larger, more controlled, 
                  * better protected server space.) 
                  */
                          
        if (loop_cycles_down = 99) {       
               loop_cycles_down = 1;
               event_msg += "WiFi Down. First notice... ";
               event_msg += loop_cycles_down;
        }else{
              event_msg += "WiFi Down.";
              if (loop_cycles_down < 10 ) { // once it misses 10 total cycles, reboot to reestablish and test connection
                  dt = get_RTC();
                  event_msg += "Loop: ";
                  event_msg += loop_cycles_down;
                  getBridgeVariables();
                  dailyTasks();
                  TheMainProgram();
                  loop_cycles_down += 1;
                  delay(30000); //slow down cycle while offline
              }else{
                  //reboot when WiFi is not up for ten loop cycles to try to force a new link 
                  event_msg+= "Rebooting ....";
                  event_log(event_msg);
                  Process p;
                  p.runShellCommand("reboot");
              }  
          }
          delay(30000);
          event_log(event_msg);
          event_msg.remove(0);

    }
    DataLog();delay(400);
    updateBridge();  delay(400);
    event_msg+="Finished this lap... Here's to another!";
    event_log(event_msg);
    event_msg.remove(0);
    
}
