#pragma once
#define pi 3.14159265359
#define PatchDensityL 10
#define PatchDensityB 10
#define PatchSize 4
#include "SB6\vmath.h"

struct wave
{
	float Q; // steep ness of wave dynamic change.
	float Qi; //steepness
	float Ampl; //amplitude
	float Lamda; //wavelenght
	float Speed; //speed
	float Rho; //calculated internally
	float nper;
	float wa;
	vmath::vec3 D; //direction of the wave
	bool toggle; //toggle wave on or off
};

class patch
{
public:
	void Allocate(); // allocates memory to poly and data, called by constructor
	void InitPoly(int scale); // set vector positions and scale them
	void InitWave(wave &wave, float Q, float Ampl, float Lamda, float Speed, float nper, vmath::vec3 D, bool toggle);
	void Animate(float t, int numOfWaves, wave *w);
	int  getPolySize() const			{return polySize;}
	vmath::vec3 polyAvg();
	
	patch()
	{
		Allocate();
	}
	vmath::vec3** dataD;
	vmath::vec3*  polyD;
private:	
	int dataSize; //number of vec3s
	int polySize; //number of vec3s
	void UpdateData(); // assigns memory location of data to poly
	void StitchPoly(); // assigns repeating values of poly to control stitching
	
	//used by patch StitchPoly and InitPoly
	inline bool isSpcColumn(int i, int &R, int &L, int &B)
	{if (R==4) { L++; R=0; if(L>PatchDensityL) { L=1; if(B<PatchDensityB) {B++;}}}return bool(i%4==0);}
	
	//used by patch StitchPoly and InitPoly
	inline bool is4thofRow(int i, int &R, int &L, int &B)
	{if((i+1)%4==0) { R++; return true; } else return false;}

	 // used by InitPoly
	inline void scalePoly(int scale)
	{for(int i=0; i<polySize; i++) {polyD[i][0]*=scale;	polyD[i][1]*=scale;	polyD[i][2]*=scale;	}	}
	
	//used by StitchPoly
	inline void patch::copy( vmath::vec3& to,  const vmath::vec3 from)
	{to[0]=from[0]; to[1]=from[1]; to[2]=from[2];}
	

	
	
	
	
	
};