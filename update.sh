#!/bin/bash
git config --global user.name "ghjiee"
git config --global user.email "9637301+ghjiee@user.noreply.gitee.com"
cp ~/shared_198/doc/markdown . -r
git add -A
git commit -m "update"
git push origin master
