# Script to reconstruct the database from scratch.

import MySQLdb
import os
import raw_data_importer

def main():
  connection = MySQLdb.connect(host='localhost',    # your host, usually localhost
                               user='root',        # your username
                               passwd='',      # your password
                               db='FilmRecommendation')         # name of the database
  cursor = connection.cursor()

  cursor.execute('''
    DROP TABLE IF EXISTS USER, TOPIC_WORDS
  ''')

  cursor.execute('''
    CREATE TABLE USER(
      uid text,
      userName text
    )
  ''')

  cursor.execute('''
    CREATE TABLE TOPIC_WORDS(
      Genre text,
      word1 text,
      word2 text,
      word3 text,
      word4 text,
      word5 text,
      word6 text,
      word7 text,
      word8 text,
      word9 text,
      word10 text
    )
  ''')

  raw_data_importer.IngestToDatabase(cursor)


  connection.commit()
  connection.close()

  print 'Rebuild database successfully!'

if __name__ == '__main__':
  main()
