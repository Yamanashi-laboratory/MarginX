# -*- coding: utf-8 -*-
import subprocess

input = input("Are you using ninniku (server)? (y / n)")

if input.lower() == 'n':
    print("Please input the command which executes JoSIM. ex:) josim_v265, josim-cli, josim etc...")
    result1 = subprocess.run(["make", "mew"], stdout=subprocess.PIPE, text=True)
    result1_exe = subprocess.run(["./mew"], stdout=subprocess.PIPE, text=True)

result2 = subprocess.run(["make", "setup"], stdout=subprocess.PIPE, text=True)
result2_exe = subprocess.run(["./setup"], stdout=subprocess.PIPE, text=True)

result3 = subprocess.run(["make", "MarginX"], stdout=subprocess.PIPE, text=True)
