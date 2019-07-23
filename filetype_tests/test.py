import subprocess
import argparse

file_type ="32 bits"
dirs = ["/bin/", "/usr/bin/", "/usr/sbin/", "/sbin/"]

for di in dirs:
    list_files = subprocess.run(["ls", di], capture_output=True).stdout.decode("utf-8").split("\n")
    for f in list_files:
        if f == "":
            continue
        ff = di + f
        out = subprocess.run(["./ft_nm", ff], capture_output=True).stdout.decode("utf-8").split("\n")
        if out[0] == file_type:
            print(ff)
