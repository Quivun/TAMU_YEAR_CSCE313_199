/*
    Joshua Chong
    CS Undergraduate
    Texas A&M University
    Date  : 5/31/20
 */
#include "common.h"
#include "FIFOreqchannel.h"

#include<iostream>
#include<string>
#include<stdio.h>
using namespace std;


int main(int argc, char *argv[]){
    int pid = fork();
    if (pid == 0){
        // Got points off last time I didn't auto summon rip
        char *args[] = { "./server", NULL};
        execvp(args[0], args);
    } else {
        int numPat = 10;
        int numReq = 100;

    }
    FIFORequestChannel chan ("control", FIFORequestChannel::CLIENT_SIDE);

    srand(time(NULL));
    // Start 1 Data point test 
    // sending a non-sense message 
    struct timeval s0,e0;
    gettimeofday(&s0, NULL);
    datamsg *nonSMsg = new data(1,0.004,2);
    chan.cwrite( nonSMsg, sizeof(datamsg) );
    double data = 0;
    chan.cread(&data, sizeof(double));
    printf("Data = %lf", data);
    delete nonSMsg;
    gettimeofday(&e0,NULL);
    // Stop 1 Data point test

    // Start Data Retrieval test
    struct timeval s1,e1;
    gettimeofday(&s1, NULL);
    // offset = 0, length = 0
    filemsg *f0 = new filemsg(0,0);
    string fNameReq = "1.csv";
    int req = sizeof(filemsg) + fNameReq.size() + 1;
    char *buf = new char[req];
    memcpy(buf, f0, sizeof(filemsg));
    strcpy(buf+sizeof(filemsg), fNameReq.c_str());
    chan.cwrite(buf, req);

    __int64_t fSize;
    chan.cread(&fSize, sizeof(__int64_t));
    
    string output_path = string("received/" + "x1.csv");
    FILE *f = fopen(output_path.c_str(), "wb");
    char *receiver = new char[MAX_MESSAGE];
    while (fSize > 0){
        int req_len = min((__int64_t)MAX_MESSAGE, fSize);
        ((filemsg *)buf)->length = req_len;
        chan.cwrite(buf, request);
        chan.cread(receiver, req_len);
        fwrite(receiver, 1, req_len, f);
        ((filemsg *)buf)->offset += req_len;
        fSize -= req_len;
    }
    fclose(f);
    delete buf;
    delete receiver;
    gettimeofday(&e1, NULL);
    // Stop Data Retrieval test

}
