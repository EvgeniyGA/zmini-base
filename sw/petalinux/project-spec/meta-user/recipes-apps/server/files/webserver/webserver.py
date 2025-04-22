# MiniZed webserver script 
import os
import re
import cv2
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
        os.system('rm /usr/bin/webserver/static/images/single_frame*')
        os.system('sudo gpioset 0 0=1')

        randNum = str(random.randrange(0, 32000, 1))
        imageName = "single_frame"+randNum+".jpg"

        camera = cv2.VideoCapture(0)
        (success,reference) = camera.read()
        cv2.imwrite('/usr/bin/webserver/static/images/'+imageName+'',reference)
        cv2.waitKey(1)
        cv2.VideoCapture.release(camera)
        cv2.waitKey(1)
        cv2.destroyAllWindows()
        cv2.waitKey(1)

        return render_template("Home/home.html", imageName=imageName)
    else:
        os.system('rm /usr/bin/webserver/static/images/single_frame*')

        randNum = str(random.randrange(0, 32000, 1))
        imageName = "single_frame"+randNum+".jpg"

        camera = cv2.VideoCapture(0)
        (success,reference) = camera.read()
        cv2.imwrite('/usr/bin/webserver/static/images/'+imageName+'',reference)
        cv2.waitKey(1)
        cv2.VideoCapture.release(camera)
        cv2.waitKey(1)
        cv2.destroyAllWindows()
        cv2.waitKey(1)

        return render_template("Home/home.html", imageName=imageName)

if __name__=='__main__':
    app.run(host='0.0.0.0', port=80, threaded=True)
