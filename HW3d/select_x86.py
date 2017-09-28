import operator
from collections import defaultdict

REGISTERS = {0: 'undefined',
             1: '%esi',
             2: '%edi',
             3: '%ebx',
             4: '%edx',
             5: '%ecx',
             6: '%eax'}

def create_env(flatast):
    env = {}
    x = 0
    for i in flatast:
        x += 4
        env[i[0]] = x
    return env

class x86bits():
    def __init__(self, fast):
        self.instructions = []
        self.x86_strings = []
        self.liveness = []
        self.env = create_env(fast)
        self.inter_graph = defaultdict(set)
        self.spill = {}
        self.colors = {}
        self.saturation = {}
        for var in self.env:
            self.colors[var] = 0
            self.saturation[var] = range(1, 7)

    def clean(self):
        '''
        if we have generated spill code we need to rerun our algorithms
        :return:
        '''
        self.instructions = []
        self.x86_strings = []
        self.liveness = []
        self.inter_graph = defaultdict(set)
        self.spill = {}
        self.colors = {}
        self.saturation = {}
        for var in self.env:
            self.colors[var] = 0
            self.saturation[var] = range(1, 7)

def const_or_var(env, val):
    if val in env.keys():
        return '{}'.format(val)
    else:
        return '${}'.format(val)

def get_loc(x86obj, var):
    if '%' in var or '$' in var:
        return var
    else:
        if var in x86obj.spill:
            print ('the value is {}'.format(x86obj.spill[var]))
            return x86obj.spill[var]
        else:
            return REGISTERS[x86obj.colors[var]]

def replace_spilled_vars(fast, x86obj):
    #loop throught the spilled vars should only be one
    for key, val in x86obj.spill.items():
        #find the instances of that var in the fast and replace it with stack location
        for instruction in fast:
            indicies = [j for j, x in enumerate(instruction) if x==key]
            if len(indicies) != 0:
                for idx in indicies:
                    instruction[idx] = val
            else:
                pass
    return fast




def generate_x86_IR(flatast, x86obj):
    #x86obj.instructions.append(['subl', '${}'.format(4*len(x86obj.env)), '%esp'])
    for i in flatast:
        if (i[1] == "Assign"):
            x86obj.instructions.append(['movl', '{}'.format(const_or_var(x86obj.env, i[2])), '{}'.format(i[0])])
        elif (i[1] == "Call"):
            x86obj.instructions.append(['call', 'input'])
            x86obj.instructions.append(['movl', '%eax', '{}'.format(i[0])])
        elif (i[1] == 'Neg'):
            x86obj.instructions.append(['movl', '{}'.format(const_or_var(x86obj.env, i[2])), '{}'.format(i[0])])
            x86obj.instructions.append(['negl', '{}'.format(i[0])])
        elif (i[1] == 'Add'):
            x86obj.instructions.append(['movl', '{}'.format(const_or_var(x86obj.env, i[2])), '{}'.format(i[0])])
            x86obj.instructions.append(['addl', '{}'.format(const_or_var(x86obj.env, i[3])), '{}'.format(i[0])])
        elif (i[1]=="Print"):
            x86obj.instructions.append(['pushl', '{}'.format(const_or_var(x86obj.env, i[2]))])
            x86obj.instructions.append(['call', 'print_int_nl'])
            x86obj.instructions.append(['addl', '$4', '%esp'])
        else:
            pass

# Generates a list of liveness sets. The list grows "downward" since sets are
# added for each instruction in reverse order.
def generate_liveness(x86obj):
    x86obj.liveness.insert(0, set([]))
    for idx, instruction in reversed(list(enumerate(x86obj.instructions))):
        # Get the previous set we just added, it will be on "top" of the list
        prev_set = x86obj.liveness[0].copy()

        if (instruction[0] == "movl"):
            # variable at [2] is being written to, remove it from set
            if bool(prev_set):
                if('%' not in instruction[2]):
                    prev_set.remove(instruction[2])
            # [1] is being read, if it's a variable, add it to the set
            new_set = set([])
            if '$' not in instruction[1] and 'eax' not in instruction[1]:
                new_set.add(instruction[1])
            # union the new variables with the remaining old variables
            unioned_set = new_set.union(prev_set)
            x86obj.liveness.insert(0, unioned_set)

        elif (instruction[0] == "addl"):
            # both variables are read in an addl, don't remove any
            new_set = set([])
            if 'esp' not in instruction[2] and 'ebp' not in instruction[2]:
                new_set.add(instruction[2])
            if '$' not in instruction[1]:
                new_set.add(instruction[1])
            # union the new variables with the remaining old variables
            unioned_set = new_set.union(prev_set)
            x86obj.liveness.insert(0, unioned_set)

        elif (instruction[0] == "negl"):
            new_set = set([])
            # we are effectively reading the variable to negate it
            if '$' not in instruction[1]:
                new_set.add(instruction[1])
            unioned_set = new_set.union(prev_set)
            x86obj.liveness.insert(0, unioned_set)

        elif (instruction[0] == "call"):
            x86obj.liveness.insert(0, prev_set)

        elif (instruction[0] == "pushl"):
            new_set = set([])
            if '$' not in instruction[1]:
                new_set.add(instruction[1])
            unioned_set = new_set.union(prev_set)
            x86obj.liveness.insert(0, unioned_set)

        else:
            raise Exception('Error in generate_liveness: unrecognized instruction {}'.format(instruction))

    # adjust the list to make every element the Lafter(k) set
    del x86obj.liveness[0]

def generate_inter_graph(x86obj):
    for instruction, live_set in zip(x86obj.instructions, x86obj.liveness):
        if len(live_set) > 0:
            if (instruction[0] == "movl"):
                for v in live_set:
                    if ('%' in v):
                        pass
                    else:
                        if (v != instruction[1] and v != instruction[2]):
                            x86obj.inter_graph[instruction[2]].add(v)
                            x86obj.inter_graph[v].add(instruction[2])
            elif (instruction[0] == "addl"):
                if 'esp' not in instruction[2]:
                    for v in live_set:
                        if ('%' in v):
                            pass
                        else:
                            if v != instruction[2]:
                                x86obj.inter_graph[instruction[2]].add(v)
                                x86obj.inter_graph[v].add(instruction[2])
            elif (instruction[0] == "negl"):
                print(instruction)
                for v in live_set:
                    if ('%' in v):
                        pass
                    else:
                        if v != instruction[1]:
                            x86obj.inter_graph[instruction[1]].add(v)
                            x86obj.inter_graph[v].add(instruction[1])
            elif (instruction[0] == "call"):
                for v in live_set:
                    if ('%' in v):
                        pass
                    else:
                        x86obj.inter_graph['eax'].add(v)
                        x86obj.inter_graph['ecx'].add(v)
                        x86obj.inter_graph['edx'].add(v)
                        x86obj.inter_graph[v].add('eax')
                        x86obj.inter_graph[v].add('ecx')
                        x86obj.inter_graph[v].add('edx')
            elif(instruction[0] == 'pushl'):
                pass
            else:
                raise Exception('Error in inerference_graphs: unrecognized instruction {}'.format(instruction))
        else:
            pass

def color_graph(x86obj):
    stack_loc = 0
    x86obj.spill = {}
    # sort the list of variables by length of their pencil marks in ascending order
    sorted_list = sorted(x86obj.saturation.items(), key=lambda item: len(item[1]))

    # loop until we've removed every var from the list
    while sorted_list:
        #grab a node from the sorted list
        #sat_map structure is ('var', [1, 2, 3, 4, 5, 6])
        sat_map = sorted_list[0]
        # if there are no options left, we must spill
        if not sat_map[1]:
            stack_loc += 4
            x86obj.spill[sat_map[0]] = '-{}(%ebp)'.format(stack_loc)
            return
        else:
            color = sat_map[1][0]
            x86obj.colors[sat_map[0]] = color
            # remove the chosen color from all interfering variables
            for node, sat in x86obj.saturation.items():
                #if this is not the node we are on
                if node != sat_map[0]:
                    #if the node we are looking at is in the interference graph of another node
                    if sat_map[0] in x86obj.inter_graph[node]:
                        if color in sat:
                            sat.remove(color)

        # remove the var and re-sort the list
        sorted_list.remove(sat_map)
        sorted_list = sorted(sorted_list, key=lambda item: len(item[1]))

    print ("spilled: {}".format(x86obj.spill))



def stringify_x86(x86obj):
    for i in x86obj.instructions:
        if (i[0] == 'movl'):
            #remove trivial moves
            if get_loc(x86obj, i[1]) == get_loc(x86obj, i[2]):
                pass
            else:
                x86obj.x86_strings.append('{} {}, {}'.format(i[0], get_loc(x86obj, i[1]), get_loc(x86obj, i[2])))
        elif (i[0] == 'addl'):
            x86obj.x86_strings.append('{} {}, {}'.format(i[0], get_loc(x86obj, i[1]), get_loc(x86obj, i[2])))
        elif (i[0] == 'negl' or i[0] == 'pushl'):
            x86obj.x86_strings.append('{} {}'.format(i[0], get_loc(x86obj, i[1])))
        elif (i[0] == 'call'):
            x86obj.x86_strings.append('{} {}'.format(i[0], i[1]))




def make_assembly(fast):
    global REGISTERS

    x86obj = x86bits(fast)

    print "\nFlat AST:"
    for tac in fast:
        print tac
    print "\nEnvironment:\n", x86obj.env

    generate_x86_IR(fast, x86obj)

    print "\nx86:"
    for instruction in x86obj.instructions:
        print instruction

    print "\ngenerate_liveness():"
    generate_liveness(x86obj)
    print "\ngenerate Interference():"
    generate_inter_graph(x86obj)

    print "\nx86 --> Liveness:"
    for instruction, live_set in zip(x86obj.instructions, x86obj.liveness):
        print instruction, " --> ", list(live_set)

    print "\nx86 Interference graph:"
    print x86obj.inter_graph

    # TODO: May not need this list sorted, leaving so we don't forget how
    # sorted_inter_graph = sorted(x86obj.inter_graph.items(), key=lambda item: len(item[1]), reverse=True)
    # print "\nSorted inter_graph:\n", sorted_inter_graph

    print "\ncolor_graph():"
    color_graph(x86obj)


    while len(x86obj.spill) > 0:
        print ("spilled: {}".format(x86obj.spill))
        new_fast = replace_spilled_vars(fast, x86obj)
        x86obj.clean()
        generate_x86_IR(new_fast, x86obj)
        generate_liveness(x86obj)
        color_graph(x86obj)
        print "\nColors:"
    print x86obj.colors

    print "\nstringify_x86():"
    stringify_x86(x86obj)

    print "\nx86 Strings"
    for i, str in zip(x86obj.instructions, x86obj.x86_strings):
        print i, " --> ", str

    x86obj.x86_strings.insert(0, 'movl %esp, %ebp')
    x86obj.x86_strings.insert(0, 'pushl %ebp')
    x86obj.x86_strings.insert(0, 'main:')
    x86obj.x86_strings.insert(0, '.global main')

    #x86obj.x86_strings.append(['addl', '${}'.format(4 * len(x86obj.env)), '%esp'])
    x86obj.x86_strings.append('movl $0, %eax')
    x86obj.x86_strings.append('leave')
    x86obj.x86_strings.append('ret')

    for str in x86obj.x86_strings:
        print str

    return x86obj.x86_strings
