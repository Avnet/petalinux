#!/usr/bin/python3

"""
Save this file as server.py
>>> python3 server.py
Serving on localhost:8000
You can use this to test GET and POST methods.
"""

import http.server
import socketserver
import logging
import cgi
import sys
import re
import subprocess
import time, struct

LED0Portnumber = str(496)
LED1Portnumber = str(497)
LED2Portnumber = str(498)
LED3Portnumber = str(499)
LED4Portnumber = str(500)
LED5Portnumber = str(501)
LED6Portnumber = str(502)
LED7Portnumber = str(503)
LED0Path = '/sys/class/gpio/gpio' + LED0Portnumber + '/value'
LED1Path = '/sys/class/gpio/gpio' + LED1Portnumber + '/value'
LED2Path = '/sys/class/gpio/gpio' + LED2Portnumber + '/value'
LED3Path = '/sys/class/gpio/gpio' + LED3Portnumber + '/value'
LED4Path = '/sys/class/gpio/gpio' + LED4Portnumber + '/value'
LED5Path = '/sys/class/gpio/gpio' + LED5Portnumber + '/value'
LED6Path = '/sys/class/gpio/gpio' + LED6Portnumber + '/value'
LED7Path = '/sys/class/gpio/gpio' + LED7Portnumber + '/value'


if len(sys.argv) > 2:
    PORT = int(sys.argv[2])
    I = sys.argv[1]
elif len(sys.argv) > 1:
    PORT = int(sys.argv[1])
    I = ""
else:
    PORT = 8000
    I = ""

class ServerHandler(http.server.SimpleHTTPRequestHandler):

    def do_GET(self):
        logging.warning("======= GET STARTED =======")
        logging.warning(self.headers)
	
        http.server.SimpleHTTPRequestHandler.do_GET(self)

    def do_POST(self):
        logging.warning("======= POST STARTED =======")
        logging.warning(self.headers)
        form = cgi.FieldStorage(
            fp=self.rfile,
            headers=self.headers,
            environ={'REQUEST_METHOD':'POST',
                     'CONTENT_TYPE':self.headers['Content-Type'],
                     })
        logging.warning("Host: %s", form.getvalue('Host'));

        if (form.getvalue('SETPLLED')):
            ledChosen = form.getvalue('PLledSel')
            logging.warning("PL LED Setting is %s", ledChosen)
            LED0File= open (LED0Path,'w')
            LED1File= open (LED1Path,'w')
            LED2File= open (LED2Path,'w')
            LED3File= open (LED3Path,'w')
            LED4File= open (LED4Path,'w')
            LED5File= open (LED5Path,'w')
            LED6File= open (LED6Path,'w')
            LED7File= open (LED7Path,'w')
            #time.sleep(0.5)
            if (int(ledChosen) == 0):
                LED0File.write('0')
                LED1File.write('0')
                LED2File.write('0')
                LED3File.write('0')
                LED4File.write('0')
                LED5File.write('0')
                LED6File.write('0')
                LED7File.write('0')
            elif (int(ledChosen) == 1):
                LED0File.write('1')
                LED1File.write('1')
                LED2File.write('1')
                LED3File.write('1')
                LED4File.write('1')
                LED5File.write('1')
                LED6File.write('1')
                LED7File.write('1')
            else:
                LED0File.write('0')
                LED1File.write('0')
                LED2File.write('0')
                LED3File.write('0')
                LED4File.write('0')
                LED5File.write('0')
                LED6File.write('0')
                LED7File.write('0')
            LED0File.close()
            LED1File.close()
            LED2File.close()
            LED3File.close()
            LED4File.close()
            LED5File.close()
            LED6File.close()
            LED7File.close()

        #if (form.getvalue('GETPLSW')):
            #SW0Portnumber = str(504)
            #SW1Portnumber = str(505)
            #SW2Portnumber = str(506)
            #SW3Portnumber = str(507)
            #SW4Portnumber = str(508)
            #SW5Portnumber = str(509)
            #SW6Portnumber = str(510)
            #SW7Portnumber = str(511)
            #SW0Path = '/sys/class/gpio/gpio' + SW0Portnumber + '/value'
            #SW1Path = '/sys/class/gpio/gpio' + SW1Portnumber + '/value'
            #SW2Path = '/sys/class/gpio/gpio' + SW2Portnumber + '/value'
            #SW3Path = '/sys/class/gpio/gpio' + SW3Portnumber + '/value'
            #SW4Path = '/sys/class/gpio/gpio' + SW4Portnumber + '/value'
            #SW5Path = '/sys/class/gpio/gpio' + SW5Portnumber + '/value'
            #SW6Path = '/sys/class/gpio/gpio' + SW6Portnumber + '/value'
            #SW7Path = '/sys/class/gpio/gpio' + SW7Portnumber + '/value'
            #SW0File= open (SW0Path,'r')
            #SW1File= open (SW1Path,'r')
            #SW2File= open (SW2Path,'r')
            #SW3File= open (SW3Path,'r')
            #SW4File= open (SW4Path,'r')
            #SW5File= open (SW5Path,'r')
            #SW6File= open (SW6Path,'r')
            #SW7File= open (SW7Path,'r')
            ##time.sleep(0.5)
            #SW0VAL = SW0File.read()
            #SW1VAL = SW1File.read()
            #SW2VAL = SW2File.read()
            #SW3VAL = SW3File.read()
            #SW4VAL = SW4File.read()
            #SW5VAL = SW5File.read()
            #SW6VAL = SW6File.read()
            #SW7VAL = SW7File.read()
            #SW8BITS = str(SW7VAL) + str(SW6VAL) + str(SW5VAL) + str(SW4VAL) + str(SW3VAL) + str(SW2VAL) + str(SW1VAL) + str(SW0VAL)
            #return SW8BITS
            #SW0File.close()
            #SW1File.close()
            #SW2File.close()
            #SW3File.close()
            #SW4File.close()
            #SW5File.close()
            #SW6File.close()
            #SW7File.close()
			

Handler = ServerHandler
httpd = socketserver.TCPServer(("", PORT), Handler)
httpd.serve_forever()
