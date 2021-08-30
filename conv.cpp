#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <iomanip>
using namespace std;

#define DEBUG

// void blah1(){
// 	std::cout<<omp_get_thread_num()<<"\n";
// }

// int main() {
// 	int numProcs = omp_get_num_procs();
// 	cout<<"numProcs"<<numProcs<<"\n";
// 	#pragma omp parallel num_threads(numProcs)
// 	{
// 		blah1();
// 	}
//    return 0;
// }

class my_mat{
    private:
        double *ary;
        int sizeX,sizeY;
        int sizeU,sizeV;
    public:
        my_mat(int sizeX, int sizeY,int sizeU,int sizeV){
            this->sizeX = sizeX;
            this->sizeY = sizeY;
            this->sizeU = sizeU;
            this->sizeV = sizeV;
            ary = new double[sizeX*sizeY];
        }
        void set(int i , int j, double k){
            ary[i*sizeY+j]=k;
        }
        double get(int i, int j){
            if (i<0 || j<0 ||i>=sizeX || j>=sizeY){
                return 0.0;
            }
            return ary[i*sizeY+j];
        }
};

void run_testcase( ifstream& infile ){
    int X, Y, U, V, N;
    int row_num,col_num;
    double xt;
    
    infile>>X>> Y>> U>> V>> N;
    my_mat main_mat = my_mat(X,Y);
    my_mat filter_mat = my_mat(U,V);
    
    for(row_num=0;row_num<X;row_num++){
        for( col_num=0; col_num<Y;col_num ++ ){
            infile>>xt;
            main_mat.set(row_num,col_num,xt);
        }
    }

    for(row_num=0;row_num<U;row_num++){
        for( col_num=0; col_num<V;col_num ++ ){
            infile>>xt;
            filter_mat.set(row_num,col_num,xt);
        }
    }

#ifdef DEBUG
    for(row_num=0;row_num<X;row_num++){
        for( col_num=0; col_num<Y;col_num ++ ){
            cerr<<main_mat.get(row_num,col_num);
        }
        cerr<<"\n";
    }

    for(row_num=0;row_num<U;row_num++){
        for( col_num=0; col_num<V;col_num ++ ){
            cerr<<filter_mat.get(row_num,col_num);
        }
        cerr<<"\n";
    }
#endif
    return;
}


int main(){
    int T;
    ifstream infile;
    infile.open("input.txt");
    infile>>T;
    for(int testcase=0; testcase<T;testcase++){
        run_testcase( infile );
    }
}