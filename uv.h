/*
 * Classes pour la gestion des UVs
*/

#ifndef UV_H
#define UV_H

#include "UTProfiler.h"

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
      code(c),titre(t),nbCredits(nbc),categorie(cat),automne(a),printemps(p), preference(0) {}
    int preference;
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
    void setPreference(int p) { preference=p; }
    int getPreference() const { return preference; }
};

QTextStream& operator<<(QTextStream& f, const UV& uv);


class UVManager: public Manager {
private:
    QMap<QString, UV*> uvs;
    void addItem(UV* uv);
    bool modification;
    UV* trouverUV(const QString& c) const;
    UVManager(const UVManager& um);
    UVManager& operator=(const UVManager& um);
    UVManager(const QString& f);
    virtual ~UVManager();
    friend struct Handler;
    struct Handler{
        UVManager* instance;
        Handler():instance(0){}
        ~Handler(){ if (instance) delete instance; instance=0; }
    };
    static Handler handler;

public:
    void lireFichier(QXmlStreamReader &xml);
    void ecrireFichier(QXmlStreamWriter *r);
    static UVManager& getInstance();
    static void libererInstance();
    void ajouterUV(const QString& c, const QString& t, unsigned int nbc, Categorie cat, bool a, bool p);
    void supprimerUV(const QString& c);
    const UV& getUV(const QString& code) const;
    UV& getUV(const QString& code);
};

#endif // UV_H
