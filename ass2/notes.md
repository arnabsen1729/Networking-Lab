# tcpdump

## Definition

`tcpdump` is a common packet-analyzer. It allows the display of TCP/IP packets and other packets being transmitted over a network to which the computer is connected.

Packet Capturing
Promiscuous Mode

Port Number: 16 bit identifier
0-1023 -> well defined port numbers
1024 - onwards -> temporary port numbers assign to web request

Generally speaking, "promiscuous mode" means that a network interface card will pass all frames received up to the operating system for processing, versus the traditional mode of operation wherein only frames destined for the NIC's MAC address or a broadcast address will be passed up to the OS. Generally, promiscuous mode is used to "sniff" all traffic on the wire.

<https://serverfault.com/questions/106647/what-does-ifconfig-promisc-mode-do-or-promiscuous-mode-in-general>

<https://www.techtarget.com/searchsecurity/definition/promiscuous-mode>

```bash
sudo tcpdump -D
```

https://opensource.com/article/18/10/introduction-tcpdump
