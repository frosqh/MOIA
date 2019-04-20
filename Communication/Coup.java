public class Coup{
	int piece,colonneDep,colonneArr,ligneDep,ligneArr,capture;

	public Coup(int p,int cd,int ld,int ca,int la,int c){
		piece = p;
		colonneDep = cd;
		ligneDep = ld;
		colonneArr = ca;
		ligneArr = la;
		capture = c;
	}

	public int getPiece(){
		return piece;
	}

	public int getCDep(){
		return colonneDep;
	}

	public int getLDep(){
		return ligneDep;
	}

	public int getCArr(){
		return colonneArr;
	}

	public int getLArr(){
		return ligneArr;
	}
	public int getCapture(){
		return capture;
	}

}