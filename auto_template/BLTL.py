class Operator(object):
    def __init__(self, name, parent, id=None, value=0):
        self.name=name
        self.parent=parent
        self.sub=[]
        self.value=value
        self.id=id

    def to_str(self):
        string = self.name
        if self.value>0:
            string+='[%s]' % self.id
        string += '('
        for i in range(len(self.sub)-1):
            string+=self.sub[i].to_str()
            string+='&'
        string += self.sub[-1].to_str()
        string += ')'
        return string


class Prd(Operator):
    def __init__(self, index, parent, high=False):
        self.index = index
        self.parent=parent
        self.sub=None
        self.high=high
        self.left= float('inf')
        self.right=float('inf')

    def to_str(self, show_high=False, show_value=False):
        p_str = 'p'
        if show_high and self.high:
            p_str+='*'
        p_str+=str(self.index)
        if(show_value):
            p_str+=':%f:%f'%(self.left, self.right)
        return p_str

class Template(object):
    def __init__(self):
        self.root = Operator('', None)
        self.prds = [Prd(0, self.root)]
        self.time_bounds = {}
        self.root.sub.append(self.prds[0])
        self.prd_count = 1
        self.constraints = []
        self.constraints_str=None
    def get_formula(self):
        return self.root.to_str()

    def get_prds(self, show_high=False, show_value=False):
        return [p.to_str(show_high, show_value) for p in self.prds]

    def get_prds_paramset(self):
        params = []
        for p in self.prds:
            params.append('p'+str(p.index)+'.left:'+'%.4f'%(p.left))
            params.append('p'+str(p.index)+'.right:'+'%.4f'%(p.right))
        return params

    def get_time_bounds(self):
        return self.time_bounds

    def get_constraints(self):
        if self.constraints_str is None:
            self.constraints_str = []
            for i in range(len(self.constraints)):
                self.constraints_str.append(self.prds[i].to_str()+self.constraints[i]+self.prds[i+1].to_str())
        return self.constraints_str
