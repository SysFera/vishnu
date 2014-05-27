NAME="centos-64"
CPU="1"
VCPU="2"
MEMORY="512"

DISK = [ IMAGE = "centos-64",
         DRIVER="qcow2"]

OS=[
  ARCH="i686",
  ROOT="sda1",
  BOOT="hd,fd,cdrom,network" ]

NIC = [ NETWORK = "FIXED-NET" ]

GRAPHICS = [
  TYPE    = "vnc",
  LISTEN  = "0.0.0.0",
  KEYMAP = "fr"]

RAW=[
  DATA="<serial type='pty'><target port='0'/></serial><console type='pty'> <target type='serial' port='0'/></console>",
  TYPE="kvm" ]

CONTEXT=[
  HOSTNAME="vm-$VMID",
  NETWORK="YES",
  ETH0_IP="$NIC[IP, NETWORK=\"FIXED-NET\"]",
  ETH0_GATEWAY="192.168.122.1",
  GATEWAY="192.168.122.1",
  FILES="/opt/software/opennebula-4.4.1/share/scripts/centos-5/context/init.sh",
  ETH0_DNS="8.8.8.8",
  NETMASK="255.255.255.0",
  TARGET="hdb" ]


