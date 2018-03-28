import subprocess
from chameleon import chameleon

import logging


def test_main(tmpdir):
    logger = logging.getLogger("test.logging")
       
    text = """ciao metti questo 
    se non ti disturba troppo
    Grazie in anticipo"""
    p = tmpdir.mkdir("sub").join("out.txt")
    logger.info("p type:"+str(type(p)))
    logger.info("p:"+str(p))
    logger.info("tmpdir:"+str(tmpdir))
    p.write(text)
    c = chameleon.chameleon('ping.exe')
    c.init()
    c.set_out(str(p))
    proc = subprocess.Popen(c.get_targetapp(), env=c.get_env(), shell=True, universal_newlines=True, stdout=subprocess.PIPE)
    stdout_value = proc.communicate()[0].strip()

    logger.info("text type:"+str(type(text)))
    logger.info("text repr:"+repr(text))
    logger.info("stdout_value type:"+str(type(stdout_value)))
    logger.info("stdout_value repr:"+repr(stdout_value))

    assert(text == stdout_value)  

def test_argv():
    c = chameleon.chameleon('pong.exe')
    c.init()
    c.enable_argv_log()
    proc = subprocess.Popen([c.get_targetapp(), '--prot','prud'], env=c.get_env(), stdout=subprocess.PIPE)
    stdout_value = proc.communicate()[0]
    argv = c.get_argv_log()
    assert(3 == argv['argc'])
    assert('pong.exe' in argv['argv'][0])
    assert('--prot' == argv['argv'][1])

def test_return():
    c = chameleon.chameleon('pang.exe')
    c.init()
    c.set_return(2)
    proc = subprocess.Popen(c.get_targetapp(), env=c.get_env(), stdout=subprocess.PIPE)
    stdout_value = proc.communicate()[0]
    assert(2 == proc.returncode)

def test_default_return():
    c = chameleon.chameleon('pong.exe')
    c.init()
    proc = subprocess.Popen(c.get_targetapp(), env=c.get_env(), stdout=subprocess.PIPE)
    stdout_value = proc.communicate()[0]
    assert(0 == proc.returncode)