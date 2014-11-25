//#include "stdafx.h"
#include <math.h>
static unsigned long int iu, iuhold, mult=663608941l;
double rnunif ()
{
 const double flt = 0.232830643654e-9;
 iu *= mult;
 /*printf ( "*** iu after --> %12lo ; %12lu.    ", iu, iu );*/
 /*return (flt*(iu>>1));*/
 return (flt*iu);
}

double rnexp()
{ return -log(rnunif());}

void rninit ( long int iufir)
{
 /*printf ( "*** iu first  --> %12lo ; %12ld.\n", iufir, iufir);*/
 iu = ( ( iufir%2 ) ? iufir : iufir + 1 );
 iuhold = iu;
 /*printf ( "*** initial   --> %12lo ; %12lu.\n", iu, iu );*/
}
void rnrest ()
{ rninit (iuhold); }
void rnconst ( unsigned long int multnew)
{
 int rest = multnew % 5;
 if ( (rest != 5) || (rest != 3) )
  mult = multnew - rest + 5;
 else
  mult = multnew;
}
unsigned long int rnlast ()
{ return ( iu ); }
unsigned long int rnfirst ()
{ return ( iuhold ); }



