

output_fname = raw_input("Enter the output filename: ")
search_text = raw_input("Enter the inclusion text: ")
f = open("features.tsv","r")
included_lines = []
for line in f.readlines():
	if search_text in line:
		included_lines.append(line)
	
print("found lines: %d" % len(included_lines))
outf = open(output_fname, "w")
for line in included_lines:
	outf.write(line)

outf.close()

