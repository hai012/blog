#!/bin/bash


git config --global user.name "ghjiee"
git config --global user.email "9637301+ghjiee@user.noreply.gitee.com"


if [ "$1" == "remote" ]
then
	rm -rf ~/doc/blog/*
	cp ~/shared_198/doc/*  .  -r
	git add -A
	git commit -m "update"
	git push origin master
elif [ "$1" == "local" ]
then
	git pull
	rm -rf ~/shared_198/doc/*
	cp ~/doc/blog/*  ~/shared_198/doc/  -r
else
	echo "usage: ./update <local/remote>"
fi
