# Autor: Marian Buzak, 266381
# 01.04.2022
# 95 / 100 - powinno być uruchamiane z linii poleceń


def is_letter(nr):
    if (65 <= nr <= 90) or (97 <= nr <= 122):
        return True
    return False


def is_small_letter(nr):
    if 97 <= nr <= 122:
        return True
    return False


def is_big_letter(nr):
    if 65 <= nr <= 90:
        return True
    return False


def is_number(key, pos):
    if len(key) > pos:
        try:
            int(key[pos])
            return True
        except ValueError:
            return False
    return False


def caesar_decoding(letter, k):
    value = ord(letter)
    if is_big_letter(value):
        return ((value - 65 - k) % 26) + 65
    elif is_small_letter(value):
        return ((value - 97 - k) % 26) + 97
    return value


def gcd(a, b):
    while a != 0:
        tmp = a
        a = b % a
        b = tmp
    return b


def inverse(number):
    p = 1
    while p <= 100:
        if (number * p) % 26 == 1:
            break
        p += 1
    if p == 100:
        return -1
    return p


def get_letter_value(character):
    var = ord(character)
    if is_small_letter(var):
        return var - 97
    elif is_big_letter(var):
        return var - 65
    return -1


# affine
def decoding(letter, a_inverse, b):
    value = ord(letter)

    if is_big_letter(value):
        return ((a_inverse * ((value - 65) - b)) % 26) + 65
    elif is_small_letter(value):
        return ((a_inverse * ((value - 97) - b)) % 26) + 97
    return value


def caesar(option):
    def code():
        def coding(letter, k):
            value = ord(letter)
            if is_big_letter(value):
                return ((value - 65 + k) % 26) + 65
            elif is_small_letter(value):
                return ((value - 97 + k) % 26) + 97
            return value
        
        source_file = open("plain.txt")
        source = source_file.read()
        source_file.close()

        key_file = open("key.txt")
        key = key_file.read()
        key_file.close()
        split_key = key.split()

        if not is_number(split_key, 0):
            print('Błędny klucz')
            return -1

        crypto = ''
        for letter in source:
            letter_code = coding(letter, int(split_key[0]))
            crypto += chr(letter_code)

        crypto_file = open("crypto.txt", 'w')
        crypto_file.write(crypto)
        crypto_file.close()

    def decode():
        crypto_file = open("crypto.txt")
        crypto = crypto_file.read()
        crypto_file.close()

        key_file = open("key.txt")
        key = key_file.read()
        key_file.close()
        split_key = key.split()

        if not is_number(split_key, 0):
            print('Błędny klucz')
            return -1

        decrypt = ''
        for letter in crypto:
            letter_code = caesar_decoding(letter, int(split_key[0]))
            decrypt += chr(letter_code)

        decrypt_file = open("decrypt.txt", 'w')
        decrypt_file.write(decrypt)
        decrypt_file.close()

    def cryptanalysis():
        def find_key(letter1, letter2):
            val1, val2 = ord(letter1), ord(letter2)
            if is_small_letter(val1) and is_small_letter(val2):
                return ((val2 - 97) - (val1 - 97)) % 26
            elif is_big_letter(val1) and is_big_letter(val2):
                return ((val2 - 65) - (val1 - 65)) % 26
            return -1

        crypto_file = open("crypto.txt")
        crypto = crypto_file.read()
        crypto_file.close()

        extra_file = open("extra.txt")
        extra = extra_file.read()
        split_extra = extra.split()
        extra_file.close()

        is_empty_extra = False
        if len(split_extra) == 0:
            is_empty_extra = True

        if is_empty_extra:
            print('Pusty plik extra.txt')
            return -1

        key_found = find_key(extra[0], crypto[0])

        if key_found == -1:
            print('Nie można odnaleźć klucza')
            return -1

        decrypt = ''
        for letter in crypto:
            letter_code = caesar_decoding(letter, key_found)
            decrypt += chr(letter_code)

        key_found_file = open("key_found.txt", 'w')
        key_found_file.write(str(key_found))
        key_found_file.close()

        decrypt_file = open("decrypt.txt", 'w')
        decrypt_file.write(decrypt)
        decrypt_file.close()

    def cryptogram_cryptanalysis():
        crypto_file = open("crypto.txt")
        crypto = crypto_file.read()
        crypto_file.close()

        decrypt = ''
        for i in range(1, 26):
            for letter in crypto:
                letter_code = caesar_decoding(letter, i)
                decrypt += chr(letter_code)
            decrypt += '\n'

        decrypt_file = open("decrypt.txt", 'w')
        decrypt_file.write(decrypt)
        decrypt_file.close()

    if option == 'e':
        code()
    elif option == 'd':
        decode()
    elif option == 'j':
        cryptanalysis()
    elif option == 'k':
        cryptogram_cryptanalysis()
    else:
        print('Niepoprawna operacja')
        

def aniphic(option):
    def code():
        def coding(letter, a, b):
            value = ord(letter)
            if is_big_letter(value):
                return (((value - 65)*a + b) % 26) + 65
            elif is_small_letter(value):
                return (((value - 97)*a + b) % 26) + 97
            return value
            
        source_file = open("plain.txt")
        source = source_file.read()
        source_file.close()

        key_file = open("key.txt")
        key = key_file.read()
        key_file.close()
        split_key = key.split()

        if not is_number(split_key, 0) or not is_number(split_key, 1):
            print('Błędny klucz')
            return -1

        crypto = ''
        for letter in source:
            letter_code = coding(letter, int(split_key[0]), int(split_key[1]))
            crypto += chr(letter_code)

        crypto_file = open("crypto.txt", 'w')
        crypto_file.write(crypto)
        crypto_file.close()

    def decode():
        crypto_file = open("crypto.txt")
        crypto = crypto_file.read()
        crypto_file.close()

        key_file = open("key.txt")
        key = key_file.read()
        split_key = key.split()
        key_file.close()

        if not is_number(split_key, 0) or not is_number(split_key, 1) or gcd(int(split_key[0]), 26) != 1:
            print('Błędny klucz')
            return -1

        decrypt = ''
        a_prim = inverse(int(split_key[0]))

        if a_prim == -1:
            print('Błąd przy odwracaniu liczby')
            return -1

        for letter in crypto:
            letter_code = decoding(letter, a_prim, int(split_key[1]))
            decrypt += chr(letter_code)

        decrypt_file = open("decrypt.txt", 'w')
        decrypt_file.write(decrypt)
        decrypt_file.close()

    def cryptanalysis():
        def find_inverse(characters):
            letters = list()
            for i in range(len(characters)):
                if is_letter(ord(characters[i])):
                    letters.append([characters[i], i])
            for i in range(0, len(letters)):
                for j in range(1, len(letters)):
                    if j > i:
                        first = get_letter_value(letters[i][0])
                        second = get_letter_value(letters[j][0])
                        if gcd(first - second, 26) == 1:
                            return [i, j]
            return False

        def find_key(crypto_chars, extra_chars, position):
            if len(crypto_chars) > position[0] and len(crypto_chars) > position[1]:
                y1 = get_letter_value(crypto_chars[position[0]])
                y2 = get_letter_value(crypto_chars[position[1]])
                x1 = get_letter_value(extra_chars[position[0]])
                x2 = get_letter_value(extra_chars[position[1]])
                if y1 != -1 and y2 != -1:
                    a = ((y1 - y2) * inverse(x1 - x2)) % 26
                    b = (y1 - (a * x1)) % 26
                    return [a, b]
            return -1
        crypto_file = open("crypto.txt")
        crypto = crypto_file.read()
        crypto_file.close()

        extra_file = open("extra.txt")
        extra = extra_file.read()
        split_extra = extra.split()
        extra_file.close()

        pair = find_inverse(extra)

        is_empty_extra = False
        if len(split_extra) == 0:
            is_empty_extra = True

        if is_empty_extra:
            print('Pusty plik extra.txt')
            return -1

        if not pair:
            print('Nie można odnaleźć klucza')
            return -1

        key_found = find_key(crypto, extra, pair)

        if key_found == -1:
            print('Błędny tekst pomocniczy')
            return -1

        a_prim = inverse(key_found[0])
        decrypt = ''
        for letter in crypto:
            letter_code = decoding(letter, a_prim, int(key_found[1]))
            decrypt += chr(letter_code)

        key_found_file = open("key_found.txt", 'w')
        key_found_file.write(str(key_found[0]) + ' ' + str(key_found[1]))
        key_found_file.close()

        decrypt_file = open("decrypt.txt", 'w')
        decrypt_file.write(decrypt)
        decrypt_file.close()

    def cryptogram_cryptanalysis():
        def decoding(character, a, b):
            value = ord(character)

            if is_big_letter(value):
                return ((a * ((value - 65) - b)) % 26) + 65
            elif is_small_letter(value):
                return ((a * ((value - 97) - b)) % 26) + 97
            return value

        def combinations():
            a_list = list()
            b_list = [x for x in range(0, 26)]
            for i in range(1, 27):
                if gcd(i, 26) == 1:
                    a_list.append(i)

            combination = list()
            for a in a_list:
                for b in b_list:
                    combination.append([a, b])
            return combination

        crypto_file = open("crypto.txt")
        crypto = crypto_file.read()
        crypto_file.close()

        decrypt = ''
        combination = combinations()
        for i in range(len(combination)):
            for letter in crypto:
                letter_code = decoding(letter, combination[i][0], combination[i][1])
                decrypt += chr(letter_code)
            decrypt += '\n'

        decrypt_file = open("decrypt.txt", 'w')
        decrypt_file.write(decrypt)
        decrypt_file.close()
    
    if option == 'e':
        code()
    elif option == 'd':
        decode()
    elif option == 'j':
        cryptanalysis()
    elif option == 'k':
        cryptogram_cryptanalysis()
    else:
        print('Niepoprawna operacja')


if __name__ == '__main__':
    print("c - szyfr Cezara")
    print("a - szyfr afiniczny")
    option1 = input("Podaj rodzaj szyfru: ")

    if option1 == 'c' or option1 == 'a':
        print("\ne - szyfrowanie")
        print("d - odszyfrowywanie")
        print("j - kryptoanaliza z tekstem jawnym")
        print("k - kryptoanaliza wyłącznie w oparciu o kryptogram")
        option2 = input("Operacja: ")

        if option1 == 'c':
            caesar(option2)
        elif option1 == 'a':
            aniphic(option2)
    else:
        print('Niepoprawny rodzaj szyfru')
