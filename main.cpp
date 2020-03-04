#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

ifstream fin("date.in");
ofstream fout("date.out");

typedef struct
{
    int x;
    int y;

}punct;

int test_orientare(punct p, punct q, punct r)
{
    int det=r.y*(q.x-p.x)+q.y*(p.x-r.x)+p.y*(r.x-q.x);
    if(det==0)
        return 0;/// cazul in care sunt coliniare
        else
            if(det>0)
                return 1;/// sens trigonometric
            else
                return 2;/// sens orar
}

int verificare_tangenta(punct P, punct Q, vector<punct> poligon, int n)
{
    int v=-1, j=0;
    /// gasim primul punct diferit de Q si salvam in v
    /// directia de orientare fata de PQ
    while(j<n && v==-1)
    {

        if(Q.x!=poligon[j].x&&Q.y!=poligon[j].y)
                v=test_orientare(P,Q,poligon[j]);
        j++;
    }
    /// verificam daca toate punctele sunt de aceeasi parte
    ///  fata de dreapta PQ
    /// in caz afirmativ PQ este tangenta
    for(int i=0;i<n;i++)
    {
        if(!(Q.x==poligon[i].x && Q.y==poligon[i].y))
            if(test_orientare(P,Q,poligon[i])!=v)
                return 0;
    }

    return 1;
}

void tangente(punct P, int n, vector<punct> poligon, punct &A, punct &B)
{
    int i,ok=0;
    /// gasim cele doua tangente PA si PB
    for(i=0;i<n;i++)
    {
        if(verificare_tangenta(P,poligon[i],poligon,n))
            {
                if(ok==0)
                {
                    A=poligon[i];
                    ok=1;
                }

                else
                        B=poligon[i];

            }
    }
}
/// calculam determinantul
/// xA yA 1
/// xB yB 1
/// xC yC 1
int determinant(punct A, punct B, punct C)
{    return  A.x*B.y+B.x*C.y+A.y*C.x-B.y*C.x-B.x*A.y-A.x*C.y;}

/// vectorii (PA, AX), (AB, BX) ,(BP, PX)
/// vor realiza mereu intotdeuna aceeasi intoarcere
/// daca punctul X este in interiorul triunghiului ABP
/// de aceea determinantii d1, d2, d3
/// trebuie sa aiba acelasi semn
int punct_int_triunghi(punct P, punct A, punct B, punct X)
{
    int d1,d2,d3;
    d1=determinant(A,B,X);
    d2=determinant(B,P,X);
    d3=determinant(P,A,X);

    if(!((d1>0&&d2>0&&d3>0)||(d1<0&&d2<0&&d3<0)))
        return 0;
    return 1;

}

void acoperire_convexa(int n, vector<punct> poligon, punct P, punct A, punct B, vector<punct> &Ac_convexa)
{

   int i;

    for(i=0;i<n;i++)
            Ac_convexa.push_back(poligon[i]);

    for(auto i=Ac_convexa.begin(); i<Ac_convexa.end();i++)
    {
            if(punct_int_triunghi(P,A,B,*i)!=0)
            {
                   Ac_convexa.erase(i);
                   i--;
            }
    }

    for(auto i=Ac_convexa.begin(); i<Ac_convexa.end();i++)
    {
            auto j=i+1;
            if(((*i).x==A.x && (*i).y==A.y && (*j).x==B.x && (*j).y==B.y ) || ((*i).x==B.x && (*i).y==B.y && (*j).x==A.x && (*j).y==A.y ))
            {
                    Ac_convexa.insert(j,P);
                    j++;
                    i=j;
                    return ;
            }

    }

       Ac_convexa.insert(Ac_convexa.begin(),P);
}

int main()
{
    vector<punct> poligon, Ac_convexa;
    int n,i;
    fin>>n;
    punct A,B,P;

    for(i=0;i<n;i++)
    {
        punct A;
        fin>>A.x>>A.y;
        poligon.push_back(A);
    }
    fin>>P.x>>P.y;

    tangente(P,n,poligon,A,B);

    acoperire_convexa(n, poligon,P, A, B, Ac_convexa);

    fout<<"Acoperirea convexa este formata din punctele: \n";
    for(unsigned int i=0;i<Ac_convexa.size();i++)
        fout<<"("<<Ac_convexa[i].x<<", "<<Ac_convexa[i].y<<")\n";

    return 0;
}
