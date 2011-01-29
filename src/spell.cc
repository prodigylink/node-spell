/*
This is my first node.js module. ;+)

See README.txt.

NOTE: This module is the Minimal Viable Product of software.

You will probably need to modify the 'Hunspell * pMS= new Hunspell();' line
if you need to support different languages or want to change the path of the dictionary files.

This code could use some refactoring to improve performance.

This will be fixed soon.

Copyright (C) <year> by <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


*/
#include <v8.h>
#include <node.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hunspell.hxx"

using namespace node;
using namespace v8;

const char* stringArgToStr(const v8::Local<v8::Value> arg)
{ 
  v8::String::AsciiValue v8Str(arg); 
  char *cStr = (char*) malloc(strlen(*v8Str) + 1); /* Potentially suboptimal */
  strcpy(cStr, *v8Str); 
  return cStr; 
} 

class spell: ObjectWrap
{
private:
  int m_count;
public:

  static Persistent<FunctionTemplate> s_ct;
  static void Init(Handle<Object> target)
  {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("spell"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "quickspellcheck", QuickSpellCheck);

    target->Set(String::NewSymbol("spell"),
                s_ct->GetFunction());
  }

  spell() :
    m_count(0)
  {
  }

  ~spell()
  {
  }

  static Handle<Value> New(const Arguments& args)
  {
    HandleScope scope;
    spell* hw = new spell();
    hw->Wrap(args.This());
    return args.This();
  }

  static Handle<Value> QuickSpellCheck(const Arguments& args)
  {
    HandleScope scope;
    if (args.Length() < 1) { return ThrowException(Exception::Error(String::New("node-spell (hunspell binding): quickspellcheck() needs a string with a word to spellcheck."))); }
    spell* hw = ObjectWrap::Unwrap<spell>(args.This());
    hw->m_count++;
    Local<Array> arr = Array::New();
    Hunspell *pMS = new Hunspell("en_US.aff", "en_US.dic"); /* Change this as needed. TODO: Expose function to node.js for selecting dictionary files. */
    const char *wordp;
    wordp = stringArgToStr(args[0]);
    int dp = pMS->spell(wordp);
    if (dp) { delete pMS; return Null(); } /* Spelling of the word was correct */
    else
    {
      /* School 'em */
      char ** wlst;
      int ns = pMS->suggest(&wlst, wordp);
      for (int i=0; i < ns; i++)
      {
        arr->Set(Integer::New(i), String::New(wlst[i]));
      }
      pMS->free_list(&wlst, ns);
      delete pMS; /* Suboptimal */
      return arr; /* Spelling of the word was incorrect, returning suggestions arra */
    } 
  }

};

Persistent<FunctionTemplate> spell::s_ct;

extern "C" {
  static void init (Handle<Object> target)
  {
    spell::Init(target);
  }

  NODE_MODULE(spell, init);
}

