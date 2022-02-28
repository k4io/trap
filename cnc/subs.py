#!/usr/bin
import sqlite3
import json

with open('subs.json', 'r') as f:
    subs_json_root = json.load(f)

allsubs = subs_json_root['subs']
homesub = allsubs[0]
hitta = allsubs[1]
hittavip = allsubs[2]
trapper = allsubs[3]
trappervip = allsubs[4]

class sub:
    def subid(self, uid: int) -> str:
        try:
            sql = "SELECT sub FROM subs WHERE uid={}"
            connection = sqlite3.connect("./trappers.db", isolation_level=None)
            cursor = connection.cursor()
            val = cursor.execute(sql.format(uid))
            connection.close()
            return val
        except Exception as e:
            print(e)
            return False

    def get_sub(self, subi: int) :
        try:
            s = allsubs[subi]
            name = allsubs['name']
            maxtime = int(allsubs['maxtime'])
            power = allsubs['power']
            con = int(allsubs['con'])
            cd = int(allsubs['cd'])
            return sub(name, maxtime, con, power, cd)
        except Exception as e:
            return 0

    def __init__(self, name: str, hittime: int, concurrents: int, power: str, cd: int) -> None:
        self.name = name
        self.hittime = hittime
        self.concurrents = concurrents
        self.power = int(self.power[:-1])
        self.cooldown = cd

def sub_count():
    try:
        connection = sqlite3.connect("./trappers.db", isolation_level=None)
        cursor = connection.cursor()
        val = cursor.execute('SELECT count(*) FROM subs').fetchone()
        connection.close()
        return int(val[0])
    except Exception as e:
        print(e)
        return -1

def create_sub(subid: int, days: int, uid: int):
        try:
            sql = "INSERT INTO subs VALUES ({},{},{},{},{})"
            connection = sqlite3.connect("./trappers.db", isolation_level=None)
            cursor = connection.cursor()
            val = cursor.execute(sql.format(sub_count(), uid, subid, "DATE('now')", f"DATETIME(DATE('now'), '+{days} days"))
            connection.close()
            return True
        except Exception as e:
            print(e)
            return False