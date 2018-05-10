#!/usr/bin/env python
# coding: utf8

import binascii
import socket
import sys
import time
import urllib
import json
from subprocess import Popen, PIPE, STDOUT

url = "http://192.168.0.206:8085/data.json"
response = urllib.urlopen(url)
data = json.loads(response.read())

cpuTemp = data['Children'][0]['Children'][1]['Children'][1]['Children'][0]['Value']



with open('/home/linaro/vlcTextTOP.txt','r+') as f:
    f.truncate()
    f.write(u' '.join(cpuTemp).encode('utf-8').strip())
    f.close()


#commands.getstatusoutput(u' '.join(('echo "CPU temp: ', cpuTemp, '" | osd_cat -o 700 -i 700 -d 30 -c green -f -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1')).encode('utf-8').strip())
#commands.getstatusoutput(u' '.join(('echo "CPU usage: ', cpuTemp, '" | osd_cat -o 900 -i 700 -d 30 -c green -f -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1')).encode('utf-8').strip())

# in_string = u' '.join(('CPU temp: ', cpuTemp)).encode('utf-8').strip()
# cmd = [
#         sys.executable,
#         '/usr/bin/osd_cat',
#         '-o 700',
#         '-i 700',
#         '-d 30',
#         '-c green',
#         '-f -adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1'
#       ]
# p = Popen(cmd, shell=False, stdin=PIPE, stdout=None, stderr=None, close_fds=True)
# p.communicate(input=in_string)[0]

sys.exit(0)


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('192.168.0.206', 27015)
sock.connect(server_address)

values = [
    ('1', 5),
    ('2', 5),
    ('3', 5)
    ]

try:
    while True:
        for i in values:
            print i
            val = i[0]
            tmp = i[1]

            sock.send(val)
            time.sleep(tmp)

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
