enum TTypePiece {
    KODAMA,
    KODAMA_SAMOURAI,
    KIRIN,
    KOROPOKKURU,
    ONI,
    SUPER_ONI;
}

enum TSensTetePiece{
    NORD,
    SUD;
}


public class Piece{

    private TSensTetePiece sensPiece;
    private TTypePiece typePiece;

    public Piece(TSensTetePiece s,TTypePiece t){
        sensPiece = s;
        typePiece = t;
    }

    public TSensTetePiece getSensPiece() {
        return sensPiece;
    }

    public TTypePiece getTypePiece() {
        return typePiece;
    }

    public void setSensPiece(TSensTetePiece sensPiece) {
        this.sensPiece = sensPiece;
    }

    public void setTypePiece(TTypePiece typePiece) {
        this.typePiece = typePiece;
    }
}