/* Test case for my first module. */
var spell = require("./spell");

var myspell = new spell.spell();
var myresult = myspell.quickspellcheck("fasxion"); /* Right now, only spellchecking one word at a time is supported. If you have a bunch of text, you'll want to split it into an array with a regex and iterate through it, checking each word with quickspellcheck() */
if (myresult == null) { console.log("Spelling is correct!"); }
else 
{
  console.log("Spelling is incorrect!");
  console.log(myresult); /* This array returned by quickspellcheck() contains suggestions you can pass to the user or use to auto-correct. */
}

