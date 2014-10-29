#include "model.h"

Model::Model()
{
}

void Model::simulate(QString s, int* ans, int n){
    for (int i = 0; i < n; i++) {
        ans[i] = ((QString)s[i]).toInt();
    }
}
