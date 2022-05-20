# Autor: Marian Buzak
# Data: 17.05.2022
# python 3


import os


def does_file_exists(filename):
	try:
		open(filename)
	except FileNotFoundError:
		return False
	return True


def get_filename(filename):
	if does_file_exists(filename):
		return filename
	return ''


def main():
	hash_file = open('hash.txt', 'w')
	
	for hash_type in types:
		os.system(f'cat {get_filename("hash-.pdf")} personal.txt | {hash_type} >> hash.txt')
		os.system(f'cat {get_filename("hash-.pdf")} personal_.txt | {hash_type} >> hash.txt')
	
	hash_file.close()
	
	hash_file = open('hash.txt', 'r')
	lines = hash_file.readlines()
	hash_file.close()
	
	diff_file = open('diff.txt', 'w')
	
	p = 0
	
	for hash_type in types:
		diff = 0
		length = 0
		
		diff_file.write(f'cat {get_filename("hash-.pdf")} personal.txt | {hash_type}\n')
		diff_file.write(f'cat h{get_filename("hash-.pdf")} personal_.txt | {hash_type}\n')
		
		obj1 = lines[p][:-4]
		diff_file.write(f'{obj1}\n')
		obj2 = lines[p+1][:-4]
		p += 2
		diff_file.write(f'{obj2}\n')
		
		for i in range(len(obj1)):
			l1_bin = format(int(obj1[i], 16), '#06b')[2:]
			l2_bin = format(int(obj2[i], 16), '#06b')[2:]
			
			for j in range(len(l1_bin)):
				length += 1
				if l1_bin[j] != l2_bin[j]:
					diff += 1

		diff_file.write(f'Liczba rozniacych sie bitow: {diff} z {length}, procentowo: {round(diff / length * 100)}%\n\n')
	
	diff_file.close()
	

if __name__ == '__main__':
	types = ['md5sum', 'sha1sum', 'sha224sum', 'sha256sum', 'sha384sum', 'sha512sum', 'b2sum']
	
	main()
	print('Gotowe')
