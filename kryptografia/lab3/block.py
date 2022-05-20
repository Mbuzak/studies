# Autor: Marian Buzak
# Data: 05.05.2022

import struct
import random


def encrypt(text, prev=None):
    if prev:
        left = text[0] ^ prev[0]
        right = text[1] ^ prev[0]
    else:
        left = text[0]
        right = text[1]

    sum = 0

    p = 0b1010110110000

    key = 'A1b2'
    key = [ord(x) for x in key]

    for i in range(32):
        sum += p

        left += ((right << 5) + key[0]) ^ (right + sum) ^ ((right >> 4) + key[1])
        right += ((left << 5) + key[2]) ^ (left + sum) ^ ((left >> 4) + key[3])

    mod = 1000000001

    return [left % mod, right % mod]


if __name__ == '__main__':
    formats = ['<2s', 'I', 'H', 'H', 'I', 'I', 'I', 'I', 'H', 'H', 'I', 'I', 'I', 'I', 'I', 'I']

    # EBC
    file_input = open('plain.bmp', 'rb')
    file_ecb = open('ecb_crypto.bmp', 'wb')

    header_data = struct.unpack('<2sIHHIIIIHHIIIIII', file_input.read(54))

    for i in range(len(formats)):
        file_ecb.write(struct.pack(formats[i], header_data[i]))

    while True:
        try:
            a = struct.unpack('I', file_input.read(4))
            b = struct.unpack('I', file_input.read(4))

            c = encrypt([a[0], b[0]])

            file_ecb.write(struct.pack('I', c[0]))
            file_ecb.write(struct.pack('I', c[1]))
        except struct.error:
            break

    file_input.close()
    file_ecb.close()

    # CBC
    file_input = open('plain.bmp', 'rb')
    file_cbc = open('cbc_crypto.bmp', 'wb')

    header_data = struct.unpack('<2sIHHIIIIHHIIIIII', file_input.read(54))

    for i in range(len(formats)):
        file_cbc.write(struct.pack(formats[i], header_data[i]))

    rand = [random.randint(1, 1000000), random.randint(1, 1000000)]

    ft = True
    c = []

    while True:
        try:
            a = struct.unpack('I', file_input.read(4))
            b = struct.unpack('I', file_input.read(4))

            if ft:
                c = encrypt([a[0], b[0]], rand)
                ft = False
            else:
                c = encrypt([a[0], b[0]], c)

            file_cbc.write(struct.pack('I', c[0]))
            file_cbc.write(struct.pack('I', c[1]))
        except struct.error:
            break

    file_input.close()
    file_cbc.close()

    print('Szyfrowanie gotowe')
