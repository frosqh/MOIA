#!/bin/bash

set -e

export LD_LIBRARY_PATH=/applis/sicstus-3.11.2/lib/

cd Communication/

make joueur -f Makefile

cd ..

cd srcMOIA/

javac -cp "/applis/sicstus-3.11.2/lib/sicstus-3.11.2/bin/jasper.jar" *.java

java "/applis/sicstus-3.11.2/lib/sicstus-3.11.2/bin/jasper.jar" Engine &

cd ../

./Communication/joueur $1

