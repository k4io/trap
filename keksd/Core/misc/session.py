import requests
import socket
import random

def giveuad():
    random_lines = random.choice(open("desktop.txt").readlines())

class ModdedHTTPAdapter(requests.adapters.HTTPAdapter):
    def __init__(self, *args, **kwargs):
        self.socket_options = kwargs.pop("socket_options", None)
        super(ModdedHTTPAdapter, self).__init__(*args, **kwargs)
    def init_poolmanager(self, *args, **kwargs):
        if self.socket_options is not None:
            kwargs["socket_options"] = self.socket_options
        super(ModdedHTTPAdapter, self).init_poolmanager(*args, **kwargs)

#adapter = ModdedHTTPAdapter(socket_options=[ (socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1), (socket.IPPROTO_TCP, socket.TCP_NODELAY, 1) ])
#pwnsession = requests.session()
#pwnsession.mount("http://", adapter)
#pwnsession.mount("https://", adapter)
#pwnsession.verify = False
#pwnsession.allow_redirects = False
#pwnsession.timeout = (5, 2)
#pwnsession.trust_env = False

def getsession():
    padapter = ModdedHTTPAdapter(socket_options=[ (socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1), (socket.IPPROTO_TCP, socket.TCP_NODELAY, 1) ])
    session = requests.session()
    session.mount("http://", padapter)
    session.mount("https://", padapter)
    session.verify = False
    session.allow_redirects = False
    session.timeout = (5, 2)
    session.trust_env = False
    return session