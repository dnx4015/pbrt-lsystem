/*
ID: diana.n1
PROG: processGFile 
LANG: C++
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef PBRT_SHAPES_LSYSTEM_PARSE_H
#define PBRT_SHAPES_LSYSTEM_PARSE_H
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
using namespace std;

#define REP(i,n) for(int i=0;i<n;i++)
#define FOR(i,a,b) for(int i=a;i<=b;i++)
#define FORIT(it,p) for(__typeof(p.end()) it=p.begin();it!=p.end();it++)
#define fst first
#define snd second
#define pb push_back

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


struct Rule{
	string func;
	vector < string > params;
	int nparams;
	string result;
	
	Rule(): func(""), nparams(0), result(""){}
	
	Rule(string f, vector<string> p, int n, string r): 
		func(f), params(p), nparams(n), result(r){}
	
	void addParam(string str){
		params.pb(str); nparams++;
	}
	
	string to_string(){
		string res = func + "(";
		FORIT(it, params)
			res += (*it + ((it + 1 != params.end()) ? ", " : ")"));
		return res += " -> " + result;
	}
};

typedef map <string, string> mapstring;
typedef vector <Rule> vrule;

void split(string str, char delim, vector<string> &list);

bool checkDone(string line, string fName, int nrep, int &n, 
				string &result);

void addRule(string function, string result, vrule &rules);

void processLine(string line, mapstring &params, 
				string &axiom, vrule &rules);

void printAll(mapstring params, string axiom, vrule rules);

float operate (float n1, float n2, char oper);

void execute (string &str, int init);

string replaceVals(string format, mapstring params);

void executeRule(string &axiom, int &init, Rule rule);

void executeRules(string axiom, int init, int nreps, vrule rules, 
				  string fName, ofstream &of, string &result);

void parseGFile(string fName, int nrep, 
				string &result, mapstring &params);

#endif // PBRT_SHAPES_LSYSTEM_PARSE_H
