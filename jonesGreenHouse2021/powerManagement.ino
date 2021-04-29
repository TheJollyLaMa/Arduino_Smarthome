void PowerCheck() {
    int cV = int(bat_V*100);
    if (cV >= 4776 && cV <= 5040){
        // Gel Cell charge state specifications (add 0.02 V per cell for lead acid)
        // 1.73V per cell * (6) cells per battery = 10.5V/bat * 4 batteries = 41.52 Volts is Dead
        // 1.99V per cell * (6) cells per battery = 11.94V/bat * 4 batteries = 47.76 Volts is %50 charged 
        // 2.10V per cell * (6) cells per battery = 12.6V/bat * 4 batteries = 50.40 Volts is %100 charged
        charge_percentage = map(cV,4776,5040,0,100);
    }else if (cV < 4776){ 
        charge_percentage = 0;
    }else if (cV > 5040){ 
        charge_percentage = 100;
    }
    Bridge.put("cV", String(cV));
    Bridge.put("Charge_Percentage", String(charge_percentage));  
    if(charge_percentage < 1 && chg_cur < 15.00) {event_msg+="Battery Voltage Low. ";event_msg+="Low Charge Current. ";event_msg+="Emergency Power Override Test ... ";Emergency_Power_Override_Test();}
    else if(charge_percentage < 1 && chg_cur > 15.00) {event_msg+="Battery Voltage Low. ";event_msg+="Charge Current greater than 15. ";event_msg+="Emergency Power Override Test ... ";Emergency_Power_Override_Test();}
    else if(charge_percentage >= 1 && chg_cur < 15.00) {event_msg+="Battery Voltage OK. ";event_msg+="Charge Current less than 15. ";event_msg+="Emergency Power Override Test ... ";Emergency_Power_Override_Test();}
    else{event_msg+="Power check complete. Conditions are favorable. ";Bridge.put("EmergencyPowerOverride", String(0));digitalWrite(linetohouse, LOW);Bridge.put("Line_To_House", String(1));}
    event_log(event_msg);delay(1000);
    event_msg.remove(0);

}
void PowerDiverter() {
  
}
void Emergency_Power_Override_Test() {
      //shut off water valves
      digitalWrite(valve1, LOW);valve1State = 0;
      digitalWrite(valve2, LOW);valve2State = 0;
      //shut off pump
      digitalWrite(wellpumpMain, HIGH);Bridge.put("wellpumpMain", String(0));  
      event_msg+="Well Pump off. Testing Battery Bank for charge ... ";
      delay(10000);//wait for recovery
      if((bat_V < 47.76)) {
         //reaffirm Emergency after 1 minute delay
         //send email
         Bridge.put("EmergencyPowerOverride", String(1));
         digitalWrite(linetohouse, HIGH);Bridge.put("Line_To_House", String(0));
         digitalWrite(wellpumpMain, HIGH);Bridge.put("wellpumpMain", String(0)); 
         digitalWrite(valve1, LOW);valve1State = 0;
         digitalWrite(valve2, LOW);valve2State = 0;  
         if ((emergency_email_sent == 0)) {
             SendEmergencyNotificationEmail(); //defined in datalog
             emergency_email_sent = 1;
         }
         event_msg+="Emergency Power Conditions still persisting. Begin Emergency Power Override Program... ";
         delay(60000);//wait another minute to stabalize recovery
      }
      else{
         if (charging_state != "NIGHT") {
             Bridge.put("EmergencyPowerOverride", String(0));
             digitalWrite(linetohouse, LOW);Bridge.put("Line_To_House", String(1));
             emergency_email_sent = 0;
             event_msg+="Emergency Power situation is resolving. Getting back to the normal program... ";
         } else if (charging_state == "NIGHT") {
             event_msg+="There was an emergency power situation and it's night time. Everything should remain off until morning.  ";
         }else {
             event_msg+="Some other power situation";
         }
      }
}
void CloudyMomentCheck() {
  if ((arr_V < 60.00 && arr_V > 45.00)&&(chg_cur > 10 && chg_cur < 30)&&(bat_V < 48.8)){
                  //getWeatherForecast() again and check the current conditions against the expected next 3 hours
                  event_msg+="Solar Array Voltage Low While Sun Up. ";
                  event_msg+="Low Charge Current. ";
                  event_msg+="Low Battery Voltage. ";
                  event_msg+="Cloudy Moment Conservation. ";
                  CloudyMomentConservation();      
   }else{
       event_msg+="The sky doesn't seem to be cloudy. Continuing with program. ";    
   }
}
void CloudyMomentConservation() {
      // Javascript front end parses weather data
      // use 3 hr forecast to throttle down power demand when overcast
      // if solar harvest drops below threshold, check forecast to see if it's expected to continue.
      // if unfavorable conditions exist do ... 
      // open weatherForecastData.json, check forecast
      // if (ThreeHourForecast = Overcast) { //turn off power suckers or turn on alternative power generation methods }
      Bridge.put("CloudyMoment", String(1));
      //shut off water valves
      digitalWrite(valve1, LOW);valve1State = 0;
      digitalWrite(valve2, LOW);valve2State = 0;
      //shut off pump
      digitalWrite(wellpumpMain, HIGH);Bridge.put("wellpumpMain", String(0)); 
      //shut off every other power hungry device
      //reaffirm CloudyMoment after 1 minute delay
      delay(60000);
      if(arr_V < 60.00 && arr_V > 50.00) {
         //still cloudy, check if it matters
         if (bat_V < 48.8){
           //pump and valves off
           Bridge.put("CloudyMoment", String(1));
           digitalWrite(wellpumpMain, HIGH);Bridge.put("wellpumpMain", String(0)); 
           digitalWrite(valve1, LOW);valve1State = 0;
           digitalWrite(valve2, LOW);valve2State = 0;  
           event_msg+="It's cloudy and there's not enough energy stored. Begin Cloudy Moment Conservation Program... ";            
         }else{
           //turn pump and valves back on if Voltage is high enough
           Bridge.put("CloudyMoment", String(0));
           digitalWrite(wellpumpMain, LOW);Bridge.put("wellpumpMain", String(1)); 
           digitalWrite(valve1, HIGH);valve1State = 1;
           digitalWrite(valve2, HIGH);valve2State = 1;
           event_msg+="It's cloudy but there's enough stored for now ... ";            
         }
      }else{ 
         //turn pump and valves back on if cloudy moment over
         Bridge.put("CloudyMoment", String(0));
         digitalWrite(wellpumpMain, LOW);Bridge.put("wellpumpMain", String(1)); 
         digitalWrite(valve1, HIGH);valve1State = 1;
         digitalWrite(valve2, HIGH);valve2State = 1; 
         event_msg+="Cloudy Moment false alarm. Getting back to the normal program... ";
      }
}
void ForecastPower() {
   /*
    * get weather forecast on python side
    * parse three days ahead
    * cut out the nighttime, or "non - sun" times, like in the valley
    * check each 3 hour forecast for three days ahead 
    *    if clear
    *     ok_to_use_power = true
    *    if clouds || rain || wtf ever
    *     ok_to_use_power = false
    *     dont use power unless voltage proves forecast to be wrong
    *    
    *    if forecast ends up being wrong, write datetime and tristar register / algo conditions to a log file
    *    Learn from log file on likelihood of successful forecast
    * 
    */
}
void loadPlan() {
 
   /*Cold_Hard_Facts() -use EnergyMonitor to ensure we:
                          - never exceed load on relay board
                          - never exceed load on main line to/from house
                          - monitor well pump 
                          
                       - use light sensors to sense how much light the greenhouse is getting before using any
                       - rain, soil moisture, and humidity sensors to calibrate H20 valve usage
                       - only sacrifice plant comfort if there is no energy to spare
                       - never sacrifice plant comfort if there is energy to spare
                          
                       - heat and water before artificial light, unless moist and hot enough    
   */
   // if battery bank below lower threshold one day, force FLOAT condition on next sunny day
   //AI_Energy();  use Cold_Hard_Facts to inform AI_Energy

   // Variables from AI_Energy: Energy_TankPercent_Full, Daily_Conciousness, Seasonal_Conciousness, Crop_Comfort_Level

   //finalize a recommended load plan based on the Cold_Hard_Facts() and AI_Energy()
   //final recommendation should include alerts for possible future weather events
}
void AI_Energy() {
   //use seasonal predictions from data and weather prediction downloads 
   //with sensory memory (historical data from sensors) 
   //in conjunction with tristar variables to balance power usage with power generation
/*rules:
          only use generated power if sensible
          only use generated power when sensible
        ie:  
          only cool the greenhouse if it's not going to be too cold tomorrow or in the next few days
          only heat the greenhouse after 2 or three days of weather below 20
          divert energy toward the house and alert the house when there is extra energy
            used to heat the house in the winter and run the fans in the summer, maybe even air conditioning
          heat water
          if there is a full battery bank && temp is "in danger" of being low
          dont forget, if the solar array is pumping away, that is a real time light sensor!
          
  
   // get past data and make a seasonal conclusion        
   // get weather report and make a weekly conclusion
   // get tristar data and get a daily/hourly conclusion 
   
          
*/
/*
    Energy_Tank_Conciousness = map_battery_storage();
    Daily_Conciousness = map_daylight();//and get hourly forecast
    Seasonal_Conciousness = map_season();//get weekly forecast.  compare to seasonal climate awareness
    Crop_Comfort_Level = map_comfort();

*/
}
String map_comfort(){
    /*
     * which crop is being grown: cold veg, warm veg, fungus cycle
     * determine comfort climate for specific crops/environmental needs
     * webcam could detect drooping leaves
     * moisture sensor
     * temp sensor
     * humidity senor
     * solar panel voltage to measure seasonal/daily light availability
     * use historical data to determine which crops to grow
     */
}
String map_season(){
    /*
     * map solstice data, weather data
     */
}
String map_daylight() {
/*
    if (Solar_Array_Voltage > 10 && Solar_Array_Voltage < 50) { It's dusk or dawn twilight }
             - trigger above ground watering before and after sun is gone
             - A.I. to make overnight power usage recommendation at dusk based on predicted weather conditions and current energy storage level
             - A.I. to make daily power usage schedule recommendation at daybreak
    if (Solar_Array_Voltage < 50) { It's nighttime }          
    if (Solar_Array_Voltage > 50 && Solar_Array_Voltage < 70) { It's daytime and it's cloudy }
    if (Solar_Array_Voltage > 70) { It's daytime and it's sunny }
    if (Solar_Array_Voltage > 85) { **HIGH VOLTAGE EVENT** or whatever voltage that is.  record data dump }
  return solar_conciousness;
*/
}
int map_battery_storage(){
/*
    storage_level = map(battery_voltage, 47760, 50400, 0, 100); // 47.76 volts to 50.4 volts for at-rest(no-charge for 3 hrs), no-load(no current draw) 

    if (Energy_tank > 75) {
        if(probability_for_recharge > 75) {
            :) :) :) Use Energy For Optimum Greenhouse Conditions  :) :) :)
        }
        if(probability_for_recharge < 75 && > 50) {
            :| :| :| Throttle Energy Usage Based on medium probability to recharge  :| :| :|
        }
        if(probability_for_recharge < 50 ) {
            :( :( :( Throttle Energy in case no sun tomorrow :( :( :(
        }
    }
    if (Energy_tank > 50 && < 75 ) {
        if(probability_for_recharge > 75) {
            :) :)  Use Energy For Optimum Greenhouse Conditions   :) :)
        }
        if(probability_for_recharge < 75 && > 50) {
             :| :| Throttle Energy Usage Based on medium probability to recharge   :| :|
        }
        if(probability_for_recharge < 50 ) {
             :( :( Throttle Energy in case no sun tomorrow  :( :(
        }
    }
    if (Energy_tank <50 && > 2) {
        if(probability_for_recharge > 75) {
            :) Use Energy For Optimum Greenhouse Conditions  :)
        }
        if(probability_for_recharge < 75 && > 50) {
            :| Throttle Energy Usage Based on medium probability to recharge  :|
        }
        if(probability_for_recharge < 50 ) {
            :( Throttle Energy in case no sun tomorrow :(
        }
    }
    if (Energy_tank > 0 && < 2) {
        ** ENERGY Storage LOW ** Enter into Power Conservation Mode. 
        No or minimum load
        run on micro controller only
          mark as LOW ENERGY EVENT
          record all data points for later processing from long term memory       
    }
   

    return storage level, energy_alert_status
*/
}
void enMonInit(){
 // Voltage: input pin, calibration, phase_shift // Current: input pin, calibration.
    emon1.voltage(0, 132.33, 1.69);emon1.current(1, 60);
    emon2.voltage(0, 132.33, 1.69);emon2.current(2, 60);       
    emon3.voltage(0, 132.33, 1.69);emon3.current(3, 60);       
    emon4.voltage(0, 132.33, 1.69);emon4.current(4, 60);  
}
void energyMonitor () {
    emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
    realPower1 = emon1.realPower;        //extract Real Power into variable
    delay(500);
    emon2.calcVI(20,2000);
    realPower2 = emon2.realPower;
    delay(500);
    emon3.calcVI(20,2000);
    realPower3 = emon3.realPower;
    delay(500);
    emon4.calcVI(20,2000);
    realPower4 = emon4.realPower;
}
