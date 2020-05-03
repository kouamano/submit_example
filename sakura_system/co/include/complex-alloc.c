/* alloc.c */

/* NEEDS     */
/* stdio.h   */
/* complex.h */

/*************************************************/
/* Copyright (c) 2003-2017 AMANO Kou             */
/* RIKEN                                         */
/* University of Tsukuba                         */
/* University of Liblary And Information Science */
/* National Institute of Agrobilogical Sciences  */
/*************************************************/

/*************************************************/
/* !! NOTATION !!                                */
/* BLAS/LAPACKではつかえないかもしれない         */
/*************************************************/

/*************************************************/
/* _alloc_vec                                    */
/*************************************************/
 

complex *x_alloc_vec(int num){
	complex *v;
	if((v = malloc((size_t)sizeof(complex)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(complex)*num);
		exit(1);
	}
	return(v);
}



/*************************************************/
/* _calloc_vec                                   */
/*************************************************/


complex *x_calloc_vec(int num){
	complex *v;
	if((v = calloc((size_t)num,(size_t)sizeof(complex))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(complex)*num);
		exit(1);
	}
	return(v);
}



/*************************************************/
/* _alloc_mat                                    */
/*************************************************/


complex **x_alloc_mat(int num, int dim){
	complex **m;
	int i;
	if((m = malloc((size_t)sizeof(complex *)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(complex *)*num);
		exit(1);
	}
	if((m[0] = malloc((size_t)sizeof(complex)*num*dim)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(complex)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		m[i] = m[i-1]+dim;
	}
	return(m);
}

/*************************************************/
/* _calloc_mat                                   */
/*************************************************/


complex **x_calloc_mat(int num, int dim){
	complex **m;
	int i;
	if((m = calloc((size_t)num,(size_t)sizeof(complex *))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(complex *)*num);
		exit(1);
	}
	if((m[0] = calloc((size_t)num*dim,(size_t)sizeof(complex))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(complex)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		m[i] = m[i-1]+dim;
	}
	return(m);
}



/*************************************************/
/* other                                         */
/*************************************************/


