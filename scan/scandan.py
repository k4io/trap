import shodan

#
#
# shodan scanner
#
#
tup = tuple(open('queries.txt', 'r'))
search_queries = []
for x in tup:
    search_queries.append(x[:-1]) # :-1 for \n

#search result class, feel free to add your own attributes here
class sr:
    def __init__(self, port: int, transport: str, version: str, ip: str, os: str, v: list):
        self.port = port
        self.transport = transport
        self.version = version
        self.ip = ip
        self.os = os
        self.vulns = v


shodan_key = '1E03IJsee3IGKuYfGfUU3a8jtbn8nI6C' #member account
#shodan_key = 'TyNizThNzsXfN5lQ60uSZZFlGgqMhMhd' #free account for testing
#shodan_key = '1gbfsV4RhYfN1rPeRcaCG92r3YdzTpwW' #free account for testing

print('[+] Connecting to shodan...')
api = shodan.Shodan(shodan_key)
print('[+] Connected')

def get_query_results(query: str):
    currentlist = []
    print('\n[+] Getting results for "%s"' % query)

    pages = 2
    page = 0
    counted = 0
    while page < pages:
        try:
            results = api.search(query, page=str(page))
            matches = results['matches']
            count = results['total']
            pages = count / 100

            if pages > 6: #max pages temp  
                pages = 6
        except Exception as e:
            if str(e).find('upgrade') != -1:
                print('\n[+] Upgrade API to search with more pages')
                return currentlist
            print('\n[+] Error with search query: %s (%s)' % (query, str(e)))
            return currentlist
        try:
            if counted == 0:
                print('[+] Search %s found %d matches, creating list  (%s pages)' % (query, count, str(count / 100),))

             #\033[F cursor up one line
            for z in range(len(matches)):
                counted += 1
                zstr = f'[+] Sorted {str(counted)}/{str(count)} results'
                
                for i in range(len(zstr)):
                    print('\b \b', end='')
                print(zstr, end='')
                try:
                    match = matches[z]
                    port = match['port']
                    ip = match['ip_str']
                    transport = match['transport']
                    os = match['os']
                    if os is None:
                        os = 'undefined'
                    try:
                        vulns = match['vulns']
                    except Exception as e:
                        vulns = 'undefined'
                    try:
                        version = match['version']
                    except Exception as e:
                        version = 'undefined'
                    currentlist.append(sr(int(port), transport, version, ip, os, vulns))
                except Exception as e:
                    print('PROBLEM IN BAD PLACE LMAO ' + e)
            
            if (pages == 1 or pages == 0) and page == 1: #page scanned?
                return
            page+=1
            
        except Exception as e:
            print('\n[+] Error with for loop: %s (%s)' % (query, str(e)))
    return currentlist

def vulnstr(vulns: list) -> str:
    zs = ''
    for v in vulns:
        zs += str(v) + ','
    return zs

print('[+] Launching scan...')
for x in search_queries:
    res = get_query_results(x)
    for m in res:
        line = m.ip + ':' + str(m.port) + '|' + m.transport + '|' + m.version + '|' + m.os + '|' + vulnstr(m.vulns) + '\n'
        with open('scan.txt', 'a') as the_file:
            the_file.write(line)
        line = m.ip + ':' + str(m.port) + '\n'
        with open('ips.txt', 'a') as the_file:
            the_file.write(line)
    exit()