################################
#
# Makefile CS (client/serveur)
#  mode connecté
#  bibliothèque : fonctionsTCP
#
################################

# For Solaris
#LD_FLAGS = -lsocket -lnsl

# For Linux
LD_FLAGS =

all: joueur serveur

#fonctionsTCP.o: fonctionsTCP.c fonctionsTCP.h
#	gcc -Wall -c fonctionsTCP.c

joueur: joueur.c fonctionsTCP.o protocolYokai.o fonctionsTCP.h protocolYokai.h
	gcc -Wall joueur.c -o joueur fonctionsTCP.o protocole.o $(LD_FLAGS)

serveur: serveur.c fonctionsTCP.o protocole.o fonctionsTCP.h protocolYokai.h
	gcc -Wall serveur.c -o serveur fonctionsTCP.o protocolYokai.o  $(LD_FLAGS)

clean:
	rm *~ ; rm -i \#* ; rm *.o; \
        rm joueur ; rm serveur
