import numpy as np
from auto_template.BLTL import Operator, Prd, Template

class Mark(object):
    def __init__(self, begin, end, sign):
        self.begin=begin
        self.end=end
        self.sign=sign
    def merge(self, mark):
        if self.end != mark.begin:
            raise Exception("Can only merge the mark next to it! %d != %d" % (self.end, mark.begin));
        self.end = mark.end

    def __str__(self):
        return "(%d, %d, %d)" % (self.begin, self.end, self.sign)

# Partition derivatives into sections according to the sign of derivatives.
# Return list of tuples (begin, end, sign).
def partition(derivatives, threshold=0):
    pruned_derivatives = []
    for d in derivatives:
        if abs(d)<threshold:
            pruned_derivatives.append(0.0)
        else:
            pruned_derivatives.append(d)
    begin = 0
    end = 1
    sign = np.sign(pruned_derivatives[0])
    partitions = []
    for i in range(len(derivatives)):
        if np.sign(pruned_derivatives[i]) != sign:
            end = i
            partitions.append(Mark(begin, end, sign))
            begin = i
            sign = np.sign(pruned_derivatives[i])
    partitions.append(Mark(begin, len(pruned_derivatives), sign))

    return partitions


def threshold_partition(partitions, data, threshold, min_length=5):
    gap = np.max(data)-np.min(data)
    for mark in partitions:
        value_change_rate = abs((data[mark.end-1] - data[mark.begin])/gap)
        if value_change_rate < threshold:
            mark.sign = 0.0


def combine_partition(partitions, data, threshold=0.01, min_length=5):
    threshold_partition(partitions, data, threshold)
    index = 0
    while index+1 < len(partitions):
        if partitions[index].sign == partitions[index+1].sign or partitions[index+1].end-partitions[index+1].begin<min_length:
            partitions[index].merge(partitions[index+1])
            del partitions[index+1]
        else:
            index += 1


def template_gen(partitions):
    template = Template()
    current = template.root
    padding = 0
    index = 0
    for mark in partitions:
        if mark.sign == 0:
            to_add = Operator("G", current, id='k'+str(index), value=mark.end-mark.begin)
            template.time_bounds[to_add.id]=to_add.value
            to_add.sub.append(current.sub[0])
            to_add.sub[0].parent = to_add
            del current.sub[0]
            current.sub.insert(0, to_add)
            padding = to_add.value
        elif mark.sign > 0:
            to_add = Operator("F", current, id='k'+str(index), value=mark.end-mark.begin+padding)
            template.time_bounds[to_add.id]=to_add.value

            p = Prd(template.prd_count, to_add, True)
            template.prds.append(p)
            to_add.sub.append(p)
            current.sub.append(to_add)
            current = to_add
            template.prd_count+=1
            template.constraints.append("<")
            padding=0
        elif mark.sign < 0:
            to_add = Operator("F", current, id='k'+str(index), value=mark.end-mark.begin+padding)
            template.time_bounds[to_add.id]=to_add.value
            p = Prd(template.prd_count, to_add, False)
            template.prds.append(p)
            to_add.sub.append(p)
            current.sub.append(to_add)
            current = to_add
            template.prd_count+=1
            template.constraints.append(">")
            padding=0
        index+=1
    return template


def get_range(data, begin, end, high):
    data2 = data[:,begin:end]
    if high:
        maxis = np.max(data2, axis=1)
        left = np.min(maxis)
        right = np.max(maxis)
    else:
        mins = np.min(data2, axis=1)
        left = np.min(mins)
        right = np.max(mins)
    return left, right

def recursive_value_gen(bltl, data, start):
    for sub in bltl.sub:
        if isinstance(sub, Prd):
            if bltl.value > 0:
                sub.left, sub.right = get_range(data,start, start+bltl.value, sub.high)
        else:
            recursive_value_gen(sub, data, start+bltl.value)



def generate_init_prd(template, data):
    recursive_value_gen(template.root, data, 0)


