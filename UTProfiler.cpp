#include "UTProfiler.h"

#include <sstream>
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>
#include <sstream>

QTextStream& operator<<(QTextStream& f, const UV& uv){
    return f<<uv.getCode()<<", "<<uv.getCategorie()<<", "<<uv.getNbCredits()<<" credits, "<<uv.getTitre();
}

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
    switch(sem.getSaison())
    {
        case Automne: str+QString("A");
            break;
        case Printemps: str+QString("P");
            break;
        default: throw UTProfilerException("Erreur, saison non traitee");
            break;
    }
    str = str+QString(sem.getAnnee());

    return str;
}

//Transformation d'une chaine de caractère en Semestre
Semestre StringToSemestre(const QString& s)
{
    return Semestre(StringToSaison(s.mid(0, 1)), s.mid(1,2).toUInt()); //utilisation de mid qui renvoie permet d'extraire des sous-chaines d'une chaine de caractères
}


UVManager::UVManager():uvs(),file(""),modification(false){
}


void UVManager::load(const QString& f){
    if (file!=f) this->~UVManager();
    file=f;

    QFile fin(file);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw UTProfilerException("Erreur ouverture fichier UV");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named uvs, we'll go to the next.
            if(xml.name() == "uvs") continue;
            // If it's named uv, we'll dig the information from there.
            if(xml.name() == "uv") {
                QString code;
                QString titre;
                unsigned int nbCredits;
                Categorie cat;
                bool automne=false;
                bool printemps=false;

                QXmlStreamAttributes attributes = xml.attributes();
                /* Let's check that uvs has attribute. */
                if(attributes.hasAttribute("automne")) {
                    QString val =attributes.value("automne").toString();
                    automne=(val == "true" ? true : false);
                }
                if(attributes.hasAttribute("printemps")) {
                    QString val =attributes.value("printemps").toString();
                    printemps=(val == "true" ? true : false);
                }

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named uv.


                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "uv")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found code.
                        if(xml.name() == "code") {
                            xml.readNext(); code=xml.text().toString();
                        }
                        // We've found titre.
                        if(xml.name() == "titre") {
                            xml.readNext(); titre=xml.text().toString();
                        }
                        // We've found credits.
                        if(xml.name() == "credits") {
                            xml.readNext(); nbCredits=xml.text().toString().toUInt();
                        }
                        // We've found categorie
                        if(xml.name() == "categorie") {
                            xml.readNext(); cat=StringToCategorie(xml.text().toString());
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                ajouterUV(code,titre,nbCredits,cat,automne,printemps);

            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw UTProfilerException("Erreur lecteur fichier UV, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
}



void UVManager::save(const QString& f){
    file=f;
    QFile newfile( file);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text)) throw UTProfilerException(QString("erreur ouverture fichier xml"));
     QXmlStreamWriter stream(&newfile);
     stream.setAutoFormatting(true);
     stream.writeStartDocument();
     stream.writeStartElement("uvs");
     for(QMap<QString, UV*>::iterator it = uvs.begin(); it!=uvs.end(); it++) {
         stream.writeStartElement("uv");
         stream.writeAttribute("automne", ((*it)->ouvertureAutomne())?"true":"false");
         stream.writeAttribute("printemps", ((*it)->ouverturePrintemps())?"true":"false");
         stream.writeTextElement("code",(*it)->getCode());
         stream.writeTextElement("titre",(*it)->getTitre());
         QString cr; cr.setNum((*it)->getNbCredits());
         stream.writeTextElement("credits",cr);
         stream.writeTextElement("categorie",CategorieToString((*it)->getCategorie()));
         stream.writeEndElement();
     }
     stream.writeEndElement();
     stream.writeEndDocument();

     newfile.close();

}

UVManager::~UVManager(){
    if (file!="") save(file);
    uvs.clear();
}

void UVManager::addItem(UV* uv){
    uvs[uv->getCode()] = uv;
}

void UVManager::ajouterUV(const QString& c, const QString& t, unsigned int nbc, Categorie cat, bool a, bool p){
    if (trouverUV(c)) {
        throw UTProfilerException(QString("erreur, UVManager, UV ")+c+QString("déja existante"));
    }else{
        UV* newuv=new UV(c,t,nbc,cat,a,p);
        addItem(newuv);
        modification=true;
    }
}

void UVManager::supprimerUV(const QString& c)
{
    UV* uv = trouverUV(c);
    if(!uv) {
        throw UTProfilerException("Erreur, supprimer UV, UV inexistante");
    }
    else {
        uvs.remove(c);
    }
}


UV* UVManager::trouverUV(const QString& c)const{
    if(uvs.contains(c))
        return uvs[c];
    return 0;
}

UV& UVManager::getUV(const QString& code){
    UV* uv=trouverUV(code);
    if (!uv) throw UTProfilerException("erreur, UVManager, UV inexistante"+code);
    return *uv;
}


const UV& UVManager::getUV(const QString& code)const{
    return const_cast<UVManager*>(this)->getUV(code);
        // on peut aussi dupliquer le code de la méthode non-const
}

UVManager::Handler UVManager::handler=Handler();

UVManager& UVManager::getInstance(){
    if (!handler.instance) handler.instance = new UVManager; /* instance créée une seule fois lors de la première utilisation*/
    return *handler.instance;
}

void UVManager::libererInstance(){
    if (handler.instance) { delete handler.instance; handler.instance=0; }
}

Dossier::Dossier(const QString& n, const QString &p): nomEtu(n), prenomEtu(p), nb_Credits(0), file(""), cursus(""), inscriptions()
{}

Dossier::~Dossier()
{
    nb_Credits=0;
    if(file!="") sauverDossier(file);
    for(unsigned int i=0; i<inscriptions.size(); i++) delete inscriptions[i];
    inscriptions.clear();
}

void Dossier::chargerDossier(const QString& f)
{
    if (file!=f) this->~Dossier();
    file=f;

    QFile fin(file);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw UTProfilerException("Erreur ouverture fichier Dossier");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named dossiers, we'll go to the next.
            if(xml.name() == "dossiers") continue;
            // If it's named etudiant, we'll dig the information from there.
            if(xml.name() == "etudiant") {
                QString nom;
                QString prenom;
                QString form;
                Note resultat;
                QString semestre;
                QString uv;

                QXmlStreamAttributes attributes = xml.attributes();
                /* Let's check that eudiants has attribute. */
                if(attributes.hasAttribute("nom")) {
                    QString val =attributes.value("nom").toString();
                    nom = val;
                }

                if(attributes.hasAttribute("prenom")) {
                    QString val =attributes.value("prenom").toString();
                    prenom = val;
                }

                if(attributes.hasAttribute("cursus")) {
                    QString val =attributes.value("cursus").toString();
                    form = val;
                }

                //If it isn't the student we target, we are going to read another XML element
                if(nom!=nomEtu || prenom!=prenomEtu || form!=cursus.getNom()) continue;

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named etudiant.


                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "etudiant")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        //If it's named inscription, we'll go next
                        if(xml.name() == "inscription") {
                            xml.readNext();
                            continue;
                        }

                        while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "inscription")) {
                            if(xml.tokenType() == QXmlStreamReader::StartElement) {
                                if(xml.name() == "resultat") {
                                    xml.readNext();
                                    resultat = StringToNote(xml.text().toString());
                                }
                                if(xml.name() == "semestre") {
                                    xml.readNext();
                                    semestre = xml.text().toString();
                                }
                                if(xml.name() == "uv") {
                                    xml.readNext();
                                    uv = xml.text().toString();
                                }
                            }
                            xml.readNext();
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                ajoutInscription(UVManager::getInstance().getUV(uv), resultat, StringToSemestre(semestre));
            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw UTProfilerException("Erreur lecteur fichier Dossier, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
}

void Dossier::sauverDossier(const QString& f)
{
    file=f;
    QFile newfile( file);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text)) throw UTProfilerException(QString("erreur ouverture fichier xml"));
     QXmlStreamWriter stream(&newfile);
     stream.setAutoFormatting(true);
     stream.writeStartDocument();
     stream.writeStartElement("dossiers");
     for(QVector<Inscription*>::iterator it = inscriptions.begin(); it!=inscriptions.end(); it++) {
         stream.writeStartElement("inscriprion");
         stream.writeTextElement("resultat", NoteToString((*it)->getResultat()));
         stream.writeTextElement("semestre",SemestreToString((*it)->getSemestre()));
         stream.writeTextElement("uv", (*it)->getUV().getCode());
         stream.writeEndElement();
     }
     stream.writeEndElement();
     stream.writeEndDocument();

     newfile.close();
}

void Dossier::setCursus(const Formation& f)
{
    cursus=f;
}

//Incrémentation du compteur de crédits ECTS du dossier par un nombre de crédits acquis durant la précédente formation
void Dossier::ajouterEquivalence(unsigned int credits)
{
    nb_Credits += credits;
}

//Ajout d'une inscription à un dossier via la méthode push_back du conteneur Vector
void Dossier::ajoutInscription(const UV& uv, Note res, Semestre sem)
{
    inscriptions.push_back(new Inscription(uv, sem, res));
}


Formation::Formation(const QString& n): nom(n), uvs(0), nb_Uvs(0), nbMaxUvs(0)
{}

Formation::~Formation()
{
    delete[] uvs;
}

//Renvoie une UV d'une formation grâce à son code
UV& Formation::getUV(const QString& code) const
{
    for(unsigned int i=0; i<nb_Uvs; i++)
    {
        if(code==uvs[i]->getCode())
            return *uvs[i];
    }
}

//Ajout d'une UV à une formation
void Formation::ajoutUV(UV& uv)
{
    if(nb_Uvs==nbMaxUvs) {
        UV** newUVs = new UV*[nbMaxUvs+10];
        for(unsigned int i=0; i<nb_Uvs; i++) newUVs[i]=uvs[i];
        nbMaxUvs += 10;
        UV** oldUvs = uvs;
        uvs = newUVs;
        delete[] oldUvs;
    }
    uvs[nb_Uvs++] = &uv;
}

CursusManager* CursusManager::instance = 0;

CursusManager& CursusManager::getInstance()
{
    if(instance == 0)
    {
        instance = new CursusManager();
    }
    return *instance;
}

CursusManager::CursusManager(): cursus()
{}

CursusManager::~CursusManager()
{
    cursus.clear(); //On efface toutes les données présentes dans le vecteur et on libére la place qu'il occupait
}

void CursusManager::libererInstance()
{
    if(instance!=0) {
        delete instance;
    }
    instance = 0;
}

void CursusManager::chargerFormation(const QString& f)
{
    if (file!=f) this->~CursusManager();
    file=f;

    QFile fin(file);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw UTProfilerException("Erreur ouverture fichier cursus");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named formations, we'll go to the next.
            if(xml.name() == "formations") continue;
            // If it's named uv, we'll dig the information from there.
            if(xml.name() == "cursus") {
                QString nom;
                QString code;

                QXmlStreamAttributes attributes = xml.attributes();
                /* Let's check that uvs has attribute. */
                if(attributes.hasAttribute("nom")) {
                    QString val =attributes.value("nom").toString();
                    nom = val;
                }
                cursus[nom] = new Formation(nom);

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named uv.


                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "cursus")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found uv
                        if(xml.name() == "uv") {
                            xml.readNext(); code=xml.text().toString();
                        }
                        cursus[nom]->ajoutUV(UVManager::getInstance().getUV(code));
                    }
                    // ...and next...
                    xml.readNext();
                }
            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw UTProfilerException("Erreur lecteur fichier Cursus, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
}

void CursusManager::supprimerCurusus(const QString& c)
{
    cursus.remove(c);
}

//Formation StringToFormation(const QString& s)
//{
//    for(CursusManager::iterateur it = CursusManager::getInstance().getIterateur(); !it.isDone(); it.next())
//    {
//        if(it.current().getNom() == s)
//            return it.current();
//    }
//}
