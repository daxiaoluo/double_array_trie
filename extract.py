import re
import string
import sys

read_file = open(sys.argv[1], 'r')
write_file = open(sys.argv[2], 'w')

strs = read_file.read()
s = re.findall("\w+", strs)
l = sorted(list(set(s)))
for st in l:
	m = re.search("\d+", st)
	n = re.search("\W+", st)
	if not m and not n:
		write_file.write(st + "\n")

write_file.close()
read_file.close()
