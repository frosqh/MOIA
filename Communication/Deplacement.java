public class Deplacement{

    private Case caseDep;
    private Case caseArr;
    private int capture;


    public Deplacement(Case cDep,Case cArr,int cpt){
            caseDep = cDep;
            caseArr = cArr;
            capture = cpt;
    }

    public Case getCaseArr() {
        return caseArr;
    }

    public Case getCaseDep() {
        return caseDep;
    }


    public int isCapture() {
        return capture;
    }

    public void setCapture(int capture) {
        this.capture = capture;
    }

    public void setCaseArr(Case caseArr) {
        this.caseArr = caseArr;
    }

    public void setCaseDep(Case caseDep) {
        this.caseDep = caseDep;
    }
}