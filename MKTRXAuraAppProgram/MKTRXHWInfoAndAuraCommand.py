#!/usr/bin/env python
# coding: utf8

import binascii
import socket
import sys
import time
import urllib
import json
from subprocess import Popen, PIPE, STDOUT

address = '192.168.0.206'
port = 27015

url = "http://192.168.0.206:8085/data.json"
response = urllib.urlopen(url)
data = json.loads(response.read())

pc = data['Children'][0]

cpu = pc['Children'][1]
cpuFreq = cpu['Children'][0]
cpuTemp = cpu['Children'][1]
cpuLoad = cpu['Children'][2]

ram = pc['Children'][2]
ramLoad = ram['Children'][0]

gpu = pc['Children'][3]
gpuTemp = gpu['Children'][0]
gpuTemp = gpu['Children'][1]

ssd = pc['Children'][5]
ssdTemp = ssd['Children'][0]

cpuLoadValue = data['Children'][0]['Children'][1]['Children'][2]['Children'][0]['Value']
cpuTempValue = data['Children'][0]['Children'][1]['Children'][1]['Children'][4]['Value']
cpuFreqValue = data['Children'][0]['Children'][1]['Children'][0]['Children'][1]['Value']
ramLoadValue = data['Children'][0]['Children'][2]['Children'][0]['Children'][0]['Value']
gpuLoadValue = data['Children'][0]['Children'][3]['Children'][2]['Children'][0]['Value']
gpuTempValue = data['Children'][0]['Children'][3]['Children'][1]['Children'][0]['Value']
gpuFreqValue = data['Children'][0]['Children'][3]['Children'][0]['Children'][0]['Value']
vRamLoadValue = data['Children'][0]['Children'][3]['Children'][2]['Children'][3]['Value']
vRamUsageValue = data['Children'][0]['Children'][3]['Children'][5]['Children'][1]['Value']
vRamFreqValue = data['Children'][0]['Children'][3]['Children'][0]['Children'][1]['Value']
ssdTempValue = data['Children'][0]['Children'][5]['Children'][0]['Children'][0]['Value']

with open('/home/linaro/vlcTextTOP.txt','r+') as f:
    f.truncate()
    f.write(u' '.join(cpuTemp).encode('utf-8').strip())
    f.close()

# xrandr --output HDMI-1 --rotate right

# touch /home/linaro/cpuLoadValue.txt
# touch /home/linaro/cpuTempValue.txt
# touch /home/linaro/cpuFreqValue.txt
# touch /home/linaro/ramLoadValue.txt
# touch /home/linaro/gpuLoadValue.txt
# touch /home/linaro/gpuTempValue.txt
# touch /home/linaro/gpuFreqValue.txt
# touch /home/linaro/vRamLoadValue.txt
# touch /home/linaro/vRamUsageValue.txt
# touch /home/linaro/vRamFreqValue.txt
# touch /home/linaro/ssdTempValue.txt


# marq{file=/home/linaro/cpuLoadValue.txt,color=0xCCCCCC,x=1913,y=1450,size=31,refresh=3000}
# marq{file=/home/linaro/cpuTempValue.txt,color=0xCCCCCC,x=1913,y=1460,size=31,refresh=3000}
# marq{file=/home/linaro/cpuFreqValue.txt,color=0xCCCCCC,x=1913,y=1470,size=31,refresh=3000}
# marq{file=/home/linaro/ramLoadValue.txt,color=0xCCCCCC,x=1913,y=1480,size=31,refresh=3000}
# marq{file=/home/linaro/gpuLoadValue.txt,color=0xCCCCCC,x=1455,y=3659,size=29,refresh=3000}
# marq{file=/home/linaro/gpuTempValue.txt,color=0xCCCCCC,x=1147,y=3659,size=29,refresh=3000}
# marq{file=/home/linaro/gpuFreqValue.txt,color=0xCCCCCC,x=1147,y=3669,size=29,refresh=3000}
# marq{file=/home/linaro/vRamLoadValue.txt,color=0xCCCCCC,x=1455,y=3669,size=29,refresh=3000}
# marq{file=/home/linaro/vRamUsageValue.txt,color=0xCCCCCC,x=1455,y=3679,size=29,refresh=3000}
# marq{file=/home/linaro/vRamFreqValue.txt,color=0xCCCCCC,x=1147,y=3679,size=29,refresh=3000}
# marq{file=/home/linaro/ssdTempValue.txt,color=0xCCCCCC,x=1913,y=1490,size=31,refresh=3000}

# vlc --extraintf rc --sub-source="marq{file=/home/linaro/cpuLoadValue.txt,color=0xCCCCCC,x=1913,y=1450,size=31,refresh=3000}:marq{file=/home/linaro/cpuTempValue.txt,color=0xCCCCCC,x=1913,y=1460,size=31,refresh=3000}:marq{file=/home/linaro/cpuFreqValue.txt,color=0xCCCCCC,x=1913,y=1470,size=31,refresh=3000}:marq{file=/home/linaro/ramLoadValue.txt,color=0xCCCCCC,x=1913,y=1480,size=31,refresh=3000}:marq{file=/home/linaro/gpuLoadValue.txt,color=0xCCCCCC,x=1455,y=3659,size=29,refresh=3000}:marq{file=/home/linaro/gpuTempValue.txt,color=0xCCCCCC,x=1147,y=3659,size=29,refresh=3000}:marq{file=/home/linaro/gpuFreqValue.txt,color=0xCCCCCC,x=1147,y=3669,size=29,refresh=3000}:marq{file=/home/linaro/vRamLoadValue.txt,color=0xCCCCCC,x=1455,y=3669,size=29,refresh=3000}:marq{file=/home/linaro/vRamUsageValue.txt,color=0xCCCCCC,x=1455,y=3679,size=29,refresh=3000}:marq{file=/home/linaro/vRamFreqValue.txt,color=0xCCCCCC,x=1147,y=3679,size=29,refresh=3000}:marq{file=/home/linaro/ssdTempValue.txt,color=0xCCCCCC,x=1913,y=1490,size=31,refresh=3000}" https://www.youtube.com/watch?v=wBp0Rb-ZJak

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
server_address = (address, port)
sock.connect(server_address)

values = [
    ('1', 5),
    ('2', 5),
    ('3', 5)
    ]

try:
    while True:
        for i in values:
            print(i)
            val = i[0]
            tmp = i[1]

            sock.send(val)
            time.sleep(tmp)

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
