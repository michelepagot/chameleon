import os, shutil, re

class chameleon(object):
    def __init__(self,app_name,work_dir='tmp'):
        self._env = list()
        self._target_app_name = app_name
        self._work_dir_name = work_dir

    def init(self):
        self._env = os.environ.copy()
        base_dir = os.path.dirname(os.path.realpath(__file__))
        self.work_dir = os.path.join(base_dir,self._work_dir_name)
        self.target_app = os.path.join(self.work_dir,self._target_app_name)

        chameleon_original = os.path.join(base_dir,*['bin','win','chameleon.exe'])

        if not os.path.exists(self.work_dir):
            os.makedirs(self.work_dir)

        shutil.copyfile(chameleon_original,self.target_app)

    def set_return(self,ret):
        self._env["CHAMELEON_RETURN"] = str(int(ret))
    
    def enable_argv_log(self):
        self._env["CHAMELEON_ARGV"] = os.path.join(self.work_dir,'argv.log')

    def get_argv_log(self):
        data = dict()
        with open(os.path.join(self.work_dir,'argv.log'),'r') as fa:
             m = re.search('argc=([0-9]+)', fa.readline())
             data['argc']=int(m.group(1))
             data['argv']=dict()
             for line in fa.readlines():
                 m = re.search('argv\[([0-9]+)\]=(.*)', line)
                 data['argv'][int(m.group(1))]=str(m.group(2))
        return data

    def set_out(self,out):
        if os.path.exists(out):
            self._env["CHAMELEON_OUT"] = out

    def get_env(self):
        return self._env

    def get_targetapp(self):
        return self.target_app
