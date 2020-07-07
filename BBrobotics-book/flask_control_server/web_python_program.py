from flask import Flask
from flask import render_template, request
import time
import os

app = Flask(__name__)

#app = Flask(__name__, template_folder='/home/debian/BBrobotics-book/flask_control_server')

program_path = "~/BBrobotics-book/rc_wheeled_auto_noimu/rc_wheeled_auto_noimu "

@app.route("/")
def index():
    return render_template('web_server_program.html')

@app.route('/left_side')
def left_side():
    data1="LEFT"
    os.system(program_path + str(90) + " " + str(0))
    return 'true'
@app.route('/right_side')
def right_side():
   data1="RIGHT"
   os.system(program_path + str(-90) + " " + str(0))
   return 'true'
@app.route('/up_side')
def up_side():
   data1="FORWARD"
   os.system(program_path + str(0) + " " + str(12))
   return 'true'

@app.route('/down_side')
def down_side():
   data1="BACK"
   os.system(program_path + str(180) + " " + str(1))
   return 'true'

@app.route('/stop')
def stop():
   data1="STOP"
   os.system(program_path + str(0) + " " + str(0))
   return  'true'

if __name__ == "__main__":
 print("Start")
 app.run(host='0.0.0.0',port=5010)

