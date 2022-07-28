# Assignment 1

MAC - Media Access Control
ICMP - Internet Control Message Protocol
ARP - Address Resoltion Packet
DNS - Domain Name System

## `ping`

- Uses ICMP (Internet Control Message Protocol).
- `ping` uses the ICMP protocol's mandatory `ECHO_REQUEST` datagram to elicit an ICMP `ECHO_RESPONSE` from a host or gateway.
- *Utility:* to check if the internet connection is available or not.

Running the `ping` command on `www.google.com` we see:

```bash
$ ping www.google.com
PING www.google.com (142.250.192.4) 56(84) bytes of data.
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=1 ttl=115 time=40.7 ms
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=2 ttl=115 time=44.6 ms
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=3 ttl=115 time=42.4 ms
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=4 ttl=115 time=41.0 ms
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=5 ttl=115 time=42.4 ms
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=6 ttl=115 time=46.3 ms
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=7 ttl=115 time=41.8 ms
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=8 ttl=115 time=41.9 ms
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=9 ttl=115 time=40.9 ms
64 bytes from bom12s14-in-f4.1e100.net (142.250.192.4): icmp_seq=10 ttl=115 time=41.6 ms
^C
--- www.google.com ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 9012ms
rtt min/avg/max/mdev = 40.721/42.364/46.303/1.689 ms
```

1. Average RTT (round trip time): **42.364 ms**.
2. **0%** packet loss.
3. Size of packet that is sent to `www.google.com` server: **56 bytes** (default in Linux).
4. Size of packet that is received by your machine: **64 bytes**.

## `ifconfig`

- used to configure kernel-resident network interfaces.
- if no arguments are given, ifconfig displays the status of the currently active interfaces.

```bash
$ ifconfig
docker0: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        inet 172.17.0.1  netmask 255.255.0.0  broadcast 172.17.255.255
        ether 14:78:2B:6E:80:F2  txqueuelen 0  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

eth0: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        ether 96:2e:5c:3a:ac:ec  txqueuelen 1000  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 268905  bytes 55376426 (52.8 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 268905  bytes 55376426 (52.8 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

wlan0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.1.4  netmask 255.255.255.0  broadcast 192.168.1.255
        inet6 fe80::1002:e323:fc1f:41ab  prefixlen 64  scopeid 0x20<link>
        ether c8:62:14:ea:c0:b9  txqueuelen 1000  (Ethernet)
        RX packets 3671291  bytes 3149569640 (2.9 GiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 1507452  bytes 312907096 (298.4 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

## `dig`

- performs DNS lookups and displays the answers that are returned from the name server(s) that were queried.
- `dig` command reads the `/etc/resolv.conf` file and query the DNS servers listed.
- `dig google.com` reads the `A` records from the domain.

```bash
$ dig google.com

; <<>> DiG 9.16.15-Debian <<>> google.com
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 32302
;; flags: qr rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 1

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 512
;; QUESTION SECTION:
;google.com.   IN A

;; ANSWER SECTION:
google.com.  131 IN A 142.250.183.206

;; Query time: 40 msec
;; SERVER: 10.10.0.1#53(10.10.0.1)
;; WHEN: Tue Jan 11 15:09:02 IST 2022
;; MSG SIZE  rcvd: 55
```

1. IP Address of `google.com` is **142.250.183.206**.
2. IP addresses of DNS servers is **10.10.0.1**.

### A Record (address)‍

Most commonly used to map a fully qualified domain name (FQDN) to an IPv4 address and acts as a translator by converting domain names to IP addresses.

### AAAA Record (quad A)

Similar to A Records but maps to an IPv6 address (smartphones prefer IPv6, if available).

### CNAME Record (Canonical Name)

An alias that points to another domain or subdomain, but never an IP address. Alias record mapping FQDN to FQDN, multiple hosts to a single location. This record is also good for when you want to change an IP address over time as it allows you to make changes without affecting user bookmarks, etc.

### ANAME Record

This record type allows you to point the root of your domain to a hostname or FQDN

### SOA Record (Start of Authority)‍

Stores information about domains and is used to direct how a DNS zone propagates to secondary name servers.

### NS Record (name server)‍

Specifies which name servers are authoritative for a domain or subdomains (these records should not be pointed to a CNAME).

### MX Record (Mail eXchange)‍

Uses mail servers to map where to deliver email for a domain (should point to a mail server name and not to an IP address).

### TXT (text) Record‍

Allows administrators to add limited human and machine-readable notes and can be used for things such as email validation, site, and ownership verification, framework policies, etc., and doesn’t require specific formatting.

### SRV Record (service)‍

Allows services such as instant messaging or VoIP to be directed to a separate host and port location.

PTR Record (pointer)
A reverse of A and AAAA records, which maps IP addresses to domain names. These records require domain authority and can’t exist in the same zone as other DNS record types (put in reverse zones).

SPF Record (sender policy framework)‍
Helps prevent email spoofing and limits spammers.
‍Note: This record type has been deprecated in favor of TXT record types and may no longer be supported by all providers. We still support modifications and deletions of SPF records currently in your Constellix account, but new configurations must be in TXT record format.

## `traceroute`

[Computerphile video](https://www.youtube.com/watch?v=75yKT3OuE44)

- sends ICMP ECHO_REQ messages to the dest with incrementally increasing TTL (Time to Live) values.
- the path displayed is the list of interfaces of the intermediate network devices b/w the source and the dest.
- "time to live" (TTL) limits how long data can “live” in an IP network. Every packet of data is assigned a TTL value. Every time a data packet reaches a hop, the TTL value is decreased by one.
- This was designed so that if there is any misconfiguration and the packets are lost in a loop then after a period of time the TTL will reach 0 and the packet won't be valid anymore.
- Traceroute will send packets with a TTL of one to the destination server. The first network device the data passes through will decrease the TTL to the value of zero, and a message informing you the packets were dropped is sent. This gives you the RTT for hop number one.
From there, the data packets are sent to the destination server with a TTL of two. As the packets pass through the first hop, the TTL decreases to one. When they pass through the second hop, it decreases to zero. The message is sent again. This gives you the RTT for hop number two.
This process will repeat until the data packets either reach the destination device or it reaches the maximum number of hops. By the end of this test, you will know the number of hops to the destination device, the RTT length for each hop, and the device name and IP address for each hop.
- the path is determined by examining the ICMP Time Exceeded messages returned by the intermediate network devices and the ECHO REPLY messages returned by the destination.
- Some devices don't return the Time Exceeded messages for the packets with expired TTL values & are invisible in the traceroute command. They are denoted by (*). In the modern network environment the traditional traceroute methods can not be always applicable, because of widespread use of firewalls. Such firewalls filter the "unlikely" UDP ports, or even ICMP echoes.
- trace the paths data packets take from their source to their destinations, allowing administrators to better resolve connectivity issues.

```bash
$ sudo traceroute www.google.com -I
[sudo] password for arnab:
traceroute to www.google.com (142.250.192.4), 30 hops max, 60 byte packets
 1  _gateway (192.168.1.1)  167.298 ms  167.278 ms  167.268 ms
 2  * * *
 3  10.10.0.5 (10.10.0.5)  167.217 ms * *
 4  103.10.208.13 (103.10.208.13)  167.184 ms * *
 5  * * *
 6  103.27.170.10 (103.27.170.10)  99.063 ms  40.413 ms  121.402 ms
 7  108.170.248.161 (108.170.248.161)  95.523 ms * *
 8  108.170.231.79 (108.170.231.79)  97.792 ms * *
 9  bom12s14-in-f4.1e100.net (142.250.192.4)  147.178 ms  144.875 ms  163.849 ms
```

## `arp`

- Address Resolution Protocol
- every device in the LAN has two addresses:
  - Logical Address: IP Address
  - Physical Address: MAC Address (typically the address of NIC)

- arp command is queries the ARP Table and displays the mapping b/w the IP address and the MAC address.
- finds out the host present in a LAN segment.
- Gateway is the device for handling req and res for the internal hosts. Gateway is connected to the switch.

```bash
arp -a
```

`netstat -rn`
