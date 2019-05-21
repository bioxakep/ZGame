import os
import sqlite3
import time

dirname, filename = os.path.split(os.path.abspath(__file__))
BASE_PATH = dirname + '/gamedata.db'
#print(BASE_PATH)
games = ['Zombie', 'Room', 'Graphics']

def clean_base():
	with sqlite3.connect(BASE_PATH) as connect:
		c = connect.cursor()
		for game in games:
			res = c.execute("""SELECT * FROM {}""".format(game))
			res = list(res)
			#print('Game {} {} lines'.format(game, len(res)))
			for r in res:
				c.execute("""DELETE FROM {} WHERE id=?""".format(game), [r[0]])
				#print('Delete {} line'.format(list(r)))
				connect.commit()

if __name__ == '__main__':
	if input('Are you really want to clean base of scores? (type YES to clean) ') == 'YES':
		try:
			clean_base()
			print('Clean Done, exit...')
			time.sleep(1)
		except Exception as e:
			print('Error: ', e)
	else:
		print('Clean cansel, exit...')
		time.sleep(1)