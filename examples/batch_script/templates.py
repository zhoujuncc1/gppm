'''
Created on 10 Aug 2016

@author: zhoujun
'''

class Template(object):
    '''
    classdocs
    '''


    def __init__(self, name, bltl_template, constraint_str):
        '''
        Constructor
        '''
        self.name = name
        self.bltl_template = bltl_template
        
        self.constraint_str = constraint_str
        self.weight = {}
    def generate_bltl(self, *args):
        result = self.bltl_template.format(*args)
        return result
    def generate_constraint(self):
        return self.constraint_str

class Templates(object):
    templates = {}
    templates['up'] = Template('up',
                         '''Predicates:
        
                        p1:{0}::
        
                        p2:{0}::
    
                        Formulas:

                        p1
                        
                        (F[k1]G[k2]p2)
                        ''',
                        'p2>p1'
                        )
    templates['pfp'] = Template('pfp',
                         '''Predicates:
        
                        p1:{0}::
        
                        p2:{0}::
    
                        Formulas:

                        p1
                        
                        (F[k1]p2)
                        ''',
                        '''p2>p1'
                        
                        p2>p1'''
                        )
    templates['updown_stay'] = Template('updown_stay',
                         '''Predicates:
    
                        p1:{0}::
    
                        p2:{0}::
    
                        p3:{0}::
    
                        Formulas:
    
                        p1
                        
                        F[k1](p2 & F[k2]G[k3]p3)''',
                         
                         '''p2>p3
                         
                         p2>p3

                         p2>p1
                         
                         p2>p1'''
        				)
      
    templates['updown'] = Template('updown',
                         '''Predicates:
    
                        p1:{0}::
    
                        p2:{0}::
    
                        p3:{0}::
    
                        Formulas:
    
                        p1
                        
                        F[k1](p2 & F[k2]p3)''',
                        '''p2>p3
                         
                         p2>p3

                         p2>p1
                         
                         p2>p1'''
        				)
    
    templates['osci'] = Template('5',
                          '''Predicates:
     
                         p1:{0}::
     
                         p2:{0}::
     
                         p3:{0}::
     
                         p4:{0}::
                         
                         Formulas:
     
                         p1 
                         
                         F[k1](p2 & F[k2](p3 & F[k3]p4))''',
 						'''p3<p2
                         
                         p3<p4'''
                         
         )
    
if __name__ == '__main__':
    t = Templates.templates['6']
    result = t.generate_bltl('x1')
    print result
    
    
