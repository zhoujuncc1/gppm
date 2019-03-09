import socket
import struct

import sys
import opentuner
from opentuner.api import TuningRunManager
from opentuner.measurement.interface import DefaultMeasurementInterface
from opentuner.resultsdb.models import Result
from opentuner.search.manipulator import ConfigurationManipulator
from opentuner.search.manipulator import IntegerParameter, FloatParameter
import argparse


HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 3333        # The port used by the server

def getScore(parameters):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        param_str = str(parameters)
        param_str = param_str.replace('\"', '')
        param_str = param_str[1:-1]
        s.sendall(bytes(param_str, 'utf-8'))
        data = s.recv(8)
    return struct.unpack('<d', data)[0]

def getParameters():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.send(b'!')
        data = s.recv(1024)
    return eval(data)

def tuneProperty():
    #Initialization
    parser = argparse.ArgumentParser(parents=opentuner.argparsers())
    args = parser.parse_args()
    manipulator = ConfigurationManipulator()
    
    parameters = getParameters()
    
    for s in parameters[0]:
        params = s.split(':')
        manipulator.add_parameter(FloatParameter(params[0], float(params[1]), float(params[2])))
    for s in parameters[1]:
        params = s.split(':')
        manipulator.add_parameter(IntegerParameter(params[0], float(params[1]), float(params[2])))
    interface = DefaultMeasurementInterface(args=args, manipulator=manipulator, project_name='examples', program_name='api_test', program_version='0.1')
    api = TuningRunManager(interface, args)
    i=0
    while i < 5000:
        desired_result = api.get_next_desired_result()
        if desired_result is None:
            break
        cfg = desired_result.configuration.data
        score=getScore(cfg)
        print(score)
        if score < 1e6:
            i+=1
        result = Result(time=score)
        api.report_result(desired_result, result)

    beststate = api.get_best_configuration()
    bestloss = getScore(beststate)
    return beststate, bestloss

if __name__ == '__main__':
    print(tuneProperty())


