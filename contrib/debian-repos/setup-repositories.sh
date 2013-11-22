#!/bin/bash
# This script allows to add packages to Debian repositories defines in the current directory
# Author : Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
# Creation : 12/11/2013

# REQUIREMENTS
#
# ** Your Debian packages MUST be available a the given directory on your local machine (deb, dsc, src, diff)
# ** The directory must contains a sub directory for any considered distribution. Currently: wheezy, squeeze
# ** You MUST install reprepro tool
# ** You MUST run the script from the current directory
# ** The script assume that 

if [ $# -ne 1 ] ; then
  echo "`basename $0` <vishnu_version>"
  exit 1
fi

VERSION=$1

if [ $# -ne 1 ]; then
  echo "basename $0 <package_bin_dir>"
  exit 1
fi

mkdir -p obs-packages; cd obs-packages

# Download packages for debian wheezy
dist="wheezy"
archs="i386 amd64"

mkdir $dist; cd $dist
wget -N http://download.opensuse.org/repositories/home:/techsysfera/Debian_7.0/all/vishnu-config_${VERSION}_all.deb 
wget -N http://download.opensuse.org/repositories/home:/techsysfera/Debian_7.0/all/vishnu-dev_${VERSION}_all.deb
wget -N http://download.opensuse.org/repositories/home:/techsysfera/Debian_7.0/vishnu-${VERSION}.tgz_${VERSION}.diff.gz
wget -N http://download.opensuse.org/repositories/home:/techsysfera/Debian_7.0/vishnu-${VERSION}.tgz_${VERSION}.dsc 
wget -N http://download.opensuse.org/repositories/home:/techsysfera/Debian_7.0/vishnu-${VERSION}.tgz_${VERSION}.orig.tar.gz
for arch in $archs; do 
  wget -N http://download.opensuse.org/repositories/home:/techsysfera/Debian_7.0/$arch/libvishnu_${VERSION}_${arch}.deb 
  wget -N http://download.opensuse.org/repositories/home:/techsysfera/Debian_7.0/$arch/vishnu-client_${VERSION}_${arch}.deb
  wget -N http://download.opensuse.org/repositories/home:/techsysfera/Debian_7.0/$arch/vishnu-server_${VERSION}_${arch}.deb
  wget -N http://download.opensuse.org/repositories/home:/techsysfera/Debian_7.0/$arch/vishnu_${VERSION}_${arch}.deb  
done 

TGZ=vishnu-${VERSION}.tgz_${VERSION}.orig.tar.gz
DSC=vishnu-${VERSION}.tgz_${VERSION}.dsc
DSC_TMP=../../vishnu.dsc
DEFAULT_VERSION=X.Y.Z

tgz_md5sum=`md5sum ${TGZ} | awk '{print $1}'`
tgz_size=`stat --printf "%s" ${TGZ}`
sed --in-place "s/$DEFAULT_VERSION/$VERSION/g" ${DSC_TMP}
sed --in-place "s/<TGZ_MD5SUM>/$tgz_md5sum/g" ${DSC_TMP}
sed --in-place "s/<TGZ_SIZE>/$tgz_size/g" ${DSC_TMP}
cp ${DSC_TMP} $DSC

# Make repository
cd ../..
echo $PWD
reprepro -Vb . includedeb $dist $PWD/obs-packages/$dist/*.deb
reprepro -Vb . includedsc $dist $PWD/obs-packages/$dist/*.dsc

# Copy to OVH
#scp -r conf/ dists/ db/ pool/ root@91.121.160.100:/var/lib/lxc/sysfera/rootfs/var/www/web/download/vishnu/repos/debs
