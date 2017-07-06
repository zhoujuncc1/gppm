import os
import random
class Data(object):
    def __init__(self, values, tag):
        self.values=values
        self.tag=tag
class DataSet(object):
    def __init__(self, foldernames):
        self.tags=['updown', 'up','updown_stay', 'osci']
        self.tagToInt = {'updown':[1,0,0,0], 'up':[0,1,0,0],'updown_stay':[0,0,1,0], 'osci':[0,0,0,1]}
        self.data = []
        self.data_by_tag={}
        self.index = 0
        for tag in self.tags:
            self.data_by_tag[tag]=[]
        for dir_prefix in foldernames:
            for tag in self.tags:
                for filename in os.listdir(dir_prefix+'/'+tag):
                    if 'txt' in filename:
                        file = open(dir_prefix+'/'+tag+'/'+filename, 'r')
                        d = Data(eval(file.read()), self.tagToInt[tag])
                        padding = 200-len(d.values)
                        if padding < 0:
                            d=d[:200]
                        for _ in range(0, padding):
                            d.values.append(d.values[-1])
                        self.data.append(d)
                        self.data_by_tag[tag].append(d)
        random.shuffle(self.data)
        self.size = len(self.data)
    def random_batch(self, size):
        xs=[]
        ys=[]
        for _ in range(0, size):
            index = random.randint(0, len(self.data)-1)
            xs.append(self.data[index].values)
            ys.append(self.data[index].tag)
        return xs, ys
    def next_batch(self, size):
        xs=[]
        ys=[]
        for _ in range(0, size):
            xs.append(self.data[self.index].values)
            ys.append(self.data[self.index].tag)
            self.index= (self.index+1) % self.size
        return xs, ys

    def get_by_tag(self, tag):
        xs=[]
        ys=[]
        for d in self.data_by_tag[tag]:
            xs.append(d.values)
            ys.append(d.tag)
        return xs, ys
