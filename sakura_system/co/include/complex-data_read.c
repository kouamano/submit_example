/* complex-data_read.c */

/* NEEDS: string.h */

/*************************************************/
/* Copyright (c) 2003-2007 AMANO Kou             */
/* University of Liblary And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* University of Tsukuba                         */
/* RIKEN BioResource Center                      */
/* kamano@affrc.go.jp                            */
/*************************************************/

#ifndef FS
#define FS ' '
#endif
#ifndef IDSIZE
#define IDSIZE 1024
#endif
#ifndef TMP_STR_LEN
#define TMP_STR_LEN 1024
#endif
#ifndef TMP_STR_LEN_LONG
#define TMP_STR_LEN_LONG 2048
#endif



int read_xtable_from_stream(int num, int dim, FILE *stream, complex **table){
	int i;
	int j;
	complex xtmp;
	double xre,xim;
	for(i=0;i<num;i++){
		for(j=0;j<dim;j++){
			fscanf(stream,"%lf+%lfI",&xre,&xim);
			xtmp = xre + xim*I;
			table[i][j] = xtmp;
		}
	}
	return(0);
}
/* format:
100+10I 200+10I
300+-10I 400+-10I
*/


