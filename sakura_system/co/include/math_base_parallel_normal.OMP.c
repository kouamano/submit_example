/* math_base.c */

/*************************************************/
/* Copyright (c) 2003-2005 AMANO Kou             */
/* University of Tsukuba                         */
/* University of Liblary And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* kamano@affrc.go.jp                            */
/*************************************************/

/**********************************/
/* COPIED from math_base.c        */
/* DONE: insert pragma            */
/* TODO: avoid segmentation-falt  */
/**********************************/

#include <math.h>
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

float norm(int len, float *list){
        int i;
        float nor = 0;
        for(i=0;i<len;i++){
                //nor += pow(list[i],2);
                nor += (list[i] * list[i]);
        }
        nor = sqrt(nor);
        return(nor);
}

float inner(int len, float *list1, float *list2){
        int i;
        float inn = 0;
        for(i=0;i<len;i++){
                inn += (list1[i] * list2[i]);
        }
        return(inn);
}

float waf(int len, float *list1, float *list2){
        int i = 0;
        float count_list1 = 0;
        float count_list2 = 0;
        float *weight = NULL;
        float *Pi = NULL;
        float ret = 0;
        for(i=0;i<len;i++){
                count_list1 += list1[i];
                count_list2 += list2[i];
        }
        weight = f_calloc_vec(len);
        for(i=0;i<len;i++){
                weight[i] = list1[i] + list2[i];
                if(weight[i] == 0){
                        ;
                }else{
                        weight[i] = 1/weight[i];
                }
        }
        Pi = f_balloc_vec(len,1);
        for(i=0;i<len;i++){
                Pi[i] = list1[i] * list2[i];
                Pi[i] = Pi[i] * weight[i];
        }
        for(i=0;i<len;i++){
                ret += Pi[i];
        }
        ret = ret * (count_list1 + count_list2) / (count_list1 * count_list2);
        free(weight);
        free(Pi);
        return(ret);
}

float sq_euc_dist(int len, float *list1, float *list2){
	int i;
	float sq_sum = 0;
	for(i=0;i<len;i++){
		sq_sum += pow(list1[i]-list2[i],2);
	}
	return(sq_sum);
}

float euc_dist(int len, float *list1, float *list2){
	int i;
	float sq_sum = 0;
	for(i=0;i<len;i++){
		sq_sum += pow(list1[i]-list2[i],2);
	}
	return(sqrt(sq_sum));
}

float cos_dist(int len, float *list1, float *list2){
        float nor1 = 0;
        float nor2 = 0;
        float dist = 0;
        nor1 = norm(len,list1);
        nor2 = norm(len,list2);
        dist = inner(len,list1,list2);
        dist = 1 - (dist / nor1 / nor2);
        return(dist);
}

float naf_dist(int len, float *list1, float *list2){
        float af11 = 0;
        float af22 = 0;
        float af12 = 0;
        float dist = 0;
        af11 = inner(len,list1,list1);
        af22 = inner(len,list2,list2);
        af12 = inner(len,list1,list2);
        //dist = af11 * af22 / af12 / af12;
        dist = af12 * af12 / af11 / af22;
        dist = 1-dist;
        return(dist);
}

float nwaf_dist(int len, float *list1, float *list2){
        return(1 - waf(len,list1,list2));
}

 
void sq_euc_dist_list(int num, int dim, float **arr, float *list, float *dist_list){
	int l;
	#pragma omp parallel for
	for(l=0;l<num;l++){
		dist_list[l] = sq_euc_dist(dim,list,arr[l]);
	}
}

void euc_dist_list(int num, int dim, float **arr, float *list, float *dist_list){
	int l;
	#pragma omp parallel for
	for(l=0;l<num;l++){
		dist_list[l] = euc_dist(dim,list,arr[l]);
	}
}

void sq_euc_dist_table(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	#pragma omp parallel for private(m)
	for(l=0;l<num;l++){
		for(m=0;m<num;m++){
			dist_table[l][m] = sq_euc_dist(dim,arr[l],arr[m]);
		}
	}
}

void sq_euc_dist_triangle(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	#pragma omp parallel for private(m)
	for(l=0;l<num;l++){
		for(m=0;m<l;m++){
			dist_table[l][m] = sq_euc_dist(dim,arr[l],arr[m]);
		}
	}
}

void euc_dist_table(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	#pragma omp parallel for private(m)
	for(l=0;l<num;l++){
		for(m=0;m<num;m++){
			dist_table[l][m] = euc_dist(dim,arr[l],arr[m]);
		}
	}
}

void euc_dist_sqtable(int num, int dim, float **arr, float **dist_table){
        int l;
        int m;
	#pragma omp parallel for private(m)
        for(l=0;l<num;l++){
                for(m=0;m<num;m++){
                        dist_table[l][m] = euc_dist(dim,arr[l],arr[m]);
                }
        }
}


void euc_dist_triangle(int num, int dim, float **arr, float **dist_table){
	int l;
	int m;
	#pragma omp parallel for private(m)
	for(l=0;l<num;l++){
		for(m=0;m<l;m++){
			dist_table[l][m] = euc_dist(dim,arr[l],arr[m]);
		}
	}
}

void float_max_list(int len, float *list, float out_max){
	int l;
	out_max = list[0];
	for(l=1;l<len;l++){
		if(out_max < list[l]){
			out_max = list[l];
		}
	}
}

void float_min_list(int len, float *list, float out_min){
	int l;
	out_min = list[0];
	for(l=1;l<len;l++){
		if(out_min > list[l]){
			out_min = list[l];
		}
	}
}

void float_min_pos_list(int len, float *list, int *pos, float *out_min){
	int l;
	*out_min = list[0];
	*pos = 0;
	for(l=1;l<len;l++){
		if(*out_min > list[l]){
			*out_min = list[l];
			*pos = l;
		}
	}
	printf("%d %f\n",*pos,*out_min);
}

void float_max_matrix(int dim, int num, float **matrix, float *out_list, int axis){
	int l;
	int m;
	if(axis == 1){
		#pragma omp parallel for private(m)
		for(l=0;l<num;l++){
			out_list[l] = matrix[l][0];
			for(m=1;m<dim;m++){
				if(out_list[l] < matrix[l][m]){
					out_list[l] = matrix[l][m];
				}
			}
		}
	}else if(axis == 0){
		#pragma omp parallel for private(l)
		for(m=0;m<dim;m++){
			out_list[m] = matrix[0][m];
			for(l=0;l<num;l++){
				if(out_list[m] < matrix[l][m]){
					out_list[m] = matrix[l][m];
				}
			}
		}
	}
}

void float_min_matrix(int dim, int num, float **matrix, float *out_list, int axis){
	int l;
	int m;
	if(axis == 1){
		#pragma omp parallel for private(m)
		for(l=0;l<num;l++){
			out_list[l] = matrix[l][0];
			for(m=1;m<dim;m++){
				if(out_list[l] > matrix[l][m]){
					out_list[l] = matrix[l][m];
				}
			}
		}
	}else if(axis == 0){
		#pragma omp parallel for private(l)
		for(m=0;m<dim;m++){
			out_list[m] = matrix[0][m];
			for(l=0;l<num;l++){
				if(out_list[m] > matrix[l][m]){
					out_list[m] = matrix[l][m];
				}
			}
		}
	}
}

void float_pos_max_matrix(int dim, int num, float **matrix, int *pos, float *out_list, int axis){
	int l;
	int m;
	if(axis == 1){
		#pragma omp parallel for private(m)
		for(l=0;l<num;l++){
			out_list[l] = matrix[l][0];
			pos[l] = 0;
			for(m=1;m<dim;m++){
				if(out_list[l] < matrix[l][m]){
					out_list[l] = matrix[l][m];
					pos[l] = m;
				}
			}
		}
	}else if(axis == 0){
		#pragma omp parallel for private(l)
		for(m=0;m<dim;m++){
			out_list[m] = matrix[0][m];
			pos[m] = 0;
			for(l=1;l<num;l++){
				if(out_list[m] < matrix[l][m]){
					out_list[m] = matrix[l][m];
					pos[m] = l;
				}
			}
		}
	}
}

void float_pos_min_matrix(int dim, int num, float **matrix, int *pos, float *out_list, int axis){
	int l;
	int m;
	if(axis == 1){
		#pragma omp parallel for private(m)
		for(l=0;l<num;l++){
			out_list[l] = matrix[l][0];
			pos[l] = 0;
			for(m=1;m<dim;m++){
				if(out_list[l] > matrix[l][m]){
					out_list[l] = matrix[l][m];
					pos[l] = m;
				}
			}
		}
	}else if(axis == 0){
		#pragma omp parallel for private(l)
		for(m=0;m<dim;m++){
			out_list[m] = matrix[0][m];
			pos[m] = 0;
			for(l=1;l<num;l++){
				if(out_list[m] > matrix[l][m]){
					out_list[m] = matrix[l][m];
					pos[m] = l;
				}
			}
		}
	}
}

void min_dist_table_position(int num, float **matrix, float *dist, int *position){
	int l;
	int m;
	float minimum = matrix[1][0];
	int l_pos = 1;
	int m_pos = 0;
	for(l=1;l<num;l++){
		for(m=0;m<l;m++){
			if(minimum > matrix[l][m]){
				minimum = matrix[l][m];
				l_pos = l;
				m_pos = m;
			}
		}
	}
	*dist = minimum;
	position[0] = l_pos;
	position[1] = m_pos;
}



float arithm_ave(int dim, float *list){
	int l;
	float sum = 0;
	#pragma omp parallel for reduction(+:sum)
	for(l=0;l<dim;l++){
		sum += list[l];
	}
	return(sum/dim);
}
void arithm_ave_list(int num, int dim, float **arr, float *list){
	int l;
	int m;
	#pragma omp parallel for private(m)
	for(l=0;l<num;l++){
		for(m=0;m<dim;m++){
			list[m] += arr[l][m];
		}
	}
	#pragma omp parallel for
	for(m=0;m<dim;m++){
		list[m] = list[m]/num;
	}
}
/*
void arithm_ave_list_from_ptr(int num, int ref, int dim, float **arr, float *list){
	int l;
	int m;
	for(l=0;l<num;l++){
		for(m=0;m<dim;m++){
			list[m] += arr[ref][m];
		}
	}
	#pragma omp parallel for
	for(m=0;m<dim;m++){
		list[m] = list[m]/num;
	}
}
*/
void sum_ref_matrix(int num, int dim, int *ref_list, float **matrix, float *list){
	int l;
	int m;
	#pragma omp parallel for private(m)
	for(l=0;l<num;l++){
		for(m=0;m<dim;m++){
			list[m] += matrix[ref_list[l]][m];
		}
	}
}

void f_dif_2lists(int num, float *list1, float *list2, float *list_out){
	int l;
	#pragma omp parallel for
	for(l=0;l<num;l++){
		list_out[l] = list2[l] - list1[l];
	}
}

void max_list_from_matrix(int dim, int num, float **matrix, float *out_list, int axis){
        int l;
        int m;
        if(axis == 1){
		#pragma omp parallel for private(m)
                for(l=0;l<num;l++){
                        out_list[l] = matrix[l][0];
                        for(m=1;m<dim;m++){
                                if(out_list[l] < matrix[l][m]){
                                        out_list[l] = matrix[l][m];
                                }
                        }
                }
        }else if(axis == 0){
		#pragma omp parallel for private(l)
                for(m=0;m<dim;m++){
                        out_list[m] = matrix[0][m];
                        for(l=0;l<num;l++){
                                if(out_list[m] < matrix[l][m]){
                                        out_list[m] = matrix[l][m];
                                }
                        }
                }
        }
}

void min_list_from_matrix(int dim, int num, float **matrix, float *out_list, int axis){
        int l;
        int m;
        if(axis == 1){
		#pragma omp parallel for private(m)
                for(l=0;l<num;l++){
                        out_list[l] = matrix[l][0];
                        for(m=1;m<dim;m++){
                                if(out_list[l] > matrix[l][m]){
                                        out_list[l] = matrix[l][m];
                                }
                        }
                }
        }else if(axis == 0){
		#pragma omp parallel for private(l)
                for(m=0;m<dim;m++){
                        out_list[m] = matrix[0][m];
                        for(l=0;l<num;l++){
                                if(out_list[m] > matrix[l][m]){
                                        out_list[m] = matrix[l][m];
                                }
                        }
                }
        }
}

void max_pos_matrix(int dim, int num, float **matrix, int *pos, float *out_list, int axis){
        int l;
        int m;
        if(axis == 1){
		#pragma omp parallel for private(m)
                for(l=0;l<num;l++){
                        out_list[l] = matrix[l][0];
                        pos[l] = 0;
                        for(m=1;m<dim;m++){
                                if(out_list[l] < matrix[l][m]){
                                        out_list[l] = matrix[l][m];
                                        pos[l] = m;
                                }
                        }
                }
        }else if(axis == 0){
		#pragma omp parallel for private(l)
                for(m=0;m<dim;m++){
                        out_list[m] = matrix[0][m];
                        pos[m] = 0;
                        for(l=1;l<num;l++){
                                if(out_list[m] < matrix[l][m]){
                                        out_list[m] = matrix[l][m];
                                        pos[m] = l;
                                }
                        }
                }
        }
}

void min_pos_matrix(int dim, int num, float **matrix, int *pos, float *out_list, int axis){
        int l;
        int m;
        if(axis == 1){
		#pragma omp parallel for private(m)
                for(l=0;l<num;l++){
                        out_list[l] = matrix[l][0];
                        pos[l] = 0;
                        for(m=1;m<dim;m++){
                                if(out_list[l] > matrix[l][m]){
                                        out_list[l] = matrix[l][m];
                                        pos[l] = m;
                                }
                        }
                }
        }else if(axis == 0){
		#pragma omp parallel for private(l)
                for(m=0;m<dim;m++){
                        out_list[m] = matrix[0][m];
                        pos[m] = 0;
                        for(l=1;l<num;l++){
                                if(out_list[m] > matrix[l][m]){
                                        out_list[m] = matrix[l][m];
                                        pos[m] = l;
                                }
                        }
                }
        }
}

void cos_dist_triangle(int num, int dim, float **arr, float **dist_table){
        int l;
        int m;
	#pragma omp parallel for private(m)
        for(l=0;l<num;l++){
                for(m=0;m<l;m++){
                        dist_table[l][m] = cos_dist(dim,arr[l],arr[m]);
                }
        }
}

void cos_dist_table(int num1, float **arr1, int num2, float **arr2, int dim, float **dist_table){
        int l;
        int m;
	#pragma omp parallel for private(m)
        for(l=0;l<num1;l++){
                for(m=0;m<num2;m++){
                        dist_table[l][m] = cos_dist(dim,arr1[l],arr2[m]);
                }
        }
}


void add_column_of_matrix(int num, int dim, int *ref_list, float **matrix, float *list){
        int l;
        int m;
	#pragma omp parallel for private(m)
        for(l=0;l<num;l++){
                for(m=0;m<dim;m++){
                        list[m] += matrix[ref_list[l]][m];
                }
        }
}

void dif_2lists(int num, float *list1, float *list2, float *list_out){
        int l;
	#pragma omp parallel for
        for(l=0;l<num;l++){
                list_out[l] = list2[l] - list1[l];
        }
}

void naf_dist_triangle(int num, int dim, float **arr, float **dist_table){
        int l;
        int m;
	#pragma omp parallel for private(m)
        for(l=0;l<num;l++){
                for(m=0;m<l;m++){
                        dist_table[l][m] = naf_dist(dim,arr[l],arr[m]);
                }
        }
}

void naf_dist_table(int num1, float **arr1, int num2, float **arr2, int dim, float **dist_table){
        int l;
        int m;
	#pragma omp parallel for private(m)
        for(l=0;l<num1;l++){
                for(m=0;m<num2;m++){
                        dist_table[l][m] = naf_dist(dim,arr1[l],arr2[m]);
                }
        }
}

void nwaf_dist_triangle(int num, int dim, float **arr, float **dist_table){
        int l;
        int m;
	#pragma omp parallel for private(m)
        for(l=0;l<num;l++){
                for(m=0;m<l;m++){
                        dist_table[l][m] = nwaf_dist(dim,arr[l],arr[m]);
                }
        }
}

void nwaf_dist_table(int num1, float **arr1, int num2, float **arr2, int dim, float **dist_table){
        int l;
        int m;
	#pragma omp parallel for private(m)
        for(l=0;l<num1;l++){
                for(m=0;m<num2;m++){
                        dist_table[l][m] = nwaf_dist(dim,arr1[l],arr2[m]);
                }
        }
}


/*
void mk_flag_list(int num, int num_flag, int *flaged_list, int *out_list){
	int l;
	int m;
	#pragma omp parallel for
	for(l=0;l<num;l++){
		out_list[l] = 0;
	}
	#pragma omp parallel for shared(m)
	for(l=0;l<num;l++){
		for(m=0;m<num_flag;m++){
			if(l == flaged_list[m]){
				out_list[l] = 1;
			}
		}
	}
}
*/
