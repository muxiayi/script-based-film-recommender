#!/bin/sh

WORKING_DIR="../data/"
EXISTING_FILES="../data/raw2015"
EXISTING_ARCH="../data/imsdb_raw_nov_2015.zip"
UNZIP_FIRST="../data/imsdb_raw_nov_2015"
TRASH_DIR="../data/__MACOSX"
WGET1='wget nldslab.soe.ucsc.edu/film_corpus_2/imsdb_raw_nov_2015.zip'

CPP_DIR="../process_database.cpp"
CPP_OBJ="../process_database"
EXEC_FILE="process_database"
PREV="../"

## change to working directory and cleanup any downloaded files and extracted rules in modsec/ directory
cd $WORKING_DIR
rm -rf $EXISTING_FILES
rm -f $EXISTING_ARCH

## wget1 to download VERSION file
`$WGET1`

## extract archive
unzip $EXISTING_ARCH

## rename the folder
mv $UNZIP_FIRST $EXISTING_FILES
## remove trash dir produced by macos
rm -rf $TRASH_DIR

##building and running cpp file
g++ $CPP_DIR -o $CPP_OBJ -std=c++11

#executing the cpp obj file
cd $PREV
./$EXEC_FILE