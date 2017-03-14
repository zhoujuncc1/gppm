'''
Created on 5 Apr 2016

@author: zhoujun
'''

from parser import Parser
import sys

import xml.etree.ElementTree as ET
import subprocess as sp


def run(variable, template, model, xml_filename):
    inputfilename = "input_%d.xml" % variable
    outputfilename = "output_%d.xml" % variable

    parser = Parser(template.generate_bltl(variable), template.generate_constraint())

    xmlData = ET.parse(xml_filename)
    updateXml(parser, xmlData)
    xml_string = ET.tostring(xmlData.getroot())
    xml_string=xml_string.replace("&amp;", "&")
    infile=open(inputfilename, "w")
    infile.write(xml_string)
    infile.close()
    command = "../%s %s >> %s"% (model, inputfilename, outputfilename)
    return Popen(command, shell=True)


def updateXml(parser, xmlData):
    prds = xmlData.find("prds")
    for prd in parser.prds:
        xml_prd = ET.SubElement(prds, "prd")
        xml_prd.text= prd
    bltls = xmlData.find("bltls")
    for bltl in parser.bltls:
        xml_prd = ET.SubElement(bltls, "bltl")
        xml_prd.text=bltl
    
    constraints = xmlData.find("constraints")
    for constraint in parser.constraints:
        xml_prd = ET.SubElement(constraints, "constraint")
        xml_prd.text=constraint
def parseXML(filename):
    tree = ET.parse(filename)
    return tree

def batch(model, variables, templates, xml_filename):
    ps = []
    for i in range(0, len(variables)):
        ps.append(run(variables[i], templates[i], model, xml_filename))
    for p in ps:
        p.wait()

def main(batch_filename):
    with open(batch_filename) as f:
        content = f.readlines()
    model = content[0]
    xml_filename = content[1]
    variables = eval(content[2])
    templates = eval[content[3]]
    batch(model, variables, templates, xml_filename)
    
if __name__=='__main__':
    if(len(sys.argv)==1):
        print "Input file with 4 lines: model=str, xml_filename=str, variables=[], templates=[]"
    main(sys.argv[1])
