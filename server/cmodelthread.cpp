#include "cmodelthread.h"

CModelThread::CModelThread()
{

}

CModelThread::~CModelThread()
{

}

CModelThread::CModelThread(modelData d, int id){
    data = d;
    userid = id;
}

void CModelThread::run(){
    //QString ans = simulation(data);
}
