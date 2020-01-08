#include <fstream>
#include <iostream>
using namespace std;

int main(){
    int data;
    int count = 0;
    ifstream infile;
    infile.open("data.txt");
    while(!infile.eof()){
        cout<<"reading file ";
        infile>>data;
        if(infile.fail()){
            break;
        }
        cout << data << " "<<endl;
        count++;
    }
    infile.close();
    cout<<count;
}
