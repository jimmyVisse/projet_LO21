#include "uv.h"

QTextStream& operator<<(QTextStream& f, const UV& uv){
    return f<<uv.getCode()<<", "<<uv.getCategorie()<<", "<<uv.getNbCredits()<<" credits, "<<uv.getTitre();
}

void UVManager::lireFichier(QXmlStreamReader& xml) {
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
}

void UVManager::ecrireFichier(QXmlStreamWriter *r) {
    r->writeStartElement("uvs");
    for(QMap<QString, UV*>::iterator it = uvs.begin(); it!=uvs.end(); it++) {
        r->writeStartElement("uv");
        r->writeAttribute("automne", ((*it)->ouvertureAutomne())?"true":"false");
        r->writeAttribute("printemps", ((*it)->ouverturePrintemps())?"true":"false");
        r->writeTextElement("code",(*it)->getCode());
        r->writeTextElement("titre",(*it)->getTitre());
        QString cr; cr.setNum((*it)->getNbCredits());
        r->writeTextElement("credits",cr);
        r->writeTextElement("categorie",CategorieToString((*it)->getCategorie()));
        r->writeEndElement();
    }
}

//Fonctions pour la mise en oeuvre du design pattern Singleton
UVManager::Handler UVManager::handler=Handler();

UVManager& UVManager::getInstance(){
    if (!handler.instance) {
        QString chemin("UV_XML.xml");
        handler.instance = new UVManager(chemin); /* instance créée une seule fois lors de la première utilisation*/
    }
    return *handler.instance;
}

void UVManager::libererInstance(){
    if (handler.instance) { delete handler.instance; handler.instance=0; }
}

//Constructeur de UVManager
UVManager::UVManager(const QString &f): Manager(f), modification(false) {}

//Destructeur de UVManager, permettant la destruction des uvs de la mémoire
UVManager::~UVManager(){
    if (file!="") {
        save(file);
    }
    uvs.clear();
}

//Actions sur les UVs via les méthodes du conteneur de la STL

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
    if (!uv) throw UTProfilerException("erreur, UVManager, UV inexistante "+code);
    return *uv;
}


const UV& UVManager::getUV(const QString& code)const{
    return const_cast<UVManager*>(this)->getUV(code);
        // on peut aussi dupliquer le code de la méthode non-const
}
