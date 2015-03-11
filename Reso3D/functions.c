/* functions.c            functions for reso.c main program calculations 
 *   Josef Sollfrank                  Nov. .98             */

// Commented and adjusted by Evan Frodermann, Aug 2005

#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#include    <stdlib.h>
#include    <assert.h>
#include	"reso.h"
#include	"functions.h" 
#include	"tools.h" 
#define     max(a,b) (a)>(b) ? (a) : (b)
//#include        <gsl/gsl_sf_bessel.h>

#define         PTCHANGE    1.0

// double thermalspectra(double pt, double T, double mass, double mu, double degen);

//*******************************************************************************
// The following arrays are set for specific integration routines and point with 
// weights for those routines

static	double	gaus2x[] = { 0.577350269189626 };
static	double	gaus4x[] = {	0.8611363115,	0.3399810435	};
static	double	gaus8x[] = {	0.9602898564,	0.7966664774,
    0.3137066458,	0.3626837833	};
static	double	gaus10x[] = {	0.1488743389,	0.4333953941, 
    0.6794095682,	0.8650633666,
    0.97390652	};
static	double	gaus12x[] = {	0.9815606342,	0.9041172563,
    0.7699026741,	0.5873179542,
    0.3678314989,	0.1252334085	};
static	double	gaus16x[] = {
    0.989400934991650,	0.944575023073233,
    0.865631202387832,	0.755404408355003,
    0.617876244402644,	0.458016777657227,
    0.281603550779259,	0.095012509837637	};
static	double	gaus20x[] = {
    0.993128599185094,	0.963971927277913,
    0.912234428251325,	0.839116971822218,
    0.746331906460150,	0.636053680726515,
    0.510867001950827,	0.373706088715419,
    0.227785851141645,	0.076526521133497	};
static	double	gaus48x[] = {
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

static	double
gala4x[] = {	0.322547689619,		1.745761101158,
    4.536620296921,		9.395070912301	};

static	double
gala8x[] = {	0.170279632305,		0.903701776799,
    2.251086629866,		4.266700170288,
    7.045905402393,		10.758516010181,
    15.740678641278,	22.863131736889	};
static	double
gala12x[] = {	0.115722117358,		0.611757484515,
    1.512610269776,		2.833751337744,
    4.599227639418,		6.844525453115,
    9.621316842457,		13.006054993306,
    17.116855187462,	22.151090379397,
    28.487967250984,	37.099121044467	};

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
gala48x[] = { 2.9811235829960e-02,   0.15710799061788,
    0.38626503757646,   0.71757469411697,
    1.1513938340264,    1.6881858234190,
    2.3285270066532,    3.0731108616526,
    3.9227524130465,    4.8783933559213,
    5.9411080546246,    7.1121105358907,
    8.3927625990912,    9.7845831846873,
    11.289259168010,    12.908657778286,
    14.644840883210,    16.500081428965,
    18.476882386874,    20.577998634022,
    22.806462290521,    25.165612156439,
    27.659128044481,    30.291071001009,
    33.065930662499,    35.988681327479,
    39.064848764198,    42.300590362903,
    45.702792038511,    49.279186382837,
    53.038498087817,    56.990624814804,
    61.146864786140,    65.520206929019,
    70.125706236113,    74.980977518911,
    80.106857350324,    85.528311116034,
    91.275707993668,    97.386667713582,
    103.908833357176,    110.90422088498,
    118.45642504628,    126.68342576889,
    135.76258957786,    145.98643270946,
    157.91561202298,    172.99632814856 };
//*********************************************************************************




/**************************************************************************
 *									  *
 *   readin() 								  *
 *									  *
 *   reads in the particle data file and stores the datas in the arrays	  *
 *   particle.* and decay.* and fills up the rest of data (antibaryons)	  *
 ***********************************************************************   *
 **************************************************************************/

void   readin(filename, particlemax, decaymax)

    char filename[FILEDIM];
    int *particlemax, *decaymax;

{
    int i=0, j=0, k, h;
    FILE	*dat;
    double dummy1;

    for(k=0;k<MAXINTV;k++) partid[k] = -1; 

    //sprintf(filename,"EOS/resoweak.dat");
    sprintf(filename,"../s95p-v1/pdg05.dat");
    dat = fopen(filename,"r");
    if(dat == NULL){
        printf(" NO file: %s  available ! \n", filename);
        printf(" GOOD BYE AND HAVE A NICE DAY! \n");
        exit(0);
    }
    // Read in the particle data from the specified resonance table 
    // Save the data is the structure particle[pn]

    while(fscanf(dat,"%i%s%lf%lf%i%i%i%i%i%i%i%i", &particle[i].monval,
                particle[i].name, &particle[i].mass, &particle[i].width,
                &particle[i].gspin, &particle[i].baryon,
                &particle[i].strange, &particle[i].charm,
                &particle[i].bottom, &particle[i].gisospin,
                &particle[i].charge, &particle[i].decays)==12)
    { 

        // Read in the unused portion of the data file 
        // fscanf(dat,"%lf%lf%lf", &dummy1, &dummy1, &dummy1); //Not used in ../s95p-v1/pdg05.dat
        printf("monval=%d\n",particle[i].monval);
        assert(MHALF+particle[i].monval<MAXINTV);
        partid[MHALF + particle[i].monval] = i;
        particle[i].stable = 0;
        if(particle[i].width < 1.0E-8)particle[i].stable = 1;

        printf("MHALF+monval=%d\n",MHALF+particle[i].monval);

        //printf("%i   %i %s %lf %lf %i %i %i %i %i %i %i %i\n", i, particle[i].monval,
        //	      particle[i].name, particle[i].mass, particle[i].width,
        //	      particle[i].gspin, particle[i].baryon,
        //	      particle[i].strange, particle[i].charm,
        //	      particle[i].bottom, particle[i].gisospin,
        //	   particle[i].charge, particle[i].decays);


        /* read in the decays */
        // These decays are saved in a seperate data set, decay[i].
        for(k=0;k<particle[i].decays;k++) 
        {
            h=fscanf(dat,"%i%i%lf%i%i%i%i%i",
                    &decay[j].reso, &decay[j].numpart, &decay[j].branch, 
                    &decay[j].part[0], &decay[j].part[1], &decay[j].part[2],
                    &decay[j].part[3], &decay[j].part[4]);
            //  printf("      %i %i %lf %i %i %i %i %i\n", decay[j].reso,
            //	      decay[j].numpart, decay[j].branch, decay[j].part[0],
            //	      decay[j].part[1], decay[j].part[2],decay[j].part[3],
            //      decay[j].part[4]);

            if (h != 8) {
                printf("Error in scanf decay \n");
                printf(" GOOD BYE AND HAVE A NICE DAY! \n");
                exit(0);
            }

            //if (decay[j].numpart == 1) particle[i].stable = 1; /* Modified on 03/10/2011

            j++; // Add one to the decay counting variable "j"
            printf("j=%d\n",j);
        }
        //printf("\n");

        /* setting of additional parameters */

        if (particle[i].baryon == 1)
        {
            i++;// If the particle is a baryon, add a particle for the anti-baryon
            // Add one to the counting variable "i" for the number of particles for the antibaryon
            particle[i].monval = -particle[i-1].monval;
            strcpy(particle[i].name, "  Anti-");
            strncat(particle[i].name, particle[i-1].name, 18);
            particle[i].mass     =  particle[i-1].mass;
            particle[i].width    =  particle[i-1].width;
            particle[i].gspin    =  particle[i-1].gspin;
            particle[i].baryon   = -particle[i-1].baryon;
            particle[i].strange  = -particle[i-1].strange;
            particle[i].charm    = -particle[i-1].charm;
            particle[i].bottom   = -particle[i-1].bottom;
            particle[i].gisospin =  particle[i-1].gisospin;
            particle[i].charge   = -particle[i-1].charge;
            particle[i].decays   = particle[i-1].decays;
            partid[MHALF + particle[i].monval] = i;
            particle[i].stable =  particle[i-1].stable;
        }
        i++; // Add one to the counting variable "i" for the meson/baryon
    }
    fclose(dat);

    *particlemax = i;   // Set the maxparticle variable to be the value of "i"
    *decaymax  = j;     // Set the maxdecays variable to be the value of "j"

    printf("particlemax=%d\n",i);
    printf("decaymax=%d\n",j);



    //printf("last particle: %i %s \n",particle[i-1].monval,
    //                  particle[i-1].name);
    //printf("# particle %5i; # decays %5i; \n\n",i,j);
    if((*particlemax) > NUMPARTICLE){
        printf("Array for particles to small!!\n");
        printf(" GOOD BYE AND HAVE A NICE DAY! \n");
        exit(0);
    } 

}

//*******************************************************************************************************
// This function reads in the dN/d3p spectra calculated by spectra via the HYDRO output.  Set the pt
// points with the gaussian values given above.  These are the same values given to the spectra in the 
// azspectra0p0 program. The default filename is phipspectra.dat

void   readspec(infile, specfile, particlemax, decaymax)

    char  infile[FILEDIM];
    char  specfile[FILEDIM];
    int *particlemax, *decaymax;
{

    FILE    *dat;
    FILE    *spec;
    char    resofile[FILEDIM];
    int i, j, k, pn, npa = 0;
    int mon, nphi, npt, nry;
    double slope, dum, dum1, dum2;
    /*dat = fopen(infile,"r");               //numbers.dat
      if(dat == NULL){
      printf(" NO file: %s  available ! \n", infile);
      printf(" GOOD BYE AND HAVE A NICE DAY! \n");
      exit(0);
      }
     */
    slope=12.0;
    nry=41;
    npt=15;
    nphi=48;


    /*
       spec = fopen(specfile,"r");
       if(spec == NULL){
       printf(" NO file: %s  available ! \n", specfile);
       printf(" GOOD BYE AND HAVE A NICE DAY! \n");
       exit(0);
       }
     */
    //fscanf(dat,"%s",resofile);        //resofile=resoweak.dat
    readin(resofile, particlemax, decaymax);
    printf("readin successful");
    //while( fscanf(dat,"%i%lf%i%i%i",&mon, &slope, &nry, &npt, &nphi) == 5){

    int pari;
    for(pari=0; pari<(*particlemax); pari++){
        mon=particle[pari].monval;
        npa++;
        pn = partid[MHALF + mon];
        //printf(" %i %i %lf %i %i %lf %lf \n",npa, mon,  slope, npt, nphi, 
        //        dum1, dum2);
        if(pn == -1){
            printf(" particle %i not in reso table ! \n",mon);
            printf(" GOOD BYE AND HAVE A NICE DAY! \n");
            exit(0);
        }
        if(slope <= 0.0){
            printf(" slope = 0 !\n",mon);
            printf(" GOOD BYE AND HAVE A NICE DAY! \n");
            exit(0);
        }
        if(npt > NPT){
            printf(" NPT = %i array to small !\n", npt);
            printf(" GOOD BYE AND HAVE A NICE DAY! \n");
            exit(0);
        }
        if(nphi > NPHI){
            printf(" NPHI = %i array to small !\n", nphi);
            printf(" GOOD BYE AND HAVE A NICE DAY! \n");
            exit(0);
        }
        switch (npt) {
            case 4:
                for(i=0;i<4;i++)
                    particle[pn].pt[i] =  gala4x[i]/slope; 
                break;
            case 8:
                for(i=0;i<8;i++)
                    particle[pn].pt[i] =  gala8x[i]/slope; 
                break;
            case 12:
                for(i=0;i<12;i++)
                    particle[pn].pt[i] =  gala12x[i]/slope; 
                break;
            case 15:
                for(i=0;i<15;i++)
                    particle[pn].pt[i] =  gala15x[i]/slope; 
                break;
            case 48:
                for(i=0;i<48;i++)
                    particle[pn].pt[i] =  gala48x[i]/slope; 
                break;
            default:
                printf(" No abscissas for npt = %i !\n",npt);
                printf(" GOOD BYE AND HAVE A NICE DAY! \n");
                exit(0);
        }
        switch (nphi) {
            case 2:
                for(i=0;i<1;i++){
                    PHI[1-i] = PI*(gaus2x[i] + 1.0);
                    PHI[i] = PI*(1.0 - gaus2x[i]);
                }
                break;
            case 4:
                for(i=0;i<2;i++){
                    PHI[3-i] = PI*(gaus4x[i] + 1.0);
                    PHI[i] = PI*(1.0 - gaus4x[i]);
                }
                break;
            case 8:
                for(i=0;i<4;i++){
                    PHI[7-i] = PI*(gaus8x[i] + 1.0);
                    PHI[i] = PI*(1.0 - gaus8x[i]);
                }
                break;
            case 10:
                for(i=0;i<5;i++){
                    PHI[9-i] = PI*(gaus10x[i] + 1.0);
                    PHI[i] = PI*(1.0 - gaus10x[i]);
                }
                break;
            case 12:
                for(i=0;i<6;i++){
                    PHI[11-i] = PI*(gaus12x[i] + 1.0);
                    PHI[i] = PI*(1.0 - gaus12x[i]);
                }
                break;
            case 16:
                for(i=0;i<8;i++){
                    PHI[15-i] = PI*(gaus16x[i] + 1.0);
                    PHI[i] = PI*(1.0 - gaus16x[i]);
                }
                break;
            case 20:
                for(i=0;i<10;i++){
                    PHI[19-i] = PI*(gaus20x[i] + 1.0);
                    PHI[i] = PI*(1.0 - gaus20x[i]);
                }
                break;
            case 48:
                for(i=0;i<24;i++){
                    PHI[47-i] = PI*(gaus48x[i] + 1.0);
                    PHI[i] = PI*(1.0 - gaus48x[i]);
                }
                break;
            default:
                printf(" No abscissas for nPhi = %i !\n",nphi);
                printf(" GOOD BYE AND HAVE A NICE DAY! \n");
                exit(0);
        }

        double ry_max=8.0;
        double ry_min=-8.0;
        double dry=(ry_max-ry_min)/(nry-1.0);
        for(i=0; i<nry; i++){
            RY[i] = ry_min+i*dry;
        }

        particle[pn].slope[npt] = slope;
        particle[pn].nphi = nphi;
        particle[pn].npt = npt;
        particle[pn].nry = nry;
        double g=particle[pn].gspin;

        if(mon>=0) sprintf(specfile,"../results/event%d/dNdYPtdPtdPhi_%d.dat",IEVENT,abs(mon));
        else if(mon<0) sprintf(specfile,"../results/event%d/dNdYPtdPtdPhi_%d.dat",IEVENT,abs(mon));
        //Anti particle has the same spectra; doing this will reduce the freeze out time
        //else if(mon<0) sprintf(specfile,"../results/event%d/dNdYPtdPtdPhi_A%d.dat",IEVENT,abs(mon));
        spec = fopen(specfile,"r");
        if(spec == NULL){
            printf(" NO file: %s  available ! \n", specfile);
            printf(" GOOD BYE AND HAVE A NICE DAY! \n");
            exit(0);
        }

        for(i=0;i<nry; i++)
            for(j=0;j<npt;j++){
                for(k=0;k<nphi;k++){
                    fscanf(spec,"%lf",&dum);
                    particle[pn].dNdyptdptdphi[i][j][k] = dum; 
                    //particle[pn].dNdyptdptdphi[i][j][k] = max(dum, 1.0E-15); //For test 04/04/2011
                    //particle[pn].dNdptdphi[j][k]= thermalspectra(particle[pn].pt[j], 0.200, particle[pn].mass, 
                    //0.200, particle[pn].gspin);
                }
            }

        /* For controll! 
           for(j=0;j<npt;j++){
           for(k=0;k<nphi;k++){
           printf(" pt:%15.8lf phi:%15.8lf dN/dptdphi: %15.8le \n",
           particle[pn].pt[j], PHI[k], particle[pn].dNdptdphi[j][k]);
           }
           } */
        fclose(spec);
    }
    //fclose(dat);
    if(npa > 0)
        printf(" Successful read in of %5i spectra !\n",npa);
}

//***********************************************************************************************************
// After calculating the spectra in the decay routines, this routine writes that data to file.  The spectra is written
// to spec_###.dat in block format for the pt/phi dependence.  The pt values for each point are saved to specPT_###.dat.
// The ### corresponds to the monte carlo value number assigned the resonance table.  The phi data was already stored in 
// angle.dat. (by default)

void   writespec(particlemax, outdir)

    int particlemax;
    char outdir[FILEDIM];

{

    FILE  *out, *out2;
    char filename[FILEDIM];
    char filename2[FILEDIM];

    char p[SSL];
    int i, j, k;

    sprintf(outdir,"../results/event%d",IEVENT);
    for(i=1;i<particlemax;i++)// Cycle through the particles
    {                          
        if(particle[i].stable == 1) //Only print out the stable particles
        {
            strcpy(filename,outdir);
            strcpy(filename2,outdir);
            strcat(filename, "/spec_");
            strcat(filename2, "/PT_");
            if(particle[i].monval < 0) 
            {
                strcat(filename,"A");
                strcat(filename2, "A");
            }

            convei(abs(particle[i].monval),p);

            strcat(filename,p);
            strcat(filename2,p);
            strcat(filename,".dat");
            strcat(filename2, ".dat");

            printf(" Produce %s \n", filename);
            printf(" Produce %s \n", filename2);
            out = fopen(filename,"w");
            out2 = fopen(filename2, "w");
            int h;
            for(h=0;h<particle[i].nry;h++)
                for(j=0;j<particle[i].npt;j++)
                {
                    for(k=0;k<particle[i].nphi;k++)//Print out the desired data.
                    {
                        fprintf(out," %11.4lE", particle[i].dNdyptdptdphi[h][j][k]);
                    }
                    fprintf(out,"\n");
                }



            for(j=0;j<particle[i].npt;j++)
            {
                fprintf(out2," %11.4lE", particle[i].pt[j]);
            }
            fclose(out);
            fclose(out2);



            /*Add to calc spctral dNdEta, dNdPt, dNdPhi, v2 */
        }
    }
}





/*************************************************
 *
 *	Edndp3
 *
 * 
 **************************************************/
// This function interpolates the needed spectra for a given pt and phi.

double	Edndp3(double yr, double ptr, double phirin, int res_num)

    /* supersedes during test the right one */
//    double	yr;		/* y  of resonance */
//    double	ptr;		/* pt of resonance */
//    double	phirin;		/* phi angle  of resonance */
//    int	res_num;	/* Montecarlo number of resonance 	*/

{
    double	phir, val;
    double        f1, f2;
    int     	pn, nry, npt, nphi;

    if(phirin < 0.0){
        printf("ERROR: phir %15.8le < 0 !!! \n", phirin);exit(0);}
    if(phirin > 2.0*PI){
        printf("ERROR: phir %15.8le > 2PI !!! \n", phirin);exit(0);}
    else phir = phirin;

    //if(phirin < 0.5*PI) phir = phirin;
    //else{
    //    if(phirin < PI) phir = PI - phirin;
    //    else{
    //        if(phirin < 1.5*PI) phir = phirin - PI;
    //        else phir = 2.0*PI - phirin;
    //    }
    //}// phi integral must be changed from 0-pi/2 to 0-2pi

    pn = partid[MHALF + res_num];

    if(yr>=RY[0] && yr<=RY[NRY-1]){
        /*For rapidity in the range ,do linear interpolation */
        nry = 1; 
        while((yr > RY[nry])&&(nry<(particle[pn].nry-1))) nry++; 
        nphi = 1; 
        while((phir > PHI[nphi])&&(nphi<(particle[pn].nphi-1))) nphi++; 
        npt = 1; 
        while((ptr > particle[pn].pt[npt]) &&
                (npt<(particle[pn].npt - 1))) npt++; 

        //    if(yr<RY[0]){
        //         yr=RY[0];
        //     }
        //    else if(yr>RY[NRY-1])yr=RY[NRY-1];

        /* phi and rapidity interpolation */

        double fy1, fy2;

        fy1 = lin_int(PHI[nphi-1], PHI[nphi], 
                particle[pn].dNdyptdptdphi[nry-1][npt-1][nphi-1], 
                particle[pn].dNdyptdptdphi[nry-1][npt-1][nphi], phir);
        fy2 = lin_int(PHI[nphi-1], PHI[nphi], 
                particle[pn].dNdyptdptdphi[nry][npt-1][nphi-1], 
                particle[pn].dNdyptdptdphi[nry][npt-1][nphi], phir);

        f1 = lin_int(RY[nry-1], RY[nry], fy1, fy2, yr);

        fy1 = lin_int(PHI[nphi-1], PHI[nphi], 
                particle[pn].dNdyptdptdphi[nry-1][npt][nphi-1], 
                particle[pn].dNdyptdptdphi[nry-1][npt][nphi], phir);
        fy2 = lin_int(PHI[nphi-1], PHI[nphi], 
                particle[pn].dNdyptdptdphi[nry][npt][nphi-1], 
                particle[pn].dNdyptdptdphi[nry][npt][nphi], phir);

        f2 = lin_int(RY[nry-1], RY[nry], fy1, fy2, yr);

        if(ptr > PTCHANGE){
            f1 = (f1>1.0E-300)?f1:1.0E-300;
            f2 = (f2>1.0E-300)?f2:1.0E-300;
            f1 = log(f1); 
            f2 = log(f2);
        }
        /* pt interpolation */
        val = lin_int(particle[pn].pt[npt-1],particle[pn].pt[npt], f1, f2, ptr);
        if(ptr > PTCHANGE)
            val = exp(val);
        val = (val>1.0E-300)?val:1.0E-300;
    }

    else {
        /*For rapidity out the range ,do exponential extrapolation */
        int hy0, hy1;
        if(yr<RY[0]){ 
            hy0 = 0;
            hy1 = 1;
        }
        else if(yr>RY[NRY-1])
        {
            hy0 = NRY-1;
            hy1 = NRY-2;
        }

        nphi = 1; 
        while((phir > PHI[nphi])&&(nphi<(particle[pn].nphi-1))) nphi++; 
        npt = 1; 
        while((ptr > particle[pn].pt[npt]) &&
                (npt<(particle[pn].npt - 1))) npt++; 

        /* phi and rapidity interpolation */

        double fy1, fy2;

        fy1 = lin_int(PHI[nphi-1], PHI[nphi], 
                particle[pn].dNdyptdptdphi[hy1][npt-1][nphi-1], 
                particle[pn].dNdyptdptdphi[hy1][npt-1][nphi], phir);
        fy2 = lin_int(PHI[nphi-1], PHI[nphi], 
                particle[pn].dNdyptdptdphi[hy0][npt-1][nphi-1], 
                particle[pn].dNdyptdptdphi[hy0][npt-1][nphi], phir);

        f1 = exp(lin_int(RY[hy1], RY[hy0], log(fy1), log(fy2), yr));

        fy1 = lin_int(PHI[nphi-1], PHI[nphi], 
                particle[pn].dNdyptdptdphi[hy1][npt][nphi-1], 
                particle[pn].dNdyptdptdphi[hy1][npt][nphi], phir);
        fy2 = lin_int(PHI[nphi-1], PHI[nphi], 
                particle[pn].dNdyptdptdphi[hy0][npt][nphi-1], 
                particle[pn].dNdyptdptdphi[hy0][npt][nphi], phir);

        f2 = exp(lin_int(RY[hy1], RY[hy0], log(fy1), log(fy2), yr));

        if(ptr > PTCHANGE){
            f1 = (f1>1.0E-300)?f1:1.0E-300;
            f2 = (f2>1.0E-300)?f2:1.0E-300;
            f1 = log(f1); 
            f2 = log(f2);
        }
        /* pt interpolation */
        val = lin_int(particle[pn].pt[npt-1],particle[pn].pt[npt], f1, f2, ptr);
        if(ptr > PTCHANGE)
            val = exp(val);
        val = (val>1.0E-300)?val:1.0E-300;
    }


    /*
       printf(" nphi  %i npt %i \n", nphi,npt);
       printf(" f1  %15.8le %15.8le  \n", f1, f2);
       printf(" phi  %15.8lf %15.8lf  \n", PHI[nphi-1], PHI[nphi]); 
       printf(" pt   %15.8lf %15.8lf  \n", particle[pn].pt[npt-1],particle[pn].pt[npt]);
       printf(" phi  %15.8lf pt %15.8lf    val %15.8lf \n", phir, ptr,val); 
       printf(" phi %15.8le %15.8le \n",particle[pn].dNdptdphi[npt][nphi-1],
       particle[pn].dNdptdphi[npt][nphi]);
       printf(" pt  %15.8le %15.8le \n",particle[pn].dNdptdphi[npt-1][nphi-1],
       particle[pn].dNdptdphi[npt-1][nphi]);

       exit(0);
     */
    return val;
}


//*****************************************************************************************************
/* // Requires either a) A definition of the K bessel function
// or b) the GSL (Gnu Scientific Library) installed.  Add the flags -lgsl -lgslcblas to the compilation
// to use the GSL library.

// Thermal function that simulates most spectra given a finte temperature.  Replaced 
// by the output of hydro in the real simulation

double thermalspectra(double pt, double T, double mass, double mu, double degen)
{

double spect = 0.0;
double mT = sqrt(pt*pt + mass * mass);
spect = degen/(4*M_PI*M_PI)*exp(mu/T)*mT *gsl_sf_bessel_K1(mT/T);

return spect;
}
 */
