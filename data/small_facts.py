
from analyze_common import filelines_to_json


# For each input automaton, prints the type of construction that had the best
# result for that input.

# Read all json data.
c_types = ['schewe', 'fritzwilke', 'iterated_moore']
g_types = ['gendet_ap1', 'gendet_ap2', 'gendet_ap3', 'detnbaut_ap1', 'detnbaut_ap2', 'detspot_ap1', 'detspot_ap2']
input_association = dict()
for c_type in c_types:
    for g_type in g_types:
        file = 'raw/' + c_type + '/' + g_type + '.json'
        for j in filelines_to_json(file):
            if (j['input_file'] not in input_association):
                input_association[j['input_file']] = dict()
            input_association[j['input_file']][c_type] = j

# input_association is now a dict
#  input_filename -> dict
#    from (c_type, g_type) -> to data as Json

def print_reduction_table(input_association):
    for input in input_association:
        def foo(c_type):
            data = input_association[input]
            if c_type not in data:
                return '?\t'
            else:
                data = data[c_type]
            return str(data['original_size'] - data['new_size']) + '\t'
        x = input_association[input]
        print(*list(map(foo, c_types)))

def print_best_algorithm(input_association):
    for input in input_association:
        best_reduction = 0
        for c_type, data in input_association[input].items():
            shrink = data['original_size'] - data['new_size']
            if shrink > best_reduction:
                best_reduction = shrink
                best_reduction_type = c_type

        if best_reduction == 0:
            print('No reduction. ', input)
        else:
            print(best_reduction_type, input)


print_best_algorithm(input_association)



