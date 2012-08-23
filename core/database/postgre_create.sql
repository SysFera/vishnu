-- This script is for initialization of the VISHNU PostgreSQL database
-- Script name          : postgre_create.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 27/01/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : initial version

-- Revision nb          : 1.2
-- Revision date        : 07/06/11
-- Revision author      : Benjamin Isnard <benjamin.isnard@sysfera.com>
-- Revision comment     : release of FMS and IMS modules

-- Revision nb          : 1.3
-- Revision date        : 02/02/12
-- Revision author      : Kevin Coulomb <kevin.coulomb@sysfera.com>
-- Revision comment     : Add auth System

-- Revision nb          : 1.4
-- Revision date        : 23/08/12
-- Revision author      : Philippe Martinez <philippe.martinez@sysfera.com>
-- Revision comment     : Vishnu database mutualization with the WebBoard

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: account; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE account (
    numaccountid bigint NOT NULL,
    aclogin character varying(255),
    home character varying(255),
    machine_nummachineid bigint NOT NULL,
    sshpathkey character varying(255),
    users_numuserid bigint NOT NULL
);


ALTER TABLE public.account OWNER TO vishnu_user;

--
-- Name: account_numaccountid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE account_numaccountid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.account_numaccountid_seq OWNER TO vishnu_user;

--
-- Name: account_numaccountid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE account_numaccountid_seq OWNED BY account.numaccountid;


--
-- Name: acl_class; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE acl_class (
    id bigint NOT NULL,
    class character varying(255) NOT NULL
);


ALTER TABLE public.acl_class OWNER TO vishnu_user;

--
-- Name: acl_entry; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE acl_entry (
    id bigint NOT NULL,
    ace_order integer NOT NULL,
    acl_object_identity bigint NOT NULL,
    audit_failure boolean NOT NULL,
    audit_success boolean NOT NULL,
    granting boolean NOT NULL,
    mask integer NOT NULL,
    sid bigint NOT NULL
);


ALTER TABLE public.acl_entry OWNER TO vishnu_user;

--
-- Name: acl_object_identity; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE acl_object_identity (
    id bigint NOT NULL,
    object_id_class bigint NOT NULL,
    entries_inheriting boolean NOT NULL,
    object_id_identity bigint NOT NULL,
    owner_sid bigint,
    parent_object bigint
);


ALTER TABLE public.acl_object_identity OWNER TO vishnu_user;

--
-- Name: acl_sid; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE acl_sid (
    id bigint NOT NULL,
    principal boolean NOT NULL,
    sid character varying(255) NOT NULL
);


ALTER TABLE public.acl_sid OWNER TO vishnu_user;

--
-- Name: application; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE application (
    id bigint NOT NULL,
    version bigint NOT NULL,
    deleted boolean NOT NULL,
    description text,
    identifier character varying(255) NOT NULL,
    name character varying(255) NOT NULL,
    class character varying(255) NOT NULL,
    author_id bigint,
    project_id bigint
);


ALTER TABLE public.application OWNER TO vishnu_user;

--
-- Name: application_parameter; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE application_parameter (
    id bigint NOT NULL,
    version bigint NOT NULL,
    application_id bigint NOT NULL,
    default_value character varying(255),
    description text,
    editable boolean NOT NULL,
    format character varying(255) NOT NULL,
    hidden boolean NOT NULL,
    label character varying(255) NOT NULL,
    max_len bigint NOT NULL,
    min_len bigint NOT NULL,
    name character varying(255) NOT NULL,
    optionnal boolean NOT NULL,
    pos bigint NOT NULL,
    possible_values character varying(255),
    regex character varying(255),
    type character varying(255) NOT NULL
);


ALTER TABLE public.application_parameter OWNER TO vishnu_user;

--
-- Name: application_version; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE application_version (
    id bigint NOT NULL,
    version bigint NOT NULL,
    application_id bigint NOT NULL,
    date_created timestamp without time zone NOT NULL,
    last_updated timestamp without time zone NOT NULL,
    pre_script character varying(255),
    pre_script_path character varying(255),
    script character varying(255) NOT NULL,
    script_path character varying(255) NOT NULL,
    validation_script character varying(255),
    validation_script_path character varying(255)
);


ALTER TABLE public.application_version OWNER TO vishnu_user;

--
-- Name: authaccount; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE authaccount (
    authaccountid bigint NOT NULL,
    aclogin character varying(255),
    authsystem_authsystemid bigint NOT NULL,
    users_numuserid bigint NOT NULL
);


ALTER TABLE public.authaccount OWNER TO vishnu_user;

--
-- Name: authaccount_authaccountid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE authaccount_authaccountid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.authaccount_authaccountid_seq OWNER TO vishnu_user;

--
-- Name: authaccount_authaccountid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE authaccount_authaccountid_seq OWNED BY authaccount.authaccountid;


--
-- Name: authsystem; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE authsystem (
    numauthsystemid bigint NOT NULL,
    authlogin character varying(255),
    authpassword character varying(255),
    authsystemid character varying(255),
    authtype integer,
    name character varying(255),
    status integer,
    uri character varying(255),
    userpwdencryption integer,
    vishnu_vishnuid bigint NOT NULL
);


ALTER TABLE public.authsystem OWNER TO vishnu_user;

--
-- Name: authsystem_numauthsystemid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE authsystem_numauthsystemid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.authsystem_numauthsystemid_seq OWNER TO vishnu_user;

--
-- Name: authsystem_numauthsystemid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE authsystem_numauthsystemid_seq OWNED BY authsystem.numauthsystemid;


--
-- Name: clmachine; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE clmachine (
    numclmachineid bigint NOT NULL,
    name character varying(255),
    sshkey character varying(255)
);


ALTER TABLE public.clmachine OWNER TO vishnu_user;

--
-- Name: clmachine_numclmachineid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE clmachine_numclmachineid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.clmachine_numclmachineid_seq OWNER TO vishnu_user;

--
-- Name: clmachine_numclmachineid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE clmachine_numclmachineid_seq OWNED BY clmachine.numclmachineid;


--
-- Name: command; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE command (
    numcommandid bigint NOT NULL,
    ctype integer,
    description text,
    endtime timestamp without time zone,
    starttime timestamp without time zone,
    status integer,
    vishnuobjectid character varying(255),
    vsession_numsessionid bigint NOT NULL
);


ALTER TABLE public.command OWNER TO vishnu_user;

--
-- Name: command_numcommandid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE command_numcommandid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.command_numcommandid_seq OWNER TO vishnu_user;

--
-- Name: command_numcommandid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE command_numcommandid_seq OWNED BY command.numcommandid;


--
-- Name: description; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE description (
    numdescriptionid bigint NOT NULL,
    description text,
    lang character varying(255),
    machine_nummachineid bigint NOT NULL
);


ALTER TABLE public.description OWNER TO vishnu_user;

--
-- Name: description_numdescriptionid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE description_numdescriptionid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.description_numdescriptionid_seq OWNER TO vishnu_user;

--
-- Name: description_numdescriptionid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE description_numdescriptionid_seq OWNED BY description.numdescriptionid;


--
-- Name: filetransfer; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE filetransfer (
    numfiletransferid bigint NOT NULL,
    clientmachineid character varying(255),
    destinationfilepath character varying(255),
    destinationmachineid character varying(255),
    errormsg text,
    filesize integer,
    processid integer,
    sourcefilepath character varying(255),
    sourcemachineid character varying(255),
    starttime timestamp without time zone,
    status integer,
    transferid character varying(255),
    trcommand integer,
    userid character varying(255),
    vsession_numsessionid bigint NOT NULL
);


ALTER TABLE public.filetransfer OWNER TO vishnu_user;

--
-- Name: filetransfer_numfiletransferid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE filetransfer_numfiletransferid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.filetransfer_numfiletransferid_seq OWNER TO vishnu_user;

--
-- Name: filetransfer_numfiletransferid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE filetransfer_numfiletransferid_seq OWNED BY filetransfer.numfiletransferid;


--
-- Name: global_project_role; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE global_project_role (
    id bigint NOT NULL,
    version bigint NOT NULL,
    description text,
    is_built_in boolean NOT NULL,
    is_default boolean NOT NULL,
    label character varying(255) NOT NULL,
    member_specific boolean NOT NULL,
    name character varying(255) NOT NULL,
    pos bigint NOT NULL
);


ALTER TABLE public.global_project_role OWNER TO vishnu_user;

--
-- Name: global_project_role_permissions; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE global_project_role_permissions (
    global_project_role_id bigint,
    permissions_string character varying(255)
);


ALTER TABLE public.global_project_role_permissions OWNER TO vishnu_user;

--
-- Name: hibernate_sequence; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE hibernate_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.hibernate_sequence OWNER TO vishnu_user;

--
-- Name: job; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE job (
    numjobid bigint NOT NULL,
    batchjobid character varying(255),
    batchtype integer,
    enddate timestamp without time zone,
    errorpath character varying(255),
    groupname character varying(255),
    job_owner_id bigint,
    jobdescription text,
    jobid character varying(255),
    jobname character varying(255),
    jobpath character varying(255),
    jobprio integer,
    jobqueue character varying(255),
    jobworkingdir character varying(255),
    machine_id bigint,
    memlimit integer,
    nbcpus integer,
    nbnodes integer,
    nbnodesandcpupernode character varying(255),
    outputdir character varying(255),
    outputpath character varying(255),
    owner character varying(255),
    scriptcontent text,
    status integer,
    submitdate timestamp without time zone,
    submitmachineid character varying(255),
    submitmachinename character varying(255),
    vsession_numsessionid bigint NOT NULL,
    wallclocklimit integer,
    workid bigint
);


ALTER TABLE public.job OWNER TO vishnu_user;

--
-- Name: job_numjobid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE job_numjobid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.job_numjobid_seq OWNER TO vishnu_user;

--
-- Name: job_numjobid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE job_numjobid_seq OWNED BY job.numjobid;


--
-- Name: ldapauthsystem; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE ldapauthsystem (
    ldapauthsystid bigint NOT NULL,
    authsystem_authsystemid bigint NOT NULL,
    ldapbase character varying(255)
);


ALTER TABLE public.ldapauthsystem OWNER TO vishnu_user;

--
-- Name: ldapauthsystem_ldapauthsystid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE ldapauthsystem_ldapauthsystid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.ldapauthsystem_ldapauthsystid_seq OWNER TO vishnu_user;

--
-- Name: ldapauthsystem_ldapauthsystid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE ldapauthsystem_ldapauthsystid_seq OWNED BY ldapauthsystem.ldapauthsystid;


--
-- Name: machine; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE machine (
    nummachineid bigint NOT NULL,
    diskspace integer,
    machineid character varying(255),
    memory integer,
    name character varying(255),
    network integer,
    site character varying(255),
    sshpublickey text,
    status integer,
    vishnu_vishnuid bigint NOT NULL
);


ALTER TABLE public.machine OWNER TO vishnu_user;

--
-- Name: machine_nummachineid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE machine_nummachineid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.machine_nummachineid_seq OWNER TO vishnu_user;

--
-- Name: machine_nummachineid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE machine_nummachineid_seq OWNED BY machine.nummachineid;


--
-- Name: notification; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE notification (
    id bigint NOT NULL,
    version bigint NOT NULL,
    clazz character varying(255) NOT NULL,
    content character varying(255) NOT NULL,
    date_created timestamp without time zone NOT NULL,
    link_to character varying(255) NOT NULL,
    notifier_id bigint,
    object_id integer NOT NULL,
    project_id bigint NOT NULL,
    type character varying(255) NOT NULL
);


ALTER TABLE public.notification OWNER TO vishnu_user;

--
-- Name: optionu; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE optionu (
    numoptionid bigint NOT NULL,
    defaultvalue integer,
    description text,
    optionid integer
);


ALTER TABLE public.optionu OWNER TO vishnu_user;

--
-- Name: optionu_numoptionid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE optionu_numoptionid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.optionu_numoptionid_seq OWNER TO vishnu_user;

--
-- Name: optionu_numoptionid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE optionu_numoptionid_seq OWNED BY optionu.numoptionid;


--
-- Name: optionvalue; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE optionvalue (
    numoptionvalueid bigint NOT NULL,
    optionu_numoptionid bigint NOT NULL,
    users_numuserid bigint NOT NULL,
    value integer
);


ALTER TABLE public.optionvalue OWNER TO vishnu_user;

--
-- Name: optionvalue_numoptionvalueid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE optionvalue_numoptionvalueid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.optionvalue_numoptionvalueid_seq OWNER TO vishnu_user;

--
-- Name: optionvalue_numoptionvalueid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE optionvalue_numoptionvalueid_seq OWNED BY optionvalue.numoptionvalueid;


--
-- Name: parameter_value; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE parameter_value (
    id bigint NOT NULL,
    version bigint NOT NULL,
    parent_id bigint NOT NULL,
    type_id bigint NOT NULL,
    value character varying(255) NOT NULL
);


ALTER TABLE public.parameter_value OWNER TO vishnu_user;

--
-- Name: permission; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE permission (
    id bigint NOT NULL,
    version bigint NOT NULL,
    label character varying(255) NOT NULL,
    member_required boolean NOT NULL,
    module_id bigint NOT NULL,
    name character varying(255) NOT NULL,
    pos bigint NOT NULL,
    project_required boolean NOT NULL
);


ALTER TABLE public.permission OWNER TO vishnu_user;

--
-- Name: permission_module; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE permission_module (
    id bigint NOT NULL,
    version bigint NOT NULL,
    name character varying(255) NOT NULL,
    pos bigint NOT NULL
);


ALTER TABLE public.permission_module OWNER TO vishnu_user;

--
-- Name: process; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE process (
    numprocess bigint NOT NULL,
    dietname character varying(255),
    launchscript text,
    machineid character varying(255),
    pstatus integer,
    uptime timestamp without time zone,
    vishnuname character varying(255)
);


ALTER TABLE public.process OWNER TO vishnu_user;

--
-- Name: process_numprocess_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE process_numprocess_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.process_numprocess_seq OWNER TO vishnu_user;

--
-- Name: process_numprocess_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE process_numprocess_seq OWNED BY process.numprocess;


--
-- Name: project; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE project (
    id bigint NOT NULL,
    version bigint NOT NULL,
    closed boolean NOT NULL,
    date_created timestamp without time zone NOT NULL,
    deleted boolean NOT NULL,
    description text NOT NULL,
    identifier character varying(255) NOT NULL,
    is_public boolean NOT NULL,
    last_updated timestamp without time zone NOT NULL,
    name character varying(30) NOT NULL,
    parent_id integer NOT NULL
);


ALTER TABLE public.project OWNER TO vishnu_user;

--
-- Name: project_application; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE project_application (
    id bigint NOT NULL,
    version bigint NOT NULL,
    application_id bigint NOT NULL,
    project_id bigint NOT NULL
);


ALTER TABLE public.project_application OWNER TO vishnu_user;

--
-- Name: project_machine; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE project_machine (
    id bigint NOT NULL,
    version bigint NOT NULL,
    machine_id bigint NOT NULL,
    max_allowed_time bigint NOT NULL,
    project_id bigint NOT NULL,
    spent_time bigint NOT NULL
);


ALTER TABLE public.project_machine OWNER TO vishnu_user;

--
-- Name: project_member; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE project_member (
    id bigint NOT NULL,
    version bigint NOT NULL,
    date_created timestamp without time zone NOT NULL,
    last_updated timestamp without time zone NOT NULL,
    mail_notifications boolean NOT NULL,
    member_id bigint NOT NULL,
    project_id bigint NOT NULL
);


ALTER TABLE public.project_member OWNER TO vishnu_user;

--
-- Name: project_member_project_role; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE project_member_project_role (
    project_member_roles_id bigint,
    project_role_id bigint
);


ALTER TABLE public.project_member_project_role OWNER TO vishnu_user;

--
-- Name: project_role; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE project_role (
    id bigint NOT NULL,
    version bigint NOT NULL,
    description text,
    is_built_in boolean NOT NULL,
    is_default boolean NOT NULL,
    label character varying(255) NOT NULL,
    member_specific boolean NOT NULL,
    name character varying(255) NOT NULL,
    pos bigint NOT NULL,
    class character varying(255) NOT NULL,
    project_id bigint
);


ALTER TABLE public.project_role OWNER TO vishnu_user;

--
-- Name: project_role_permissions; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE project_role_permissions (
    project_role_id bigint,
    permissions_string character varying(255)
);


ALTER TABLE public.project_role_permissions OWNER TO vishnu_user;

--
-- Name: role; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE role (
    id bigint NOT NULL,
    version bigint NOT NULL,
    authority character varying(255) NOT NULL,
    description text NOT NULL
);


ALTER TABLE public.role OWNER TO vishnu_user;

--
-- Name: state; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE state (
    numstateid bigint NOT NULL,
    cpuload integer,
    diskspace integer,
    machine_nummachineid bigint NOT NULL,
    memory integer,
    "time" timestamp without time zone
);


ALTER TABLE public.state OWNER TO vishnu_user;

--
-- Name: state_numstateid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE state_numstateid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.state_numstateid_seq OWNER TO vishnu_user;

--
-- Name: state_numstateid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE state_numstateid_seq OWNED BY state.numstateid;


--
-- Name: test_report; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE test_report (
    id bigint NOT NULL,
    version bigint NOT NULL,
    name character varying(255) NOT NULL
);


ALTER TABLE public.test_report OWNER TO vishnu_user;

--
-- Name: threshold; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE threshold (
    thresholdid bigint NOT NULL,
    machine_nummachineid bigint NOT NULL,
    typet integer,
    users_numuserid bigint NOT NULL,
    value integer
);


ALTER TABLE public.threshold OWNER TO vishnu_user;

--
-- Name: threshold_thresholdid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE threshold_thresholdid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.threshold_thresholdid_seq OWNER TO vishnu_user;

--
-- Name: threshold_thresholdid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE threshold_thresholdid_seq OWNED BY threshold.thresholdid;


--
-- Name: user_role; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE user_role (
    role_id bigint NOT NULL,
    user_id bigint NOT NULL
);


ALTER TABLE public.user_role OWNER TO vishnu_user;

--
-- Name: users; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE users (
    numuserid bigint NOT NULL,
    account_expired boolean,
    account_locked boolean,
    confirm_code character varying(255),
    date_created timestamp without time zone,
    email character varying(255),
    enabled boolean,
    firstname character varying(255),
    last_updated timestamp without time zone,
    lastname character varying(255),
    pwd character varying(255) NOT NULL,
    password_expired boolean,
    passwordstate integer,
    privilege integer,
    status integer,
    userid character varying(255) NOT NULL,
    vishnu_vishnuid bigint NOT NULL
);


ALTER TABLE public.users OWNER TO vishnu_user;

--
-- Name: users_numuserid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE users_numuserid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.users_numuserid_seq OWNER TO vishnu_user;

--
-- Name: users_numuserid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE users_numuserid_seq OWNED BY users.numuserid;


--
-- Name: vishnu; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE vishnu (
    vishnuid bigint NOT NULL,
    formatidauth character varying(255),
    formatidfiletransfer character varying(255),
    formatidjob character varying(255),
    formatidmachine character varying(255),
    formatiduser character varying(255),
    formatidwork character varying(255),
    updatefreq integer
);


ALTER TABLE public.vishnu OWNER TO vishnu_user;

--
-- Name: vishnu_vishnuid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE vishnu_vishnuid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.vishnu_vishnuid_seq OWNER TO vishnu_user;

--
-- Name: vishnu_vishnuid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE vishnu_vishnuid_seq OWNED BY vishnu.vishnuid;


--
-- Name: vsession; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE vsession (
    numsessionid bigint NOT NULL,
    authid character varying(255),
    clmachine_numclmachineid bigint NOT NULL,
    closepolicy integer,
    closure timestamp without time zone,
    creation timestamp without time zone,
    lastconnect timestamp without time zone,
    sessionkey character varying(255),
    state integer,
    timeout integer,
    users_numuserid bigint NOT NULL,
    vsessionid character varying(255)
);


ALTER TABLE public.vsession OWNER TO vishnu_user;

--
-- Name: vsession_numsessionid_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE vsession_numsessionid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.vsession_numsessionid_seq OWNER TO vishnu_user;

--
-- Name: vsession_numsessionid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE vsession_numsessionid_seq OWNED BY vsession.numsessionid;


--
-- Name: work; Type: TABLE; Schema: public; Owner: vishnu_user; Tablespace: 
--

CREATE TABLE work (
    id bigint NOT NULL,
    application_id bigint,
    date_created timestamp without time zone NOT NULL,
    date_ended timestamp without time zone,
    date_started timestamp without time zone,
    description text,
    done_ratio bigint NOT NULL,
    due_date timestamp without time zone,
    estimated_hours double precision NOT NULL,
    identifier character varying(255) NOT NULL,
    last_updated timestamp without time zone NOT NULL,
    machine_id bigint,
    nbcpus integer NOT NULL,
    owner_id bigint NOT NULL,
    priority integer NOT NULL,
    project_id bigint,
    start_date timestamp without time zone,
    status integer NOT NULL,
    subject character varying(255) NOT NULL,
    submit_date timestamp without time zone
);


ALTER TABLE public.work OWNER TO vishnu_user;

--
-- Name: work_id_seq; Type: SEQUENCE; Schema: public; Owner: vishnu_user
--

CREATE SEQUENCE work_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.work_id_seq OWNER TO vishnu_user;

--
-- Name: work_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: vishnu_user
--

ALTER SEQUENCE work_id_seq OWNED BY work.id;


--
-- Name: work_id_seq; Type: SEQUENCE SET; Schema: public; Owner: vishnu_user
--

SELECT pg_catalog.setval('work_id_seq', 1, false);

--
-- Name: numaccountid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY account ALTER COLUMN numaccountid SET DEFAULT nextval('account_numaccountid_seq'::regclass);


--
-- Name: authaccountid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY authaccount ALTER COLUMN authaccountid SET DEFAULT nextval('authaccount_authaccountid_seq'::regclass);


--
-- Name: numauthsystemid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY authsystem ALTER COLUMN numauthsystemid SET DEFAULT nextval('authsystem_numauthsystemid_seq'::regclass);


--
-- Name: numclmachineid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY clmachine ALTER COLUMN numclmachineid SET DEFAULT nextval('clmachine_numclmachineid_seq'::regclass);


--
-- Name: numcommandid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY command ALTER COLUMN numcommandid SET DEFAULT nextval('command_numcommandid_seq'::regclass);


--
-- Name: numdescriptionid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY description ALTER COLUMN numdescriptionid SET DEFAULT nextval('description_numdescriptionid_seq'::regclass);


--
-- Name: numfiletransferid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY filetransfer ALTER COLUMN numfiletransferid SET DEFAULT nextval('filetransfer_numfiletransferid_seq'::regclass);


--
-- Name: numjobid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY job ALTER COLUMN numjobid SET DEFAULT nextval('job_numjobid_seq'::regclass);


--
-- Name: ldapauthsystid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY ldapauthsystem ALTER COLUMN ldapauthsystid SET DEFAULT nextval('ldapauthsystem_ldapauthsystid_seq'::regclass);


--
-- Name: nummachineid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY machine ALTER COLUMN nummachineid SET DEFAULT nextval('machine_nummachineid_seq'::regclass);


--
-- Name: numoptionid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY optionu ALTER COLUMN numoptionid SET DEFAULT nextval('optionu_numoptionid_seq'::regclass);


--
-- Name: numoptionvalueid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY optionvalue ALTER COLUMN numoptionvalueid SET DEFAULT nextval('optionvalue_numoptionvalueid_seq'::regclass);


--
-- Name: numprocess; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY process ALTER COLUMN numprocess SET DEFAULT nextval('process_numprocess_seq'::regclass);


--
-- Name: numstateid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY state ALTER COLUMN numstateid SET DEFAULT nextval('state_numstateid_seq'::regclass);


--
-- Name: thresholdid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY threshold ALTER COLUMN thresholdid SET DEFAULT nextval('threshold_thresholdid_seq'::regclass);


--
-- Name: numuserid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY users ALTER COLUMN numuserid SET DEFAULT nextval('users_numuserid_seq'::regclass);


--
-- Name: vishnuid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY vishnu ALTER COLUMN vishnuid SET DEFAULT nextval('vishnu_vishnuid_seq'::regclass);


--
-- Name: numsessionid; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY vsession ALTER COLUMN numsessionid SET DEFAULT nextval('vsession_numsessionid_seq'::regclass);

--
-- Name: id; Type: DEFAULT; Schema: public; Owner: vishnu_user
--

ALTER TABLE work ALTER COLUMN id SET DEFAULT nextval('work_id_seq'::regclass);

--
-- Name: account_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY account
    ADD CONSTRAINT account_pkey PRIMARY KEY (numaccountid);


--
-- Name: acl_class_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY acl_class
    ADD CONSTRAINT acl_class_pkey PRIMARY KEY (id);


--
-- Name: acl_entry_acl_object_identity_ace_order_key; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY acl_entry
    ADD CONSTRAINT acl_entry_acl_object_identity_ace_order_key UNIQUE (acl_object_identity, ace_order);


--
-- Name: acl_entry_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY acl_entry
    ADD CONSTRAINT acl_entry_pkey PRIMARY KEY (id);


--
-- Name: acl_object_identity_object_id_class_object_id_identity_key; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY acl_object_identity
    ADD CONSTRAINT acl_object_identity_object_id_class_object_id_identity_key UNIQUE (object_id_class, object_id_identity);


--
-- Name: acl_object_identity_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY acl_object_identity
    ADD CONSTRAINT acl_object_identity_pkey PRIMARY KEY (id);


--
-- Name: acl_sid_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY acl_sid
    ADD CONSTRAINT acl_sid_pkey PRIMARY KEY (id);


--
-- Name: acl_sid_sid_principal_key; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY acl_sid
    ADD CONSTRAINT acl_sid_sid_principal_key UNIQUE (sid, principal);


--
-- Name: application_identifier_key; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY application
    ADD CONSTRAINT application_identifier_key UNIQUE (identifier);


--
-- Name: application_parameter_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY application_parameter
    ADD CONSTRAINT application_parameter_pkey PRIMARY KEY (id);


--
-- Name: application_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY application
    ADD CONSTRAINT application_pkey PRIMARY KEY (id);


--
-- Name: application_version_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY application_version
    ADD CONSTRAINT application_version_pkey PRIMARY KEY (id);


--
-- Name: authaccount_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY authaccount
    ADD CONSTRAINT authaccount_pkey PRIMARY KEY (authaccountid);


--
-- Name: authsystem_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY authsystem
    ADD CONSTRAINT authsystem_pkey PRIMARY KEY (numauthsystemid);


--
-- Name: clmachine_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY clmachine
    ADD CONSTRAINT clmachine_pkey PRIMARY KEY (numclmachineid);


--
-- Name: command_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY command
    ADD CONSTRAINT command_pkey PRIMARY KEY (numcommandid);


--
-- Name: description_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY description
    ADD CONSTRAINT description_pkey PRIMARY KEY (numdescriptionid);


--
-- Name: filetransfer_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY filetransfer
    ADD CONSTRAINT filetransfer_pkey PRIMARY KEY (numfiletransferid);


--
-- Name: global_project_role_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY global_project_role
    ADD CONSTRAINT global_project_role_pkey PRIMARY KEY (id);


--
-- Name: job_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY job
    ADD CONSTRAINT job_pkey PRIMARY KEY (numjobid);


--
-- Name: ldapauthsystem_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY ldapauthsystem
    ADD CONSTRAINT ldapauthsystem_pkey PRIMARY KEY (ldapauthsystid);


--
-- Name: machine_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY machine
    ADD CONSTRAINT machine_pkey PRIMARY KEY (nummachineid);


--
-- Name: notification_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY notification
    ADD CONSTRAINT notification_pkey PRIMARY KEY (id);


--
-- Name: optionu_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY optionu
    ADD CONSTRAINT optionu_pkey PRIMARY KEY (numoptionid);


--
-- Name: optionvalue_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY optionvalue
    ADD CONSTRAINT optionvalue_pkey PRIMARY KEY (numoptionvalueid);


--
-- Name: parameter_value_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY parameter_value
    ADD CONSTRAINT parameter_value_pkey PRIMARY KEY (id);


--
-- Name: permission_module_name_key; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY permission_module
    ADD CONSTRAINT permission_module_name_key UNIQUE (name);


--
-- Name: permission_module_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY permission_module
    ADD CONSTRAINT permission_module_pkey PRIMARY KEY (id);


--
-- Name: permission_name_key; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY permission
    ADD CONSTRAINT permission_name_key UNIQUE (name);


--
-- Name: permission_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY permission
    ADD CONSTRAINT permission_pkey PRIMARY KEY (id);


--
-- Name: process_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY process
    ADD CONSTRAINT process_pkey PRIMARY KEY (numprocess);


--
-- Name: project_application_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY project_application
    ADD CONSTRAINT project_application_pkey PRIMARY KEY (id);


--
-- Name: project_identifier_key; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY project
    ADD CONSTRAINT project_identifier_key UNIQUE (identifier);


--
-- Name: project_machine_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY project_machine
    ADD CONSTRAINT project_machine_pkey PRIMARY KEY (id);


--
-- Name: project_member_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY project_member
    ADD CONSTRAINT project_member_pkey PRIMARY KEY (id);


--
-- Name: project_name_key; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY project
    ADD CONSTRAINT project_name_key UNIQUE (name);


--
-- Name: project_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY project
    ADD CONSTRAINT project_pkey PRIMARY KEY (id);


--
-- Name: project_role_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY project_role
    ADD CONSTRAINT project_role_pkey PRIMARY KEY (id);


--
-- Name: role_authority_key; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY role
    ADD CONSTRAINT role_authority_key UNIQUE (authority);


--
-- Name: role_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY role
    ADD CONSTRAINT role_pkey PRIMARY KEY (id);


--
-- Name: state_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY state
    ADD CONSTRAINT state_pkey PRIMARY KEY (numstateid);


--
-- Name: test_report_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY test_report
    ADD CONSTRAINT test_report_pkey PRIMARY KEY (id);


--
-- Name: threshold_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY threshold
    ADD CONSTRAINT threshold_pkey PRIMARY KEY (thresholdid);


--
-- Name: user_role_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY user_role
    ADD CONSTRAINT user_role_pkey PRIMARY KEY (role_id, user_id);

--
-- Name: users_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY users
    ADD CONSTRAINT users_pkey PRIMARY KEY (numuserid);


--
-- Name: vishnu_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY vishnu
    ADD CONSTRAINT vishnu_pkey PRIMARY KEY (vishnuid);


--
-- Name: vsession_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY vsession
    ADD CONSTRAINT vsession_pkey PRIMARY KEY (numsessionid);


--
-- Name: work_pkey; Type: CONSTRAINT; Schema: public; Owner: vishnu_user; Tablespace: 
--

ALTER TABLE ONLY work
    ADD CONSTRAINT work_pkey PRIMARY KEY (id);


--
-- Name: fk143bf46a813ac84c; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY user_role
    ADD CONSTRAINT fk143bf46a813ac84c FOREIGN KEY (user_id) REFERENCES users(numuserid) ON DELETE CASCADE;


--
-- Name: fk143bf46adc10046c; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY user_role
    ADD CONSTRAINT fk143bf46adc10046c FOREIGN KEY (role_id) REFERENCES role(id) ON DELETE CASCADE;


--
-- Name: fk19b3b13b9edd7798; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_member_project_role
    ADD CONSTRAINT fk19b3b13b9edd7798 FOREIGN KEY (project_member_roles_id) REFERENCES project_member(id);


--
-- Name: fk19b3b13bc94ba396; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_member_project_role
    ADD CONSTRAINT fk19b3b13bc94ba396 FOREIGN KEY (project_role_id) REFERENCES project_role(id);


--
-- Name: fk19bbd355bf2a6; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY job
    ADD CONSTRAINT fk19bbd355bf2a6 FOREIGN KEY (job_owner_id) REFERENCES users(numuserid);


--
-- Name: fk19bbd9207fb3b; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY job
    ADD CONSTRAINT fk19bbd9207fb3b FOREIGN KEY (machine_id) REFERENCES machine(nummachineid);


--
-- Name: fk19bbdf381dc90; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY job
    ADD CONSTRAINT fk19bbdf381dc90 FOREIGN KEY (workid) REFERENCES work(id);


--
-- Name: fk19bbdf58538bc; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY job
    ADD CONSTRAINT fk19bbdf58538bc FOREIGN KEY (vsession_numsessionid) REFERENCES vsession(numsessionid) on delete cascade;


--
-- Name: fk237a88eb7276891; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY notification
    ADD CONSTRAINT fk237a88eb7276891 FOREIGN KEY (notifier_id) REFERENCES users(numuserid);


--
-- Name: fk237a88ebbd4995a3; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY notification
    ADD CONSTRAINT fk237a88ebbd4995a3 FOREIGN KEY (project_id) REFERENCES project(id);


--
-- Name: fk2a2bb00970422cc5; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY acl_object_identity
    ADD CONSTRAINT fk2a2bb00970422cc5 FOREIGN KEY (object_id_class) REFERENCES acl_class(id);


--
-- Name: fk2a2bb00990ec1949; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY acl_object_identity
    ADD CONSTRAINT fk2a2bb00990ec1949 FOREIGN KEY (owner_sid) REFERENCES acl_sid(id);


--
-- Name: fk2a2bb009a50290b8; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY acl_object_identity
    ADD CONSTRAINT fk2a2bb009a50290b8 FOREIGN KEY (parent_object) REFERENCES acl_object_identity(id);


--
-- Name: fk2c887f85a63719f2; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY authaccount
    ADD CONSTRAINT fk2c887f85a63719f2 FOREIGN KEY (users_numuserid) REFERENCES users(numuserid) on delete cascade;


--
-- Name: fk2c887f85fc5a9563; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY authaccount
    ADD CONSTRAINT fk2c887f85fc5a9563 FOREIGN KEY (authsystem_authsystemid) REFERENCES authsystem(numauthsystemid) on delete cascade;


--
-- Name: fk2e32855b3ccf497f; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY parameter_value
    ADD CONSTRAINT fk2e32855b3ccf497f FOREIGN KEY (type_id) REFERENCES application_parameter(id);


--
-- Name: fk2e32855b9d863974; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY parameter_value
    ADD CONSTRAINT fk2e32855b9d863974 FOREIGN KEY (parent_id) REFERENCES work(id);


--
-- Name: fk2ec53e80bd4995a3; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_member
    ADD CONSTRAINT fk2ec53e80bd4995a3 FOREIGN KEY (project_id) REFERENCES project(id);


--
-- Name: fk2ec53e80da00831d; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_member
    ADD CONSTRAINT fk2ec53e80da00831d FOREIGN KEY (member_id) REFERENCES users(numuserid);


--
-- Name: fk30e4e8befc5a9563; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY ldapauthsystem
    ADD CONSTRAINT fk30e4e8befc5a9563 FOREIGN KEY (authsystem_authsystemid) REFERENCES authsystem(numauthsystemid) on delete cascade;


--
-- Name: fk31314447c2584ca8; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY machine
    ADD CONSTRAINT fk31314447c2584ca8 FOREIGN KEY (vishnu_vishnuid) REFERENCES vishnu(vishnuid);


--
-- Name: fk37c7113462ea58; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY work
    ADD CONSTRAINT fk37c7113462ea58 FOREIGN KEY (application_id) REFERENCES application_version(id);


--
-- Name: fk37c7119207fb3b; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY work
    ADD CONSTRAINT fk37c7119207fb3b FOREIGN KEY (machine_id) REFERENCES machine(nummachineid) on delete cascade;


--
-- Name: fk37c711bd4995a3; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY work
    ADD CONSTRAINT fk37c711bd4995a3 FOREIGN KEY (project_id) REFERENCES project(id) on delete cascade;


--
-- Name: fk37c711ed217864; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY work
    ADD CONSTRAINT fk37c711ed217864 FOREIGN KEY (owner_id) REFERENCES users(numuserid) on delete cascade;


--
-- Name: fk37fff5dcbd4995a3; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_role
    ADD CONSTRAINT fk37fff5dcbd4995a3 FOREIGN KEY (project_id) REFERENCES project(id);


--
-- Name: fk38a5df4bf58538bc; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY command
    ADD CONSTRAINT fk38a5df4bf58538bc FOREIGN KEY (vsession_numsessionid) REFERENCES vsession(numsessionid) on delete cascade;


--
-- Name: fk5302d47d8fdb88d5; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY acl_entry
    ADD CONSTRAINT fk5302d47d8fdb88d5 FOREIGN KEY (sid) REFERENCES acl_sid(id);


--
-- Name: fk5302d47db0d9dc4d; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY acl_entry
    ADD CONSTRAINT fk5302d47db0d9dc4d FOREIGN KEY (acl_object_identity) REFERENCES acl_object_identity(id);


--
-- Name: fk581b3160a63719f2; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY vsession
    ADD CONSTRAINT fk581b3160a63719f2 FOREIGN KEY (users_numuserid) REFERENCES users(numuserid) on delete cascade;


--
-- Name: fk581b3160c401bd40; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY vsession
    ADD CONSTRAINT fk581b3160c401bd40 FOREIGN KEY (clmachine_numclmachineid) REFERENCES clmachine(numclmachineid);


--
-- Name: fk5ca40550bd4995a3; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY application
    ADD CONSTRAINT fk5ca40550bd4995a3 FOREIGN KEY (project_id) REFERENCES project(id);


--
-- Name: fk5ca40550e1f3ba8c; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY application
    ADD CONSTRAINT fk5ca40550e1f3ba8c FOREIGN KEY (author_id) REFERENCES users(numuserid);


--
-- Name: fk68ac4911cfedefc; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY state
    ADD CONSTRAINT fk68ac4911cfedefc FOREIGN KEY (machine_nummachineid) REFERENCES machine(nummachineid) on delete cascade;


--
-- Name: fk6a68e08c2584ca8; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY users
    ADD CONSTRAINT fk6a68e08c2584ca8 FOREIGN KEY (vishnu_vishnuid) REFERENCES vishnu(vishnuid);


--
-- Name: fk74e3bbd8d89fc97; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY global_project_role_permissions
    ADD CONSTRAINT fk74e3bbd8d89fc97 FOREIGN KEY (global_project_role_id) REFERENCES global_project_role(id);


--
-- Name: fk993583fc1cfedefc; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY description
    ADD CONSTRAINT fk993583fc1cfedefc FOREIGN KEY (machine_nummachineid) REFERENCES machine(nummachineid) on delete cascade;


--
-- Name: fka2850d219207fb3b; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_machine
    ADD CONSTRAINT fka2850d219207fb3b FOREIGN KEY (machine_id) REFERENCES machine(nummachineid);


--
-- Name: fka2850d21bd4995a3; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_machine
    ADD CONSTRAINT fka2850d21bd4995a3 FOREIGN KEY (project_id) REFERENCES project(id);


--
-- Name: fka3e1e46b1cfedefc; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY threshold
    ADD CONSTRAINT fka3e1e46b1cfedefc FOREIGN KEY (machine_nummachineid) REFERENCES machine(nummachineid) on delete cascade;


--
-- Name: fka3e1e46ba63719f2; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY threshold
    ADD CONSTRAINT fka3e1e46ba63719f2 FOREIGN KEY (users_numuserid) REFERENCES users(numuserid);


--
-- Name: fkadc771d7c2584ca8; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY authsystem
    ADD CONSTRAINT fkadc771d7c2584ca8 FOREIGN KEY (vishnu_vishnuid) REFERENCES vishnu(vishnuid);


--
-- Name: fkadd15ea1c94ba396; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_role_permissions
    ADD CONSTRAINT fkadd15ea1c94ba396 FOREIGN KEY (project_role_id) REFERENCES project_role(id);


--
-- Name: fkb9d38a2d1cfedefc; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY account
    ADD CONSTRAINT fkb9d38a2d1cfedefc FOREIGN KEY (machine_nummachineid) REFERENCES machine(nummachineid) on delete cascade;


--
-- Name: fkb9d38a2da63719f2; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY account
    ADD CONSTRAINT fkb9d38a2da63719f2 FOREIGN KEY (users_numuserid) REFERENCES users(numuserid) on delete cascade;


--
-- Name: fkc5b678e99fcaff4; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY application_version
    ADD CONSTRAINT fkc5b678e99fcaff4 FOREIGN KEY (application_id) REFERENCES application(id);


--
-- Name: fke125c5cf6e594880; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY permission
    ADD CONSTRAINT fke125c5cf6e594880 FOREIGN KEY (module_id) REFERENCES permission_module(id);


--
-- Name: fkebde0a1ca63719f2; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY optionvalue
    ADD CONSTRAINT fkebde0a1ca63719f2 FOREIGN KEY (users_numuserid) REFERENCES users(numuserid) on delete cascade;


--
-- Name: fkebde0a1cbe23b1a5; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY optionvalue
    ADD CONSTRAINT fkebde0a1cbe23b1a5 FOREIGN KEY (optionu_numoptionid) REFERENCES optionu(numoptionid);


--
-- Name: fkfa594f7a3462ea58; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY application_parameter
    ADD CONSTRAINT fkfa594f7a3462ea58 FOREIGN KEY (application_id) REFERENCES application_version(id);


--
-- Name: fkfad3d12a9fcaff4; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_application
    ADD CONSTRAINT fkfad3d12a9fcaff4 FOREIGN KEY (application_id) REFERENCES application(id);


--
-- Name: fkfad3d12abd4995a3; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY project_application
    ADD CONSTRAINT fkfad3d12abd4995a3 FOREIGN KEY (project_id) REFERENCES project(id);


--
-- Name: fkfce97167f58538bc; Type: FK CONSTRAINT; Schema: public; Owner: vishnu_user
--

ALTER TABLE ONLY filetransfer
    ADD CONSTRAINT fkfce97167f58538bc FOREIGN KEY (vsession_numsessionid) REFERENCES vsession(numsessionid) on delete cascade;


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;

-- Role Creation;

CREATE USER vishnu_user WITH PASSWORD 'vishnu_user';
CREATE USER vishnu_db_admin;

grant connect
on database vishnu
to vishnu_user;

grant connect
on database vishnu
to vishnu_db_admin;


GRANT ALL ON vishnu TO "vishnu_db_admin";
GRANT SELECT ON vishnu TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON users TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON machine TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON clmachine TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionu TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON state TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON description TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON vsession TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON account TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionvalue TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON threshold TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON command TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON filetransfer TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON job TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON process TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_db_admin";


--CREATE ROLE vishnu_user;

--Grant on right on table

GRANT SELECT, INSERT, UPDATE, DELETE ON vishnu TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON users TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON machine TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON clmachine TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionu TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON state TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON description TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON vsession TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON account TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionvalue TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON threshold TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON command TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON filetransfer TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON job TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON process TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_user";

--Grant on sequences

GRANT ALL ON SEQUENCE account_numaccountid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE clmachine_numclmachineid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE command_numcommandid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE description_numdescriptionid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE filetransfer_numfiletransferid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE job_numjobid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE machine_nummachineid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE optionu_numoptionid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE optionvalue_numoptionvalueid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE state_numstateid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE threshold_thresholdid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE users_numuserid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE vishnu_vishnuid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE vsession_numsessionid_seq TO vishnu_user;
GRANT ALL ON SEQUENCE process_numprocess_seq TO vishnu_user;
GRANT ALL ON SEQUENCE authaccount_authaccountid_seq TO "vishnu_user";
GRANT ALL ON SEQUENCE authsystem_numauthsystemid_seq TO "vishnu_user";
GRANT ALL ON SEQUENCE ldapauthsystem_ldapauthsystid_seq TO "vishnu_user";




