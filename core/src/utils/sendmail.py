#! /usr/bin/python

import sys
import smtplib
import subprocess as sb
from optparse import OptionParser

from email.MIMEText import MIMEText

# TODO: move to argparse when we don't support python runtime older's than 2.7
def setup_parser():
    parser = OptionParser()
    # by default optparse sets action=store, type=string
    parser.add_option("-f", "--filename", dest="filename", help="mail content (file)")
    parser.add_option("--from", dest="sender", help="email sender")
    parser.add_option("--to", dest="recipients", help="email recipients")
    parser.add_option("-s", "--subject", dest="subject", help="email subject")
    parser.add_option("--login", dest="login", help="smtp login")
    parser.add_option("--password", dest="password", help="smtp password")
    parser.add_option("--hostname", dest="host", help="smtp host", default="localhost")
    parser.add_option("--port", dest="port", help="smtp port [default: 25]", type=int)
    parser.add_option("--ssl", action="store_true", dest="use_ssl", help="enable ssl support [default: %default - default port: 465]", default=False)
    parser.add_option("--debug", action="store_true", dest="use_debug", help="enable debugging message [default: %default]", default=False)
    return parser


def send_mail(options, args):
    if options.port is None:
        if options.use_ssl:
            options.port = 465
        else:
            options.port = 25
    msg = MIMEText("")
    try:
        if options.filename:
            f = open(options.filename, "rb")
            msg = MIMEText(f.read())
            f.close()
        else:
            # strictly not an error but simplify script
            raise Exception("no filename provided")
    except Exception, e:
        if args:
            msg = MIMEText(args[0])

    print msg
    # set mail headers
    msg["Subject"] = options.subject
    msg["From"] = options.sender
    msg["To"] = options.recipients

    # send email 
    try:
        s = smtplib.SMTP()
        s.set_debuglevel(options.use_debug)
        s.connect(options.host, options.port)
        # check if we use authentification
        if options.use_ssl:
            s.starttls()
        s.ehlo(options.sender)
        if options.login and options.password:
            s.login(options.login, options.password)
        s.sendmail(options.sender, options.recipients, msg.as_string())
    except smtplib.SMTPException, e:
        print e
        s.quit()
        

if __name__ == '__main__':
    p = setup_parser()
    options, args = p.parse_args()
    if len(sys.argv) == 1:
        p.print_help()
        sys.exit(-1)
    send_mail(options, args)
    sys.exit(0)
