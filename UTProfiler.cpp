#include "UTProfiler.h"

#include <sstream>
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>

QTextStream& operator<<(QTextStream& f, const UV& uv){
    return f<<uv.getCode()<<", "<<uv.getCategorie()<<", "<<uv.getNbCredits()<<" credits, "<<uv.getTitre();
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

/*QString NoteToString(Note n)
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
    str+=sem.getAnnee();

    return str;
}

Semestre StringToSemestre(const QString& s)
{
    return Semestre(s.mid(0, 1), s.mid(1,2).toUInt());
}*/


UVManager::UVManager():uvs(0),nbUV(0),nbMaxUV(0),file(""),modification(false){
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
     for(unsigned int i=0; i<nbUV; i++){
         stream.writeStartElement("uv");
         stream.writeAttribute("automne", (uvs[i]->ouvertureAutomne())?"true":"false");
         stream.writeAttribute("printemps", (uvs[i]->ouverturePrintemps())?"true":"false");
         stream.writeTextElement("code",uvs[i]->getCode());
         stream.writeTextElement("titre",uvs[i]->getTitre());
         QString cr; cr.setNum(uvs[i]->getNbCredits());
         stream.writeTextElement("credits",cr);
         stream.writeTextElement("categorie",CategorieToString(uvs[i]->getCategorie()));
         stream.writeEndElement();
     }
     stream.writeEndElement();
     stream.writeEndDocument();

     newfile.close();

}

UVManager::~UVManager(){
    if (file!="") save(file);
    for(unsigned int i=0; i<nbUV; i++) delete uvs[i];
    delete[] uvs;
}

void UVManager::addItem(UV* uv){
    if (nbUV==nbMaxUV){
        UV** newtab=new UV*[nbMaxUV+10];
        for(unsigned int i=0; i<nbUV; i++) newtab[i]=uvs[i];
        nbMaxUV+=10;
        UV** old=uvs;
        uvs=newtab;
        delete[] old;
    }
    uvs[nbUV++]=uv;
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
        unsigned int i=0;
        while(uvs[i]!=uv)
            i++;
        delete uvs[i];
        while(i<nbUV)
        {
            uvs[i] = uvs[i+1];
            i++;
        }
        nbUV--;
    }
}


UV* UVManager::trouverUV(const QString& c)const{
    for(unsigned int i=0; i<nbUV; i++)
        if (c==uvs[i]->getCode()) return uvs[i];
    return 0;
}

UV& UVManager::getUV(const QString& code){
    UV* uv=trouverUV(code);
    if (!uv) throw UTProfilerException("erreur, UVManager, UV inexistante");
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

Dossier::Dossier(const QString& n, const QString &p): nomEtu(n), prenomEtu(p), nb_Credits(0), inscriptions(0), file(""), cursus("")
{}

Dossier::~Dossier()
{
    nb_Credits=0;
    if(file!="") sauverDossier(file);
    delete[] inscriptions;
}

/*void Dossier::chargerDossier(const QString& f)
{
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
            if(xml.name() == "dossiers") continue;
            // If it's named uv, we'll dig the information from there.
            if(xml.name() == "etudiant") {
                QString nom;
                QString prenom;
                Formation cursus;
                //Inscription** insc;

                QXmlStreamAttributes attributes = xml.attributes();*/
                /* Let's check that uvs has attribute. */
                /*if(attributes.hasAttribute("nom")) {
                    QString val =attributes.value("nom").toString();
                    nom = val;
                }

                if(attributes.hasAttribute("prenom")) {
                    QString val =attributes.value("prenom").toString();
                    prenom = val;
                }

                if(attributes.hasAttribute("cursus")) {
                    QString val =attributes.value("cursus").toString();
                    cursus = StringToFormation(val);
                }

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named uv.


                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "etudiant")) {
                    Note resultat;
                    Semestre semestre;
                    UV uv;
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found code.
                        if(xml.name() == "inscription") {
                            //xml.readNext(); =xml.text().toString();
                            QXmlStreamAttributes attributes = xml.attributes();*/
                            /* Let's check that uvs has attribute. */
                            /*if(attributes.hasAttribute("uv")) {
                                QString val =attributes.value("uv").toString();
                                uv = UVManager::getInstance().getUV(val);
                            }

                            if(attributes.hasAttribute("resultat")) {
                                QString val =attributes.value("resultat").toString();
                                resultat = StringToNote(val);
                            }

                            if(attributes.hasAttribute("semestre")) {
                                QString val =attributes.value("semestre").toString();
                                semestre = StringToSemestre(val);
                            }
                            ajoutInscription(uv, resultat, semestre);
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                //ajouterUV(code,titre,nbCredits,cat,automne,printemps);
            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw UTProfilerException("Erreur lecteur fichier UV, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
}*/

void Dossier::sauverDossier(const QString& f)
{
}

void Dossier::setCursus(const Formation& f)
{
    cursus=f;
}

Formation::Formation(const QString& n): nom(n), uvs(0), nb_UVs(0), nbMaxUVs(0)
{}

Formation::~Formation()
{
    delete[] uvs;
}

UV& Formation::getUV(const QString& code) const
{
    for(unsigned int i=0; i<nb_UVs; i++)
    {
        if(code==uvs[i]->getCode())
            return *uvs[i];
    }
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

CursusManager::CursusManager(): cursus(0), nb_cursus(0), nb_max_cursus(0)
{}

CursusManager::~CursusManager()
{
    for(unsigned int i=0; i<nb_cursus; i++) delete cursus[i];
    delete[] cursus;
}

void CursusManager::libererInstance()
{
    if(instance!=0) {
        delete instance;
    }
    instance = 0;
}

Formation StringToFormation(const QString& s)
{
    for(CursusManager::iterateur it = CursusManager::getInstance().getIterateur(); !it.isDone(); it.next())
    {
        if(it.current().getNom() == s)
            return it.current();
    }
}
