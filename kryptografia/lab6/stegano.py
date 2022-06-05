# Autor: Marian Buzak
# Data: 05.06.2022
# python 3

import sys


def bin_form():
    file = open('mess.txt')
    mess = file.read()
    file.close()

    ret = ''

    bites = [format(int(x, 16), '#06b')[2:] for x in mess]

    for bit in bites:
        ret += bit

    return ret


def hex_form(bin_form):
    lst, ret = [], ''

    size = len(bin_form) // 4
    for i in range(size):
        lst.append(bin_form[i*4:(i+1)*4])

    for pos in lst:
        ret += hex(int(pos, 2))[2:]

    return ret


def read(filename):
    file = open(filename)
    data = file.read()
    file.close()
    return data


def read_lines(filename):
    file = open(filename)
    data = file.readlines()
    file.close()
    return data


def hide(option):
    def first():
        cover = read_lines('cover.html')
        cover_lines = len(cover)

        if bits_count > cover_lines:
            print('Za mały plik html do zanurzenia wiadomości')
            return -1

        for i in range(len(binary)):
            cover[i] = cover[i][:-1]

            if binary[i] == '1':
                cover[i] += ' '
            cover[i] += '\n'

            #print(cover[i])

        return cover

    def second():
        # is enough spaces?
        cover = read('cover.html')
        cover_spaces = len([i for i in cover if i == ' '])
        #print(cover_spaces)

        if bits_count > cover_spaces:
            print('Za mały plik html do zanurzenia wiadomości')
            return -1

        # hiding
        p = 0
        str = ''
        for pos in binary:
            while cover[p] != ' ':
                str += cover[p]
                p += 1

            if pos == '1':
                str += '  '
            else:
                str += ' '
            p += 1
        str += cover[p:len(cover)]
        return str

    def third():
        cover = read('cover.html')
        
        size = 0
        start, end = '<P', '>'
        for i in range(len(cover) - 2):
            if cover[i:i+2] == '<P':
                size += 1
        
        size *= 2  # attribute with 2 arguments
        if bits_count > size:
            print('Za mały plik html do zanurzenia wiadomości')
            return -1
        
        # print(size, bits_count)
        
        valid_attr = ['margin-bottom: 0cm;', 'line-height: 100%;']
        invalid_attr = ['margin-botom: 0cm;', 'lineheight: 100%;']
        str = ''
        p, i, q = 0, 0, 0
        while p < bits_count:
            if cover[i:i+2] == start:
                str += cover[i:i+2] + ' style="'
                
                q = i + 2
                while cover[q] != end:
                    q += 1
                #print(q)
                #i += 1
                
                for j in range(len(valid_attr)):
                   
                    if valid_attr[j] in cover[i+2 : q]:
                        pass
                    else:
                        if p < len(binary):
                            if binary[p] == '0':
                                str += valid_attr[j]
                            else:
                                str += invalid_attr[j]
                            p += 1
                        else:
                            break
                str += '">'
                i = q + 1
            else:
                str += cover[i]
                i += 1
        str += cover[i+1:len(cover)]
        
        return str

    def fourth():
        cover = read('cover.html')

        # can be saved
        marker, size, p = '</P>', 0, 0
        while p < len(cover) - len(marker):
            if cover[p:p + len(marker)] == marker:
                size += 1
                p += 4
                continue
            p += 1
        #print(size)

        if bits_count > size:
            print('Za mały plik html do zanurzenia wiadomości')
            return -1

        # hiding
        str, p, i = '', 0, 0
        while p < bits_count:
            if cover[i:i + len(marker)] == marker:
                if binary[p] == '1':
                    str += '</P><P>'
                else:
                    str += '</P>'
                p += 1
                i += 4
                continue
            str += cover[i]
            i += 1

        str += cover[i+1:len(cover)]
        return str

    binary = bin_form()
    bits_count = len(binary)

    """
    file = open('cover.html')
    cover = file.readlines()
    file.close()
    """

    if option == '-1':
        new = first()
    elif option == '-2':
        new = second()
    elif option == '-3':
        new = third()
    elif option == '-4':
        new = fourth()
    else:
        print('Błędny drugi parametr, wybierz [-1, -2, -3, -4]')

    if new != -1:
        file = open('watermark.html', 'w')
        for line in new:
            file.write(line)
        file.close()
        print('Gotowe')


def find(option):
    def first():
        msg = ''
        watermark = read_lines('watermark.html')
        for i in range(len(watermark)):
            if i >= msg_len:
                break

            watermark[i] = watermark[i][:-1]

            if len(watermark[i]) >= 1 and watermark[i][-1] == ' ':
                msg += '1'
            else:
                msg += '0'

        return msg

    def second():
        msg = ''
        watermark = read('watermark.html')
        p, k = 0, 0
        while k < msg_len:
            if watermark[p] == ' ':
                if watermark[p+1] == ' ':
                    msg += '1'
                    p += 1
                else:
                    msg += '0'
                k += 1
            p += 1
        return msg

    def third():
        msg = ''
        watermark = read('watermark.html')
        p, i, q = 0, 0, 0
        start, end = '<P', '>'
        attr = {'margin-bottom: 0cm;': '0', 'line-height: 100%;': '0', 'margin-botom: 0cm;': '1', 'lineheight: 100%;': '1'}
        while p < msg_len:
            if watermark[i:i+2] == start:
                q = i + 2
                while watermark[q] != end:
                    q += 1
                text = watermark[i+2:q]
                tmp = []
                print(text, tmp)
                for obj in attr.keys():
                    #print(obj)
                    if text.find(obj) != -1:
                        tmp.append([text.find(obj), attr[obj]])
                #print(tmp)
                tmp = sorted(tmp, key=lambda x: x[0])
                print(tmp)
                for j in range(len(tmp)):
                    msg += tmp[j][1]
                    p += 1
                i = q + 1
            else:
                i += 1
        print(p)
        print(msg)
        
        return msg

    def fourth():
        msg = ''
        marker = '</P>'
        watermark = read('watermark.html')
        p, i = 0, 0
        while p < msg_len:
            if watermark[i:i+4] == marker:
                if watermark[i+4:i+7] == '<P>':
                    msg += '1'
                else:
                    msg += '0'
                p += 1
                i += 4
                continue
            i += 1

        return msg

    msg_len = 2 * 4

    if option == '-1':
        new = first()
    elif option == '-2':
        new = second()
    elif option == '-3':
        new = third()
    elif option == '-4':
        new = fourth()
    else:
        print('Błędny drugi parametr, wybierz [-1, -2, -3, -4]')

    hexadecimal = hex_form(new)

    file = open('detect.txt', 'w')
    file.write(hexadecimal)
    file.close()
    print('Gotowe')
    


if __name__ == '__main__':
    # print(sys.argv[1])
    try:
        op_1 = sys.argv[1]
        op_2 = sys.argv[2]
    except IndexError:
        print('Za mała liczba parametrów')
        exit(0)

    # zanurzenie wiadomosci
    if sys.argv[1] == '-e':
        hide(sys.argv[2])
    # wyodrebnienie wiadomosci
    elif sys.argv[1] == '-d':
        find(sys.argv[2])
    else:
        print('Błędny pierwszy parametr, wybierz [-e, -d]')

