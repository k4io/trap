import json

with open('subs.json', 'r') as f:
    data = json.load(f)

print(data['subs'][0])
print(data['subs'][1])
print(data['subs'][2])
print(data['subs'][3])
print(data['subs'][4])