#!/usr/bin/env python
import smtplib, ssl
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders

def send_morning_email(total_kWh,total_days):
    USERNAME = 'SolarMail888@gmail.com'
    PASSWORD = ''
    MAILTO  = 'boyyee44@gmail.com'

    msg = MIMEMultipart()
    msg['Subject'] = 'Good Morning From The Sun'
    msg['From'] = USERNAME
    msg['To'] = MAILTO
    html = """
        <html>
            <head></head>
            <body>
                <p>Good Morning <h3>Jolly LaMa!</h3></p>
                <p>
                    It's your <h3>SolarMail.</h3>
                    Om namaho Bhagavate Vyasudeva.
                </p>
                <p>
                    Total Sun Collected: """ +str(total_kWh)+ """
                </p>
                <p>
                    It is day """ +total_days+ """ of the year.
                </p>
                <p>
                    Hare Krsna Hare Krsna Krsna Krsna Hare Hare.
                    Hare Rama Hare Rama Rama Rama Hare Hare.
                </p>
            </body>
        </html>
    """
    msg.attach(MIMEText(html, 'html'))

    server = smtplib.SMTP('smtp.gmail.com:587')
    server.starttls()
    server.login(USERNAME,PASSWORD)
    server.sendmail(USERNAME, MAILTO, msg.as_string())
    server.quit()

def send_emergency_notification_email(Greenhouse_Data, Tristar_Data):
    USERNAME = 'SolarMail888@gmail.com'
    PASSWORD = ''
    MAILTO  = 'boyyee44@gmail.com'

    msg = MIMEMultipart()
    msg['Subject'] = 'Emergency Power Situation!'
    msg['From'] = USERNAME
    msg['To'] = MAILTO
    html = """
        <html>
            <head></head>
            <body>
                <p><h3>There's a situation on the Solar Farm!</h3></p>

                <p>
                    Better Go check it out. Here's a snapshot of the Register Logs:
                </p>
                <p>
                    Greenhouse_Data: """ +str(Greenhouse_Data)+ """
                </p>
                <p>
                    Tristar_Data: """ +str(Tristar_Data)+ """
                </p>
            </body>
        </html>
    """
    msg.attach(MIMEText(html, 'html'))

    server = smtplib.SMTP('smtp.gmail.com:587')
    server.starttls()
    server.login(USERNAME,PASSWORD)
    server.sendmail(USERNAME, MAILTO, msg.as_string())
    server.quit()
