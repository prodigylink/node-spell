/* Test case for my first module. This could be a REST spellchecking web service */

var spell = require("./spell");

var http = require('http');
http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'
  });

  var myspell = new spell.spell();
  var myresult = myspell.quickspellcheck("fasxion"); /* Right now, only spellchecking one word at a time is supported. If you have a bunch of text, you'll want to split it into an array with a regex and iterate through it, checking each word with quickspellcheck() */
  if (myresult == null) { res.write("Spelling is correct!"); }
  else 
  {
    res.write("Spelling is incorrect!");
    res.write(myresult); /* This array returned by quickspellcheck() contains suggestions you can pass to the user or use to auto-correct. */
  }

  res.end('\nTested node-spell (node-hunspell)\n');
}).listen(81);
console.log('Server running at http://127.0.0.1:8124/');


