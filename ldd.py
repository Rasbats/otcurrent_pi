import glob; import subprocess 
lib = glob.glob("app/*/plugins/*.dll")[0] 
subprocess.call(['dumpbin', '/dependents', lib], shell=True) 
