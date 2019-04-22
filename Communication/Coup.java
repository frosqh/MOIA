








public class Coup{

    IdRequest idRequete;
    int numPartie;
    TCoup typeCoup;
    Piece piece;
    Deplacement deplacement;
    Depos depos;


    public Coup(IdRequest idReq, int numP, TCoup typeC, Piece p, Deplacement d){
        idRequete = idReq;
        numPartie = numP;
        typeCoup = typeC;
        piece = p;
        deplacement = d;
    }

    public Coup(IdRequest idReq, int numP, TCoup typeC, Piece p, Depos d){
        idRequete = idReq;
        numPartie = numP;
        typeCoup = typeC;
        piece = p;
        depos = d;
    }

    public IdRequest getIdRequete() {
        return idRequete;
    }

    public int getNumPartie() {
        return numPartie;
    }

    public Piece getPiece() {
        return piece;
    }

    public TCoup getTypeCoup() {
        return typeCoup;
    }

    public Depos getDepos() {
        return depos;
    }

    public Deplacement getDeplacement() {
        return deplacement;
    }

    public void setDeplacement(Deplacement deplacement) {
        this.deplacement = deplacement;
    }

    public void setDepos(Depos depos) {
        this.depos = depos;
    }

    public void setIdRequete(IdRequest idRequete) {
        this.idRequete = idRequete;
    }

    public void setNumPartie(int numPartie) {
        this.numPartie = numPartie;
    }

    public void setPiece(Piece piece) {
        this.piece = piece;
    }

    public void setTypeCoup(TCoup typeCoup) {
        this.typeCoup = typeCoup;
    }
}