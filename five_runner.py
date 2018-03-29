import sys
import os

def words_checker():
	if ('-h' in sys.argv or '--help' in sys.argv):
		print("counter.cpp\nv1.0\nMade by Roman Stepaniuk and Arsen Tymchyshyn\nCounts number of occurrences of the words from the file and shows time of execution"\
        "Argument should be name of txt file with arguments or <-auto> and default file conf.txt will be used\nOtherwise default arguments will be used")
		return 0
	five_times = []
	five_texts = []
	#thr = []
	for i in range(5):
		os.popen("g++ words_counter.cpp -o words_counter")
		if len(sys.argv) == 2:
			str_cmd = "words_counter {a1}".format(a1=sys.argv[1])
		else:
			str_cmd = "words_counter"
		p = os.popen(str_cmd)
		for j in p.readlines():
			if str(j).startswith("Total"):
				five_times.append(float(str(j).split()[-2]))
			#elif str(j).startswith("Thread"):
			#	thr.append(int(str(j).split()[-1]))
		with open('res_a.txt', 'r') as myfile:
			data = myfile.read().replace('\n', ' ').replace('\t', '')
			#print("HOOK" , type(data))
			five_texts.append(data)
	are_same = True
	for i in range(4):
		for j in range(i + 1, 5):
			if five_texts[i] != five_texts[j]:
				are_same = False
	#print("Q", five_times, len(five_texts), thr)
	if are_same:
		print("Results are the same")
	else:
		print("Results differ")
	print("Minimal time of execution of program is " + str(min(five_times)))
	return are_same, min(five_times)
words_checker()
