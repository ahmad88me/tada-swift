import sys
if len(sys.argv) == 1:
	print("expecting the dir of the csv file")
else:
	sep = ','
	f = open(sys.argv[1])
	first_line = 0
	error_found = False
	for idx, line in enumerate(f.readlines()):
		n = len(line.split(sep))
		if first_line != 0:
			if n!= first_line:
				error_found = True
				print("Error in line ("+str(idx+1)+")  found ("+str(n)+") instead of ("+str(first_line)+")")
		else:
			first_line = n
	if not error_found:
		print("congrats, your file is correct")



