#include "patch.h"
#include <assert.h>

void patch::InitPoly(int scale = 1)
{
	float	intX		= 0;
	float	intY		= 0;
	float	incriment	= 1.0f/3.0f;
	int		breadth		= 0;
	int		length		= 0;
	int		insert		= 0;
	int		R			= 0;	
	int		L			= 1;	
	int		B			= 1;

	for(int i=0; i<polySize; i++)
		{
			polyD[i][0]=0.0f;
			polyD[i][1]=0.0f;
			polyD[i][2]=0.0f;
		}
		
	for(int i=0; i<polySize; i++)
	{
		if(length == PatchDensityL)
		{
			length = 0;
			breadth++;
		}
		if(isSpcColumn(i, R, L, B))
		{
			if(R == 0)
				intX=1;
			else if(R == 1)
				intX-=incriment;
			else if(R == 2)
				intX-=(incriment);
			else if(R == 3)
				intX=0;
		}
		else
			intY += incriment;
		
		polyD[i][0]=intY+(float)length;
		polyD[i][2]=intX-(float)breadth;
		polyD[i][1]=0;

		if((i+1)%4==0 && R==3)
			length++;
		if(is4thofRow(i, R, L, B))
			intY=0;
	}

	scalePoly(scale);
}
void patch::UpdateData()
{
	int R = 0;
	int L = 1;	
	int B = 1;
	int insert = 0;
	for(int i=0; i<polySize; i++)
	{
		if(i<16)
			dataD[i]=&polyD[i];
		if(isSpcColumn(i, R, L, B) && L>1)
			insert++;
		else if(R==0 && B>1)
			insert++;
		else
			dataD[i- insert]=&polyD[i];

		bool nada = is4thofRow(i, R, L, B);
	}
}
void patch::StitchPoly()
{
	int R = 0; //row, keeps trach of the current row
	int L = 1; 
	int B = 1;
	int insert = 0;
	for(int i=0; i<polySize; i++)
	{
		if(isSpcColumn(i, R, L, B) && L>1)
		{
			copy(polyD[i],polyD[i-13]);
			insert++;
		}
		else if(R==0 && B>1)
		{
			copy(polyD[i],polyD[i- 4 - (16*(PatchDensityL-1))]);
			insert++;
		}
		is4thofRow(i, R, L, B);
	}
}
void patch::Allocate()
{
	dataSize = ( (PatchSize* PatchDensityL)-(PatchDensityL-1)) *  ((PatchSize* PatchDensityB)-(PatchDensityB-1));
	polySize = PatchSize*PatchSize * PatchDensityL * PatchDensityB;
	dataD = new  vmath::vec3*[dataSize];
	polyD = new  vmath::vec3[polySize];
}
double patch::distanceSqr(vmath::vec3 a, vmath::vec3 b)
{
	return (((a[0]-b[0])*(a[0]-b[0])) + ((a[1]-b[1])*(a[1]-b[1])) + ((a[2]-b[2])*(a[2]-b[2])));
}
//this is just a temprary random way to make the distance a more manageable number for tessellation 
int patch::det(double distSq)
{
	int now = ((300000 - distSq)/10000)/3;
	if(now<1)
	{
		now=1;
	}
	//now *=now;
	return now;
}
//used to count control points on the surface
bool patch::is4th(int i, int &R)
{
	if((i+1)%4==0)
	{ 
		R++; 
		return true; 
	} 
	else 
		return false;
	if(R>4)
	{
		R=1;
	}
}
//this function might need to move to the shader, 
//maybe? these vec operations should be fater there.
//because passing all these valus using opengl get uniform is expenssive on string ops
//maybe? organise the data differently so its easy to pass in the shader, and then unpack there.
void patch::LoD(vmath::vec3 camPos)
{
	int R = 1;
	int num = 0;

	vmath::vec3 corners[4];
	vmath::vec3 midPoint[5];
	for (int i = 0; i < polySize; i++)// for each data point
	{
		if(R==1 && is4th(i, R))
		{
			corners[0]=polyD[i-3];
			corners[1]=polyD[i];
		}
		else if(R==4 && is4th(i, R))
		{
			corners[2]=polyD[i-3];
			corners[3]=polyD[i];

			midPoints[num].level[0] = (corners[0]+corners[1])*0.5;
			midPoints[num].level[1] = (corners[1]+corners[3])*0.5;
			midPoints[num].level[2] = (corners[3]+corners[2])*0.5;
			midPoints[num].level[3] = (corners[2]+corners[0])*0.5;

			midPoints[num].level[4] = (midPoints[num].level[0]+midPoints[num].level[1])*0.5;
			num++;
			R=1;
		}
		else
			is4th(i, R);
	}

	for(int i=0; i<PatchDensityL * PatchDensityB; i++)
	{
		detail[i].dist[0] = det( distanceSqr(camPos,midPoints[i].level[0]) ) ;
		detail[i].dist[1] = det( distanceSqr(camPos,midPoints[i].level[1]) ) ;
		detail[i].dist[2] = det( distanceSqr(camPos,midPoints[i].level[2]) ) ;
		detail[i].dist[3] = det( distanceSqr(camPos,midPoints[i].level[3]) ) ;
		detail[i].dist[4] = det( distanceSqr(camPos,midPoints[i].level[4]) ) ;
	}

}
void patch::Animate(float t, int numOfWaves, wave *w)
{
	UpdateData();


	vmath::vec3 pos = vmath::vec3(0.0f, 0.0f, 0.0f);
	vmath::vec3 data= vmath::vec3(0.0f, 0.0f, 0.0f);
	float director	= 0.0f;
	float Rep		= 0.0f;
	float cosRep	= 0.0f;
	float C			= 0.5f;
	float fi		= 0.0f;

	for(int j = 0; j < numOfWaves; j++)//for each wave
		{
		for (int i = 0; i < dataSize; i++)// for each data point
			{
				data = *dataD[i];//storing data in temp
				assert( dataSize-1 != 0);//stop universe imploding
				fi = (float)(i+1)/(float)(dataSize-1);
				director = vmath::dot(w[j].D,data);

				if(w[j].toggle)
				{
					pos = vmath::vec3(data[0],0,data[2]);
					director = vmath::dot(w[j].D,data);
					Rep = fi + ((w[j].wa*director) + (w[j].Rho*t)); 
					cosRep = cos(Rep);

					data[0] += ((w[j].Qi * w[j].Ampl) * w[j].D[0] * cosRep) ;
					data[2] += ((w[j].Qi * w[j].Ampl) * w[j].D[2] * cosRep);
					data[1] +=  w[j].Ampl * sin(((Rep)));
				}

				*dataD[i]=data;// storing temp back to data thus also altering poly
			}
		}
	StitchPoly();
}

void  patch::InitWave(wave &wave, float Q, float Ampl, float Lamda, float Speed, float nper, vmath::vec3 D, bool toggle)
{
	wave.Q		= Q;
	wave.Ampl	= Ampl; 
	wave.Lamda	= Lamda; 
	wave.Speed	= Speed;
	wave.nper	= nper; 
	wave.D		= D; 
	wave.toggle	= toggle;
	assert( wave.Lamda != 0);//stop universe imploding
	wave.Rho	= wave.Speed * ((2*pi)/wave.Lamda); 
	wave.wa = (2.f*pi)/wave.Lamda;
	assert(wave.nper!=0 || wave.Ampl!=0 || wave.wa!=0 );//stop universe imploding
	wave.Qi =wave.Q / ((wave.Ampl*wave.wa)*wave.nper);
}

vmath::vec3 patch::polyAvg()
{
	vmath::vec3 avg = vmath::vec3(0.0f,0.0f,0.0f);
	for(int i=0; i<polySize; i++)
	{
		avg += polyD[i];
	}
	avg /= polySize;

	return avg;
}