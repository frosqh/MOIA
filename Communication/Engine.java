//
// Prgm: socket de base en java, serveur
//
// auteur : LP
//
// date : 7/03/06
//

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;
import java.nio.*;



public class Engine {
	
	public static void main(String[] args) {
		if (args.length != 1) {
			System.out.println("argument - port ");
			System.exit(1);
		}
		ServerSocket srv;
		int port = Integer.parseInt(args[0]);


		
			try {
				
				//creation de la socket serveur sur le meme port que le client
				srv = new ServerSocket(port);
				Socket s = srv.accept();

				while(true){
					OutputStream os = s.getOutputStream();
					DataOutputStream dos= new DataOutputStream(os);
					
					Scanner scan = new Scanner(System.in);
					//construction d'un coup manuellement
					System.out.println("Choisir le numero de la partie (1 ou 2) : ");
					int numPartie = scan.nextInt();

					//TCoup typeCoup = scan.nextInt();
					System.out.println("Choisir le type du coup (0 pour DEPLACER) : ");
					int netTcoup = scan.nextInt();
					
					
					//TSensTetePiece sensPiece ;
					System.out.println("Choisir le sens de la piece à deplacer (0 = NORD ou 1 = SUD) : ");
					int netSPiece = scan.nextInt();
					
					//TTYpePiece typePiece;
					
					System.out.println("Choisir le type de la piece à deplacer : ");
					int netTPiece = scan.nextInt();
					
					//Piece piece = new Piece(sensPiece,typePiece);

					
					//TCol colonneDep;
					System.out.println("Choisir la colonne de depart (A,B) : ");
					int netCDep = scan.nextInt();
					
					
					int ligneDep;
					System.out.println("Choisir la ligne de depart (UN,DEUX,...SIX) : ");
					int netLDep = scan.nextInt();
					
					

					//Case caseDepart = new Case(colonneDep,ligneDep);
					//dos.writeInt(ligneDep);
					//TCol colonneArr;
					System.out.println("Choisir la colonne de depart (A,B) : ");
					int netCArr = scan.nextInt();
							
					//TLg ligneArr;
					System.out.println("Choisir la ligne de depart (UN,DEUX,...SIX) : ");
					int netLArr = scan.nextInt();
					
					//Case caseArrivee = new Case(colonneArr,ligneArr);
					//dos.writeInt(ligneArr);
					System.out.println("Capture ? 1 : 0 ");
					int capture = scan.nextInt();
					//dos.writeInt(capture);
					//Deplacement deplacement = new Deplacement(caseDepart,caseArrivee,capture);
					//Utile pour apres
					//Coup coup = new Coup(TCoup.COUP,numPartie,typeCoup,piece,deplacement);

					dos.writeInt(1);
					dos.writeInt(numPartie);
					dos.writeInt(netTcoup);
					dos.writeInt(netSPiece);
					dos.writeInt(netTPiece);
					dos.writeInt(netCDep);
					dos.writeInt(netLDep);
					dos.writeInt(netCArr);
					dos.writeInt(netLArr);
					dos.writeInt(capture);
					

				}

				// 
				
				//srv.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		
	}
}