#include<iostream>
#include<fstream>
using namespace std;

class cls
{
private : 
    int prv[3];
public : 
    int pbl[5];

    void set()
    {
        prv[0] = 1;
        prv[1] = 34;
        prv[2] = 78;

        pbl[0]= 12;
        pbl[1]= 34;
        pbl[2]= 56;
        pbl[3]= 78;
        pbl[4]= 90;
    }

    int* getprv(){
        return prv;
    }

    void printprv(){
        for (int i=0; i<3; i++)
        {
            cout<<prv[i]<<"---> ";
        }
        cout<<endl;
    }

    void printpbl()
    {
    for (int i=0; i<5; i++)
    {
        cout<<(pbl[i])<<"---> ";
    }
    cout<<endl;
    }

};

int main()
{
    cls dat1, dat2, dat3;
    dat1.set();

    dat1.printprv();
    dat1.printpbl();

    ofstream o("output.test", ios::binary|ios::out);
    o.write((char*)&dat1, sizeof(dat1));

    dat1.printprv();
    dat1.printpbl();

    ifstream i("output.test", ios::binary | ios::in);
    if(!i) cout<<"File cannot be opened."<<endl;
    i.seekg(0);
    i.read((char*)&dat2, sizeof(dat2));
    
    dat2.printprv();
    dat2.printpbl();

    return  0;
}