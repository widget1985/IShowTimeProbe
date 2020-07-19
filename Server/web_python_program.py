from flask import Flask
from flask import render_template, request
import time, os, requests, logging

app = Flask(__name__)
#app = Flask(__name__, template_folder='/home/debian/BBrobotics-book/flask_control_server')
logging.basicConfig(level=logging.DEBUG)

program_path = "~/BBrobotics-book/rc_wheeled_auto_noimu/rc_wheeled_auto_noimu "
app.true_name = None
app.possession_time = 0
app.max_inactivity = 8 # seconds until an inactive user is booted

@app.route("/")
def index():
    randoname = requests.get('https://frightanic.com/goodies_content/docker-names.php').text
    return render_template('web_server_program.html', name=randoname)

@app.route('/left_side')
def left_side():
    data1="LEFT"
    name=request.args.get('formname')
    if name == app.true_name:
        os.system(program_path + str(10) + " " + str(0))
        app.possession_time = time.time()
        app.logger.debug('left command accepted from ' + name)
    else:
        app.logger.debug('left command REJECTED from ' + name)
    return 'true'

@app.route('/right_side')
def right_side():
    data1="RIGHT"
    name=request.args.get('formname')
    if name == app.true_name:
        os.system(program_path + str(-10) + " " + str(0))
        app.possession_time = time.time()
        app.logger.debug('right command accepted from ' + name)
    else:
        app.logger.debug('right command REJECTED from ' + name)
    return 'true'

@app.route('/up_side')
def up_side():
    data1="FORWARD"
    name=request.args.get('formname')
    if name == app.true_name:
        os.system(program_path + str(0) + " " + str(24))
        app.possession_time = time.time()
        app.logger.debug('forward command accepted from ' + name)
    else:
        app.logger.debug('forward command REJECTED from ' + name)
    return 'true'

@app.route('/down_side')
def down_side():
    data1="BACK"
    name=request.args.get('formname')
    if name == app.true_name:
        os.system(program_path + str(0) + " " + str(-24))
        app.possession_time = time.time()
        app.logger.debug('down command accepted from ' + name)
    else:
        app.logger.debug('down command REJECTED from ' + name)
    return 'true'

@app.route('/stop')
def stop():
    data1="STOP"
    name=request.args.get('formname')
    if name == app.true_name:
        os.system(program_path + str(0) + " " + str(0))
        app.possession_time = time.time()
        app.logger.debug('stop command accepted from ' + name)
    else:
        app.logger.debug('stop command REJECTED from ' + name)
    return  'true'

@app.route('/assume_control')
def assume_control():
    formname = request.args.get('formname')
    if (app.true_name is None) or (time.time() - app.possession_time > app.max_inactivity):
        app.true_name = formname
        app.possession_time = time.time()
        app.logger.debug(formname + ' had possessed the robo!')
    else:
        app.logger.debug(formname + ' attempted to possess the robo but was DENIED')
    return 'true'

@app.route('/feedback')
def feedback():
  formname = request.args.get('formname')
  if (formname == app.true_name) and ispossessed():
      return {
          "control": True,
          "possessed": ispossessed(),
          "formname": formname,
          "timeleft": app.max_inactivity - (time.time() - app.possession_time)
      }
  else:
      return {
          "control": False,
          "possessed": ispossessed(),
          "formname": formname,
          "timeleft": app.max_inactivity - (time.time() - app.possession_time)
      }

@app.route('/polar_in')
def polar_in():
    name=request.args.get('formname')
    radians=request.args.get('radians')
    distance=request.args.get('distance')
    if name == app.true_name:
        #os.system(program_path + str(0) + " " + str(-24))
        # insert system call here using variables radians and distance
        app.possession_time = time.time()
        app.logger.debug('polar command accepted from ' + name + ' for ' + radians + ' radians and ' + distance + ' from center') 
    else:
        app.logger.debug('polar command REJECTED from ' + name)
    return 'true'

def ispossessed():
  
  return (app.true_name is not None) and (time.time() - app.possession_time < app.max_inactivity)


if __name__ == "__main__":
    print("Start")
    #app.run(host='0.0.0.0',port=5010)
    app.run(host='0.0.0.0',port=8090)

