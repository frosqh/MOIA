enum TLg{
    UN,
    DEUX,
    TROIS,
    QUATRE,
    CINQ,
    SIX; };

enum TCol{
    A,
    B,
    C,
    D,
    E; };


public class Case{
    private TCol colonne;
    private TLg ligne;

    public Case(TCol c, TLg l){
        colonne = c;
        ligne = l;
    }


    public TCol getColonne() {
        return colonne;
    }

    public TLg getLigne() {
        return ligne;
    }

    public void setColonne(TCol colonne) {
        this.colonne = colonne;
    }

    public void setLigne(TLg ligne) {
        this.ligne = ligne;
    }


    
}