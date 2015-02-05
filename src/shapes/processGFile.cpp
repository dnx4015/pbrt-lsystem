/*
ID: diana.n1
PROG: processGFile 
LANG: C++
*/

#include "stdafx.h"
#include "shapes/processGFile.h"
#include "paramset.h"
#include <unistd.h>
using namespace std;

#define MAX 100
#define VOC_SIZE 11
#define NONE '#'
char vocabulary[] = {'F', 'f', 
					'+', '-', '|',
					'&', '^', 
					 '/', '\\',  
					'[', ']'};
#define DEFINE "define"
#define AXIOM "axiom"
#define PROD "prod"


void parseGFile(string fName, int nrep, string &result, mapstring &params){
	ifstream ifs (fName.c_str(), ifstream::in);
	ifstream auxfs ("tmp_lsystem.txt", ifstream::in);
	ofstream ofs ("tmp_lsystem.txt", ofstream::app);

	string axiom, line;
	vrule rules;
	int n = 0;	
	
	while (getline(ifs, line)){
		processLine(line, params, axiom, rules);
	}
	
	printf("looking for: %s %d\n", fName.c_str(), nrep);
	while (getline(auxfs, line)) {
		if (checkDone(line, fName, nrep, n, result)){
			printf("found: %s %d\n", fName.c_str(), nrep);
			ifs.close();
			auxfs.close();
			ofs.close();
			return;	
		} else if (n != 0){
			//printf("found close: %s %d ~ %d\n", fName.c_str(), nrep, n);
			axiom = result;
		}	
	}
	
	executeRules(axiom, n, nrep, rules, fName, ofs, result);
	
	//printAll(params, axiom, rules);
	//printf("Result:%s\n", result.c_str());
	
	ifs.close();
	auxfs.close();
	ofs.close();
}

bool checkDone (string line, string fName, int nrep, int &n, 
				string &result) {	
	vector<string> words;
	split(line, ' ', words);
	int num;
	if (words[0] == fName){
		num = atoi(words[1].c_str()); 
		if( num == nrep){
			result = words[2];
			return true;
		}else if ( num > n && num < nrep){
			n = num;
			result = words[2];
		}
	}
	return false;	
}

void processLine(string line, mapstring &params, 
				string& axiom, vrule &rules){
	vector<string> words;
	split(line, ' ', words);
	if (words[0] == DEFINE){
		params[words[1]] = words[2];			
	}else if (words[0] == AXIOM){	
		axiom = words[1];
	}else if (words[0] == PROD){
		addRule(words[1], words[3], rules);
	}
}

void split(string str, char delim, vector<string>&list){
	istringstream stream(str);
	string word;
	while(getline(stream, word, delim))
		list.pb(word);
}

void addRule(string function, string result, vrule &rules){
	Rule rule;
	rule.result = result;
	int found = function.find('(');
	if (found == string::npos){
		rule.func = function;
		rules.pb(rule);
		return;
	}
	rule.func = function.substr(0, found);
	int nfound = function.find(',', ++found);
	while ( nfound != string::npos) {
		rule.addParam(function.substr(found, nfound - found));
		found = nfound + 1;
		nfound = function.find(',', found);
	}
	nfound = function.find(')', found);
	rule.addParam(function.substr(found, nfound - found));
	rules.pb(rule);
}

void executeRules(string axiom, int init, int nreps, vrule rules, 
				  string fName, ofstream &ofs, string &result){
	int i = 0, j;
	bool found = false;
	string line;
	FOR(n, init + 1, nreps){
		i = 0; 
		while (i < axiom.length()){
			found = false;
			FORIT(it, rules){
				j = axiom.find(it->func, i);
				if (j != string::npos && i == j) {
					found = true;
					executeRule(axiom, i, *it);
					if( i == j) return;
					break;
				}
			}
			i += found ? 0 : 1;
		}
		printf("creating %s %d \n", fName.c_str(), n);
		line = fName + " " + SSTR(n) + " " + axiom + "\n";
		ofs << line;
	}
	result = axiom;
}

void executeRule(string &axiom, int &init, Rule rule){
	mapstring params;
	int n = rule.nparams, total = rule.nparams, l = 0,
		found = init, nfound = init;
	
	if (n == 0) {
		l = rule.func.length();	
	} else {
		found = axiom.find('(', found) + 1;
		while (n > 1){
			nfound = axiom.find(',', found);
			params[rule.params[total - n]] = 
				axiom.substr(found, nfound - found);
			found = nfound + 1;
			n--;
		}
		nfound = axiom.find(')', found);
		params[rule.params[total -1]] = 
			axiom.substr(found, nfound - found);
		l = nfound - init + 1;
	}
	
	string result = replaceVals(rule.result, params);
	axiom.replace(init, l, result);
	init += result.length();
}

string replaceVals(string format, mapstring params){
	int i;
	FORIT(it, params){
		i = format.find(it->fst);
		while (i != string::npos){
			format.replace(i, it->fst.length(), it->snd);
			i = format.find(it->fst, i + it->snd.length());
		}
	}
	
	i = format.find('(');
	while ( i != string::npos) {
		execute(format, i+1);
		i = format.find('(', i+1);
	}
	
	i = format.find(',');
	while ( i != string::npos) {
		execute(format, i+1);
		i = format.find(',', i+1);
	}

	return format;
}

void execute (string &str, int init){
	int i = init, j = 0;
	char tmp[MAX], oper = NONE;
	float result = 0;
	while (i < str.length()){
		if (isdigit(str[i]) || str[i] == '.'){
			tmp[j++] = str[i];
		} else if (str[i] == '+' || str[i] == '-' ||
				 str[i] == '*' || str[i] == '/' ||
				 str[i] == ')' || str[i] == ','){
			tmp[j] = '\0'; j = 0; 
			result = (oper == NONE) ? 
					 atof(tmp) : 
					 operate(result, atof(tmp), oper);
			oper = str[i];
			if (str[i] == ')' || str[i] == ',')	break;
		}
		i++;
	}
	
	ostringstream os;
	os << result;
	string res = string(os.str()) + str[i];
	str.replace(init, i-init+1, res);
}

float operate (float n1, float n2, char oper){
	switch (oper){
		case '+': return n1 + n2;
		case '-': return n1 - n2;
		case '*': return n1 * n2;
		case '/': return n1 / n2;
	}
	return 0;
}

void printAll(mapstring params, string axiom, vrule rules){
	printf("Parameters: \n");
	FORIT(it, params)	
		printf("%s->%s\n", it->fst.c_str(), it->snd.c_str());
	printf("Axiom: %s\n", axiom.c_str());
	printf("Rules: \n");
	FORIT(it, rules)
		printf("%s\n", it->to_string().c_str());	
	
}
