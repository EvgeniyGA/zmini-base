# MiniZed webserver script 
import os
import re
import sys
import glob
import time
import uuid
import crypt
import signal
import random
import os.path
import subprocess
import multiprocessing  
from flask import Flask, render_template, request
from multiprocessing import Pipe

CUR_DIRECTORY = os.path.split(os.path.abspath(__file__))[0]
ALERT = -1

app = Flask(__name__)
app.debug = True

global timer_status, timeout, elapsed_time, pconn, cconn
timeout = 15
elapsed_time = 5
timer_status = "timer_disabled"
pconn, cconn = Pipe()

print("Hello MiniZed!!!")
print(CUR_DIRECTORY)

@app.route('/')
@app.route('/home.html', methods=['GET','POST'])
def home():
    if request.method == "POST":
        phrase = request.form.get("phrase", None)

        if phrase!=None:
            proc = subprocess.Popen('python3 /usr/bin/webserver/spell_phrase.py '+phrase+'' ,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=True)
            output,err = proc.communicate()

            return render_template("Home/home.html",output=output,phrase=phrase)

    phrase = None
    return render_template("Home/home.html")

if __name__=='__main__':
    app.run(host='0.0.0.0', port=80, threaded=True)
