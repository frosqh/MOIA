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
					System.out.println("Choisir la piece à deplacer : ");
					int piece = scan.nextInt();
					dos.writeInt(piece);
					System.out.println("Choisir la colonne de depart (A,B,...E) : ");
					int colonneDep = scan.nextInt();
					dos.writeInt(colonneDep);
					System.out.println("Choisir la ligne de depart (UN,DEUX,...SIX) : ");
					int ligneDep = scan.nextInt();
					dos.writeInt(ligneDep);
					System.out.println("Choisir la colonne d'arrivée (A,B,...E) : ");
					int colonneArr = scan.nextInt();
					dos.writeInt(colonneArr);
					System.out.println("Choisir la ligne d'arrivée (UN,DEUX,...SIX) : ");
					int ligneArr = scan.nextInt();
					dos.writeInt(ligneArr);
					System.out.println("Capture ? 1 : 0 ");
					int capture = scan.nextInt();
					dos.writeInt(capture);
				}

				// 
				
				//srv.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		
	}
}