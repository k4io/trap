import sqlite3
import hashlib
import uuid

user_table_definition = """
CREATE TABLE users (
    uid INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL,
    salt TEXT NOT NULL,
    hpassword TEXT NOT NULL,
    last_ip TEXT NOT NULL,
    status INT DEFAULT 1, 
    admin INT DEFAULT 0,
    super INT DEFAULT 1
)"""
sub_table_definition = """
CREATE TABLE subs (
    sub_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    uid INT NOT NULL,
    sub INT NOT NULL,
    startdt INT NOT NULL,
    enddt INT NOT NULL
)"""
attack_table_definition = """
CREATE TABLE attacks (
    attack_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    uid INT NOT NULL,
    method TEXT NOT NULL,
    host TEXT NOT NULL,
    port TEXT NOT NULL,
    seconds INT NOT NULL
)"""
add_user_sql = "INSERT INTO users VALUES (0, '{}','{}','{}','{}','{}','{}','{}')"

connection = sqlite3.connect("./trappers.db")
cursor = connection.cursor()

cursor.execute(user_table_definition)
cursor.execute(sub_table_definition)
cursor.execute(attack_table_definition)


# Add incoming user
username = "admin"
password = "admin"

salt = uuid.uuid4().hex
hashedpassword = hashlib.sha512((salt + password).encode("UTF-8")).hexdigest()

cursor.execute(add_user_sql.format(username, salt, hashedpassword, '0.0.0.0', 1, 1, 1))

# Check incoming user

username = "admin"
password = "admin"

row = cursor.execute("SELECT salt, hpassword FROM users WHERE username = '{}'".format(username)).fetchone()

salt, hpassword = row  # Unpacking the row information - btw this would fail if the username didn't exist

hashedIncomingPwd = hashlib.sha512((salt + password).encode("UTF-8")).hexdigest()

if hashedIncomingPwd == hpassword:
    print("Success!")
else:
    print("Failed login with password!")

connection.commit()