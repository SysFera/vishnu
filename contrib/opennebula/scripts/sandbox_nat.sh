#!/bin/bash

iptables -A FORWARD -i br0 -j ACCEPT
iptables -A FORWARD -o br0 -j ACCEPT
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
