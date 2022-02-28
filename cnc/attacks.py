#all attack methods

connected = 0
running = 0

class method:
    def attack(self, params: list) -> bool:
        ip = str(params[0])
        port = int(params[1])
        time = int(params[2])
        return True

    def __init__(self, name: str) -> None:
        self.name = name

udp = method("udp")
tcp = method("tcp")
dns = method("dns")

methodlist = {
    "udp": udp,
    "tcp": tcp,
    "dns": dns
}