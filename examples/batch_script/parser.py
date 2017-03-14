'''
Created on 14 Mar 2017

@author: zhoujun
'''
""" 
preprocess: Remove space and separate into lines
"""

import re

class Parser:
    def __init__(self, bltl_str, constraint_str):
        self.constraints=[]
        self.bltl_str = bltl_str
        self.constraint_str=constraint_str
        self.parse_bltl()
        self.parse_constraint()
        
    def preprocess(self, s):
        return re.sub('[ \t]+', '', s).split('\n\n')


    def parse_constraint(self):
        if(self.constraint_str!=None):
            self.constraints = self.preprocess(self.constraint_str.rstrip())

    def parse_bltl(self):
        s = self.bltl_str.rstrip()
        inputLines = self.preprocess(s)
        self.get_prds(inputLines)
        self.bltls = inputLines[inputLines.index('Formulas:') + 1:]
        
    def get_prds(self, inputLines):
        self.prds = []
        index_start = inputLines.index('Predicates:') + 1
        index_end = inputLines.index('Formulas:')
        for s in inputLines[index_start:index_end]:
            self.prds.append(s)
