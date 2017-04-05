import os
import random
class Data(object):
    def __init__(self, values, tag):
        self.values=values
        self.tag=tag
class DataSet(object):
    def __init__(self):
        self.tags=['updown', 'up','updown_stay', 'osci']
        self.tagToInt = {'updown':[1,0,0,0], 'up':[0,1,0,0],'updown_stay':[0,0,1,0], 'osci':[0,0,0,1]}
        self.dir_prefix = 'dataset'
        self.data = []
        for tag in self.tags:
            for filename in os.listdir(self.dir_prefix+'/'+tag):
                file = open(self.dir_prefix+'/'+tag+'/'+filename, 'r', encoding = "us-ascii")
                input = file.read().rstrip().replace(' ', ', ')
                self.data.append(Data(eval('['+input+']'), self.tagToInt[tag]))
                self.data[-1].values.append(self.data[-1].values[-1])
    def next_batch(self, size):
        xs=[]
        ys=[]
        for _ in range(0, size):
            index = random.randint(0, len(self.data)-1)
            xs.append(self.data[index].values)
            ys.append(self.data[index].tag)
        return xs, ys
