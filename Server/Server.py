#!/usr/bin/python3
# -*- coding: utf-8 -*-
'''
СОСТОЯНИЕ	СЕРВЕР	ОПЕРАТОР	ОЧКИ	МАСТЕР	МОСТ
1. Ждем 
'''
import os
from datetime import datetime
from flask import Flask
from flask import request
import sqlite3
import logging
import json
import csv

IDLE = 0x01
WAIT_NAME = 0x02
WAIT_START = 0x03
PLAYING = 0x04

STATE = IDLE

dirname, filename = os.path.split(os.path.abspath(__file__))
BASE_PATH = dirname + '/gamedata.db'
# 'Dropbox/Rafael/ZGame/Server/gamedata.db'

#0	  1 	2		3		4		5		6		7		8	  9		10	  11	 12		13	  14	 15	  16	 17 	 18
#ID, DATE, NAME, SCORES, RADIO, GEN/FUEL, GEN/RUN, METER, CODE, FUSES, DOOR, WINDOW, GAS, SHELF, E.M.P, MAP, FLARE, ZOMBIE, TOTAL_TIME
app = Flask(__name__)
CMD_NAME = ''
games = ['Zombie', 'Room', 'Graphics']
monitorLogger = logging.getLogger('monitorLogger')
scoresLogger = logging.getLogger('scoresLogger')

def e_print(text):
	dt = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
	print('{}: {}'.format(dt, text), end='\n')

def clean_base():
	with sqlite3.connect(BASE_PATH) as connect:
		c = connect.cursor()
		game_results = c.execute("""SELECT * FROM Zombie""")
		for r in game_results:
			if r[3] == 0:
				c.execute("""DELETE FROM Zombie WHERE id=?""", [r[0]])
				connect.commit()

def get_game_scores(game_name):
	scores = list()
	with sqlite3.connect(BASE_PATH) as connect:
		c = connect.cursor()
		results = c.execute("""SELECT * from {name}""".format(name = game_name)).fetchall()
		while results:
			r = results.pop()
			scores.append(dict(zip(["Name","Time"],[r[2],r[3]])))
		scores = sorted(scores, key=lambda k: k['Time']) 
		uns = json.dumps(scores)
		if uns:
			return uns
		else:
			return None

class Command:
	def __init__(self, data):
		self.id = data[0]
		self.dt = data[1]
		self.name = data[2]
		self.total_time = data[3]
		self.gadget_time = data[4:]
		self.all_data = data
	def to_view(self):
		return dict({"Name":self.name, "Time":self.total_time})
	def all_to_csv(self):
		if self.all_data:
			return self.all_data


@app.errorhandler(404)
def not_found(error):
    return 'error'

@app.route('/connect')
def connect():
	return 'OK'

# ========================= MONITOR ROUTES =========================
@app.route('/monitor', methods = ['GET'])
def reset():
	global STATE
	global CMD_NAME
	STATE = WAIT_NAME
	CMD_NAME = ''
	e_print('MASTER WORKS, WAIT NAME AND START')
	return 'OK'

@app.route('/startgame', methods = ['GET'])
def prepare():
	global STATE
	if STATE == WAIT_START:
		e_print('GAME START')
		STATE = PLAYING
		return 'OK'
	elif STATE == WAIT_NAME:
		e_print('GAME START WITHOUT NAME')
		STATE = IDLE
		return 'OK'
	else:
		if STATE == PLAYING:
			e_print('START GAME ERROR: GAME IN PLAY')
		if STATE == IDLE:
			e_print('START GAME ERROR: SERVER IN IDLE')
		return 'errorPost'

@app.route('/endgame', methods = ['POST'])
def endgame():
	global STATE
	global CMD_NAME
	if STATE == PLAYING and len(CMD_NAME) > 0:
		times = list(request.form['gdata'].split(','))
		total_scores = request.form['scores']
		if len(times) == 14:
			with sqlite3.connect(BASE_PATH) as connect:
				c = connect.cursor()
				NEXT_ID = -1
				res = c.execute("""SELECT * FROM Zombie""").fetchall()
				if len(res) > 0:
					ids = [r[0] for r in res]
					print('IDS from BASE {}'.format(ids))
					max_id = max(ids) # в случае присутствия всех по порядку соблюдается max_id = res.count() - 1
					if max_id > len(ids) - 1: # 1 4 5 6 10 т.е. есть пропуски индексов
						for i in range(max_id):
							if i not in ids:
								NEXT_ID = i
				else:
					NEXT_ID = 0
				DT = datetime.now().strftime("%Y-%m-%d %H:%M")
				DATA = list()
				DATA.append(str(NEXT_ID))
				DATA.append(DT)
				DATA.append(CMD_NAME)
				DATA.append(str(sum([int(g) for g in times]))) # TOTAL_TIME
				DATA.extend(times) # TIMES
				c.execute("""INSERT INTO Zombie VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)""",DATA)
				print('RESULTS WROTE OK')
				connect.commit()
				CMD_NAME = ''
				STATE = IDLE
				print('SERVER GO TO SHOW_RECS STATE')
			return 'game stop'
		else:
			e_print('WRONG GADGETS DATA RECIEVED')
			return 'WRONG GADGETS DATA RECIEVED'
	else:
		e_print('GAME NOT RUNNING')
		return 'GAME NOT RUNNING'


@app.route('/getname', methods = ['GET'])
def getname():
	global CMD_NAME
	if STATE == WAIT_START:
		return CMD_NAME
	elif STATE == WAIT_NAME:
		e_print('SERVER WAITING NAME FROM SCORES')
		return 'SERVER_WAIT_NAME'

@app.route('/savetocsv')
def savetocsv():
	with sqlite3.connect(BASE_PATH) as connect:
			try:
				c = connect.cursor()
				fields = c.execute("""pragma table_info('Zombie')""").fetchall()
				fields = [f[1] for f in fields]
				print(fields)
				results = []
				gRes = c.execute("""SELECT * FROM Zombie""").fetchall()
				while gRes:
					c = gRes.pop()
					results.append(c)
				if not os.path.exists('FTP'):
					os.mkdir('FTP')
				with open('FTP/Zombie.csv', "w") as f:
					writer = csv.writer(f)
					writer.writerow(fields)
					writer.writerows(results)
					return 'Saved'
			except Exception as e:
				e_print(e)
				return 'Error'
	return 'Error while connect to base'
			

# ========================= SCORES ROUTES =========================
@app.route('/setname', methods = ['POST'])
def setname():
	global STATE
	global CMD_NAME
	if request.method == 'POST' and STATE == WAIT_NAME:
		CMD_NAME = request.form['cname']
		if len(CMD_NAME) > 0:
			STATE = WAIT_START
			print('CMD NAME: {}, WAIT START FROM MASTER'.format(CMD_NAME))
			return 'SERVER_SET_CMDNAME_OK'
		else:
			e_print('SERVER_SET_CMDNAME_ERROR')
			return 'SERVER_SET_CMDNAME_ERROR'
	else:
		e_print('SERVER_ERROR_REQUEST')
		return 'SERVER_ERROR_REQUEST'

@app.route('/setdata', methods = ['POST'])
def setdata():
	if request.method == 'POST':
		gInd = request.form['gnum']
		gName = games[int(gInd)]
		cName = request.form['cname']
		cTime = request.form['ctime']
		cDate = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
		with sqlite3.connect(BASE_PATH) as connect:	
			c = connect.cursor()
			res = c.execute("""SELECT * from {}""".format(gName)).fetchall()
			next_id = -1
			if len(res) > 0:
				for r in res:
					if r[0] > next_id:
						next_id = r[0]
			next_id += 1
			c.execute("""INSERT INTO {} VALUES (?,?,?,?)""".format(gName),[next_id, cDate, cName, cTime])
			connect.commit()
		e_print('SERVER_ADD_DATA_OK')
		return 'SERVER_ADD_DATA_OK'
	else:
		e_print('ERROR')
		return 'ERROR'

@app.route('/getscores')
def getscores():
	game_name = request.args.get('game')
	if game_name in games:
		uns = get_game_scores(game_name)
		if uns:
			return uns
		else:
			e_print('NO_SCORES_FOR_GAME')
			return 'NO_SCORES_FOR_GAME'
	else:
		e_print('ERROR_GAME_NAME')
		return 'ERROR_GAME_NAME'

@app.route('/getstate', methods = ['GET'])
def getstate():
	if STATE == WAIT_NAME:
		return 'WAIT_NAME'
	elif STATE == PLAYING:
		return 'PLAYING'
	elif STATE == WAIT_START:
		return 'WAIT_START'
	elif STATE == IDLE:
		return 'IDLE'
	else:
		return 'ERROR'
	return 'ERROR'

if __name__ == '__main__':
	print('START SERVER OF ZGAME')
	try:
		clean_base()
	except Exception as e:
		e_print('CLEAN BASE:' + e)
	app.run(debug=False, host = '0.0.0.0', port=int('8484'))