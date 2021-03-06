#if defined(CONFIGT) && defined(MC)
/* ======================================================================== */
/* eurey2_mc.cpp                                                            */
/*                                                                          */
/* Written by Thomas A. Knotts IV, 1 Sep 2005                               */
/* ======================================================================== */
#ifndef NEUTRAL
#include "defines.h"

/* ************************************************************************ */
/* ======================== Begin Subroutine ============================== */
/* ************************************************************************ */

double eurey2_mc (int ibox)
{

  int k = ibox;
  /* ================================================================== */
  /* Zero out the energy accumulator                                    */
  /* ================================================================== */
  double eurey = 0.0;

  /* ================================================================== */
  /* Zero out the virial accumulators                                   */
  /* ================================================================== */
#ifdef PRESSURE
  double wxx = 0.0;
  double wyx = 0.0;
  double wyy = 0.0;
  double wzx = 0.0;
  double wzy = 0.0;
  double wzz = 0.0;
#endif

  double lx = box[k].lx;
  double ly = box[k].ly;
  double lz = box[k].lz;
  double hx = box[k].hx;
  double hy = box[k].hy;
  double hz = box[k].hz;
  /* ================================================================== */
  /* Calculate the energy and force associated with each bond.  The     */
  /* loop is over the number of bonds.                                  */
  /* ================================================================== */
  for(int i=0; i<ureyN[k]; i++) {
  /* ----------------------------------------- */
  /* Assign the atoms involved in the bonding. */
  /* ----------------------------------------- */
	int ia = urey[k][i].a;
    int ib = urey[k][i].b;

  /* ----------------------------------------- */
  /* Calculate the distances between the atoms */
  /* involved in the bonding.                  */
  /* ----------------------------------------- */
    double xab = atom[k][ib].x - atom[k][ia].x;
    double yab = atom[k][ib].y - atom[k][ia].y;
    double zab = atom[k][ib].z - atom[k][ia].z;

  /* ----------------------------------------- */
  /* Apply minimum image convention.           */
  /* ----------------------------------------- */
    if(xab >  hx) xab -= lx;
    else if(xab < -hx) xab += lx;
    if(yab >  hy) yab -= ly;
    else if(yab < -hy) yab += ly;
    if(zab >  hz) zab -= lz;

  /* ----------------------------------------- */
  /* Calculate the length of the bond and then */
  /* the energy using the CHARMM force field.  */
  /* ----------------------------------------- */

    double rab2 = xab*xab + yab*yab + zab*zab;
    double rab  = sqrt(rab2);
    double dri  = 1.0 / rab;
	double dt   = rab -urey[k][i].Seq;
    double fr = -urey[k][i].k * dt*2.0;
    eurey += -(fr * dt * 0.5);
  /*------------------------------------------ */
  /* Calculate the contribution of bonds to	   */
  /* to diagonal of hessian					   */
  /*------------------------------------------ */
	double deddt	= -fr;
	double d2eddt2	= 2.0*urey[k][i].k;
	double de		= deddt/rab;
	double term		= (d2eddt2-de) / rab2;
	double termx	= term * xab;
    double termy	= term * yab;
	double termz	= term * zab;
    double d2e11	= termx*xab + de;
	double d2e22	= termy*yab + de;
	double d2e33	= termz*zab + de;

	hesox	   += 2.0* d2e11;
	hesoy	   += 2.0* d2e22;
	hesoz	   += 2.0* d2e33;


  /* ----------------------------------------- */
  /* Calculate the forces.                     */
  /* ----------------------------------------- */
    double fx = fr * dri * xab;
    double fy = fr * dri * yab;
    double fz = fr * dri * zab;

    ffox[k][ia] -= fx;
    ffoy[k][ia] -= fy;
    ffoz[k][ia] -= fz;
    ffox[k][ib] += fx;
    ffoy[k][ib] += fy;
    ffoz[k][ib] += fz;

  /* ----------------------------------------- */
  /* If PRESSURE is defined, accumulate the    */
  /* components of the virial tensor.          */
  /* ----------------------------------------- */
#ifdef PRESSURE
    wxx += fx * xab;
    wyx += fy * xab;
    wyy += fy * yab;
    wzx += fz * xab;
    wzy += fz * yab;
    wzz += fz * zab;
#endif

	}

  /* ================================================================== */
  /*                                                                    */
  /* After the loop, if PRESSURE is defined, store the bonding          */
  /* contribution to the virial tensor to pvir[k].bond[i].              */
  /*                                                                    */
  /* ================================================================== */
#ifdef PRESSURE
  pviro[k].urey[0] = wxx;
  pviro[k].urey[1] = wyx;
  pviro[k].urey[2] = wyy;
  pviro[k].urey[3] = wzx;
  pviro[k].urey[4] = wzy;
  pviro[k].urey[5] = wzz;
#endif

  return (eurey);

}
#endif//CONFIGT
#endif//NEUTRAL

