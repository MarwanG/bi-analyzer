#! /bin/bash

# DIFFERENT COMMANDS FOR SESSION TREATEMENT

echo "FILE TO TREAT : " $1 


echo "Creating inverse cumulative distribution of number of sessions"
awk '{print $2}' $1| sort | uniq -c | awk '{print $1}' | sort -n -r | uniq -c | awk '{n=n+$1}{print $2,n}' > $1_dist_nb_sessions.txt


