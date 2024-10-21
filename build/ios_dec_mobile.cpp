// ================================================

//Code is designed for 1 to 1 Face recognition demo of SCRIPTS

//Date: July 2022

// ================================================


#include <iostream>
#include <vector>
#include <ctime>
#include<sys/time.h>
#include<time.h>
#include<unistd.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <cstdint>
#include <algorithm>
#include <cmath>
//#include <immintrin.h>
#include <string>


using namespace std;

int scaler0 = 40;                         // scaler for input vector

int scaler1 = 40;                       // Scaler for Weights 

//double threshold = 0.468;


/*
//for test

vector<double> test_vector1;
vector<double> test_vector2;
//read test vectors and labels
void read_vector1() {
  ifstream infile;
  infile.open("input_Img.txt");
  for (int j = 0; j < 512; ++j) {
    double t;
    infile >> t;
    test_vector1.push_back(t);
  }
  infile.close();
}

//read test vectors and labels
void read_vector2() {
  ifstream infile;
  infile.open("Registered_Img.txt");
  for (int j = 0; j < 512; ++j) {
    double t;
    infile >> t;
    test_vector2.push_back(t);
  }
  infile.close();
}

void cmp_pt(){
  double ip = 0.0;
  for (int i = 0; i < 512; ++i){
    ip += (test_vector1[i]*scaler0*scaler1*test_vector2[i]);
  }
  cout <<"ip in pt: "<<ip<<endl;
  if (ip > threshold){
    cout <<"result in pt: Accept"<<endl;
  }
  else{
    cout <<"result in pt: reject"<<endl;
  }
}
*/




// ======================================================  LWE Encryption Scheme  ==========================================================================

//mod q
inline void modq(int64_t number, int64_t q) {
  if(number < 0){
      int64_t temp = (-1*number)/q + 1;
      number += temp*q;
    }
    if(number >= q){
      int64_t temp = number/q;
      number -= temp*q;
    }
    if(number >= q/2){
      number -= q;
    }
  return ;
}


// LWE Decryption
int64_t Decrypt(int64_t q, int64_t p, int n, const vector<int64_t>& c, const vector<int>& x) {
  // INPUT: modulus q, dimension n, LWE ciphertext c = (vec(a),b), LWE key x
  // OUTPUT: Decryption (b + <a,x>) mod q
  int64_t alpha = q/p;
  int64_t ip2 = c[n];
  for (int i = 0; i < n; ++i) {
    ip2 += (c[i]*(int64_t)x[i]);
      modq(ip2, q);
  }

  while(ip2 < 0){
      ip2 += q;
    }
    int64_t temp2 = (ip2 + alpha/2) % q;
    temp2 /= alpha;
    if (temp2 > p/2){
      temp2 -= p;
    }
    return temp2;

}


string ios_enc_vec_dec(){

  // ======================== Initialization and Information Output ==========================

  srand(time(0));
  //read_vector();

  //set n,q,t for RLWE scheme
  const int n1 = 1024;//2048;                        //polynomial degree 
  const int64_t q1 = 3221225473;//206158430209;//2748779069441;//576460752154525697;           //ciphertext modulus
  const int p1 = 6000;//12289;                       //plaintext modulus

  //network parameter
  const int l1 = 512;


  //read LWE secret key
  ifstream fin;
  fin.open("rlwe_sk.txt");
  vector<int> x(n1);
  for (int i=0;i<n1;++i){
    fin >> x[i];
  }
  fin.close();
  cout <<"read LWE secret key."<<endl;

  //read encrypted result

  vector<int64_t> ct_ip1(n1+1,0);
  vector<int64_t> ct_ip2(n1+1,0);

  fin.open("encrypted_result.txt");
  if(!fin.is_open()){
    cout <<"Cannot open file encrypted_result.txt"<<endl;
  }
  for(int i = 0 ; i < (n1+1) ; ++i){
    fin >>ct_ip1[i];
  }
  for(int i = 0 ; i < (n1+1) ; ++i){
    fin >>ct_ip2[i];
  }
  fin.close();
  cout <<"Read encrypted result from encrypted_result.txt"<<endl;

  int64_t dec_ip1 = Decrypt(q1,p1,n1,ct_ip1,x);
  cout <<"decryption result of ip1 = "<<dec_ip1+1600<<endl;

  int64_t dec_ip2 = Decrypt(q1,p1,n1,ct_ip2,x);
  cout <<"decryption result of ip2 = "<<dec_ip2-1600<<endl;

  cout <<"verification result: ";
  if(dec_ip1 + 1600 < dec_ip2-1600){
    string a = "yes";
    cout <<a <<endl;
    return a;
  }
  else{
    string a = "no";
    cout <<a <<endl;
    return a;
  }

 // int64_t trd = (int64_t)(threshold*(double)scaler0*(double)scaler1);
 // cout <<"threshold * s0 * s1 = "<<trd<<endl;

//  ofstream fout;
//  fout.open("decrypted_result.json");
//  fout <<"{"<<endl;

  // add time stamp
  // time_t ltime;

  // time(&ltime); 
  // fout << "\"id\" : ";
  // fout << "\""<<ctime(&ltime) << "\"," << endl;


/*
  fout<<"\"result\" : ";

    int result = -1;
    if(dec_ip1 > trd){
      result = 1;
      cout <<"Accept. "<<endl;
      fout<<"\"Accept\"}"<<endl;
    }
    else{
      result = 0;
      cout <<"Reject. "<<endl;
      fout<<"\"Reject\""<<endl;
      fout<<"}"<<endl;
    }
*/
    


}


int main(){
  string b = ios_enc_vec_dec();
}





