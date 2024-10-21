#include <iostream>
#include <fstream>
#include "include.hpp"
using namespace std;


extern void eval_test(){
  const int degree = 1024;//2048;                        //polynomial degree 
  const int64_t ct_modulus = 3221225473;//206158430209;//2748779069441;//576460752154525697;           //ciphertext modulus
  const int64_t modulus = 6000;//12289;                       //plaintext modulus

  //int64_t ct_modulus = 576460752154525697;
  int64_t delta = 1;
  int logq = 32;//38;//42;//60;
  //int degree = 2048;
  double var = 3.2;
  int64_t b = 2;
  int logb = 1;
  //int64_t modulus = 12289; 
  int64_t primitive_root = 3;

  //read input ct
  //vector<vector<int64_t> > ct(2,vector<int64_t>(degree,0));
  ifstream fin;
  fin.open("ciphertext.txt");
  if(!fin.is_open()){
      cout <<"Cannot open file ciphertext.txt"<<endl;
    }
  
  vector<vector<int64_t> > ct(2,vector<int64_t>(degree,0));
  for(int i = 0 ; i < 2 ; ++i){
    for(int j = 0 ; j < degree ; ++j){
	     fin >>ct[i][j];
    }
  }
  fin.close();
  cout <<"Read input ciphertext from ciphertext.txt"<<endl;

  //read encrypt network
  vector<vector<int64_t> > ct1(2,vector<int64_t>(degree,0));
  vector<vector<int64_t> > ct2(2,vector<int64_t>(degree,0));

  fin.open("test_enc_nn.txt");
  if(!fin.is_open()){
      cout <<"Cannot open file test_enc_nn.txt"<<endl;
    }
  for(int i = 0 ; i < 2 ; ++i){
    for(int j = 0 ; j < degree ; ++j){
      fin >>ct1[i][j];
  //cout << ct2[i][j]<< endl;
    }
  }
  for(int i = 0 ; i < 2 ; ++i){
    for(int j = 0 ; j < degree ; ++j){
      fin >>ct2[i][j];
	//cout << ct2[i][j]<< endl;
    }
  }
  fin.close();
  cout <<"read encrypted registered vector from test_enc_nn.txt"<<endl;
  

  polynomial s(degree,0);
  /*
  fin.open("rlwe_sk.txt");
  for(int i=0;i<degree;++i){
    fin>>s[i];
  }
  fin.close();
  cout <<"Read RLWE secret key. (only for test)"<<endl;

  int *lwe_s = new int [degree];
  for(int i = 0 ; i < degree ; ++i){
    lwe_s[i] = (int)s[i];
  }
*/
  //vector<vector<polynomial>>RelK=RelK_Gen(s,degree,ct_modulus,ct_modulus,logq,var,b,logb);
  
  //read RelK
  vector<vector<polynomial>> RelK(logq, vector<polynomial>(2, polynomial(degree,0)));
  fin.open("rlwe_relk.txt");
  if(!fin.is_open()){
      cout <<"Cannot open file rlwe_relk.txt"<<endl;
    }
  for (int i = 0; i < logq; ++i){
    for (int j = 0; j < 2; ++j){
      for(int k = 0 ; k < degree ; ++k){
        fin >>RelK[i][j][k];
      }
    }
  }
  fin.close();

/*
  for(int i = 0 ; i < 3 ; ++i){
    polynomial out = RLWE64_Dec(degree,ct_modulus,modulus,s,RelK[i]);
    cout <<"Decryption of "<<i<<"-th RLWE ct in RelK: "<<endl;
    print_polynomial(out);
  }
*/
  vector<polynomial> RLWE_mul1 = ct_multiplication(ct,ct1, RelK, degree, ct_modulus,modulus, 
    logq, b, logb,s);

  vector<polynomial> RLWE_mul2 = ct_multiplication(ct,ct2, RelK, degree, ct_modulus,modulus, 
    logq, b, logb,s);



  //polynomial dec = RLWE64_Dec(degree,ct_modulus,modulus,s,RLWE_mul);

/*
  cout <<"Decryption output: "<<endl;
  for(int i = 0 ; i < degree ; ++i){
    cout <<dec[i]<<" ";
  }
  cout <<endl;
*/

  vector<int64_t> LWE_ip1 = extract_0(RLWE_mul1,degree);
  vector<int64_t> LWE_ip2 = extract_0(RLWE_mul2,degree);

  ofstream fout;
  fout.open("encrypted_result.txt");

  for(int i = 0 ; i < (degree+1) ; ++i){
      fout <<LWE_ip1[i]<<" ";
  }
  fout <<endl;
  for(int i = 0 ; i < (degree) ; ++i){
      fout <<LWE_ip2[i]<<" ";
  }
  fout<<LWE_ip2[degree];
  fout.close();
  cout <<"Stored the encrypted result in encrypted_result.txt"<<endl;

  /*
  //output the encrypted result 
  ofstream fout;
  fout.open("/share/encrypted_result.txt");
  if(!fout.is_open()){
    cout <<"Cannot open file encrypted_result.txt"<<endl;
  }
  fout << timestamp << endl;
  fout << test_name << endl;
  for(int i = 0 ; i < (degree+1) ; ++i){
      fout <<LWE_ip1[i]<<" ";
  }
  fout <<endl;
  for(int i = 0 ; i < (degree+1) ; ++i){
      fout <<LWE_ip2[i]<<" ";
  }
  fout<<endl;
  fout.close();
  cout <<"Stored the encrypted result in encrypted_result.txt"<<endl;
  */

  //cout<<"Decryption result(for test) = " <<LWE64_Dec(modulus,ct_modulus,degree,LWE_ip,lwe_s)<<endl;
}

int main(){
  eval_test();
}
  
