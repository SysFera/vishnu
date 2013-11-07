Name:           vishnu
Summary:    	VISHNU is a modular toolkit for distributed resource management
Version:        X.Y.Z
Release:        build<CI_CNT>.<B_CNT>
Group:          System/Administration
License:        Cecil
URL:            http://www.sysfera.fr/sysfera-ds/vishnu.html
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %_tmppath/%name-%version-build

%define _prefix /usr/local

BuildRequires: gcc-c++ zeromq-devel cmake >= 2.8.5 mysql-devel postgresql-devel openssl-devel 
BuildRequires: boost-devel > 1.46 swig java-1.5.0-gcj java-1.5.0-gcj-devel python python-devel
 
#%if 0%{?suse_version} >= 1140
#BuildRequires: gcc-c++ >= 4.6.3
#%endif

#%if 0%{?fedora}
#BuildRequires: gcc-c++
#%endif

#%if 0%{?centos_version}
#BuildRequires:  gcc-c++
#%endif

%description
VISHNU is modular and high-level middleware for tasks, files and information 
management in heterogeneous and distributed HPC environments.

VISHNU VISHNU offers to end-users the capability of running computation tasks (jobs) 
on remote servers through a simple interface that offers performance and interoperability 
with different kind of batch scheduler softwares running locally on the servers. 
VISHNU also offers files management services to create, modify and transfer files 
on remote hosts using secure transfer protocols. The VISHNU system is also capable 
of monitoring the performance of the servers and to recover automatically from server 
failures.

%package libvishnu
Summary: vishnu librairies
%description libvishnu
This package contains common vishnu library files

%package client
Summary: vishnu CLI and API
Requires: libvishnu
%description client
This package contains CLI and API of vishnu

%package server
Summary: Dispatcher and SeDs
Requires: libvishnu
%description server
This package contains the server components of vishnu

%package libvishnu-devel
Summary: Headers and librairies for C++, Python and Java APIs
Requires: libvishnu
%description libvishnu-devel
This contains headers files and librairies for C++, Python and Java developements

%prep
%setup -q

cmake . \
  -DCOMPILE_UMS=ON \
  -DCOMPILE_FMS=ON \
  -DCOMPILE_IMS=OFF \
  -DCOMPILE_TMS=ON \
  -DCOMPILE_CLIENT_CLI=ON \
  -DCOMPILE_SERVERS=ON \
  -DENABLE_POSTGRESQL=ON \
  -DENABLE_MYSQL=ON \
  -DBUILD_TESTING=OFF \
  -DENABLE_SWIG=ON \
  -DENABLE_PYTHON=ON \
  -DENABLE_JAVA=ON \
  -DENABLE_LDAP=OFF \
  -DBOOST_ROOT=/usr \
  -DBoost_DIR=/usr \
  -DBoost_INCLUDE_DIR=/usr/include \
  -DBoost_LIBRARY_DIRS=/usr/lib \
  -DOPENSSL_INCLUDE_DIR=/usr/include \
  -DVISHNU_VERSION=%{version} \
  -DVISHNU_BATCH=POSIX \
  -DVISHNU_BATCH_VERSION=1.0
  

%build
make -j4

%install
make DESTDIR=%{buildroot} install

%clean
make clean
rm -rf %{buildroot}

%post
/sbin/ldconfig

%postun
#TODO


%files
%defattr(-,root,root)
%{_prefix}/lib
%{_prefix}/bin
%{_prefix}/include/
%{_prefix}/lib
%{_prefix}/sbin
%doc %{_prefix}/share
%config(noreplace) %{_prefix}/etc

%files libvishnu
%defattr(-,root,root)
%{_prefix}/lib/libemf4cpp-vishnu.*
%{_prefix}/lib/libzmq_helper.*
%{_prefix}/lib/libvishnu-core.*
%{_prefix}/lib/libvishnu-core-server.*
%{_prefix}/lib/libvishnu-?ms-server.*
%{_prefix}/lib/libvishnu-?ms-client.*
%{_prefix}/lib/_VISHNU.*
%{_prefix}/lib/libVISHNU.*
%{_prefix}/lib/swig_output


%files client
%defattr(-,root,root)
%{_prefix}/bin/vishnu_*
%doc %{_prefix}/share/man/man?/vishnu*
%config(noreplace) %{_prefix}/etc/vishnu-sample.cfg

%files server
%defattr(-,root,root)
%{_prefix}/sbin
%config(noreplace) %{_prefix}/etc/vishnu-sample.cfg
%config(noreplace) %{_prefix}/etc/supervisord-sample.cfg

%files libvishnu-devel
%defattr(-,root,root)
%{_prefix}/include/
%doc %{_prefix}/share/man/man?/VISHNU.*


%changelog
* Mon Aug 21 2013 Rodrigue Chakode <rodrigue.chakode@sysfera.com>
- See the web site to learn what changed in the different versions of the software
