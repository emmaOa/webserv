#!/bin/bash
while [ 1 ]
do 
    leaks webserv | grep "leaks for"
    sleep 1
done