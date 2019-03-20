import os
from datetime import datetime
from flask import Flask
from flask import request
import sqlite3
WAITSTART = 0x01
WAITNAME = 0x02
PLAYING = 0x03

STATE = WAITSTART
'''
Запросы

Присваиваем записи данные
cursor.execute("""UPDATE ODB SET Family = ?, Name = ?, FathName = ?, Birthdate = ? WHERE id = ?""", [man_lastname, man_name, man_fathname, man_bdate, str(tmp_objID)]) # добавляем объект

Добавляем запись в БД
cursor.execute("""INSERT INTO Phones VALUES (?,?,?)""", [str(tmp_objID), man_phone, ''])
'''

app = Flask(__name__)
conn = sqlite3.connect("playdb")
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

@app.route('/test')
def test():
	return 'OK'
# ========================= MASTER ROUTES =========================
@app.route('/rst', methods = ['GET'])
def reset():
	global STATE
	global curr_id
	STATE = WAITSTART
	curr_id = -1
	return 'OK'

@app.route('/initgame', methods = ['POST'])
def prepare():
	global STATE
	if request.method == 'POST' and STATE == WAITSTART:
		passwd = request.form['password']
		if passwd == 'master':
			STATE = WAITNAME
			print('SERVER GO TO WAITNAME STATE')
			return 'OK'
		else:
			return 'errorPswd'
	else:
		return 'errorPost'


@app.route('/getname')
def ready():
	global curr_id
	if STATE == PLAYING:
		with sqlite3.connect("playdb.db") as connect:
			com = Command(curr_id, connect)
			if len(com.name) > 0:
				return com.name
	return 'command not set'


@app.route('/endgame', methods = ['POST'])
def endgame():
	global curr_id
	global STATE
	if request.method == 'POST' and curr_id > -1 and STATE == PLAYING:
		g_data = list(request.form['gdata'].split(','))
		total_scores = request.form['scores']
		if len(g_data) == 10:
			data_list = list()
			data_list.append(str(curr_id))
			data_list.append(total_scores)
			data_list.extend(g_data)
			with sqlite3.connect("playdb.db") as connect:
				c = connect.cursor()
				c.execute("""INSERT INTO Scores VALUES (?,?,?,?,?,?,?,?,?,?,?,?)""",data_list)
				connect.commit()
				curr_id = -1
				STATE = WAITSTART
				print('SERVER GO TO WAITSTART STATE')
			return 'game stop'
		else:
			return 'wrong gdata'
	elif curr_id == -1:
		return 'first start game'
	else:
		return 'wrong method'

# ========================= VIEW ROUTES =========================
@app.route('/sendname', methods = ['POST'])
def setname():
	global curr_id
	global STATE
	if request.method == 'POST' and curr_id == -1 and STATE == WAITNAME:
		cName = request.form['cname']
		dt = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
		if len(cName) > 0:
			with sqlite3.connect("playdb.db") as connect:
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
				STATE = PLAYING
				print('SERVER GO TO PLAYING STATE')
			return str(curr_id) + ':' + cName
		else:
			return 'wrong name'
	else:
		return 'wrong request'

@app.route('/getscores', methods = ['GET'])
def getscores():
	if request.method == 'GET' and STATE == WAITSTART:
		get_id = request.args.get('id')
		with sqlite3.connect("playdb.db") as conn:
			command = Command(get_id, conn)
			return command.to_string()

@app.route('/getstate')
def state():
	if STATE == WAITNAME:
		return 'WAITNAME'
	elif STATE == PLAYING:
		return 'PLAYING'
	elif STATE == WAITSTART:
		'WAITSTART'
	else:
		return 'ERROR'
	return 'ERROR'

if __name__ == '__main__':
	app.run(debug=False, host = '0.0.0.0', port=int('8484'))