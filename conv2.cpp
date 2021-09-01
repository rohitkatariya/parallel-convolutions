#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <iomanip>
using namespace std;

#define DEBUG


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

void computeConvolutions(TestCase obj_testcase){
    for(int n=0;n<obj_testcase.N;n++) {
        
    }
}

void print_mat(my_mat main_mat,int X,int Y){

    int row_num,col_num;
    for(row_num=0;row_num<X;row_num++){
        for( col_num=0; col_num<Y;col_num ++ ){
            cerr<<main_mat.get(row_num,col_num);
        }
        cerr<<"\n";
    }
    return;
}



int main(){
    
    int T;
    int X, Y, U, V, N;
    int row_num,col_num;
    double xt;
    
    ifstream infile;
    infile.open("input5.txt");
    
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
    
    // Printing matrices
    #ifdef DEBUG
        for(int testcase=0; testcase<T;testcase++){
            print_mat(*(testCasesArr[testcase].main_mat_next),testCasesArr[testcase].X,testCasesArr[testcase].Y);
            print_mat(*(testCasesArr[testcase].filter_mat),testCasesArr[testcase].U,testCasesArr[testcase].V);
        }
    #endif
    // Now we start parallel computation of convolutions

    #pragma omp parallel num_threads(T) 
    {
        int this_thread_num = omp_get_thread_num();
        TestCase tt=testCasesArr[this_thread_num];
        computeConvolutions(testCasesArr[this_thread_num]);
    }

    
}


// pragma omp parallel for simd
// pragma omp declare simd