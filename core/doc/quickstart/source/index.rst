***************************
SysFera-DS QuickStart Guide
***************************
This document provides a 5-min guide to get started with SysFera-DS.

.. warning::

  Please bear in mind that this reference guide is based on ``CentOS or Red Hat
  Enterprise Linux (RHEL), version 6.x``. Hence it may not work if you are
  using another distribution/version of operating system.

.. contents:: Contents
   :depth: 2


What You Will Need
==================

Required
--------
* Centos/RHEL 6.x
* A working MySQL installation, version 5 or higher with root access.
* Network access to your installation server


Not essential but helpful to have
---------------------------------
* The installation will be done in the folder ``/opt/sysfera``.
   You can change this to another path, subject
   to adapt the commands provided in this guide to that path.
* You must have root privileges on your system.


Get and Untar the SysFera-DS Distribution
=========================================
You'd have received the SysFera-DS binary tarball either via our CDROM,
or via our download site.

From a terminal, run the following command to uncompress the distribution.

::

  tar zxvf SysFera-DS-X.Y.Z.tar.gz # replace X.Y.Z with the version

Move to the SysFera-DS base distribution directory.

::
  
  cd SysFera-DS-X.Y.Z


.. note::

  For all the steps described below, always ensure that you're in the base
  directory of SysFera-DS distribution.

Create the SysFera-DS Database
==============================
Create the SysFera-DS database

::

  $ echo "create database sysferads" | mysql -u root -p

You'll then be prompted to enter the MySQL root password.

Initialize the Database
=======================
From a terminal run the following two commands to initialize the database:

::

  $ mysql -uvishnu -p sysferads < contrib/bdd/mysql_create.sql
  $ mysql -uvishnu -p sysferads < contrib/bdd/database_init.sql

In the both cases, you'll be prompted to enter the MySQL root password.

Copy Binaries
=============

Create the installation directory tree
::

  $ mkdir -p /opt/sysfera/{core,portal}


Untar binaries to the installation directory.

::

  $ tar zxvf core/distrib.tar.gz -C /opt/sysfera/core
  $ tar xvf portal/portal.tar -C /opt/sysfera/portal


Start Services
==============

Start the SysFera-DS core service

::

  $ /etc/init.d/sysfera-core start


Start the Web portal

::

  $ /etc/init.d/sysfera-portal start

Check the Installation
======================

Log files
---------
The core middleware of SysFera-DS sends logs to syslog, so on CentOS and RHEL
you'll find log in ``/var/log/messages``.

Access the services
-------------------
Launch a browser and go to the URL of SysFera-DS portal: ``http://server_ip_or_hostname/sysferads``.
Replace ``server_ip_or_hostname`` with the IP address or the host name of the server where you
installed SysFera-DS.

Default credentials are Login: ``admin``, Password: ``admin123``.

Sign in and enjoy SysFera-DS.

Next Steps
==========

For day-to-day operations, you may need to read the following documents:

.. FIXME: ajouter des liens ou rÃ©fÃ©rences croisÃ©es

+ Concept Guide
+ Advanced admin guide
+ SysFera-DS core user guide


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`






