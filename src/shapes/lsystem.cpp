
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */


// shapes/lsystem.cpp*
#include "stdafx.h"
#include "shapes/lsystem.h"
#include "shapes/processGFile.h"
#include "cmath"
#include "ctime"

#define REP(i,n) for(int i=0;i<n;i++)
#define fst first
#define snd second
#define pb push_back

#define ANGLE 90.0

// LSystem Method Definitions
LSystem::LSystem(string gFile, int n){
	parseGFile(gFile, n, result, constants);
	createAll();
}

LSystem *CreateLSystemShape(const ParamSet &params) {
    string gFile = params.FindOneString("grammarFile", 
										"lsystems/grammar1.in");
   	int nrep = params.FindOneInt("nrep", 1);
    new LSystem(gFile, nrep);
	return NULL;
}

void LSystem::createAll(){
	int i = 0, j = 0, l = result.length();
	string func, aux;
	while(i < l){
		if (constants.count(result.substr(i, 1))){
			if(result[i+1] == '('){
				j = result.find(')', i + 1);
				func = result.substr(i, j - i);//skip ) char
				i = j + 1;
			}else
				func = result[i++];
			createShape(func);
		}else if (result[i] == '['){
			pbrtAttributeBegin();	
			i++;
		}else if (result[i] == ']'){
			pbrtAttributeEnd();	
			i++;
		}else{
			createTransformation(result[i++]);
		}
	}

}

void LSystem::createShape(string str){	
	ParamSet p;
	vector<string> params;

	if (str.size() > 1)
		split(str.substr(2), ',', params);
	
	switch(str[0]){
		case 'F':
		case 'W':
			createBranch(str.substr(0,1), params, p);
			break;
		case 'Q':
			createLeaf(str.substr(0,1), params);
			break;
	}
}

void LSystem::createBranch(string str, vector<string> params, ParamSet p){
	//Cylinder params default
	float r = (params.size() >= 2) ? 
		atof(params[1].c_str()) : 0.1;
	float zmin = 0.0;
	float zmax = (params.size() >= 1) ? 
		atof(params[0].c_str()) : 1;
	p.AddFloat(string("radius"), &r, 1);
	p.AddFloat(string("zmin"), &zmin, 1);
	p.AddFloat(string("zmax"), &zmax, 1);
	
	pbrtShape(constants[str], p);
	
	float ratio = constants["r"] != "" ? 
		atof(constants["r"].c_str()) : 1;
	pbrtTranslate(0.0, 0.0, zmax * ratio);
}

void LSystem::createLeaf(string str, vector<string> params){
	//Leaf params default
	float size = (params.size() >= 1) ? 
		atof(params[0].c_str()) : 1; 
	float r = (params.size() >= 2) ?
		atof(params[1].c_str()) * 1.9 : 0.1*1.9;
	pbrtAttributeBegin();
	pbrtScale(size, size, size);
	pbrtRotate(ANGLE, 0.0, 0.0, 1.0);
	pbrtTranslate(r, 0.0, 0.0);
	pbrtObjectInstance(constants[str]);
	pbrtAttributeEnd();
}

void LSystem::createTransformation(char c){
	float angle = (constants["a"] != "") ?
		atof(constants["a"].c_str()) : 90,
		zmax = 1;

	switch(c){
		case '+':
			pbrtRotate(angle, 0.0, 1.0, 0.0);
			break;
		case '-':
			pbrtRotate(-angle, 0.0, 1.0, 0.0);
			break;		
		case '&':
			pbrtRotate(angle, 1.0, 0.0, 0.0);
			break;
		case '^':
			pbrtRotate(-angle, 1.0, 0.0, 0.0);
			break;
		case '\\':
			pbrtRotate(angle, 0.0, 0.0, 1.0);
			break;
		case '/':
			pbrtRotate(-angle, 0.0, 0.0, 1.0);
			break;
		case '|':
			pbrtRotate(180.0, 0.0, 1.0, 0.0);
			break;
		case 'f':
			pbrtTranslate(0.0, 0.0, zmax);
			break;
	}
}
