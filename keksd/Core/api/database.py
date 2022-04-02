from json import JSONEncoder
import mysql.connector, datetime, uuid, json

tbnm = {
    0: 'api_blacklist',
    1: 'api_client_all',
    2: 'api_client_online',
    3: 'api_client_attacks',
    4: 'api_client_requests',
    5: 'api_client_users',
    6: 'api_client_user_log',
    7: 'api_sublist'
}

class req():
    def __init__(self, request_id, client_id, user_id, request_data, timestamp):
        self.request_id = request_id
        self.client_id = client_id
        self.user_id = user_id
        self.request_data = request_data
        self.timestamp = timestamp
        

class sub_package():
    def __init__(self, sub_id: int, max_pps: int, concurrent: int, max_time: int, cooldown: int, name: str):
        self.sub_id = sub_id
        self.max_pps = max_pps
        self.concurrent = concurrent
        self.max_time = max_time
        self.cooldown = cooldown
        self.name = name

class attack(dict):
    def __init__(self, aid: int, cid: int, h: str, p: int, m: str, t: int, a: str, uid: int, ts: int):
        self.attack_id = aid
        self.client_id = cid
        self.host = h
        self.port = p
        self.method = m
        self.time = t
        self.attack_cmd = a
        self.user_id = uid
        self.timestamp = ts
        dict.__init__(self, attack_id=aid, client_id=cid, host=h, port=p, method=m, time=t, attack_cmd=a, user_id=uid, timestamp=int(ts.utcnow().timestamp()))

class User:
    def __init__(self, uid, token, sub_id, fake_bots, admin, super):
        self.uid = uid
        self.token = token
        self.sub_id = sub_id
        self.fake_bots = fake_bots
        self.admin = admin
        self.super = super

    def is_admin(self):
        if self.admin == 1 or self.super == 1:
            return True
        else: return False

    def get_requests(self, d, count: int):
        lst = d.get_user_requests(self.uid)[::-1]
        if len(lst) > int(count):
            return lst[:int(count)]
        return lst

class db:
    def __init__(self, connection):
       self.connection = connection
       self.cursor = self.connection.cursor()
    
    def exec(self, query: str):
        try:
            self.cursor.execute(query)
            self.cursor.close()
            self.connection.commit()
        except Exception as e:
            print(e)

    def valid_token(self, token: str) -> bool:
        for x in self.cursor.execute(f'select * from {tbnm[5]} where token=\'{token}\';'):
            return True
        return False

    def utoken(self, token: str) -> User:
        self.exec(f'select user_id, sub_id, fake_bots, admin, super from {tbnm[5]} where token=\'{token}\';')
        for (uid, sub, fb, a, s) in self.cursor:
            return User(uid, token, sub, fb, a, s)

    def uuid(self, uid: int) -> User:
        self.exec(f'select token, sub_id, fake_bots, admin, super from {tbnm[5]} where user_id=\'{uid}\';')
        for (token, sub, fb, a, s) in self.cursor:
            return User(uid, token, sub, fb, a, s)

    def does_host_exist(self, host: str):
        self.exec(f'select id from {tbnm[1]} where host=\'{host}\';')
        for (id) in self.cursor:
            return True
        return False

    def get_host_id(self, host: str):
        self.cursor.execute(f'select id from {tbnm[1]} where host=\'{host}\';')
        for (id) in self.cursor:
            return id[0]
        return -1

    def register_client(self, token: str, host: str, permissions: int, max_users: int):
        if self.user(token).is_admin() == True:
            self.exec(f'insert into {tbnm[1]} (token, host, permission_level, max_users) values (\'{token}\', \'{host}\', {permissions}, {max_users});')
            print('client registered')
        return False

    def client_connected(self, host: str, connected_users: int):
        for x in self.cursor.execute(f'select * from {tbnm[2]} where host={host};'): 
            self.exec(f'delete from {tbnm[2]} where host={host};')
        self.exec(f'insert into {tbnm[2]} (host, current_users) values ({host},{connected_users});')
        return False

    def client_disconnected(self, id):
        self.exec(f'delete from {tbnm[2]} where id={id};')
        return False

    def log_attack(self, client_id: int, host: str, port: int, method: str, time: int, attack_cmd: str, client_user_id: int):
        self.exec(f'insert into {tbnm[3]} (client_id, host, port, method, time, attack_cmd, client_user_id, timestamp) ({client_id}, \'{host}\', {port}, \'{method}\', {time}, \'{attack_cmd}\', {client_user_id}, CURRENT_TIMESTAMP);')
        return False

    def log_request(self, client_id: int, user_id: int, request_data: str):
        self.cursor.execute(f'insert into {tbnm[4]} (client_id, user_id, request_data, timestamp) values ({client_id},{user_id},\'{request_data}\', CURRENT_TIMESTAMP);')
        self.connection.commit()
        return False

    def register_user(self, token: str, sub_id: int, fake_bots: int, admin: int, super: int):
        self.exec(f'insert into {tbnm[5]} (token, sub_id, fake_bots, admin, super) values (\'{token})\', {sub_id}, {fake_bots}, {admin}, {super})')
        return False

    def log_user(self, user_id: int, host: str):
        self.exec(f'insert into {tbnm[6]} (user_id, host, timestamp) values ({user_id}, \'{host}\', CURRENT_TIMESTAMP);')
        return False

    def blacklist_host(self, host, reason):
        self.exec(f'insert into {tbnm[7]} (host, reason) values (\'{host}\', {reason});')
        return False

    def get_sublist(self):
        lst = []
        self.cursor.execute(f'select * from {tbnm[7]};')
        for (sid,pps,c,mt,c,n) in self.cursor.fetchall():
            lst.append(sub_package(sid,pps,c,mt,c,n))
        self.connection.commit()
        return lst

    def get_user_requests(self, user_id: int):    
        lst = []
        self.cursor.execute(f'select request_id,client_id,request_data,timestamp from {tbnm[4]} where user_id=\'{user_id}\';')
        for (r,c,d,t) in self.cursor.fetchall():
            lst.append(req(r,c,user_id,d,t))
        self.connection.commit()
        return lst

    def get_user_attacks(self, user_id: int):
        lst = []
        self.cursor.execute(f'select attack_id, client_id, host, port, method, time, attack_cmd, client_user_id, timestamp from {tbnm[3]} where client_user_id=\'{user_id}\';')
        for (aid, cid, h, p, m, t, c, uid, ts) in self.cursor.fetchall():
            lst.append(attack(aid, cid, h, p, m, t, c, uid, ts))
        self.connection.commit()
        return lst