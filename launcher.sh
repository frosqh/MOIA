#!/bin/bash

set -e

export LD_LIBRARY_PATH=/usr/local/sicstus4.4.1/lib/

cd Communication/

make joueur -f Makefile

cd ..

cd srcMOIA/

javac -cp /usr/local/sicstus4.4.1/lib/sicstus-4.4.1/bin/jasper.jar *.java
jar cvfm Engine.jar otherMF/META-INF/MANIFEST.MF *.class
nohup java -jar Engine.jar $1 &

sleep 1

cd ../

./Communication/joueur $2 $3 $1

fg