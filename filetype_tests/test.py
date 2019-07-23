import subprocess
import argparse

file_type ="fat file"
dirs = ["/bin/", "/usr/bin/", "/usr/sbin/", "/sbin/", "/usr/local/bin/", "/usr/lib/", "/usr/libexec/", "/usr/local/lib/"]

for di in dirs:
    list_files = subprocess.run(["ls", di], capture_output=True).stdout.decode("utf-8").split("\n")
    for f in list_files:
        if f == "":
            continue
        ff = di + f
        out = subprocess.run(["./ft_nm", ff], capture_output=True).stdout.decode("utf-8").split("\n")
        if out[0] == file_type:
            print(ff)
