#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <iomanip>

using namespace std;

//#define DEBUG



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
        void incr(int i,int j, double v){
            ary[i*sizeY+j]+=v;
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
        void printTestCase(){
            cout<<"X"<<X<<"\nY"<<Y<<"\nU"<<U<<"\nV"<<V<<"\nN"<<N;
        }
};

void computeConvolutions(TestCase obj_testcase){
    //obj_testcase.printTestCase();
    //int numProcs = omp_get_num_procs();
    for(int n=0;n<obj_testcase.N;n++) {
        int prob_size = obj_testcase.X*obj_testcase.Y;
        #pragma omp parallel for
        for( int ij=0; ij<prob_size;ij++){
            int i=ij/obj_testcase.Y;
            int j=ij%obj_testcase.Y;
            obj_testcase.main_mat_next->set(i,j,0.0);
            for(int i_f=0;i_f<obj_testcase.U;i_f++){
                for(int j_f=0;j_f<obj_testcase.V;j_f++){
                    obj_testcase.main_mat_next->incr(i,j,  obj_testcase.main_mat->get(i+i_f-obj_testcase.U/2,j+j_f-obj_testcase.V/2) * obj_testcase.filter_mat->get(i_f,j_f) );
                }
            }
        }
        // #pragma omp parallel for
        // for( int ij=0; ij<prob_size;ij++){
        //     int i=ij/obj_testcase.Y;
        //     int j=ij%obj_testcase.Y;
        //     obj_testcase.main_mat_next->set(i,j,0.0);   
        // }
        // #pragma omp parallel for collapse(3)
        // for( int ij=0; ij<prob_size;ij++){
        //     for(int i_f=0;i_f<obj_testcase.U;i_f++){
        //         for(int j_f=0;j_f<obj_testcase.V;j_f++){
        //             int i=ij/obj_testcase.Y;
        //             int j=ij%obj_testcase.Y;
        //             obj_testcase.main_mat_next->incr(i,j,  obj_testcase.main_mat->get(i+i_f-obj_testcase.U/2,j+j_f-obj_testcase.V/2) * obj_testcase.filter_mat->get(i_f,j_f) );
        //         }
        //     }
        // }


        my_mat *temp = obj_testcase.main_mat;
        obj_testcase.main_mat = obj_testcase.main_mat_next;
        obj_testcase.main_mat_next=temp;


    }
}

void print_mat_file(my_mat main_mat,int X,int Y,ofstream &fout){

    int row_num,col_num;
    for(row_num=0;row_num<X;row_num++){
        for( col_num=0; col_num<Y;col_num ++ ){
            fout<<std::scientific<<std::setprecision(10)<<main_mat.get(row_num,col_num)<<" ";
        }
        fout<<"\n";
    }
    return;
}

void print_mat(my_mat main_mat,int X,int Y){

    int row_num,col_num;
    for(row_num=0;row_num<X;row_num++){
        for( col_num=0; col_num<Y;col_num ++ ){
            cerr<<std::scientific<<std::setprecision(10)<<main_mat.get(row_num,col_num)<<" ";
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
    infile.open("input1.txt");
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
    
    ofstream outfile;
    outfile.open("output.txt", ios::out | ios::trunc );
    // testCasesArr[0].printTestCase();
    // Sequential Execution of test cases
    for(int t=0;t<T;t++){
        // cout<<"T"<<t<<"\n";
        computeConvolutions(testCasesArr[t]);
        //print_mat(*(testCasesArr[0].main_mat_next), testCasesArr[0].X,testCasesArr[0].Y);
        print_mat_file(*(testCasesArr[0].main_mat_next), testCasesArr[0].X,testCasesArr[0].Y,outfile);
    }
    // computeConvolutions(testCasesArr[0]);
    // print_mat(*(testCasesArr[0].main_mat_next), testCasesArr[0].X,testCasesArr[0].Y);
    // cout<<"testCasesArr[T]"<<testCasesArr[0].X;

    // Now we start parallel computation of convolutions
    // #pragma omp parallel num_threads(T) 
    // {
    //     int this_thread_num = omp_get_thread_num();
    //     computeConvolutions(testCasesArr[this_thread_num]);
    // }

    
}


// pragma omp parallel for simd
// pragma omp declare simd
