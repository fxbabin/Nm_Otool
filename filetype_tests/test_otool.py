# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_nm.py                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arobion <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/01 15:45:46 by arobion           #+#    #+#              #
#    Updated: 2019/07/31 18:11:48 by fbabin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import subprocess
import argparse
from color_defines import *

files_fail = []
files_segv = []

def launch_test(exec_cmd, f):
    nb_tests = 0
    nb_success = 0
    nb_fail = 0
    nb_segv = 0
    for line in f.readlines():
        line = line.strip()
        out = subprocess.run([exec_cmd + " " + line], shell=True, stdout=subprocess.PIPE)
        if out.returncode != 0 and out.returncode != 1:
            print("{} => {}SEGV{} ".format(line, FAIL, ENDC))
            files_segv.append(line)
            nb_segv += 1
        else:
            true_out = subprocess.run(["otool -t" + " " + line], shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            if true_out.stdout == out.stdout or true_out.stderr != b'':
                print("{} => {}SUCCESS{}".format(line, OKGREEN, ENDC))
                nb_success += 1
            else:
                print("{} => {}FAIL{}".format(line, WARNING, ENDC))
                nb_fail += 1
                files_fail.append(line)
    return [nb_tests, nb_success, nb_fail, nb_segv]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--directory", type=str, help="optional, choose a directory")
    parser.add_argument("-f", "--file", type=str, help="optional")
    args = parser.parse_args()
    exec_cmd = "./ft_otool"

    print("compilation and copying of ft_nm")
    subprocess.run(["rm -f ft_otool"], shell=True)
    ret = subprocess.run(["cd .. ; make re ; cp ft_otool filetype_tests/"], shell=True, stdout=subprocess.PIPE)
    if ret.returncode != 0:
        print("\t{}compilation FAILURE {}".format(FAIL, ENDC))
        return
    else:
        print("\t{}compilation SUCCESS {}\n".format(OKGREEN, ENDC))

    with open(args.file) as f:
        nb_tests, nb_success, nb_fail, nb_segv = launch_test(exec_cmd, f)
        print("\nRESUME\n\tnb tests : {}\n\tnb success : {}{}{}\n\tnb_fail : {}{}{}\n\tnb_segv : {}{}{}".format(nb_tests, OKGREEN, nb_success, ENDC, WARNING, nb_fail, ENDC, FAIL, nb_segv, ENDC))

if __name__ == "__main__":
    main()
