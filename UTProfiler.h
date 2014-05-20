#ifndef UT_PROFILER_h
#define UT_PROFILER_h

#include <QString>
#include <QTextStream>
#include <QMap>
#include <QVector>
//#include <type_traits>
using namespace std;

class UTProfilerException{
public:
    UTProfilerException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};

enum Categorie {
    /* Connaissances Scientifiques */ CS,  /* Techniques et Méthodes */ TM,
    /* Technologies et Sciences de l'Homme */ TSH, /* Stage et Projet */ SP,
    //first=CS, last=SP
};

QTextStream& operator<<(QTextStream& f, const Categorie& s);

Categorie StringToCategorie(const QString& s);
QString CategorieToString(Categorie c);
QTextStream& operator>>(QTextStream& f, Categorie& cat);

enum Note { A, B, C, D, E, F, FX, RES, ABS, /* en cours */ EC  };

QString NoteToString(Note n);
Note StringToNote(const QString& s);

/*class NoteIterator {
    Note value;
    NoteIterator(Note val):value(val){}
public:
    static NoteIterator getFirst() { return NoteIterator(first); }
    bool isDone() const { return value>last; }
    Note operator*() const { return value; }
    void next() { std::underlying_type<Note>::type(value)++; }
};*/

enum Saison { Automne, Printemps };
Saison StringToSaison(const QString& s);
inline QTextStream& operator<<(QTextStream& f, const Saison& s) { if (s==Automne) f<<"A"; else f<<"P"; return f;}

/*template<typename EnumType>
class EnumIterator {
    static_assert(is_enum<EnumType>::value, "EnumType have to be an enum");
    EnumType value;
    EnumIterator(EnumType val):value(val){}
public:
    static EnumIterator getFirst() { return EnumIterator(EnumType::first); }
    bool isDone() const { return value>EnumType::last; }
    EnumType operator*() const { return value; }
    void next() { value=(EnumType)(std::underlying_type<EnumType>::type(value)+1); }
};

typedef EnumIterator<Note> NoteIterator;
typedef EnumIterator<Categorie> CategorieIterator;
typedef EnumIterator<Saison> SaisonIterator;
*/

class Semestre {
    Saison saison;
    unsigned int annee;
public:
    Semestre(Saison s, unsigned int a):saison(s),annee(a){ if (annee<1972||annee>2099) throw UTProfilerException("annee non valide"); }
    Saison getSaison() const { return saison; }
    unsigned int getAnnee() const { return annee; }
};

QString SemestreToString(const Semestre& sem);
Semestre StringToSemestre(const QString& s);

inline QTextStream& operator<<(QTextStream& f, const Semestre& s) { return f<<s.getSaison()<<s.getAnnee()%100; }

class UV {
    QString code;
    QString titre;
    unsigned int nbCredits;
    Categorie categorie;
    bool automne;
    bool printemps;
    UV(const UV& u);
    UV& operator=(const UV& u);
    UV(const QString& c, const QString& t, unsigned int nbc, Categorie cat, bool a, bool p):
      code(c),titre(t),nbCredits(nbc),categorie(cat),automne(a),printemps(p){}
    friend class UVManager;
public:
    QString getCode() const { return code; }
    QString getTitre() const { return titre; }
    unsigned int getNbCredits() const { return nbCredits; }
    Categorie getCategorie() const { return categorie; }
    bool ouvertureAutomne() const { return automne; }
    bool ouverturePrintemps() const { return printemps; }
    void setCode(const QString& c) { code=c; }
    void setTitre(const QString& t) { titre=t; }
    void setNbCredits(unsigned int n) { nbCredits=n; }
    void setCategorie(Categorie c) { categorie=c; }
    void setOuvertureAutomne(bool b) { automne=b; }
    void setOuverturePrintemps(bool b) { printemps=b; }
};

QTextStream& operator<<(QTextStream& f, const UV& uv);


class UVManager {
private:
    QMap<QString, UV*> uvs;
    void addItem(UV* uv);
    bool modification;
    UV* trouverUV(const QString& c) const;
    UVManager(const UVManager& um);
    UVManager& operator=(const UVManager& um);
    UVManager();
    ~UVManager();
    QString file;
    friend struct Handler;
    struct Handler{
        UVManager* instance;
        Handler():instance(0){}
        ~Handler(){ if (instance) delete instance; instance=0; }
    };
    static Handler handler;

public:

    void load(const QString& f);
    void save(const QString& f);
    static UVManager& getInstance();
    static void libererInstance();
    void ajouterUV(const QString& c, const QString& t, unsigned int nbc, Categorie cat, bool a, bool p);
    void supprimerUV(const QString& c);
    const UV& getUV(const QString& code) const;
    UV& getUV(const QString& code);
};


class Inscription {
    const UV* uv;
    Semestre semestre;
    Note resultat;
public:
    Inscription(const UV& u, const Semestre& s, Note res=EC):uv(&u),semestre(s),resultat(res){}
    const UV& getUV() const { return *uv; }
    Semestre getSemestre() const { return semestre; }
    Note getResultat() const { return resultat; }
    void setResultat(Note newres) { resultat=newres; }
};

class Formation {
private:
    QString nom;
    UV** uvs;
    unsigned int nb_Uvs;
    unsigned int nbMaxUvs;
    //friend class CursusManager;

public:
    Formation(const QString& n);
    ~Formation();
    const QString getNom() const { return nom; }
    UV& getUV(const QString &code) const;
    void ajoutUV(UV& uv);
};


class Dossier {
private:
    const QString nomEtu;
    const QString prenomEtu;
    QVector<Inscription*> inscriptions;
    Formation cursus;
    unsigned int nb_Credits;
    QString file;

public:
    Dossier(const QString &n, const QString& p);
    ~Dossier();
    void chargerDossier(const QString& f);
    void sauverDossier(const QString& f);
    const QString getNom() const { return nomEtu; }
    const QString getPrenom() const { return prenomEtu; }
    unsigned int getNbCredits() const { return nb_Credits; }
    void setCursus(const Formation& f);
    void ajouterEquivalence(unsigned int credits);
    void ajoutInscription(const UV& uv, Note res, Semestre sem);
    void enregistrerSolution(UV** sol);
    UV** completerCursus();
};


class CursusManager {
    QMap<QString, Formation*> cursus;
    CursusManager();
    ~CursusManager();
    QString file;
    static CursusManager* instance;

public:
    static CursusManager& getInstance();
    static void libererInstance();
    void chargerFormation(const QString& f);
    void supprimerCurusus(const QString& c);
    void sauverFormation(const QString &f);
    QMap<QString, Formation*> getCursus() const { return cursus; }
};

Formation StringToFormation(const QString& s);


#endif
