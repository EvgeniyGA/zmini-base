import os
import re
import sys 
import time
import requests
import subprocess

#print("Hello Gameboard")

def spell_phrase(phrase):
    if(phrase!=None):
        print(phrase)
    else:
        phrase = "noPhrase"

    return phrase

def main(arg):
    phraseOut = spell_phrase(arg)
    #print(phraseOut)

if __name__ == "__main__":
    main(sys.argv[1])
