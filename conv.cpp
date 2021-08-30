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
    public:
        my_mat(int sizeX, int sizeY){
            this->sizeX = sizeX;
            this->sizeY = sizeY;
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

class TestCase{
    public:
        int X, Y, U, V, N;
        my_mat *filter_mat;
        my_mat *main_mat;
        my_mat *main_mat_next;

        void setup_test_case(int x,int y,int u,int v,int n){
            X = x;
            Y = y;
            U = u;
            V = v;
            N = n;
            main_mat = new my_mat(X,Y);
            main_mat_next = new my_mat(X,Y);
            filter_mat = new my_mat(U,V);
        }
        
};

void print_mat(my_mat main_mat,int X,int Y){
// #ifdef DEBUG
    int row_num,col_num;
    for(row_num=0;row_num<X;row_num++){
        for( col_num=0; col_num<Y;col_num ++ ){
            cerr<<main_mat.get(row_num,col_num);
        }
        cerr<<"\n";
    }
// #endif
    return;
}


int main(){
    
    int T;
    int X, Y, U, V, N;
    int row_num,col_num;
    double xt;
    
    ifstream infile;
    infile.open("input.txt");
    
    // Get number of test cases
    infile>>T;
    TestCase *testCasesArr = new TestCase[T];
    for(int testcase=0; testcase<T;testcase++){
        // Input test case no testcase
        // run_testcase( infile );
        infile>>X >> Y>> U>> V>> N;
        testCasesArr[testcase].setup_test_case(X,Y,U,V,N);

        for(row_num=0;row_num<X;row_num++){
            for( col_num=0; col_num<Y;col_num ++ ){
                infile>>xt;
                testCasesArr[testcase].main_mat->set(row_num,col_num,xt);
            }
        }

        for(row_num=0;row_num<U;row_num++){
            for( col_num=0; col_num<V;col_num ++ ){
                infile>>xt;
                testCasesArr[testcase].filter_mat->set(row_num,col_num,xt);
            }
        }
    }

    // Now we start parallel computation of convolutions




}