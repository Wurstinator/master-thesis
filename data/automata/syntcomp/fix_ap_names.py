import glob, os, re

def make_ap_line(ap_count):
    return 'AP: ' + str(ap_count) + ' ' + ' '.join(['"a' + str(i) + '"' for i in range(1, ap_count+1)]) + '\n'

def process_textline(line):
    matches = re.search("AP: (\d*)", line)
    if matches is None:
        return line
    else:
        return make_ap_line(int(matches[1]))

for filename in glob.glob('dpas/*.hoa'):
    with open(filename, 'r') as file:
        contents = file.readlines()
    with open(filename, 'w') as file:
        file.write(''.join(map(process_textline, contents)))
