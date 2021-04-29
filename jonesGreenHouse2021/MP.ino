void TheMainProgram () {
    event_msg="The Main Program: ";
    getBridgeVariables();
    if (true){ //(EPO_state < 1)
/* 
            ***** The MAIN PROGRAM *****
*/
//            event_msg+="Charge available, Running The Main Program.";
//            event_msg+="Line To House is ON.";
//            if(charging_state != "NIGHT"){event_msg+="Cloudy Moment Check:";CloudyMomentCheck();}
//            event_log(event_msg);delay(1000);
//            event_msg.remove(0);
/*
            *****Environment Control*****  (garden programs )
 */
            sun();delay(200);rain();delay(200);wind();delay(200);   //earth();data(ether);energyMonitor();          
            
            event_msg+="Completed Sun, Rain, and Wind Programs.";
 
//         if (charging_state == "FLOAT") {
//             PowerDiverter();
//             event_msg+="Plenty of power coming in. Charge State in FLOAT.";
//         }
//         if (charging_state == "ABSORPTION") {
//             PowerDiverter();
//             event_msg+="Plenty of power coming in. Charge State in ABSORPTION.";
//         }
         
    }else{ 
        event_msg+="Emergency Power Override State is affirmative. ";
         //battery bank is low and EPO_state is already 1
        if (charging_state == "NIGHT"){
             event_msg+="It's night time! Trying to sleep! ";
             if (charge_percentage <= 0 && bat_V <= 48 ) {
                 event_msg+="Battery all used up! Running datalog and taking a nap. ";
                 // while Emergency Power Override program is triggered 
                 // and it's NIGHT OR the charging is low
                 // take a 10 second nap. Go to sleep.
                 // subconcious activities continue
                 delay(10000);
             }else{
                 event_msg+="still got juice. Resetting Emergency Power Override. ";  
                 Bridge.put("EmergencyPowerOverride", String(0));
             }
         }else if (charging_state == "MPPT"){
             if (chg_cur > 15.00){
                 event_msg+="Looks like there's charge out there! Things should be up and running soon! Let's try ... ";
                 Bridge.put("EmergencyPowerOverride", String(0));
              }else{
                 event_msg+="EPO on but there's a trickle charge coming in. Sunrise or Sunset conditions ... ";
              }
         }else{
              // Default condition to force loop if not NIGHT and not MPPT but Emergency Power Override is on for some reason
              Bridge.put("EmergencyPowerOverride", String(0));
              event_msg+="It's not Night, charger not in MPPT. Not sure what's going on ... ";
              event_msg+="Emergency Power Override set back to 0.";
         }
         event_log(event_msg);delay(1000);
         event_msg.remove(0);
    }
}
