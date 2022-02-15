symbols = dict.fromkeys([chr(i) for i in range(33,  127)], 0)
f = open('/media/sf_raid/wordlists/rockyou.txt', encoding='latin-1')
for char in f.read():
    ord_char = ord(char)
    if ord_char > 32 and ord_char < 127:
        symbols[char] += 1
print(symbols)
list_sym = list(symbols.items())
list_sym.sort(key=lambda i: i[1], reverse=True)
for i in list_sym:
    print(i[0], end='')
