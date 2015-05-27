#include "dossier.h"

//Constructeur du dossier
Dossier::Dossier(const QString& n, const QString &p): nomEtu(n), prenomEtu(p), inscriptions(), cursus(""), nb_CreditsCS(0), nb_CreditsTM(0), nbSemestre(0), file("")
{}

//Destructeur du dossier, qui supprimer toutes les inscriptions car une inscription est liée à un dossier
Dossier::~Dossier()
{
    if(file!="") sauverDossier(file);
    for(int i=0; i<inscriptions.size(); i++) delete inscriptions[i];
    inscriptions.clear();
    nb_CreditsCS=0;
    nb_CreditsTM=0;
}

//Définition des méthodes permettant de mettre en oeuvre le design pattern Singleton
Dossier::Handler Dossier::handler=Handler();

Dossier& Dossier::getInstance(){
    if (!handler.instance) {
        handler.instance = new Dossier("", ""); /* instance créée une seule fois lors de la première utilisation*/
    }
    return *handler.instance;
}

void Dossier::libererInstance(){
    if (handler.instance) { delete handler.instance; handler.instance=0; }
}

//Chargement du dossier
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
                if(xml.name() == "dossier") {
                    QString nom;
                    QString prenom;
                    QString form;
                    unsigned int sem;

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
                    if(attributes.hasAttribute("semestre")) {
                        unsigned int val =attributes.value("semestre").toString().toUInt();
                        sem = val;
                    }
                    nomEtu = nom;
                    prenomEtu = prenom;
                    cursus = CursusManager::getInstance().getFormation(form);
                    nbSemestre = sem;

                    //Test sur le cursus pour avoir quelle stratégie adopter quand on compléte le cursus
                    if(form=="TC") {
                        strat = new StrategieTC;
                    }
                    else if(form=="HuTech") {
                        strat = new StrategieHuTech;
                    }
                    else {
                        strat = new StrategieGX;
                    }
                }

                // If it's named inscription, we'll dig the information from there.
                if(xml.name() == "inscription") {
                    QString resultat;
                    QString semestre;
                    QString uv;

                    xml.readNext();
                    //We're going to loop over the things because the order might change.
                    //We'll continue the loop until we hit an EndElement named etudiant.


                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "inscription")) {
                        if(xml.tokenType() == QXmlStreamReader::StartElement) {
                            if(xml.name() == "resultat") {
                                xml.readNext();
                                resultat = xml.text().toString();
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
                    //Ajout de l'inscription précédemment lue
                    ajoutInscription(uv, resultat, semestre);
                }
            }
        }
        //Ajout des crédits correspondants à toutes les inscriptions du dossier
        addCredits();
        // Error handling.
        if(xml.hasError()) {
            throw UTProfilerException("Erreur lecteur fichier Dossier, parser xml");
        }
        // Removes any device() or data from the reader * and resets its internal state to the initial state.
        xml.clear();
}

//Sauvegarde du dossier
void Dossier::sauverDossier(const QString& f)
{
    file=f;
    QFile newfile(file);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text)) throw UTProfilerException(QString("erreur ouverture fichier xml"));
     QXmlStreamWriter stream(&newfile);
     stream.setAutoFormatting(true);
     stream.writeStartDocument();
     stream.writeStartElement("dossier");
     stream.writeAttribute("nom", nomEtu);
     stream.writeAttribute("prenom", prenomEtu);
     stream.writeAttribute("cursus", cursus.getNom());
     QString nb;
     nb.setNum(nbSemestre);
     stream.writeAttribute("semestre", nb);
     for(QVector<Inscription*>::Iterator it = inscriptions.begin(); it!=inscriptions.end(); it++) {
         stream.writeStartElement("inscription");
         stream.writeTextElement("resultat", NoteToString((*it)->getResultat()));
         stream.writeTextElement("semestre",SemestreToString((*it)->getSemestre()));
         stream.writeTextElement("uv", (*it)->getUV().getCode());
         stream.writeEndElement();
     }
     stream.writeEndElement();
     stream.writeEndDocument();

     newfile.close();
}

//Accesseurs en écriture du cursus
void Dossier::setCursus(const Formation& f)
{
    cursus=f;
}

//Méthode pour rajouter les crédits correspondants aux inscriptions
void Dossier::addCredits() {
    //On parcourt toutes les inscriptions grâce à l'itérateur de QVector
    for(QVector<Inscription*>::Iterator it = inscriptions.begin(); it!=inscriptions.end(); it++) {
        //on récupère la catégorie pour changer les bons crédits
        Categorie cat = (*it)->getUV().getCategorie();
        if(cat==CS) {
            switch((*it)->getResultat()) {
                case A: case B: case C: case D: case E: {
                        QString codeUV = (*it)->getUV().getCode();
                        unsigned int cred = UVManager::getInstance().getUV(codeUV).getNbCredits();  //si le résultat est bon, on ajoute les crédits qui correspondent
                        nb_CreditsCS += cred;
                    }
                    break;
                default: break;
            }
        }
        else if(cat==TM) {
            switch((*it)->getResultat()) {
                case A: case B: case C: case D: case E: {
                        QString codeUV = (*it)->getUV().getCode();
                        unsigned int cred = UVManager::getInstance().getUV(codeUV).getNbCredits();
                        nb_CreditsTM += cred;
                    }
                    break;
                default: break;
            }
        }
    }
}

//Incrémentation du compteur de crédits ECTS du dossier par un nombre de crédits acquis durant la précédente formation
void Dossier::ajouterEquivalence(unsigned int creditsCS, unsigned int creditsTM)
{
    nb_CreditsCS += creditsCS;
    nb_CreditsTM += creditsTM;
}

//Ajout d'une inscription à un dossier via la méthode push_back du conteneur Vector
void Dossier::ajoutInscription(const QString &uv, const QString &res, const QString &sem)
{
    UV& u = UVManager::getInstance().getUV(uv);
    Note r = StringToNote(res);
    Semestre s = StringToSemestre(sem);
    inscriptions.push_back(new Inscription(u, s, r));
}

//Méthode permettant d'appliquer la procédure de complétion du dossier
void Dossier::completerCursus() {
    //Création d'une nouvelle solution  et copie des inscriptions du dossier dans cette solution
    Solution* sol = new Solution(nb_CreditsCS, nb_CreditsTM);
    for(int i=0; i<inscriptions.size(); i++) {
        sol->inscription.push_back(inscriptions[i]);
    }
    //Vecteur d'uvs non effectuées par l'étudiant, pour le moment vide
    QVector<UV*> uvs;
    //Parcours de toutes les UVs du cursus
    bool trouve;
    for(Formation::Iterateur it = cursus.begin(); !it.isDone(); it.next()) {
        trouve = false;
        //Parcours de toutes les inscriptions
        for(QVector<Inscription*>::iterator it2 = inscriptions.begin(); it2!=inscriptions.end(); it2++) {
            //Si l'uv correspond à l'inscription, on ne l'ajoute au vecteur d'uvs restantes
            if((*it2)->getUV().getCode() == it.currentItem().getCode()) {
                trouve = true;
            }
        }
        if(trouve==false) {
            uvs.push_back(&it.currentItem());
        }
    }
    if(uvs.size()==0) {
        throw UTProfilerException(" probleme");
    }
    //Tri du vecteur d'UVs suivant les préférences de l'utilisateur (tri par bulles)
    bool permutation;
    int longueur = uvs.size()-1;
    do {
        permutation = false;
        for(int i=0; i<longueur; i++) {
            if(uvs[i]->getPreference() < uvs[i+1]->getPreference()) {
                UV* temp = uvs[i];
                uvs[i]=uvs[i+1];
                uvs[i+1]=temp;
                permutation =true;
            }
        }
        longueur--;
    }while(permutation);
    //Appel à l'algorithme dynamiquement via le design pattern Stratégie
    strat->completer(sol, nbSemestre, uvs);

    //Enfin, on enregistre cette solution dans un fichier
    enregistrerSolution(sol);
}

//Enregistrement d'une solution
void Dossier::enregistrerSolution(Solution *s) {
    QFile newfile(s->file);
    if (!newfile.open(QIODevice::Append | QIODevice::Text)) throw UTProfilerException(QString("erreur ouverture fichier xml"));
    QXmlStreamWriter stream(&newfile);
     stream.setAutoFormatting(true);
     if(newfile.size()==0) {
         stream.writeStartDocument();
     }
     stream.writeStartElement("solutions");
     stream.writeAttribute("nom", nomEtu);
     stream.writeAttribute("prenom", prenomEtu);
     stream.writeAttribute("cursus", cursus.getNom());
     for(QVector<Inscription*>::Iterator it =s->inscription.begin(); it!=s->inscription.end(); it++) {
         stream.writeStartElement("inscription");
         stream.writeTextElement("semestre",SemestreToString((*it)->getSemestre()));
         stream.writeTextElement("uv", (*it)->getUV().getCode());
         stream.writeEndElement();
     }
     stream.writeEndElement();
     stream.writeEndDocument();

     newfile.close();
}

//Fonction de lecture d'une solution pour un affichage ultérieur (non implémenté ici)
void Dossier::lireSolution(const QString &f)
{
        QFile fin(f);
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
                if(xml.name() == "solutions") {
                    sol.push_back(new Solution());
                }

                // If it's named inscription, we'll dig the information from there.
                if(xml.name() == "inscription") {
                    QString semestre;
                    QString uv;

                    xml.readNext();
                    //We're going to loop over the things because the order might change.
                    //We'll continue the loop until we hit an EndElement named etudiant.


                    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "inscription")) {
                        if(xml.tokenType() == QXmlStreamReader::StartElement) {
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
                    //Ajout de l'inscription précédemment lue à la solution
                    ajoutInscSolution(uv, semestre);
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

//Ajout d'une inscription à une solution
void Dossier::ajoutInscSolution(const QString &uv, const QString &s) {
    sol.back()->ajoutInscription(uv, s);
}
