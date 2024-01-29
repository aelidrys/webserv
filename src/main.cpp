#include "multiblex.hpp"

int main(){
    try{
        multiblex mlt;
        mlt.m_server();
    }
    catch(const char* exp){
        cout<<exp<<endl;
    }
    catch(string exp){
        cout<<exp<<endl;
    }
}