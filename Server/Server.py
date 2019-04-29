#!/usr/bin/python3
# -*- coding: utf-8 -*-
import os
from datetime import datetime
from flask import Flask
from flask import request
import sqlite3
import logging
import json
import csv

SHOW_RECS = 0x01
WAIT_NAME = 0x02
WAIT_START = 0x03
PLAYING = 0x04

STATE = SHOW_RECS

dirname, filename = os.path.split(os.path.abspath(__file__))
BASE_PATH = dirname + '/gamedata.db'
# 'Dropbox/Rafael/ZGame/Server/gamedata.db'

#0	  1 	2		3		4		5		6		7		8	  9		10	  11	 12		13	  14	 15	  16	 17 	 18
#ID, DATE, NAME, SCORES, RADIO, GEN/FUEL, GEN/RUN, METER, CODE, FUSES, DOOR, WINDOW, GAS, SHELF, E.M.P, MAP, FLARE, ZOMBIE, TOTAL_TIME
app = Flask(__name__)
curr_id = -1
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

# ========================= MASTER ROUTES =========================
@app.route('/rst', methods = ['GET'])
def reset():
	global STATE
	global curr_id
	STATE = WAIT_NAME
	curr_id = -1
	e_print('MASTEROPERATOR ONLINE, WAIT RUN GAME FROM MASTER')
	return 'OK'

@app.route('/startgame', methods = ['GET'])
def prepare():
	global STATE
	if STATE == WAIT_NAME or STATE == WAIT_START:
		STATE = PLAYING
		e_print('SERVER GO TO PLAYING STATE')
		return 'OK'
	else:
		e_print('ERROR WHILE START GAME')
		return 'errorPost'

@app.route('/endgame', methods = ['POST'])
def endgame():
	global curr_id
	global STATE
	if curr_id > -1 and STATE == PLAYING:
		try:
			g_data = list(request.form['gdata'].split(','))
			total_scores = request.form['scores']
			if len(g_data) == 14:
				data_list = list()
				data_list.extend(g_data)
				data_list.append(str(sum([int(g) for g in g_data])))
				data_list.append(str(curr_id))
				with sqlite3.connect(BASE_PATH) as connect:
					c = connect.cursor()
					c.execute("""UPDATE Zombie SET RADIO = ?, FUEL = ?, RUN = ?, METER = ?, CODE = ?, FUSES = ?, DOOR = ?, \
						WINDOW = ?, GAS = ?, SHELF = ?, EMP = ?, MAP = ?, FLARE = ?, ZOMBIE = ?, TOTAL_TIME = ? WHERE ID == ?""",data_list)
					print('Command data updated')
					connect.commit()
					curr_id = -1
					STATE = SHOW_RECS
					print('SERVER GO TO SHOW_RECS STATE')
				return 'game stop'
			else:
				e_print('WRONG GADGETS DATA RECIEVED')
				return 'WRONG GADGETS DATA RECIEVED'
		except Exception as e:
			print(e)
			return 'ERROR_WHILE_DB_WRITE'
	else:
		e_print('GAME NOT RUNNING')
		return 'GAME NOT RUNNING'


@app.route('/getname', methods = ['GET'])
def getname():
	global curr_id
	if STATE == WAIT_START:
		with sqlite3.connect(BASE_PATH) as connect:
			c = connect.cursor()
			cmd_res = c.execute("""SELECT NAME FROM Zombie WHERE id=?""", [curr_id]).fetchall()
			name = cmd_res[0][0]
			if len(name) > 0:
				e_print('NAME SEND TO MASTEROPERATOR: {}'.format(name))
				return name
	elif STATE == WAIT_NAME:
		e_print('SERVER WAIT NAME FROM SCORES')
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
	global curr_id
	global STATE
	if request.method == 'POST' and curr_id == -1 and STATE == WAIT_NAME:
		cName = request.form['cname']
		dt = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
		if len(cName) > 0:
			try:
				with sqlite3.connect(BASE_PATH) as connect:
					c = connect.cursor()
					res = c.execute("""SELECT * from Zombie""").fetchall()
					next_id = -1
					if len(res) > 0:
						for r in res:
							if r[0] > next_id:
								next_id = r[0]
					next_id += 1
					c.execute("""INSERT INTO Zombie VALUES (?,?,?,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)""",[next_id, dt, cName])
					connect.commit()
					curr_id = next_id
					STATE = WAIT_START
					print('CMD NAME: {}, WAIT START FROM MASTER'.format(cName))
			except Exception as e:
				e_print('SET_NAME_ERROR: {}'.format(e))
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
	elif STATE == SHOW_RECS:
		return 'SHOW_RECS'
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