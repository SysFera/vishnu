***************************
SysFera-DS QuickStart Guide
***************************
This document provides a 5-min guide to get started with SysFera-DS.

.. warning::

  Please bear in mind that this reference guide is based on ``x86_64 Linux-based operating system,
  with kernel version equal or greater 2.6.32``. Hence it may not work if you are using another
  distribution/version of operating system.

.. contents:: Contents
   :depth: 2


What You Will Need
==================

Required
--------
+ An x86_64 Linux-based operating system with a kernel version equal or greater than 2.6.32
+ A working MySQL installation, version 5 or higher
+ Root access to your MySQL installation
+ Network access to your installation server

Not essential but helpful to have
---------------------------------
+ The installation will be done in the folder ``${SYSFERA_DS_ROOT}``, default ``/opt/sysfera``. 
  You can change this to another path, subject  to adapt the 
  commands provided in this guide to that path.
+ Having root privileges on your operating system.  If you have no root privileges you should choose
  an installation directory where you have read/write permission. In this case, you’ll not be able to
  handle multi-user submissions with the default settings.


Get and Untar the SysFera-DS Distribution
=========================================
You'd have received the SysFera-DS binary tarball via our download site.

From a terminal, run the following command to uncompress the distribution.

::

  tar zxvf SysFera-DS-X.Y.Z.tar.gz # replace X.Y.Z with the version

Move to the SysFera-DS base distribution directory.

::
  
  $ cd SysFera-DS-X.Y.Z


.. note::

  For all the steps described below, always ensure that you're in the base
  directory of SysFera-DS distribution.


Set Installation Directory
==========================

::
 
  $ export SYSFERA_DS_ROOT=/opt/sysferads


Settings for SysFera-DS Database
=================================
Create the database
::

  $ echo "create database sysferads" | mysql -u root -p

When prompted, enter the MySQL root password.

Create the database user

::

  $ echo "CREATE USER 'sysferads_db_user' IDENTIFIED BY 'sysferads_db_pass'" | mysql -u root -p

Then enter the MySQL root password.

Copy Files
==============

Create the installation directory tree
::

  $ mkdir -p ${SYSFERA_DS_ROOT}/{etc,core,portal}


Untar binaries to the installation directory.

::

  $ tar zxvf core/distrib.tar.gz -C ${SYSFERA_DS_ROOT}/core
  $ tar xvf portal/portal.tar -C ${SYSFERA_DS_ROOT}/portal
  $ install -m 600 contrib/conf/portal.groovy.sample ${SYSFERA_DS_ROOT}/etc/portal.groovy
  $ install -m 600 contrib/conf/vishnu-sample.cfg ${SYSFERA_DS_ROOT}/etc/vishnu.cfg
  $ install -m 755 contrib/bin/sds_{core,portal}_run.sh ${SYSFERA_DS_ROOT}/core/bin
  $ install -m 755 contrib/bin/sds_{core,portal}_run.sh ${SYSFERA_DS_ROOT}/portal/bin

Set Minimal Configuration for your Installation
===============================================
Copy then edit the SysFera-DS portal configuration file using your favorite editor, e.g. emacs

::

  $ nano ${SYSFERA_DS_ROOT}/etc/portal.groovy

Set the database username/password to the corresponding values

Copy and edit the SysFera-DS core configuration file using your favorite editor, e.g. emacs

:: 

  $ nano ${SYSFERA_DS_ROOT}/etc/vishnu.server.conf

Set the correct values to the following keys. Lines begining with  # are comments.

::

  # Sets the mail handler path. IMPORTANT: replace ${SYSFERA_DS_ROOT} with 
  # the absolute path to the installation direcory. 
  # Environments variables are not taken into account.
  sendmailScriptPath=${SYSFERA_DS_ROOT}/core/bin/sendmail.py

  #  Sets the hostname of the database server
  databaseHost = 127.0.0.1

  # Sets the username to connect to the database
  databaseUserName = sysferads_db_user 

  # Sets the password to connect to the database. 
  databaseUserPassword = sysferads_db_pass

Start Services
==============

::

  ${SYSFERA_DS_ROOT}/core/bin/sds_core_run.sh
  ${SYSFERA_DS_ROOT}/core/bin/sds_portal_run.sh

You can use the equivalent \*_shutdown.sh scripts to stop the services.

Check the Installation
======================

Log files
----------
The core middleware of SysFera-DS sends logs to syslog, so on CentOS and RHEL
you'll find log in ``/var/log/messages``.

Access the services
---------------------
Launch a browser and go to the URL of SysFera-DS portal: ``http://server_ip_or_hostname/sysferads``.

Replace ``server_ip_or_hostname`` with the IP address or the host name of the server where you
installed SysFera-DS.

Default credentials are Login: ``admin``, Password: ``admin123``.

Sign in and enjoy SysFera-DS.

Next Steps
==========

For day-to-day operations, you may need to read the following documents:

.. FIXME: ajouter des liens ou references croisees

+ Concept Guide
+ Advanced admin guide
+ SysFera-DS core user guide


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`





