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

all: joueur 

#fonctionsTCP.o: fonctionsTCP.c fonctionsTCP.h
#	gcc -Wall -c fonctionsTCP.c

joueur: joueur.c fonctionsTCP.o  fonctionsTCP.h protocolYokai.h
	gcc -Wall joueur.c -o joueur fonctionsTCP.o  $(LD_FLAGS)



clean:
	rm *~ ; rm -i \#* ; rm *.o; \
        rm joueur ; 
