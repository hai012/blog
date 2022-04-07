#!/bin/bash
cp ~/shared_198/doc/markdown . -r
git add -A
git commit -m "update"
git push origin master
