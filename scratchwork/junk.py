x=1
class A:
    x = 2
    def __init__(self):    #f_init1
        self.x = 3
    def m(a):              #f_m1
        return x
class B(A):
    x = 4                  
    def __init__(self):    #f_init2
        self.x = 5
    b = A.m

a = A()
b = B()
print b.b()
class B(B):
    x = 6
    def __init__(self):    #f_init3
        x = 7
    def m(self):           #f_m2
        return self.x
    b = B.b
b = B()
print b.b()
