Lecture section : D1
TA's name : Scott Kristjanson

Description :
1. This is the last assignment of SFU-CMPT-300.

2. How to run it
    For server:
        cd Server
        make
        ./lyrebird.server [config_file.txt] [log_file.txt]
    For client
        cd Client
        make
        ./lyrebird.client [server_ip] [server_port]

3. Each file
    Client:
        awesome.h : head file
        awesome.c : some functions used in this assignment such as fastMod
        mypipe.h : head file about pipelines
    	mypipe.h : functions relavant to pipelines.
    	main.c : as its name.
        monster : a script to kill remaining chile process
    Server:
        ipaddr.h/.c : Functions about ipaddress
        server.c : server

4. Honesty code outlines:
    In this program, I used some code from code from stackoverflow. (Socket connection, Pthread)
    The other parts are all done by myself.
