String getServerTime() {
    String result;
    Process time;
    time.begin("date");
    time.addParameter("+%D-%Y-%T");
    time.run();
    while (time.available() > 0) {
      String timeString = time.readString();
      int firstForSlash = timeString.indexOf("/");
      int secondForSlash = timeString.lastIndexOf("/");
      int firstColon = timeString.indexOf(":");
      int secondColon = timeString.lastIndexOf(":");
      int firstDash = timeString.indexOf("-");
      int secondDash = timeString.lastIndexOf("-");
      String monthString = timeString.substring(0, firstForSlash);
      String dayString = timeString.substring(firstForSlash + 1, secondForSlash);
      String yearString = timeString.substring(firstDash + 1, secondDash);
      String hourString = timeString.substring(secondDash + 1, firstColon);
      String minString = timeString.substring(firstColon + 1, secondColon);
      String secString = timeString.substring(secondColon + 1);
      Solarmonth = monthString.toInt();
      Solarday = dayString.toInt();
      Solaryear = yearString.toInt();
      Solarhours = hourString.toInt();
      Solarminutes = minString.toInt();
      Solarseconds = secString.toInt();
      lastSecond = Solarseconds;
      ding=timeString.substring(0, secondForSlash + 3);
      ting=timeString.substring(secondForSlash + 4, secondColon + 1);
      result = timeString;
      result.trim();
    }
    total_days= dayCount(Solarmonth);
    update_RTC();
    return result;
}
void update_RTC () {
    Rtc.Begin();
    Rtc.SetDateTime(RtcDateTime(Solaryear, Solarmonth, Solarday, Solarhours, Solarminutes, Solarseconds));
    RtcDateTime now = Rtc.GetDateTime();
    RTC_dt = printDateTime(now); 
    Bridge.put("RTC_dt", String(RTC_dt));

}
void dailyTasks(){
    if (yesterday < total_days) {
          //daily tasks
          GetWeatherForecast();
          SendMorningEmail();
          yesterday += 1;
          Bridge.put("Yesterday", String(yesterday));
          event_msg+="Daily Tasks Complete.";
    }else{
      return;
    }
}
String get_RTC () {
    RtcDateTime now = Rtc.GetDateTime();
    RTC_dt = printDateTime(now); 
    int firstForSlash = RTC_dt.indexOf("/");
    int secondForSlash = RTC_dt.lastIndexOf("/");
    int firstColon = RTC_dt.indexOf(":");
    int secondColon = RTC_dt.lastIndexOf(":");
    String monthString = RTC_dt.substring(0, firstForSlash);
    String dayString = RTC_dt.substring(firstForSlash + 1, secondForSlash);
    String yearString = RTC_dt.substring(secondForSlash + 1, secondForSlash + 5);
    String hourString = RTC_dt.substring(firstColon - 2, firstColon);
    String minString = RTC_dt.substring(firstColon + 1, secondColon);
    String secString = RTC_dt.substring(secondColon + 1);
    Solarmonth = monthString.toInt();
    Solarday = dayString.toInt();
    Solaryear = yearString.toInt(); 
    Solarhours = hourString.toInt();
    Solarminutes = minString.toInt();
    Solarseconds = secString.toInt();
    lastSecond = Solarseconds;
    ding=RTC_dt.substring(0, secondForSlash + 3);
    ting=RTC_dt.substring(secondForSlash + 4, secondColon + 1);
    RTC_dt.trim();
    total_days= dayCount(Solarmonth);
  return RTC_dt; 
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

String printDateTime(const RtcDateTime& dt) {
    char datestring[20];
    snprintf_P(datestring, 
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(),dt.Day(),dt.Year(),
               dt.Hour(),dt.Minute(),dt.Second()
               );
               
    return datestring;
}

int dayCount (int month) {
  switch (month) {
    case 1 :total_days = Solarday ;break;
    case 2 :total_days = Solarday + 31;break;
    case 3 :total_days = Solarday + 59;break;
    case 4 :total_days = Solarday + 90;break;
    case 5 :total_days = Solarday + 120;break;
    case 6 :total_days = Solarday + 151;break;
    case 7 :total_days = Solarday + 181;break;
    case 8 :total_days = Solarday + 212;break;
    case 9 :total_days = Solarday + 243;break;
    case 10:total_days = Solarday + 273;break;
    case 11:total_days = Solarday + 304;break;
    case 12:total_days = Solarday + 334;break;
  }return total_days;
}
