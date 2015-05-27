#include "formation.h"

Formation::Formation(const QString& n): nom(n), uvs(0), nb_Uvs(0), nbMaxUvs(0)
{}

//Destructeur de formation qui ne supprime que le tableau d'UVs mais pas les UVs
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

//Suppression d'une UV d'une formation
void Formation::supprimerUV(UV &uv)
{
    unsigned int i=0;
    bool trouver=false;
    while(i<nb_Uvs && trouver==false) {
        if(uvs[i]->getCode()!=uv.getCode()) i++;
        else trouver=true;
    }
    if(i<nb_Uvs) {
        for(unsigned int j=i; j<nb_Uvs-1; j++)
            uvs[j]=uvs[j+1];
        nb_Uvs--;
    }
    else
        throw UTProfilerException("Erreur, uv non présente dans ce cursus");
}


void CursusManager::ecrireFichier(QXmlStreamWriter *r) {
    r->writeStartElement("formations");
    for(QMap<QString, Formation*>::iterator it = cursus.begin(); it!=cursus.end(); it++) {
        r->writeStartElement("cursus");
        r->writeAttribute("nom", (*it)->getNom());
        for(Formation::Iterateur it2 = (*it)->begin(); !it2.isDone(); it2.next())
            r->writeTextElement("uv", it2.currentItem().getCode());
        r->writeEndElement();
    }
}

CursusManager::CursusManager(const QString &f): Manager(f), cursus()
{}

CursusManager::~CursusManager()
{
    if(file!="") save(file);
    cursus.clear(); //On efface toutes les données présentes dans le vecteur et on libére la place qu'il occupait
}

//Définition des fonctions mettant en oeuvre le design pattern Singleton
CursusManager::Handler CursusManager::handler=Handler();

CursusManager& CursusManager::getInstance(){
    if (!handler.instance) {
        QString chemin("UV_Cursus.xml");
        handler.instance = new CursusManager(chemin); /* instance créée une seule fois lors de la première utilisation*/
    }
    return *handler.instance;
}

void CursusManager::libererInstance(){
    if (handler.instance) { delete handler.instance; handler.instance=0; }
}

void CursusManager::lireFichier(QXmlStreamReader& xml) {
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
                ajouterCursus(nom);

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
}

//Fonction utile permettant de trouver un cursus, via la méthode contains de QMap
Formation *CursusManager::trouverCursus(const QString& f) const {
    if(cursus.contains(f))
        return cursus[f];
    return 0;
}

//Suppression d'un cursus grâce à une méthode de QMap
void CursusManager::supprimerCursus(const QString& c)
{
    if(!trouverCursus(c))
        throw UTProfilerException("Erreur supprimer cursus, formation inexistante");
    cursus.remove(c);
}

void CursusManager::ajouterCursus(const QString& n)
{
    if(trouverCursus(n))
        throw UTProfilerException("Erreur ajouter cursus, formation non existante");
    cursus[n]=new Formation(n);
}

void CursusManager::ajouterUVCursus(const QString &n, const QString &uv) {
    if(!trouverCursus(n))
        throw UTProfilerException("Erreur ajouter une UV à un cursus, formation non existante");
    UV& u = UVManager::getInstance().getUV(uv);
    cursus[n]->ajoutUV(u);
}

Formation& CursusManager::getFormation(const QString& f) {
    if(!trouverCursus(f))
        throw UTProfilerException("Erreur, cursus inexistant");
    return *cursus[f];
}

void CursusManager::supprimerUVCursus(const QString &n, const QString &uv)
{
    if(!trouverCursus(n))
        throw UTProfilerException("Erreur, suppirmer une UV à un cursus, cursus inexistant");
    UV& u = UVManager::getInstance().getUV(uv);
    cursus[n]->supprimerUV(u);
}
