/*
 * dist2nj
 * Date: May-22-2018 
 * Author : Gabriel Renaud gabriel.reno [at sign here ] gmail.com
 *
 */

#include <sstream>
#include <iostream>
#include <fstream>

#include "NjTree.h"

#include "utils.h"

using namespace std;

vector<double> insertZeroAtIndex(const vector<double> disttoadd,unsigned int indexToAdd){
    vector<double> toreturn;
    for(unsigned int i=0;i<disttoadd.size();i++){
	if(i==indexToAdd)
	    toreturn.push_back(0);
	toreturn.push_back( disttoadd[i] );	
    }
    
    if(indexToAdd == disttoadd.size()){
	toreturn.push_back( 0 );
    }
    return toreturn;
}

string dist2nj(const string & distfile){
    igzstream myfile;
    //cout<<distfile<<endl;
    myfile.open(distfile.c_str(), ios::in);

    if(!myfile.good()){
	cerr << "Unable to open distance file "<<distfile<<endl;
	exit(1);
    }
    string line;
    bool foundAll=false;
    vector< vector<double> > dist;
    vector<double> disttoadd;
    string lastPopNameFound="-1";
    unsigned int disttoaddsize;
    vector<string> popnames;
    while ( getline (myfile,line)){   
        trimWhiteSpacesBothEnds(&line);
	//cout<<foundAll<<" "<<line<<endl;
	if(line == "---------------ALL---------------"){ foundAll=true; continue; }
	if(!foundAll) continue;
	
	//cout<<line<<endl;
        vector<string> fields = allTokensWhiteSpaces( line );
        vector<string> fieldnames = allTokens( fields[0] , '-' );
	
	if(fields.size() < 20){
	    cerr<<"Problem with line "<<line<<" has less than 20 fields"<<endl;
	    exit(1);
	}
	
	if(fieldnames.size() != 2 ){
	    cerr<<"Problem with line "<<line<<" name cannot be divided into 2 fields"<<endl;
	    exit(1);
	}

	if(  fieldnames[0] == "anc" ||
	     fieldnames[1]  == "anc"){
	    continue;
	}
	//cout<<endl<<fieldnames[0]<<" "<<fieldnames[1]<<" "<<fields[19]<<endl;

	//cout<<"lastPopNameFound "<<lastPopNameFound<<" "<<dist.size()<<" "<<disttoadd.size()<<endl;
	if(lastPopNameFound != fieldnames[0]){
	    //cout<<"diff"<<endl;
	    if( disttoadd.size() == 0){//first record
		lastPopNameFound = fieldnames[0];
		disttoadd.push_back(destringify<double>(fields[19]));

		popnames.push_back(fieldnames[0]);
		//cout<<fieldnames[0]<<" "<<fieldnames[1]<<" "<<fields[19]<<endl;
		continue;
	    }

	    if( dist.size() == 0){//first record
		disttoaddsize = disttoadd.size();
	    }//else{
	    // 	dist.push_back(disttoadd);
	    // }
	    if(disttoaddsize != disttoadd.size()){
		cerr<<"Problem with line "<<line<<" the previous "<<lastPopNameFound<<" had "<<disttoadd.size()<<" instead of "<<disttoaddsize<<" records"<<endl;
		exit(1);
	    }
	    lastPopNameFound = fieldnames[0];

	    disttoadd=insertZeroAtIndex(disttoadd,popnames.size()-1);
	    
	    dist.push_back(disttoadd);
	    
	    popnames.push_back(fieldnames[0]);
	    disttoadd.clear();
	    disttoadd.push_back(destringify<double>(fields[19]));
	}else{
	    disttoadd.push_back(destringify<double>(fields[19]));
	}
	//cout<<fieldnames[0]<<" "<<fieldnames[1]<<" "<<fields[19]<<endl;
    }
    disttoadd=insertZeroAtIndex(disttoadd,popnames.size()-1);
     
    dist.push_back(disttoadd);
    myfile.close();
    // for(unsigned int i=0;i<dist.size();i++){
    // 	for(unsigned int j=0;j<dist[i].size();j++){
    // 	    cout<<i<<" "<<j<<" "<<dist[i][j]<<endl;
    // 	}
    // }

    //cout<<vectorToString(popnames)<<endl;
    stringstream ss;
    ss<<*neighborJoinFromDist(dist,
			      popnames,
			      popnames.size());
    return ss.str();
}

int main (int argc, char *argv[]) {


    cout<<dist2nj(string(argv[argc-1]))<<endl;
   // string line;
   // ifstream myFile;
   // string filename = string(argv[1]);
   // myFile.open(filename.c_str(), ios::in);

   // if (myFile.is_open()){
   //   while ( getline (myFile,line)){

   //   }
   //   myFile.close();
   // }else{
   //     cerr << "Unable to open file "<<filename<<endl;
   //     return 1;
   //  }
    return 0;
}

