/*
 * Définition des fonctions et méthodes du fichier UTProfiler.h
*/

#include "UTProfiler.h"

Saison StringToSaison(const QString& s)
{
    if(s=="A")
        return Automne;
    else if(s=="P")
        return Printemps;
    else
        throw UTProfilerException("Erreur, saison inexistante");
}

QTextStream& operator>>(QTextStream& f, Categorie& cat){
    QString str;
    f>>str;
    if (str=="CS") cat=CS;
    else
    if (str=="TM") cat=TM;
    else
    if (str=="SP") cat=SP;
    else
    if (str=="TSH") cat=TSH;
    else {
        throw UTProfilerException("erreur, lecture categorie");
    }
    return f;
}

Categorie StringToCategorie(const QString& str){
    if (str=="CS") return CS;
    else
    if (str=="TM") return TM;
    else
    if (str=="SP") return SP;
    else
    if (str=="TSH") return TSH;
    else {
        throw UTProfilerException(QString("erreur, StringToCategorie, categorie ")+str+" inexistante");
    }
}

QString CategorieToString(Categorie c){
    switch(c){
    case CS: return "CS";
    case TM: return "TM";
    case SP: return "SP";
    case TSH: return "TSH";
    default: throw UTProfilerException("erreur, categorie non traitee");
    }
}

QTextStream& operator<<(QTextStream& f, const Categorie& cat){
    return f<<CategorieToString(cat);
}

//Fonction permettenat de renvoyer une note transformée en chaine de caractères
QString NoteToString(Note n)
{
    switch(n) {
    case A: return "A";
    case B: return "B";
    case C: return "C";
    case D: return "D";
    case E: return "E";
    case F: return "F";
    case FX: return "FX";
    case RES: return "RES";
    case ABS: return "ABS";
    case EC: return "EC";
    default: throw UTProfilerException("Erreur, note non traitee");
    }
}

//Transformation d'une chaine de caractères en Note
Note StringToNote(const QString& s)
{
    if(s=="A")
        return A;
    if(s=="B")
        return B;
    if(s=="C")
        return C;
    if(s=="D")
        return D;
    if(s=="E")
        return E;
    if(s=="F")
        return F;
    if(s=="FX")
        return FX;
    if(s=="RES")
        return RES;
    if(s=="ABS")
        return ABS;
    if(s=="EC")
        return EC;
    else {
        throw UTProfilerException(QString("Erreur dans StringToNote, note ")+s+QString(" non reconnue"));
    }
}

//Fonction permettant de transformer un semestre en chaine de caractère via stringstream
QString SemestreToString(const Semestre& sem)
{
    QString str;
    //Test sur l'année
    switch(sem.getSaison())
    {
        case Automne: str="A";
            break;
        case Printemps: str="P";
            break;
        default: throw UTProfilerException("Erreur, saison non traitee");
            break;
    }
    QString an;
    //Transformation du nombre en QString
    an.setNum(sem.getAnnee());
    //Concaténation de la lettre du semestre et de l'année
    str = str+an;

    return str;
}

//Transformation d'une chaine de caractère en Semestre
Semestre StringToSemestre(const QString& s)
{
    QString saison = s.mid(0,1);
    unsigned int annee = s.mid(1, 4).toUInt();
    return Semestre(StringToSaison(saison), annee); //utilisation de mid qui renvoie permet d'extraire des sous-chaines d'une chaine de caractères
}

//Surcharge de l'opérateur de comparaison pour les semestre
bool Semestre::operator>(const Semestre& s) {
    if(annee>s.getAnnee())
        return true;
    else if(annee==s.getAnnee()) {
        if(saison==Automne && s.getSaison()==Printemps)
            return true;
        else
            return false;
    }
    else
        return false;
}

//Surcharge de l'opérateur d'affectation pour un semesree
Semestre Semestre::operator =(const Semestre& s) {
    annee = s.annee;
    saison = s.saison;
    return *this;
}

//Surcharge de l'opérateur d'incrémentation préfixé pour un semestre
Semestre& Semestre::operator ++() {
    if(saison==Automne) {
        saison = Printemps;
        annee++;
    }
    else
        saison = Automne;
    return *this;
}

//Constructeur de Manager qui initialise l'attribut du fichier
Manager::Manager(const QString &f): file(f) {}

//Destructeur du Manager
Manager::~Manager() {}

//Utilisation du design pattern Template method pour le chargement des fichiers
void Manager::load(const QString &f) {
    //Activité d'ouverture du fichier commun à tous les algorithmes de chargement
    if (file!=f) this->~Manager();
    file=f;
    QFile fin(file);
    if(!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw UTProfilerException("Erreur ouverture du fichier");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    //Appel à la méthode abstraite qui différera si c'est CursusManager ou UVManager (design pattern Template Method)
    lireFichier(xml);

    // Error handling.
    if(xml.hasError()) {
        throw UTProfilerException("Erreur lecteur fichier, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();

}

void Manager::save(const QString &f) {
    file=f;
    QFile newfile(file);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text)) throw UTProfilerException(QString("erreur ouverture fichier xml"));
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    //Appel à la méthode abstraite (Template Method)
    ecrireFichier(&stream);

    stream.writeEndElement();
    stream.writeEndDocument();

    newfile.close();
}
