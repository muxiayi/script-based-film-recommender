var express = require('express');
var router = express.Router();

var mysql = require('mysql');
var dbconfig = require('../db/DBConfig');
var userSQL = require('../db/Usersql');

var pool = mysql.createPool(dbconfig.mysql);

router.get('/addUser', function(req, res, next){
  pool.getConnection(function(err, connection) { 
    var param = req.query || req.params; 
    connection.query(userSQL.insert, [param.uid,param.name], function(err, result) { 
	if(result) { 
		result = { code: 200, msg:'Add Success' }; 
	}
        res.send(result);
	connection.release(); 
    }); 
  }); 
});


router.get('/showAllUser', function(req, res, next){
  pool.getConnection(function(err, connection) { 
    var param = req.query || req.params; 
    connection.query(userSQL.queryAll, function(err, result) { 
        res.send(result);
	connection.release(); 
    }); 
  }); 
});


router.get('/userInput', function(req, res, next){
  var param = req.query || req.params;
  var fs = require('fs');
  var path = "../userword.txt";
  var data = param.content;

  fs.writeFile(path, data, function(error) {
	  if(error)
	    console.error("write error:  " + error.message);
	  else
	    console.log("No writing error reported to userword.txt");
  	}
  );
  //res.send( {text:param.content} );   //just input text, return it back to front end to check

  var exec = require('child_process').exec;
  exec('./process_word', function(error) {
    if (!error) {
      //res.send( {text : 'Shell command Success: process_word exe'} );
    } else {
      //res.send( {text : error.message} );
    }
  });

  var text = fs.readFileSync('../similar_movie_from_word.txt','utf8');

  res.send({content:text});
});


router.post('/userInputScript', function (req, res) {
  var fs = require('fs');
  var path = "../usertext.txt";
  var data = req.body.text;
  //console.log(data);

  fs.writeFile(path, data, function(error) { //write what front end has passed to you
    if(error)
      console.error("write error:  " + error.message);
    else
      console.log("writing success reported to userscript.txt");
    }
  );
  //put cpp and py files in one doc, otherwise we cannot control the sequence of child processes 
  var exec = require('child_process').exec;
  exec('./process_text', function(error) {
    if (!error)
      console.log('select top 10 movies and return the 3 possible genres');
  });

  var text = fs.readFileSync('../similar_movie_from_script.txt','utf8');
  var three_genre_words = fs.readFileSync('../three_genre_words.txt','utf8');
/*
  //further we use python svm to do the recommendation
  var PythonShell = require('python-shell');
  PythonShell.run('./svm.py',function(err){
    if(err) console.log('executing python shell error!\n');
    else console.log('using svm.py finished');
  });

  exec('./match_genre', function(error) {
    if (error) console.log('match the indexes to the real names of categories');
    else console.log('matching index to genre success');
  });
*/
  var genre_names = fs.readFileSync('../genre_names.txt','utf8');


  res.send({content:text, genre:genre_names, words:three_genre_words});

});


router.post('/userInputQuery', function (req, res) {
  var fs = require('fs');
  var path = "./search_engine/cranfield-queries.txt";
  var data = req.body.text;

  console.log("user input query: " + data);

  fs.writeFile(path, data, function(error) { //write what front end has passed to you
    if(error)
      console.error("write error:  " + error.message);
    else
      console.log("writing success to cranfield-queries.txt");
    }
  );
/*
  //using python script to build the search engine
  const spawn = require('child_process').spawn;
  const ls = spawn('python', ['./search_engine/search_eval.py', './search_engine/config.toml']);
  ls.stdout.on('data', (data) => {
    console.log(`stdout: ${data}`);
  }); 

  ls.stderr.on('data', (data) => {
    console.log(`stderr: ${data}`);
  });

  ls.on('close', (code) => {
    console.log(`child process exited with code ${code}`);
  });
*/

  //using C++ to find the text according to index
  //but we also integrate py into cpp because two child processes cannot be ordered
  var exec = require('child_process').exec;
  exec('./search_result', function(error) {
    if (!error)
      console.log('mapping index to text for search engine output success');
    else
      console.log("mapping index to text error: " + error.message);
  });

  var texts = fs.readFileSync('./search_outcome.txt','utf8');
  console.log(texts);
  res.send({content:texts});
})


/* GET users listing. */
router.get('/', function(req, res, next) {
  res.send('respond with a resource');
});

module.exports = router;
