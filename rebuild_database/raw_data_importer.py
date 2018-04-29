# Used to parse raw data and convert them to the database schema.

#import data_util
#import game_info
import math
import os

def ParseTopicWords():
  genre_list = []

  current_path = os.getcwd()
  topic_words_filename = current_path + '/topic_words.txt'
  with open(topic_words_filename) as f:
    next(f)
    for line in f:
      splitted = line.split(',')
      if len(splitted) != 11:
        continue
      genre_name = splitted[0]

      word1 = splitted[1]
      word2 = splitted[2]
      word3 = splitted[3]
      word4 = splitted[4]
      word5 = splitted[5]
      word6 = splitted[6]
      word7 = splitted[7]
      word8 = splitted[8]
      word9 = splitted[9]
      word10 = splitted[10]
      genre_list.append( (genre_name, word1, word2, word3, word4, word5, word6, word7, word8, word9, word10) )
  return genre_list

def IngestToDatabase(cursor):
  genre_list = ParseTopicWords()

  for genre in genre_list:
    cursor.execute('''
      INSERT INTO TOPIC_WORDS
      VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)
    ''', genre)
  print len(genre_list), 'rows in TOPIC_WORDS ingested'

if __name__ == '__main__':
  genre_list = ParseTopicWords()

  #for item in unmatched_list:
  #  print item
