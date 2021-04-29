void commsCheck() {
    event_msg+="Comms check...";
    Process cc;
    cc.begin("python");
    cc.addParameter("/scripts/CommsToHouse.py");
    cc.run();
    char stats = cc.read();
    WiFiStatus = String(stats); 
    Bridge.put("CommsCheck", WiFiStatus);
    event_msg+="Comms are: ";
    event_msg+=WiFiStatus;
    event_msg+=". ";
}
void paveRouteToTristar() {
    event_msg+="Paving Route to Tristar...";
    Process pave_route;
    pave_route.runShellCommand("ip route add 192.168.0.244 dev eth0");
}
void SendMorningEmail() {
    event_msg+="Sending Morning Email...";
    Process send_morning_email;
    send_morning_email.runShellCommand("python /scripts/MorningSolarMail.py");
}
void GetWeatherForecast() {
    event_msg+="Getting Weather Forecast...";
    Process get_weather_forecast;
    get_weather_forecast.runShellCommand("python /scripts/OpenWeather.py");
}
void SendEmergencyNotificationEmail() {
    event_msg+="Sending Emergency Power Notification Email...";
    Process send_emergency_notification_email;
    send_emergency_notification_email.runShellCommand("python /scripts/EmergencySolarMail.py");  
}
