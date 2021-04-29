void DataLog() {
    event_msg+="Logging Data ...";
    String d = "/overlay/mnt/sd/arduino/www/jonesGreenHouse/data/"+ String(Solaryear) + "/doy/"+ String(total_days) + "/";
    if(!FileSystem.exists(d.c_str())){FileSystem.mkdir(d.c_str());}
    LogPowerData();delay(400);
    LogClimateData(d);delay(400); 
    LogSunData(d);delay(400);
    LogRainData(d);delay(400);
    LogWindData(d);delay(400);
}
void event_log(String msg) {
    dt = get_RTC();
    String event_log_str = dt;event_log_str += " - ";event_log_str += msg;
    Bridge.put("Event_Log_String", event_log_str);
    Process Event_Log;
    Event_Log.runShellCommand("python /scripts/event_log.py");
}
void LogPowerData() {
    Process log_power_data;
    log_power_data.runShellCommand("python /scripts/fetch_and_log.py");
}
void LogClimateData(String d) {
    ClimateDataString += dt;ClimateDataString += ",";
    ClimateDataString += String(insideTemp);ClimateDataString += ",";
    ClimateDataString += String(insideHumidity);ClimateDataString += ",";
    ClimateDataString += String(outsideTemp);ClimateDataString += ",";
    ClimateDataString += String(outsideHumidity);
    d += "climateData.csv";   
    File climateDataFile = FileSystem.open(d.c_str(), FILE_APPEND);
    if (climateDataFile) { climateDataFile.print(ClimateDataString);climateDataFile.println(",");climateDataFile.close();}else {return;}
}
void LogSunData(String d) { 
    SunDataString += dt;SunDataString += ",";
    SunDataString += String(sunMHState);SunDataString += ",";
    d += "sunData.csv"; 
    File sunDataFile = FileSystem.open(d.c_str(), FILE_APPEND);
    if (sunDataFile) { sunDataFile.println(SunDataString);sunDataFile.close();}else {return;} 
}
void LogRainData(String d) {
    RainDataString += dt;RainDataString += ",";
    RainDataString += String(valve1State);RainDataString += ",";
    RainDataString += String(valve2State);
    d += "rainData.csv"; 
    File rainDataFile = FileSystem.open(d.c_str(), FILE_APPEND);
    if (rainDataFile) { rainDataFile.println(RainDataString);rainDataFile.close();}else {return;}
}
void LogWindData(String d) {
    WindDataString += dt;WindDataString += ",";
    WindDataString += String(inletFanState);WindDataString += ",";
    WindDataString += String(outletFanState);WindDataString += ",";
    WindDataString += String(geoFanState);WindDataString += ",";
    WindDataString += String(heaterState);
    d += "windData.csv"; 
    File windDataFile = FileSystem.open(d.c_str(), FILE_APPEND);
    if (windDataFile) { windDataFile.println(WindDataString);windDataFile.close();}else {return;}
}
void updateBridge() {
    event_msg+="Updating Bridge Variables...";
    Bridge.put("Event_Message", event_msg);
    Bridge.put("lastcheck", String(dt));  
    Bridge.put("total_days", String(total_days));  
    Bridge.put("year", String(Solaryear));
    Bridge.put("insideTemp", String(insideTemp));
    Bridge.put("insideHumidity", String(insideHumidity));
    Bridge.put("outsideTemp", String(outsideTemp));
    Bridge.put("outsideHumidity", String(outsideHumidity));
    Bridge.put("sunMHState", String(sunMHState));
    Bridge.put("valve1State", String(valve1State));
    Bridge.put("valve2State", String(valve2State));
    Bridge.put("inletFanState", String(inletFanState));
    Bridge.put("outletFanState", String(outletFanState));
    Bridge.put("geoFanState", String(geoFanState));
    Bridge.put("heaterState", String(heaterState));
}
void getBridgeVariables() {
   event_msg+="Fetching Bridge Variables...";
   char data[10];Bridge.get("RainyDay",data,10);rainyday = String(data);
        data[10];Bridge.get("CloudyMoment",data,10);cloudymoment = String(data);
        data[10];Bridge.get("Charge_Percentage",data,10);charge_percentage = atoi(data);
        data[10];Bridge.get("MakeItRain",data,10);make_it_rain = String(data);
        data[10];Bridge.get("battery_sense_voltage",data,10);bat_V = atof(data);
        data[10];Bridge.get("array_voltage",data,10);arr_V = atof(data);
        data[10];Bridge.get("charging_current",data,10);chg_cur = atof(data);
        data[10];Bridge.get("charging_state",data,10);charging_state = String(data);
        data[10];Bridge.get("ThreeDayRain",data,10);three_day_rain = String(data);
        data[10];Bridge.get("EmergencyPowerOverride",data,10);EPO_state = atoi(data);
        data[10];Bridge.get("Yesterday",data,10);yesterday = String(data).toInt();
        data[10];Bridge.get("total_days",data,10);total_days = String(data).toInt();

}
