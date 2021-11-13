#include <iostream>
#include <typeinfo>
#include <stdexcept>
#define VEL 1000000
#include <string>

using namespace std;


template <typename Tip>

class Lista
{
public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const=0;
    virtual Tip trenutni() const=0;
    virtual Tip &trenutni()=0;
    virtual bool prethodni()=0;
    virtual bool sljedeci()=0;
    virtual void pocetak()=0;
    virtual void kraj()=0;
    virtual void obrisi()=0;
    virtual void dodajIspred(const Tip &el)=0;
    virtual void dodajIza(const Tip &el)=0;
    virtual Tip &operator[](int i)=0;
    virtual Tip operator[](int i) const=0;


};

template <typename Tip>
class NizLista : public Lista <Tip>
{
    int kapacitet,broj_elemenata,t;
    Tip** lista;

public:
    NizLista() : Lista<Tip>(), lista(new Tip*[VEL]), kapacitet(VEL), broj_elemenata(0),t(0) {}
    ~NizLista()
    {

        //Destruktor
        for(int i=0; i<broj_elemenata; i++) {
            delete lista[i];
        }
        delete [] lista;
    }

    NizLista(const NizLista <Tip> &l)
    {
        kapacitet=l.kapacitet;
        broj_elemenata=l.broj_elemenata;
        t=l.t;
        lista=new Tip*[l.kapacitet];
        for(int i=0; i<broj_elemenata; i++) {
            lista[i]=new Tip(*l.lista[i]);
        }
    }

    NizLista<Tip> &operator = (const NizLista<Tip> &l)
    {
        if(this==&l) return *this;
        for(int i=0; i<broj_elemenata; i++) {
            delete lista[i];
        }
        delete[] lista;
        kapacitet=l.kapacitet;
        broj_elemenata=l.broj_elemenata;
        t=l.t;
        lista=new Tip*[l.kapacitet];
        for(int i=0; i<broj_elemenata; i++) {
            lista[i]=new Tip(*l.lista[i]);
        }
        return *this;
    }

    //Metoda geter za vracanje broja elemenata
    int brojElemenata() const override
    {
        return broj_elemenata;
    }

    //Daj mjesto trenutnog elementa
    Tip trenutni() const override
    {
        if(broj_elemenata==0) throw "Izuzetak!   ";
        return *lista[t];
    }


    Tip &trenutni() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        return *lista[t];
    }

    //Metoda obrisi
    void obrisi() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        if(t==broj_elemenata-1 && broj_elemenata>0) {
            delete lista[t];
            t--;
        } else {
            delete lista[t];
            for(int i=t; i<broj_elemenata; i++) {
                lista[i]=lista[i+1];
            }
        }
        broj_elemenata--;
    }

    //Uzima kraj, tacnije pozicionira trenutni na kraj
    void kraj() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        t=broj_elemenata-1;
    }

    //Pocetak
    void pocetak() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        t=0;
    }


    bool prethodni() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        if(t==0) return false;
        t--;
        return true;
    }

    bool sljedeci() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        if(t==broj_elemenata-1) return false;
        t++;
        return true;
    }

    void dodajIza(const Tip &el) override
    {
        if(broj_elemenata==0) {
            broj_elemenata++;
            lista[0]=new Tip(el);
            t=0;
        } else if(kapacitet==broj_elemenata) {
            kapacitet*=2;
            Tip **n_lista=new Tip*[kapacitet];
            for(int i=0; i<broj_elemenata; i++) {
                n_lista[i]=new Tip(*lista[i]);
            }
            for(int i=0; i<broj_elemenata; i++) {
                delete lista[i];
            }
            delete [] lista;
            lista=n_lista;
        } else {
            for(int i=broj_elemenata; i>t+1; i--) {
                lista[i]=lista[i-1];
            }
            lista[t+1]=new Tip(el);
            broj_elemenata++;
        }
    }



    void dodajIspred(const Tip &el) override
    {
        if(broj_elemenata==0) {
            broj_elemenata++;
            lista[0]=new Tip(el);
            t=0;
        } else if(kapacitet==broj_elemenata) {
            kapacitet*=2;
            Tip **n_lista=new Tip*[kapacitet];
            for(int i=0; i<broj_elemenata; i++)
                n_lista[i]=new Tip(*lista[i]);
            for(int i=0; i<broj_elemenata; i++)
                delete lista[i];
            delete[] lista;
        } else {
            for(int i=broj_elemenata; i>t; i--)
                lista[i]=lista[i-1];
            lista[t]=new Tip(el);
            broj_elemenata++;
            if(broj_elemenata!=1) t++;
        }
    }

    //Preklopljeni operatori
    Tip &operator[](int i)
    {
        if(i>=brojElemenata() || i<0) throw "Izuzetak! ";
        return *lista[i];
    }

    Tip operator[](int i) const
    {
        if(i>=brojElemenata() || i<0) throw "Izuzetak! ";
        return *lista[i];
    }

};



template <typename Tip>
class JednostrukaLista : public Lista <Tip>
{

    int broj_elemenata;

    struct Cvor {
        Tip element;
        Cvor *sljedeci;
    };

    Cvor *pocetni, *posljednji, *t;

public:
    JednostrukaLista() : pocetni(nullptr), posljednji(nullptr),  t(nullptr), broj_elemenata(0) {}
    JednostrukaLista(const JednostrukaLista &l1);
    ~JednostrukaLista();

    void obrisi() override;

    void pocetak() override
    {
        if(pocetni==nullptr) throw "Izuzetak!   ";
        t=pocetni;
    }

    void kraj() override
    {
        if(pocetni==nullptr) throw "Izuzetak!   ";
        t=posljednji;
    }

    Cvor *DajTekuci() const
    {
        return t;
    }

    Cvor *DajPocetak() const
    {
        return pocetni;
    }

    int brojElemenata() const override
    {
        return broj_elemenata;
    }

    bool prethodni() override;
    bool sljedeci() override;
    Tip trenutni() const override;
    Tip &trenutni() override;
    Tip &operator[](int i) override;
    Tip operator[](int i) const;
    JednostrukaLista &operator =(const JednostrukaLista &l1);
    void dodajIspred(const Tip &el) override;
    void dodajIza(const Tip &el) override;

};

template <typename Tip>
void JednostrukaLista<Tip>::obrisi()
{
    if(pocetni==nullptr) throw "Izuzetak!   ";
    broj_elemenata--;
    if(t==pocetni) {
        pocetni=pocetni->sljedeci;
        delete t;
        t=nullptr;
        t=pocetni;

        if(broj_elemenata==0) {
            posljednji=nullptr;
            pocetni=nullptr;
            t=nullptr;
        }
    } else {
        Cvor *pomocni(pocetni);

        while(pomocni->sljedeci!=t) {
            pomocni=pomocni->sljedeci;
        }
        pomocni->sljedeci=t->sljedeci;
        delete t;
        t=pomocni->sljedeci;
        if(t==nullptr) {
            posljednji=pomocni;
            t=pomocni;
        }
    }
}

template <typename Tip>
bool JednostrukaLista<Tip>::prethodni()
{
    if(pocetni==nullptr) throw ("Izuzetak!   ");
    if(t==pocetni) return false;
    Cvor *pomocniocni(pocetni);
    while(pomocniocni->sljedeci!=t)
        pomocniocni=pomocniocni->sljedeci;
    t=pomocniocni;
    return true;
}


template <typename Tip>
bool JednostrukaLista<Tip>::sljedeci()
{
    if(pocetni==nullptr) throw "Izuzetak!   ";
    if(t->sljedeci==nullptr) return false;
    t=t->sljedeci;
    return true;
}

template <typename Tip>
Tip &JednostrukaLista<Tip>::trenutni()
{
    return t->element;
}

template <typename Tip>
Tip JednostrukaLista<Tip>::trenutni() const
{
    return t->element;
}

template <typename Tip>
Tip &JednostrukaLista<Tip>::operator [](int i)
{
    if(i<0 || i>=broj_elemenata) throw "Izuzetak!   ";
    Cvor *pomocniocni(pocetni);
    for(int j=0; j<i; j++)
        pomocniocni=pomocniocni->sljedeci;
    return pomocniocni->element;
}

template <typename Tip>
Tip JednostrukaLista<Tip>::operator[](int i) const
{
    if(i<0 || i>=broj_elemenata) throw "Izuzetak!   ";
    Cvor *pomocniocni(pocetni);
    for(int j=0; j<i; j++)
        pomocniocni=pomocniocni->sljedeci;
    return pomocniocni->element;
}

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(const JednostrukaLista &l1)
{
    broj_elemenata=l1.brojElemenata();
    Cvor *pomocni1(l1.DajPocetak());
    Cvor *pomocni2(nullptr);

    while(pomocni1!=nullptr) {
        Cvor *novi=new Cvor;
        novi->element=pomocni1->element;
        novi->sljedeci=nullptr;
        if(pomocni2==nullptr) {
            pocetni=novi;
        } else {
            pomocni2->sljedeci=novi;
        }
        pomocni2=novi;
        if(pomocni1=l1.DajTekuci()) {
            t=novi;
        }
        pomocni1=pomocni1->sljedeci;
    }
    posljednji=pomocni2;
}

template<typename Tip>
JednostrukaLista<Tip>&JednostrukaLista<Tip>::operator=(const JednostrukaLista<Tip> &l1)
{
    if(this==&l1) return *this;
    Cvor *pomocni(pocetni);
    Cvor *privremeni2(nullptr);
    while(pomocni!=nullptr) {
        privremeni2=pomocni;
        pomocni=pomocni->sljedeci;
        delete privremeni2;
    }
    broj_elemenata=l1.brojElemenata();
    Cvor *pomocni1(l1.DajPocetak());
    Cvor *pomocni2(nullptr);
    while(pomocni1!=nullptr) {
        Cvor *novi=new Cvor;
        novi->element=pomocni1->element;
        novi->sljedeci=nullptr;
        if(pomocni2==nullptr) {
            pocetni=novi;
        } else {
            pomocni2->sljedeci=novi;
        }
        pomocni2=novi;
        if(pomocni1=l1.DajTekuci()) {
            t=novi;
        }
        pomocni1=pomocni1->sljedeci;
    }
    posljednji=pomocni2;

    return *this;
}

template <typename Tip>
JednostrukaLista<Tip>::~JednostrukaLista()
{
    Cvor *pomocni(pocetni);
    Cvor *privremeni2(nullptr);
    while(pomocni!=nullptr) {
        privremeni2=pomocni;
        pomocni=pomocni->sljedeci;
        delete privremeni2;
    }
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIspred(const Tip &element)
{
    Cvor *pomocni1=new Cvor;
    pomocni1->element=element;
    pomocni1->sljedeci=nullptr;
    broj_elemenata++;
    if(pocetni==nullptr) {
        pocetni=pomocni1;
        posljednji=pomocni1;
        t=pomocni1;
    } else if(t==pocetni) {
        pomocni1->sljedeci=pocetni;
        pocetni=pomocni1;
    } else {
        Cvor *pomocni(pocetni);
        while(pomocni->sljedeci!=t)
            pomocni=pomocni->sljedeci;
        pomocni1->sljedeci=pomocni->sljedeci;
        pomocni->sljedeci=pomocni1;
    }
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip &element)
{
    Cvor *pomocni1=new Cvor;
    pomocni1->element=element;
    pomocni1->sljedeci=nullptr;
    broj_elemenata++;
    if(pocetni==nullptr) {
        posljednji=pomocni1;
        pocetni=pomocni1;
        t=pomocni1;
    } else {
        pomocni1->sljedeci=t->sljedeci;
        t->sljedeci=pomocni1;
        if(posljednji==t) {
            posljednji = pomocni1;
        }

    }
}



//Test dodaj iza
bool TestDodajIza(NizLista<int>&l)
{
    for(int i=1; i<=5; i++) l.dodajIza(i);
    for(int i=1; i<=5; i++) {
        if(l[0]==1 && l[1]==5 && l[2]==4) return true;
    }
    return false;
}

//Test dodaj iza 1
bool TestDodajIza1(JednostrukaLista<int>&j)
{
    for(int i=1; i<=5; i++) j.dodajIza(i);
    for(int i=1; i<=5; i++) {
        if((i==1 && j.trenutni()==1) || (i==2 && j.trenutni()==5)) return true;
    }
    return false;
}


//Test broj elemenata
bool TestBrojElemenata(NizLista<int>&l)
{
    if(l.brojElemenata()==5) return true;
    return false;
}

//Test broj elemenata 1
bool TestBrojElemenata1(JednostrukaLista<int>&j)
{
    if(j.brojElemenata()==5) return true;
    return false;
}


//test trenutni
bool TestTrenutni(NizLista<int>&l)
{
    int i=1;
    bool t=false;
    do {
        if((i==1 && l.trenutni()==1) || (i==3 && l.trenutni()==4)) t=true;
        else t=false;
        i++;
    } while(l.sljedeci());
    if(t=true) return true;
    return false;
}

//test trenutni 1
bool TestTrenutni1(JednostrukaLista<int>&j)
{
    int i=1;
    bool t=false;
    do {
        if((i==1 && j.trenutni()==1) || (i==3 && j.trenutni()==4)) t=true;
        else t=false;
        i++;
    } while(j.sljedeci());
    if(t=true) return true;
    return false;
}


//test prethodni 
bool TestPrethodni(NizLista<int> &l)
{
    if(l.prethodni()) return true;
    return false;
}

//test prethodni 1
bool TestPrethodni1(JednostrukaLista<int>&j)
{
    if(j.prethodni()) return true;
    return false;
}

bool TestDodajIspred(NizLista<int> &l1)
{
    for(int i=5; i>=1; i--) l1.dodajIspred(i);
    bool t=false;
    for(int i=5; i>=1; i--) {
        if((i==5 && l1.trenutni()==1) || (i==1 && l1.trenutni()==5)) t=true;
        else t=false;
    }
    if(t=true) return true;
    return false;
}

bool TestDodajIspred1(JednostrukaLista<int> &j1)
{
    for(int i=5; i>=1; i--) j1.dodajIspred(i); 
    bool t=false; 
    for(int i=5; i>=1; i--) {
        if((i==5 && j1.trenutni()==1) || (i==1 && j1.trenutni()==5)) t=true;
        else t=false;
    }
    
    if(t=true) return true; 
    return false;
    
}

bool TestSljedeci(NizLista<int>&l)
{
    l.dodajIspred(6);
    return l.sljedeci();
}

bool TestSljedeci1(JednostrukaLista<int> &j)
{
    j.dodajIspred(6);
    return j.sljedeci();
   
}

bool TestPocetak(NizLista<int>&l)
{
    l.pocetak();
    if(l.trenutni()!=1) return false;
    return true;
}

bool TestPocetak1(JednostrukaLista<int> &j)
{
    j.pocetak();
    if(j.trenutni()!=1) return false;
    return true;
}

bool TestKraj(NizLista<int> &l)
{
    l.kraj();
    if(l.trenutni()!=2) return false;
    return true;
}

bool TestKraj1(JednostrukaLista<int> &j)
{
    j.kraj();
    if(j.trenutni()!=2) return false;
    return true;
}

bool TestObrisi(NizLista<int>&l)
{
    int z=l.brojElemenata();
    l.obrisi();
    if(l.brojElemenata()!=z-1) return false;
    return true;
}

bool TestObrisi1(JednostrukaLista<int>&j)
{
    int z=j.brojElemenata();
    j.obrisi();
    if(j.brojElemenata()!=z-1) return false;
    return true;
}

bool TestOperatora(NizLista<int> &l)
{
    if(l[0]==1) return true;
    return false;
}

int main()
{
    NizLista<int>lista;
    JednostrukaLista<int>jlista;

    try {
        if(TestDodajIza(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test nije uspjesan!";
        std::cout << std::endl;

        if(TestDodajIza1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test2 nije uspjesan!";
        std::cout << std::endl;

        if(TestBrojElemenata(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test3 nije uspjesan!";
        std::cout << std::endl;

        if(TestBrojElemenata1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test4 nije uspjesan!";
        std::cout << std::endl;

        if(TestTrenutni(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test5 nije uspjesan!";
        std::cout << std::endl;

        if(TestTrenutni1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test6 nije uspjesan!";
        std::cout << std::endl;

        if(TestPrethodni(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test7 nije uspjesan!";
        std::cout << std::endl;

        if(TestPrethodni1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test8 nije uspjesan!";
        std::cout << std::endl;

        if(TestDodajIspred(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test9 nije uspjesan!";
        std::cout << std::endl;

        if(TestDodajIspred1(jlista)) std::cout << "Test je uspjesan!";
        else cout << "Test10 nije uspjesan!";
        cout << endl;

        if(TestSljedeci(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test11 nije uspjesan!";
        std::cout << std::endl;

        if(TestSljedeci1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test12 nije uspjesan!";
        std::cout << std::endl;

        if(TestPocetak(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test13 nije uspjesan!";
        std::cout << std::endl;

        if(TestPocetak1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test14 nije uspjesan!";
        std::cout << std::endl;

        if(TestKraj(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test15 nije uspjesan!";
        std::cout << std::endl;

        if(TestKraj1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test16 nije uspjesan!";
        std::cout << std::endl;

        if(TestObrisi(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test17 nije uspjesan!";
        std::cout << std::endl;

        if(TestObrisi1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Tes18 nije uspjesan!";
        std::cout << std::endl;

        if(TestOperatora(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test19 nije uspjesan!";
        std::cout << std::endl;


    } catch(...) {
        std::cout << "Uhvacen je izuzetak! ";
    }
    return 0;
}



/*#include <iostream>
#include <string>
#include <typeinfo>
#include <stdexcept>
#define VEL 100000

using namespace std;


template <typename Tip>

class Lista
{
public:
    virtual ~Lista() {}
    virtual int brojElemenata() const=0;
    virtual Tip trenutni() const=0;
    virtual Tip &trenutni()=0;
    virtual bool prethodni()=0;
    virtual bool sljedeci()=0;
    virtual void pocetak()=0;
    virtual void kraj()=0;
    virtual void obrisi()=0;
    virtual void dodajIspred(const Tip &el)=0;
    virtual void dodajIza(const Tip &el)=0;
    virtual Tip &operator[](int i)=0;
    virtual Tip operator[](int i) const=0;
};

template <typename Tip>
class NizLista : public Lista <Tip>
{
    int kapacitet, broj_elemenata, t;
    Tip **lista;

public:
    NizLista() : Lista<Tip>(), lista(new Tip*[VEL]), kapacitet(VEL), broj_elemenata(0), t(0) {}
    ~NizLista()
    {

        //Destruktor
        for(int i=0; i<broj_elemenata; i++) {
            delete lista[i];
        }
        delete[] lista;
    }



    NizLista(const NizLista<Tip>&l)
    {
        kapacitet=l.kapacitet;
        broj_elemenata=l.broj_elemenata; //mislim da je ovo greska
        t=l.t;
        lista=new Tip*[l.kapacitet];
        for(int i=0; i<broj_elemenata; i++)
            lista[i]=new Tip(*l.lista[i]);

    }

    NizLista<Tip> &operator = (const NizLista<Tip> &l)
    {
        if(this==&l) return *this;
        for(int i=0; i<broj_elemenata; i++) {
            delete lista[i];
        }
        delete [] lista;
        kapacitet=l.kapacitet;
        broj_elemenata=l.broj_elemenata;
        t=l.t;
        lista=new Tip*[l.kapacitet];
        for(int i=0; i<broj_elemenata; i++)
            lista[i]=new Tip(*l.lista[i]);
        return *this;
    }

    //Metoda getter za vracanje broja elemenata
    int brojElemenata() const override
    {
        return broj_elemenata;
    }

    //Vraća mjesto trenutnog elementa
    Tip trenutni() const override
    {
        if(broj_elemenata==0) throw "Izuzetak!    ";
        return *lista[t];

    }

    Tip &trenutni() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        return *lista[t];
    }


    void obrisi() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        if(t==broj_elemenata-1 && broj_elemenata>0) {
            delete lista[t];
            t--;
        } else {
            delete lista[t];
            for(int i=t; i<broj_elemenata; i++) {
                lista[i]=lista[i+1];
            }
        }
        broj_elemenata--;
    }

    void kraj() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        t=broj_elemenata-1;
    }

    void pocetak() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        t=0;
    }


    bool sljedeci() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        if(t==broj_elemenata-1) return false;
        t++;
        return true;
    }


    bool prethodni() override
    {
        if(broj_elemenata==0) throw "Izuzetak! ";
        if(t==0) return false;
        t--;
        return true;
    }

    void dodajIspred(const Tip &el) override
    {
        if(broj_elemenata==0) {
            broj_elemenata++;
            lista[0]=new Tip(el);
            t=0;
        } else if(kapacitet==broj_elemenata) {
            kapacitet*=2;
            Tip **n_lista=new Tip*[kapacitet];
            for(int i=0; i<broj_elemenata; i++) {
                n_lista[i]=new Tip(*lista[i]);
            }
            for(int i=0; i<broj_elemenata; i++)
                delete lista[i];
            delete[] lista;
        } else {
            for(int i=broj_elemenata; i>t; i--)
                lista[i]=lista[i-1];
            lista[t]=new Tip(el);
            broj_elemenata++;
            if(broj_elemenata!=1) t++;
        }
    }


    void dodajIza(const Tip &el) override
    {
        if(broj_elemenata==0) {
            broj_elemenata++;
            lista[0]=new Tip(el);
            t=0;
        } else if(kapacitet==broj_elemenata) {
            kapacitet*=2;
            Tip **n_lista=new Tip*[kapacitet];
            for(int i=0; i<broj_elemenata; i++)
                n_lista[i]=new Tip(*lista[i]);
            for(int i=0; i<broj_elemenata; i++) {
                delete lista[i];
            }
            delete[] lista;
            lista=n_lista;
        } else {
            for(int i=broj_elemenata; i>t+1; i--) {
                lista[i]=lista[i-1];
            }
            lista[t+1]=new Tip(el);
            broj_elemenata++;
        }
    }


//Preklopljeni operatori
    Tip &operator[](int i)
    {
        if(i>=brojElemenata() || i<0) throw "Izuzetak! ";
        return *lista[i];
    }

    Tip operator[](int i) const
    {
        if(i>=brojElemenata() || i<0) throw "Izuzetak! ";
        return *lista[i];
    }
};


template <typename Tip>
class JednostrukaLista : public Lista <Tip>
{
    int broj_elemenata;

    struct Cvor {
        Tip element;
        Cvor *sljedeci;
    };

    Cvor  *t, *pocetni, *posljednji;

public:
    JednostrukaLista() : pocetni(nullptr), posljednji(nullptr), t(nullptr), broj_elemenata(0) {}
    JednostrukaLista(const JednostrukaLista &l1);
    ~JednostrukaLista();
    void obrisi() override;

    void kraj() override
    {
        if(pocetni==nullptr) throw "Izuzetak!   ";
        t=posljednji;
    }

    void pocetak() override
    {
        if(pocetni==nullptr) throw "Izuzetak!   ";
        t=pocetni;
    }

    Cvor *DajPocetak() const
    {
        return pocetni;
    }

    Cvor *DajTekuci() const
    {
        return t;
    }

    bool prethodni() override;
    bool sljedeci() override;
    Tip trenutni() const override;
    Tip &trenutni() override;

    int brojElemenata() const override
    {
        return broj_elemenata;
    }

    Tip &operator[](int i) override;
    Tip operator [](int i) const;
    JednostrukaLista &operator =(const JednostrukaLista &l1);
    void dodajIspred(const Tip &el) override;
    void dodajIza(const Tip &el) override;
};

template <typename Tip>
void JednostrukaLista<Tip>::obrisi()
{
    if(pocetni==nullptr) throw "Izuzetak!   ";
    broj_elemenata--;
    if(t==pocetni) {
        pocetni=pocetni->sljedeci;
        delete t;
        t=nullptr;
        t=pocetni;

        if(broj_elemenata==0) {
            t=nullptr;
            pocetni=nullptr;
            posljednji=nullptr;
        }
    } else {
        Cvor *pomocni(pocetni);

        while(pomocni->sljedeci!=t) {
            pomocni=pomocni->sljedeci;
        }
        pomocni->sljedeci=t->sljedeci;
        delete t;
        t=pomocni->sljedeci;
        if(t==nullptr) {
            posljednji=pomocni;
            t=pomocni;
        }
    }
}

template <typename Tip>
bool JednostrukaLista<Tip>::prethodni()
{
    if(pocetni==nullptr) throw ("Izuzetak!   ");
    if(t==pocetni) return false;
    Cvor *pomocniocni(pocetni);
    while (pomocniocni->sljedeci!=t)
        pomocniocni=pomocniocni->sljedeci;
    t=pomocniocni;
    return true;
}

template <typename Tip>
bool JednostrukaLista<Tip>::sljedeci()
{
    if(pocetni==nullptr) throw "Izuzetak!   ";
    if(t->sljedeci=nullptr) return false;
    t=t->sljedeci;
    return true;
}


template <typename Tip>
Tip &JednostrukaLista<Tip>::trenutni()
{
    return t->element;
}

template <typename Tip>
Tip JednostrukaLista<Tip>::trenutni() const
{
    return t->element;
}

template <typename Tip>
Tip &JednostrukaLista<Tip>::operator [](int i)
{
    if(i<0 || i>=broj_elemenata) throw "Izuzetak!   ";
    Cvor *pomocniocni(pocetni);
    for(int j=0; j<i; i++)
        pomocniocni=pomocniocni->sljedeci;
    return pomocniocni->element;
}


template <typename Tip>
Tip JednostrukaLista<Tip>::operator [](int i) const
{
    if(i<0 || i>=broj_elemenata) throw "Izuzetak!    ";
    Cvor *pomocniocni(pocetni);
    for(int j=0; j<i; j++)
        pomocniocni=pomocniocni->sljedeci;
    return pomocniocni->element;
}

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(const JednostrukaLista &l1)
{
    broj_elemenata=l1.brojElemenata();
    Cvor *pomocni1(l1.DajPocetak());
    Cvor *pomocni2(nullptr);

    while(pomocni1!=nullptr) {
        Cvor *novi=new Cvor;
        novi->element=pomocni1->element;
        novi->sljedeci=nullptr;
        if(pomocni2=nullptr) {
            pocetni=novi;
        } else {
            pomocni2->sljedeci=novi;
        }
        pomocni2=novi;
        if(pomocni1=l1.DajTekuci()) {
            t=novi;
        }
        pomocni1=pomocni1->sljedeci;

    }
    posljednji=pomocni2;
}

template<typename Tip>
JednostrukaLista<Tip>&JednostrukaLista<Tip>::operator =(const JednostrukaLista<Tip> &l1)
{

    if(this==&l1) return *this;
    Cvor *pomocni(pocetni);
    Cvor *privremeni2(nullptr);
    while(pocetni!=nullptr) {
        privremeni2=pomocni;
        pomocni=pomocni->sljedeci;
        delete privremeni2;
    }

    broj_elemenata=l1.brojElemenata();
    Cvor *pomocni1(l1.DajPocetak());
    Cvor *pomocni2(nullptr);

    while(pomocni1!=nullptr) {
        Cvor *novi=new Cvor;
        novi->element=pomocni1->element;
        novi->sljedeci=nullptr;
        if(pomocni2==nullptr) {
            pocetni=novi;
        } else {
            pomocni2->sljedeci=novi;
        }
        pomocni2=novi;
        if(pomocni1=l1.DajTekuci()) {
            t=novi;
        }
        pomocni1=pomocni1->sljedeci;
    }
    posljednji=pomocni2;

    return *this;
}

template <typename Tip>
JednostrukaLista<Tip>::~JednostrukaLista()
{
    Cvor *pomocni(pocetni);
    Cvor *privremeni2(nullptr);
    while(pomocni!=nullptr) {
        privremeni2=pomocni;
        pomocni=pomocni->sljedeci;
        delete privremeni2;
    }
}


template <typename Tip>
void JednostrukaLista<Tip>::dodajIspred(const Tip &element)
{
    Cvor *pomocni1=new Cvor;
    pomocni1->element=element;
    pomocni1->sljedeci=nullptr;
    broj_elemenata++;

    if(pocetni==nullptr) {
        pocetni=pomocni1;
        posljednji=pomocni1;
        t=pomocni1;
    } else if(t==pocetni) {
        pomocni1->sljedeci=pocetni;
        pocetni=pomocni1;
    } else {
        Cvor *pomocni(pocetni);
        while(pomocni->sljedeci!=t)
            pomocni=pomocni->sljedeci;
        pomocni1->sljedeci=pomocni->sljedeci;
        pomocni->sljedeci=pomocni1;
    }
}


template <typename Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip &element)
{
    Cvor *pomocni1=new Cvor;
    pomocni1->element=element;
    pomocni1->sljedeci=nullptr;
    broj_elemenata++;
    if(pocetni==nullptr) {
        posljednji=pomocni1;
        pocetni=pomocni1;
        t=pomocni1;
    } else {
        pomocni1->sljedeci=t->sljedeci;
        t->sljedeci=pomocni1;
        if(posljednji==t) {
            posljednji=pomocni1;
        }
    }
}



bool TestDodajIza(NizLista<int>&l)
{
    for(int i=1; i<=5; i++) l.dodajIza(i);
    for(int i=1; i<=5; i++) {
        if(l[0]==1 && l[1]==5 && l[2]==4) return true;
    }
    return false;
}


bool TestDodajIza1(JednostrukaLista<int>&j)
{
    for(int i=1; i<=5; i++) j.dodajIza(i);
    for(int i=1; i<5; i++) {
        if((i==1 && j.trenutni()==1) || (i==2 && j.trenutni()==5)) return true;
    }
    return false;
}

bool TestBrojElemenata(NizLista<int> &l)
{
    if(l.brojElemenata()==5) return true;
    return false;
}

bool TestBrojElemenata1(JednostrukaLista<int>&j)
{
    if(j.brojElemenata()==5) return true;
    return false;
}


bool TestTrenutni(NizLista<int>&l)
{
    int i=1;
    bool t=false;
    do {
        if((i==1 && l.trenutni()==1) || (i==3 && l.trenutni()==4)) t=true;
        else t=false;
        i++;
    } while(l.sljedeci());
    if(t==true) return true;
    return false;
}

bool TestTrenutni1(JednostrukaLista<int>&j)
{
    int i=1;
    bool t=false;
    do {
        if((i==1 && j.trenutni()==1 || (i==3 && j.trenutni()==4))) t=true;
        else t=false;
        i++;
    } while (j.sljedeci());
    if(t=true) return true;
    return false;

}

bool TestPrethodni(NizLista<int> &l)
{
    if(l.prethodni()) return true;
    return false;
}

bool TestPrethodni1(JednostrukaLista<int>&j)
{
    if(j.prethodni()) return true;
    return false;
}

bool TestDodajIspred(NizLista<int> &l1)
{
    for(int i=5; i>=1; i--) l1.dodajIspred(i);
    bool t=false;
    for(int i=5; i>=1; i--) {
        if((i==5 && l1.trenutni()==1) || (i==1 && l1.trenutni()==5)) t=true;
        else t=false;
    }
    if(t=true) return true;
    return false;
}

bool TestDodajIspred1(JednostrukaLista<int> &j1)
{
    for(int i=5; i>=1; i--) j1.dodajIspred(i);
    bool t=false;
    for(int i=5; i>=1; i--) {
        if((i==5 && j1.trenutni()==1) || (i==1 && j1.trenutni()==5)) t=true;
        else t=false;
    }
    if(t=true) return true;
    return false;
}

bool TestSljedeci(NizLista<int>&l)
{
    l.dodajIspred(6);
    return l.sljedeci();
}

bool TestSljedeci1(JednostrukaLista<int> &j)
{
    j.dodajIspred(6);
    return j.sljedeci();
}


bool TestPocetak(NizLista<int>&l)
{
    l.pocetak();
    if(l.trenutni()!=1) return false;
    return true;
}

bool TestPocetak1(JednostrukaLista<int> &j)
{
    j.pocetak();
    if(j.trenutni()!=1) return false;
    return true;
}

bool TestKraj(NizLista<int> &l)
{
    l.kraj();
    if(l.trenutni()!=2) return false;
    return true;
}

bool TestKraj1(JednostrukaLista<int> &j)
{
    j.kraj();
    if(j.trenutni()!=2) return false;
    return true;
}

bool TestObrisi(NizLista<int>&l)
{
    int z=l.brojElemenata();
    l.obrisi();
    if(l.brojElemenata()!=z-1) return false;
    return true;
}

bool TestObrisi1(JednostrukaLista<int>&j)
{
    int z=j.brojElemenata();
    j.obrisi();
    if(j.brojElemenata()!=z-1) return false;
    return true;
}

bool TestOperatora(NizLista<int>&l)
{
    if(l[0]==1) return true;
    return false;
}


int main()
{
    NizLista<int> lista, lista1;
    JednostrukaLista<int> jlista,jlista1;


    try {
        if(TestDodajIza(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test nije uspjesan!";
        std::cout<< std::endl;

        if(TestDodajIza1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test2 nije uspjesan!";
        std::cout << std::endl;

        if(TestBrojElemenata(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test3 nije uspjesan!";
        std::cout << std::endl;

        if(TestBrojElemenata1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test4 nije uspjesan!";
        std::cout << std::endl;

        if(TestTrenutni(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test5 nije uspjesan!";
        std::cout << std::endl;

        if(TestTrenutni1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test6 nije uspjesan!";
        std::cout << std::endl;

        if(TestPrethodni(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test7 nije uspjesan!";
        std::cout << std:: endl;

        if(TestPrethodni1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test8 nije uspjesan!";
        std::cout << std::endl;

        if(TestDodajIspred(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test9 nije uspjesan!";
        std::cout << std::endl;

        if(TestDodajIspred1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test10 nije uspjesan!";
        std::cout << std::endl;

        if(TestSljedeci(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test11 nije uspjesan!";
        std::cout << std::endl;

        if(TestSljedeci1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test12 nije uspjesan!";
        std::cout << std::endl;

        if(TestPocetak(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test13 nije uspjesan!";
        std::cout << std::endl;

        if(TestPocetak1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test14 nije uspjesan!";
        std::cout << std::endl;

        if(TestKraj(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test15 nije uspjesan!";
        std::cout << std::endl;

        if(TestKraj1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test16 nije uspjesan!";
        std::cout << std::endl;

        if(TestObrisi(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test17 nije uspjesan!";
        std::cout << std::endl;

        if(TestObrisi1(jlista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test18 nije uspjesan!";
        std::cout << std::endl;

        if(TestOperatora(lista)) std::cout << "Test je uspjesan!";
        else std::cout << "Test19 nije uspjesan!";
        std::cout << std::endl;
    } catch(...) {
        std::cout << "Izuzetak!"; //Uhvacen je izuzetak;
    }
    return 0;
}*/
