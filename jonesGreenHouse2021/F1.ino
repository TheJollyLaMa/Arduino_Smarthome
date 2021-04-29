void sun() { 
    //wrapped in temperature check
    event_msg+= "Sun Program: "; 
    if (!isnan(insideTemp) && insideTemp > 80){
    // LIGHTS OFF - too hot
       event_msg+="Too hot, lights off ... ";
       digitalWrite(sunMH, HIGH);sunMHState = 0;
    // digitalWrite(bloomSunMH, HIGH);bloomSunMHState = 0;
    }else if (!isnan(insideTemp) && insideTemp < 50){
    // LIGHTS ON to warm up
       event_msg+="Too cold, lights on ... ";
       digitalWrite(sunMH, LOW);sunMHState = 1;
    // digitalWrite(bloomSunMH, LOW);bloomSunMHState = 0;
    }else{ 
      //wrap in pump check
      if(wellpumpMainState == 0){
        char data[10]; 
        String schedule = String(Bridge.get("Light",data,10));
        if (schedule = "Veg"){
          event_msg+="Veg Schedule Chosen:";
          vegSun();
        }else if (schedule = "Bloom"){
          event_msg+="Bloom Schedule Chosen:";
          bloomSun();
        }
       }  
    }
}
void vegSun(){
// VEG SCHEDULE 18 hour cycle instead of 24 hour cycle provides more grow days per year
//18 Solarhours = one day ; 3 (24) hour days = 4(18) hour days
    event_msg+= "Veg Schedule running: ";
    if (total_days == 1 || total_days % 3 == 1) {
          event_msg+="Day 1 in Veg Sun cycle. ";
          if (Solarhours >= 0 && Solarhours < 14) {
            // 14 Solarhours lights ON
            digitalWrite(sunMH, LOW);sunMHState = 1; 
          } else if (Solarhours >= 14 && Solarhours < 18) {
            // 4 Solarhours lights OFF
            digitalWrite(sunMH, HIGH);sunMHState = 0;   
          } else {
            digitalWrite(sunMH, LOW);sunMHState = 1;
          }
    } else if (total_days == 2 || total_days % 3 == 2) {
          event_msg+="Day 2 in Veg Sun cycle. ";
          if (Solarhours >= 0 && Solarhours < 8) {
            // 14 Solarhours lights ON
            digitalWrite(sunMH, LOW);sunMHState = 1;
          } else if (Solarhours >= 8 && Solarhours < 12) {
            // 4 Solarhours lights OFF
            digitalWrite(sunMH, HIGH);sunMHState = 0;   
          }else { 
            digitalWrite(sunMH, LOW);sunMHState = 1;
          }
    } else if (total_days % 3 == 0) {
          event_msg+="Day 3 in Veg Sun cycle. ";
          if (Solarhours >= 0 && Solarhours < 2) {
            // 2 Solarhours lights ON
            digitalWrite(sunMH, LOW);sunMHState = 1;
          } else if (Solarhours >= 2 && Solarhours < 6) {
            // 4 Solarhours lights OFF
            digitalWrite(sunMH, HIGH);sunMHState = 0;              
          } else if (Solarhours >= 6 && Solarhours < 20) {
            // 14 Solarhours lights ON      
            digitalWrite(sunMH, LOW);sunMHState = 1;
          } else {
            // 4 Solarhours lights OFF
            digitalWrite(sunMH, HIGH);sunMHState = 0;   
          }
 
    }  
}
void bloomSun() {
// BLOOM SCHEDULE
     event_msg+= "Bloom Schedule Running: ";
     if (Solarhours >= 0 && Solarhours < 5) {
        // 5 Solarhours lights OFF
          digitalWrite(sunMH, HIGH);sunMHState = 0;
          event_msg+="The Sun is down for now ... ";
      } else if (Solarhours >= 4 && Solarhours < 20) {
        // 11 Solarhours lights ON
        if(wellpumpMainState == 0){ // kill light while pump on to stop from overloading circuit
          digitalWrite(sunMH, LOW);sunMHState = 1;
          event_msg+="The Sun is UP and SHINING! O Glory Day!... ";
        }else{digitalWrite(sunMH, HIGH);sunMHState = 0;}
      }else{
      // 8 Solarhours lights OFF
          digitalWrite(sunMH, HIGH);sunMHState = 0;
          event_msg+="The Sun is down for now ... ";
      }
}
void rain() {
  event_msg+= "Rain Program: ";
  if (rainyday == "1") { 
       event_msg+="Rainy Day Program. No need to water. Pumps and valves are off ... ";
       digitalWrite (valve1, HIGH);valve1State = 0;
       digitalWrite (valve2, HIGH);valve2State = 0;
       digitalWrite(wellpumpMain, HIGH);Bridge.put("wellpumpMain", String(0));wellpumpMainState=0;                      
  } else if (make_it_rain == "1") {
       digitalWrite(sunMH, HIGH);sunMHState = 0;   // lights off so as not to overload the circuit.  Here's where a smart timer can pay! more efficient timer mechanisims can allow you to allocate loads with logic gates to minimize raw material costs and a messy can-o-worms
//       digitalWrite(linefromhouse, LOW);Bridge.put("linefromhouse", String(1)); linefromhouseState=1;
       digitalWrite(wellpumpMain, LOW);wellpumpMainState = 1;Bridge.put("wellpumpMain", String(1));
       digitalWrite (valve1, LOW);valve1State = 1;
       digitalWrite (valve2, LOW);valve2State = 1;  
       event_msg+="Make It Rain Program running. pumps and valves are ON 30 seconds ... ";
       delay(30000);                   
       digitalWrite (valve1, HIGH);valve1State = 0;
       digitalWrite (valve2, HIGH);valve2State = 0;
       digitalWrite(wellpumpMain, HIGH);Bridge.put("wellpumpMain", String(0));wellpumpMainState=0;
//       digitalWrite(linefromhouse, HIGH);Bridge.put("linefromhouse", String(0)); linefromhouseState=0;
       event_msg+="Manual Watering Complete. Pumps and valves are now OFF.";
  } else if (three_day_rain == "1") {
      ThreeDayRain();
  }else{
      event_msg+="Rain Programs Not Running ... ";
//      if ((Solarminutes == 0 || Solarminutes == 1 || Solarminutes == 2) 
//          || (Solarminutes % 20 == 0 || Solarminutes % 20 == 1 || Solarminutes % 20 == 2)) {
//         char data[10];Bridge.get("battery_sense_voltage",data,10);bat_V = atof(data);
//         if (bat_V > 48 + 1){
//              digitalWrite(wellpumpMain, LOW);Bridge.put("wellpumpMain", String(1));  
//              event_msg+="Periodic pressurization";   
//         }else{
//              event_msg+="Not enough energy stored for Periodic pressurization";  
//         }
//      }else{
//         digitalWrite(wellpumpMain, HIGH);Bridge.put("wellpumpMain", String(0)); 
//         digitalWrite (valve1, HIGH);valve1State = 0;
//         digitalWrite (valve2, HIGH);valve2State = 0; 
//         event_msg+="Pumps and valves are OFF.";      
//      }
  }
  event_log(event_msg);delay(1000);
  event_msg.remove(0);
}
void ThreeDayRain() {
  event_msg += "3day Rain: ";
  // three day rain cycle. adjust accordingly.  looking to produce fluctuations in root zone h2o and o2
  // each time new water comes through the soil pores, it brings new oxygen and nitrogen
  if ((total_days == 1 || total_days % 3 == 1) && ((Solarhours == 0 || Solarhours % 2 == 0)  && Solarminutes < 5)) {
            // day 1 - 5 min every 2 Solarhours
            digitalWrite(sunMH, HIGH);sunMHState = 0;   // lights off so as not to overload the circuit.  Here's where a smart timer can pay! more efficient timer mechanisims can allow you to allocate loads with logic gates to minimize raw material costs and a messy can-o-worms
            digitalWrite(wellpumpMain, LOW);Bridge.put("wellpumpMain", String(1));wellpumpMainState=1;
            digitalWrite (valve1, LOW);valve1State = 1;
            digitalWrite (valve2, LOW);valve2State = 1;
            event_msg+="Raining according to Day 1 Rain Cycle ... ";
            event_msg+="Pumps and valves are ON ... ";
    } else if ((total_days == 2 || total_days % 3 == 2) && ((Solarhours == 0 || Solarhours % 3 == 0)  && Solarminutes < 5)) {
            //day 2 - 15 min every 3 Solarhours
            digitalWrite(sunMH, HIGH);sunMHState = 0;   // lights off so as not to overload the circuit.  Here's where a smart timer can pay! more efficient timer mechanisims can allow you to allocate loads with logic gates to minimize raw material costs and a messy can-o-worms
            digitalWrite(wellpumpMain, LOW);Bridge.put("wellpumpMain", String(1));wellpumpMainState=1;
            digitalWrite (valve1, LOW);valve1State = 1;
            digitalWrite (valve2, LOW); valve2State = 1;
            event_msg+="Raining according to Day 2 Rain Cycle ... ";
            event_msg+="Pumps and valves are ON ... ";
    } else if ((total_days % 3 == 0) && ((Solarhours == 0 || Solarhours % 2 == 0)  && Solarminutes < 5)) {
            // day 3 - 5 min every 2
            digitalWrite(sunMH, HIGH);sunMHState = 0;   // lights off so as not to overload the circuit.  Here's where a smart timer can pay! more efficient timer mechanisims can allow you to allocate loads with logic gates to minimize raw material costs and a messy can-o-worms
            digitalWrite(wellpumpMain, LOW);Bridge.put("wellpumpMain", String(1));wellpumpMainState=1;
            digitalWrite (valve1, LOW);valve1State = 1;  
            digitalWrite (valve2, LOW);valve2State = 1; 
            event_msg+="Raining according to Day 3 Rain Cycle ... ";
            event_msg+="Pumps and valves are ON ... ";
    }else{ 
//            if ((Solarminutes == 0 || Solarminutes == 1 || Solarminutes == 2) 
//                || (Solarminutes % 20 == 0 || Solarminutes % 20 == 1 || Solarminutes % 20 == 2)) {
//               char data[10];Bridge.get("battery_sense_voltage",data,10);bat_V = atof(data);
//               if (bat_V > 47){  // water pump has approximately 1volt voltage drop when accompanied on load
//                    digitalWrite(wellpumpMain, LOW);Bridge.put("wellpumpMain", String(1));  wellpumpMainState=1;
//                    event_msg+="Periodic pressurization";   
//               }else{
//                    event_msg+="Not enough energy stored for Periodic pressurization";  
//               }      
//            }else{
                 digitalWrite(wellpumpMain, HIGH);Bridge.put("wellpumpMain", String(0)); wellpumpMainState=0;
                 digitalWrite (valve1, HIGH);valve1State = 0;
                 digitalWrite (valve2, HIGH);valve2State = 0; 
                 event_msg+="Pumps and valves are OFF.";      
//            }
    }
}

void Raise_CO2() {
    if (CO2 = "1") {
        digitalWrite (co2pump1, LOW);co2pump1State = 1;
        event_msg+="Turning on CO2 pump ... ";
    }else{
        digitalWrite (co2pump1, HIGH);co2pump1State = 0;
    }
}
void wind() {
    insideHumidity = insidedht.readHumidity();delay(500);
    insideTemp = insidedht.readTemperature(true);delay(500);
    outsideHumidity = outsidedht.readHumidity();delay(500);
    outsideTemp = outsidedht.readTemperature(true);delay(500);
    // add CO2 function to turn on air stone in yeast/sugar bucket when temperatures are high to maximize growth and yield
    // Raise_CO2(); 
    if (isnan(insideTemp) || isnan(outsideTemp)) {
        if (isnan(insideTemp)){
             event_msg+="No insideTemp to work with. ";
        }
        if (isnan(outsideTemp)){
             event_msg+="No outsideTemp to work with. ";
        }
    }else{
        if (insideTemp > 78 && outsideTemp > 78) {  // geo cooling when inside and outside Temp is too hot
          digitalWrite(heater, HIGH);heaterState = 0;
          digitalWrite(geoFan, LOW);geoFanState = 1;
          digitalWrite(inletFan, HIGH);inletFanState = 0;
          digitalWrite(outletFan, HIGH);outletFanState = 0;
          event_msg+="Temperature getting too hot. Turning on Geo Fan cool down. ";
        }else if (insideTemp < 60 && outsideTemp < 60){ // geo warming when inside and outside temp is too cold
          digitalWrite(heater, HIGH);heaterState = 0;
          digitalWrite(geoFan, LOW);geoFanState = 1;
          digitalWrite(inletFan, HIGH);inletFanState = 0;
          digitalWrite(outletFan, HIGH);outletFanState = 0;
          event_msg+="Temperature getting too cold. Turning on Geo Fan to warm up. ";
        }else if (insideTemp > 78 && outsideTemp < 78) { // if inside temp is too hot and outside temp is cooler, turn on intake fan 
          digitalWrite(heater, HIGH);heaterState = 0;
          digitalWrite(geoFan, HIGH);geoFanState = 0;
          digitalWrite(inletFan, LOW);inletFanState = 1;
          digitalWrite(outletFan, HIGH);outletFanState = 0; 
          event_msg+="Getting hot in here. Feels better outside. Turning on Exhaust Fan. ";
        }else if (insideTemp < 60 && outsideTemp > 60) { // if inside temp is too cool and outside temp is warmer, turn on intake fan
          digitalWrite(heater, HIGH);heaterState = 0;
          digitalWrite(geoFan, HIGH);geoFanState = 0;
          digitalWrite(inletFan, LOW);inletFanState = 0;
          digitalWrite(outletFan, HIGH);outletFanState = 0;
          event_msg+="Getting cold in here. Feels better outside. Turning on Intake Fan. ";
        }else if (insideTemp < 50) { //if inside temp drops below threshold, turn geofan on, heater on
          digitalWrite(heater, LOW);heaterState = 1;
          digitalWrite(geoFan, LOW);geoFanState = 1;
          digitalWrite(inletFan, HIGH);inletFanState = 0;
          digitalWrite(outletFan, HIGH);outletFanState = 0;
          event_msg+="Getting cold in here, and it's no better outside. Turning on Geo Fan and Heater. ";
        }else if (!isnan(insideHumidity) && insideHumidity > 65) {
          digitalWrite(heater, HIGH);heaterState = 0;
          digitalWrite(geoFan, HIGH);geoFanState = 0;
          digitalWrite(inletFan, HIGH);inletFanState = 0;
          digitalWrite(outletFan, LOW);outletFanState = 1;
          event_msg+="Too Humid inside. Turning on Exhaust Fan. ";
        }else {
          digitalWrite(heater, HIGH);heaterState = 0;
          digitalWrite(geoFan, LOW);geoFanState = 1;
          digitalWrite(inletFan, LOW);inletFanState = 1;
          digitalWrite(outletFan, LOW);outletFanState = 1;
          event_msg+="No wind. Should be smooth sailing and green growing. ";
        }
    }
    event_log(event_msg);
    event_msg.remove(0);
}
void earth() {
 /* - soil moisture sensor
  - soil temperature
  - geo boiler - pump water from well into 55gal drums to act as north wall.
                 drums can be outfitted with heating elements
                 in summer months, acts as a geothermal cooler with elements off and valves open 
                 provides water storage
                 in winter months, with elements on and valves closed, acts as a heater.
                 add pressure valves and close the system
                 arrange double wall of greenhouse so hotair escapes toward the greenhouse,
                 either inner or in between for a jacket effect.
  - drop temperatures at opportune times in the season to allow for maximum favorable fungal growth
  - should assist in seed propagation, as well as co2 output for hungry and developing foliage and trunks
  - Maximum Yield! Harvest the mushrooms to help pay for the whole opperation and win Mad $tack$ back for staying home and saving Earth
  
*/  
}
 /*
    char data[10]; 
    cloudymoment = int(Bridge.get("CloudyMoment", data, 10));
    if(!cloudymoment){

    }else{
      //if its a cloudy moment is there still a reason to water?
      return;
    }*/

  /*
   * Water Line Freeze protection - with another e-valve down hill you could automate a system purge to 
   * take water out of the exposed lines on the worst of the winter days.
   * &
   * 3 day rain cycle to coincide with sun() program designed to
   * cycle through hardening and softening to promote root growth with frequent soil gas exchange
  if (outsideTemp < 32) {  //50deg water from well turned on to prevent lines from freezing and warm greenhouse
       //FreezeProtection=True
       //valves on, pump on, everything else off to preserve power for line pressure
       digitalWrite (valve1, LOW);valve1State = 1;
       digitalWrite (valve2, LOW);valve2State = 1;
  }else{
       ThreeDayRain();
  }
  */
