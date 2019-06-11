import os
import sqlite3
import time
import sys

dirname, filename = os.path.split(os.path.abspath(__file__))
BASE_PATH = dirname + '/gamedata.db'
#print(BASE_PATH)
games = ['Zombie', 'Room', 'Graphics']

def clean_base(game_number):
	with sqlite3.connect(BASE_PATH) as connect:
		c = connect.cursor()		
		res = c.execute("""SELECT * FROM {}""".format(games[game_number]))
		res = list(res)
		#print('Game {} {} lines'.format(game, len(res)))			
		for r in res:
			c.execute("""DELETE FROM {} WHERE id=?""".format(games[game_number]), [r[0]])
			#print('Delete {} line'.format(list(r)))
			connect.commit()

if __name__ == '__main__':
	game_count = 0
	for game in games:
		game_count += 1
		print('{}. {}'.format(game_count, game))
	game_number = input('Select number of game to clean scores: ')
	if game_number in [str(a) for a in range(1,len(games))]:
		try:
			clean_base(int(game_number))
			print('Clean Done, exit...')
			time.sleep(1)
		except Exception as e:
			print('Error: ', e)
	else:
		print('Clean cansel, exit...')
		time.sleep(1)