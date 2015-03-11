#ifndef INT_H
#define INT_H

#include<iostream>
#include<cstdlib>
#include<cmath>
using namespace std;

/******************************************************
*
*	gauss
*
*
* Gauss-Legendre Quadrature w/ switchable no of points 
* 4 Jun 90, es
********************************************************/

double gauss(int n, double (*f)(double), double xlo, double xhi, void *optvec);

/***************************************************
*
*	gala()
*
* Gauss-Laguerre quadrature extends to +oo
* adapted from old FORTRAN, 24 JUL 90, es, 17 aug 90, 27 Mar 91
****************************************************/
double	gala(int n, double (*f)(double), double xlo, double invslope, void *optvec);
//	int	n;	/* number of points		*/
//	double	(*f)();	/* function to integrate	*/
//	double	xlo;	/* lower limit of integration	*/
/*	double	invslope; approximate inverse slope of decaying 
			   function is roughly proportional to the
			   integration region, needed for placing 
			   the points. i.e. =1 for exp(-x), 
			   invslope=0.5 for exp(-2), etc	*/
//	void	*optvec; /* optional vector, passesd to function f() */




/**************************************************************
*
*	gala?x[], gala?w[]
*
* data for Gauss-Laguerre integration, from Abramowitz, es
***************************************************************/
static	double 
gala4x[] = {	0.322547689619,		1.745761101158,
		4.536620296921,		9.395070912301	};
static	double
gala4w[] = {	0.832739123838,		2.04810243845,
		3.63114630582,		6.48714508441	};
static	double
gala8x[] = {	0.170279632305,		0.903701776799,
		2.251086629866,		4.266700170288,
		7.045905402393,		10.758516010181,
		15.740678641278,	22.863131736889	};
static	double
gala8w[] = {	0.437723410493,		1.03386934767,
		1.66970976566,		2.37692470176,
		3.20854091335,		4.26857551083,
		5.81808336867,		8.90622621529	};
static	double
gala12x[] = {	0.115722117358,		0.611757484515,
		1.512610269776,		2.833751337744,
		4.599227639418,		6.844525453115,
		9.621316842457,		13.006054993306,
		17.116855187462,	22.151090379397,
		28.487967250984,	37.099121044467	};
static	double
gala12w[] = {	0.297209636044,		0.696462980431,
		1.10778139462,		1.53846423904,
		1.99832760627,		2.50074576910,
		3.06532151828,		3.72328911078,
		4.52981402998,		5.59725846184,
		7.21299546093,		10.5438374619	};

static	double
gala15x[] = {	0.093307812017,         0.492691740302,
		1.215595412071,         2.269949526204,
		3.667622721751,         5.425336627414,
		7.565916226613,        10.120228568019, 
	       13.130282482176,        16.654407708330,
               20.776478899449,        25.623894226729,
               31.407519169754,        38.530683306486,
	       48.026085572686	};
static	double
gala15w[] = {	0.239578170311,         0.560100842793,
		0.887008262919,         1.22366440215,
		1.57444872163,          1.94475197653,
		2.34150205664,          2.77404192683,
		3.25564334640,          3.80631171423,
                4.45847775384,          5.27001778443,
                6.35956346973,          8.03178763212,
	       11.5277721009   };

/**************************************************************
*
*	gahe?x[], gahe?w[]
*
* data for Gauss-Hermite integration, from Abramowitz, es
***************************************************************/
static	double	gahep4[]  = {	0.524647623275290,	1.650680123885785 };
static	double	gahew4[]  = {	1.0599644828950,	1.2402258176958 };
static	double	gahep8[]  = {	0.381186990207322,	1.157193712446780,
				1.981656756695843,	2.930637420257244 };
static	double	gahew8[]  = {	0.7645441286517,	0.7928900483864,
				0.8667526065634,	1.0719301442480 };
static	double	gahep16[] = {	0.27348104613815,	0.82295144914466,
				1.38025853919888,	1.95178799091625,
				2.54620215784748,	3.17699916197996,
				3.86944790486012,	4.68873893930582 };
static	double	gahew16[] = {	0.5473752050378,	0.5524419573675,
				0.5632178290882,	0.5812472754009,
				0.6097369582560,	0.6557556728761,
				0.7382456222777,	0.9368744928841 };
static	double	gahep20[] = {	0.2453407083009,	0.7374737285454,
				1.2340762153953,	1.7385377121166,
				2.2549740020893,	2.7888060584281,
				3.3478545673832,	3.9447640401156, 
                                4.6036824495507,        5.3874808900112};
static	double	gahew20[] = {	0.4909215006677,	0.4938433852721,
				0.4999208713363,	0.5096790271175,
				0.5240803509486,        0.5448517423644,
                        	0.5752624428525,        0.6222786961914,
				0.7043329611769,        0.8985919614532 };

			
/****************************************************************
*
*	gaule?x[], gaule?w[]
*
* data for Gauss-Legendre integration, Abramowitz and other, es
*****************************************************************/

static	double	gaulep4[] = {	0.8611363115,	0.3399810435	};
static	double	gaulew4[] = {	0.3478548451,	0.6521451548	};
static	double	gaulep8[] = {	0.9602898564,	0.7966664774,
				0.5255324099,	0.1834346424	};
static	double	gaulew8[] = {	0.1012285362,	0.2223810344,
				0.3137066458,	0.3626837833	};
static	double	gaulep10[] = {	0.1488743389,	0.4333953941, 
				0.6794095682,	0.8650633666,
						0.97390652	};
static	double	gaulew10[] = {	0.2955242247,	0.2692667193,
				0.2190863625,	0.1494513491,
						0.06667134	};
static	double	gaulep12[] = {	0.9815606342,	0.9041172563,
				0.7699026741,	0.5873179542,
				0.3678314989,	0.1252334085	};
static	double	gaulew12[] = {	0.0471753363,	0.1069393259,
				0.1600783285,	0.2031674267,
				0.2334925365,	0.2491470458	};
static	double	gaulep16[] = {
		0.989400934991650,	0.944575023073233,
		0.865631202387832,	0.755404408355003,
		0.617876244402644,	0.458016777657227,
		0.281603550779259,	0.095012509837637	};
static	double	gaulew16[] = {
		0.027152459411754,	0.062253523938648,
		0.095158511682493,	0.124628971255534,
		0.149595988816577,	0.169156519395003,
		0.182603415044924,	0.189450610455069	};
static	double	gaulep20[] = {
		0.993128599185094,	0.963971927277913,
		0.912234428251325,	0.839116971822218,
		0.746331906460150,	0.636053680726515,
		0.510867001950827,	0.373706088715419,
		0.227785851141645,	0.076526521133497	};
static	double	gaulew20[] = {
		0.017614007139152,	0.040601429800386,
		0.062672048334109,	0.083276741576704,
		0.101930119817240,	0.118194531961518,
		0.131688638449176,	0.142096109318382,
		0.149172986472603,	0.152753387130725	};
static	double	gaulep48[] = {
		0.998771007252426118601,	0.993530172266350757548,
		0.984124583722826857745,	0.970591592546247250461,
		0.952987703160430860723,	0.931386690706554333114,
		0.905879136715569672822,	0.876572020274247885906,
		0.843588261624393530711,	0.807066204029442627083,
		0.767159032515740339254,	0.724034130923814654674,
		0.677872379632663905212,	0.628867396776513623995,
		0.577224726083972703818,	0.523160974722233033678,
		0.466902904750958404545,	0.408686481990716729916,
		0.348755886292160738160,	0.287362487355455576736,
		0.224763790394689061225,	0.161222356068891718056,
		0.097004699209462698930,	0.032380170962869362033 };
static	double	gaulew48[] = {
		0.003153346052305838633,	0.007327553901276262102,
		0.011477234579234539490,	0.015579315722943848728,
		0.019616160457355527814,	0.023570760839324379141,
		0.027426509708356948200,	0.031167227832798088902,
		0.034777222564770438893,	0.038241351065830706317,
		0.041545082943464749214,	0.044674560856694280419,
		0.047616658492490474826,	0.050359035553854474958,
		0.052890189485193667096,	0.055199503699984162868,
		0.057277292100403215705,	0.059114839698395635746,
		0.060704439165893880053,	0.062039423159892663904,
		0.063114192286254025657,	0.063924238584648186624,
		0.064466164435950082207,	0.064737696812683922503 };




#endif