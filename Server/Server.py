import os
from datetime import datetime
from flask import Flask
from flask import request
import sqlite3
SHOW_RECS = 0x01
WAIT_NAME = 0x02
WAIT_START = 0x03
PLAYING = 0x04

STATE = SHOW_RECS
'''
Запросы

Присваиваем записи данные
cursor.execute("""UPDATE ODB SET Family = ?, Name = ?, FathName = ?, Birthdate = ? WHERE id = ?""", [man_lastname, man_name, man_fathname, man_bdate, str(tmp_objID)]) # добавляем объект

Добавляем запись в БД
cursor.execute("""INSERT INTO Phones VALUES (?,?,?)""", [str(tmp_objID), man_phone, ''])
'''

app = Flask(__name__)
conn = sqlite3.connect("play.db")
c = conn.cursor()
curr_id = -1

class Command:
	def __init__(self, id, connect):
		self.id = id
		c = connect.cursor()
		res = c.execute("""SELECT * FROM Commands WHERE id=?""", [self.id]).fetchall()
		if len(res) > 0:
			self.dt = str(res[0][1])
			self.name = str(res[0][2])
		res = c.execute("""SELECT * FROM Scores WHERE id=?""", [self.id]).fetchall()
		if len(res) > 0:
			res = res[0]
			self.scores = str(res[1])
			self.times = res[2:]
	def to_string(self):
		return '|'.join([self.dt,self.name,self.scores])

@app.errorhandler(404)
def not_found(error):
    return 'error'

@app.route('/connect')
def test():
	return 'OK'
# ========================= MASTER ROUTES =========================
@app.route('/rst', methods = ['GET'])
def reset():
	global STATE
	global curr_id
	STATE = WAIT_NAME
	curr_id = -1
	return 'OK'

@app.route('/startgame', methods = ['GET'])
def prepare():
	global STATE
	if STATE == WAIT_NAME or STATE == WAIT_START:
		STATE = PLAYING
		print('SERVER GO TO WAITNAME STATE')
		return 'OK'
	else:
		return 'errorPost'

@app.route('/endgame', methods = ['GET'])
def endgame():
	global curr_id
	global STATE
	if curr_id > -1 and STATE == PLAYING:
		g_data = list(request.form['gdata'].split(','))
		total_scores = request.form['scores']
		if len(g_data) == 10:
			data_list = list()
			data_list.append(str(curr_id))
			data_list.append(total_scores)
			data_list.extend(g_data)
			with sqlite3.connect("play.db") as connect:
				c = connect.cursor()
				c.execute("""INSERT INTO Scores VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)""",data_list)
				connect.commit()
				curr_id = -1
				STATE = SHOW_RECS
				print('SERVER GO TO WAITSTART STATE')
			return 'game stop'
		else:
			return 'wrong gdata'
	elif curr_id == -1:
		return 'first start game'
	else:
		return 'wrong method'


@app.route('/getname')
def ready():
	global curr_id
	if STATE == WAIT_START:
		with sqlite3.connect("play.db") as connect:
			com = Command(curr_id, connect)
			if len(com.name) > 0:
				return com.name
	elif STATE == WAIT_NAME:
		return 'SERVER_WAIT_NAME'
# ========================= VIEW ROUTES =========================

@app.route('/sendname', methods = ['POST'])
def setname():
	global curr_id
	global STATE
	if request.method == 'POST' and curr_id == -1 and STATE == WAIT_NAME:
		cName = request.form['cname']
		dt = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
		if len(cName) > 0:
			with sqlite3.connect("play.db") as connect:
				c = connect.cursor()
				res = c.execute("""SELECT * from Commands""").fetchall()
				next_id = -1
				if len(res) > 0:
					for r in res:
						if r[0] > next_id:
							next_id = r[0]
				next_id += 1
				c.execute("""INSERT INTO Commands VALUES (?,?,?)""",[next_id,dt,cName])
				connect.commit()
				curr_id = next_id
				STATE = WAIT_START
				print('CMD NAME: {}, WAIT START FROM MASTER'.format(cName))
			return 'SERVER_SET_CMDNAME_OK'
		else:
			return 'SERVER_SET_CMDNAME_ERROR'
	else:
		return 'SERVER_ERROR_REQUEST'

@app.route('/getscores', methods = ['GET'])
def getscores():
	if request.method == 'GET' and STATE == WAITSTART:
		get_id = request.args.get('id')
		with sqlite3.connect("play.db") as conn:
			command = Command(get_id, conn)
			return command.to_string()

@app.route('/getstate')
def state():
	if STATE == WAIT_NAME:
		return 'WAIT_NAME'
	elif STATE == PLAYING:
		return 'PLAYING'
	elif STATE == WAIT_START:
		return 'WAIT_START'
	elif STATE == SHOW_RECS:
		return 'SHOW_RECS'
	else:
		return 'ERROR'
	return 'ERROR'

if __name__ == '__main__':
	app.run(debug=False, host = '0.0.0.0', port=int('8484'))