<div>
    <h1>Greenhouse MEAN Stack</h1>
    <p align="center"><img src="./arduino.jpg"><img src="./morningstar.png"></p>
    
A Full Software Stack to gather, interact with, and record data from an environmental sensing, storing, and harvesting Hardware Stack.

FrontEnd
Client Side
	Uses css and/or animation styles to visually display vitals and sig events
SmartHome Dashboard:
	Displays all controllers with links to their homepages
	Displays Power data from MorningStar and keeps historical database
	Displays status of water lines from Arduino Yun garden program
	Allows for control of certain actuators via web UI
	Displays all useful incoming Bridge data points real-time
	Has navigation buttons to all live “rooms” under control 
(i.e., Brewery, Shroomery, Greenhouse, ChickenCoop, Surveillance)
	Ledger – track income and expenses
	Inventory Database
	Displays historical charts
	Weather Station (compares forecast to actual experience)
	Links to Bankless Store Front with crowdfunding capability 
BackEnd
Java
Environment Sensing
Power Management (uses incoming MonringStar data to run a power management program)
GreenHouse/Garden Control (controls water, artificial light, airflow, humidty, etc.)
Task Timing/Control (what happens when how often)
Datalog
Python Calls
Low Voltage Actualization/Realization 

Python
Link to MorningStar via Modbus
Log data to file
Get Weather
Send Sig Event Emails
Filesystem functions

The Hardware
Arduino micro controller
coupled with a solar circuit using a Morningstar solar charge controller complete with panels and battery bank

**Though the microcontrollers hold significant amounts of data and are stand alone units running SmartHome© and GreenHouse© , they are ultimately meant to connect to a localhost running on a more sophisticated processor where StoreFront©, BehindTheCounter©, and AngelTokens© can be implemented for Sales Inventory and Payflow as well as Crowdfunding.**


</div>  
<hr/>

![PowerDemo](https://user-images.githubusercontent.com/75486638/134360883-0160120a-41fe-4a07-850f-14e03be9f175.png)

![powerNight](https://user-images.githubusercontent.com/75486638/134360691-9bb2c9de-e3cb-4803-990f-5c536fb11b21.png)

![ChartsDemo](https://user-images.githubusercontent.com/75486638/134360847-09d73560-56ec-49de-848f-7ab9ce15bcd8.png)

![SmarthomeScreenshot](https://user-images.githubusercontent.com/75486638/119382357-eb5cba00-bc8f-11eb-8f67-6b60b4703688.png)

![HomeDemo](https://user-images.githubusercontent.com/75486638/134360854-8baa8306-39c4-4c5b-ab7b-5a7e5d0871df.png)

![WaterDemo](https://user-images.githubusercontent.com/75486638/134360864-7627cc5d-9033-410a-a4c7-f878eb1a0bc8.png)
