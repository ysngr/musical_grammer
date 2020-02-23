/* interval.c */
#include<stdio.h>
#include<stdlib.h>

#define True 1
#define False 0

#define NATURAL_TONE 0
#define ACCIDENTAL 1

#define NC 0
#define ND 1
#define NE 2
#define NF 3
#define NG 4
#define NA 5
#define NH 6

#define DOUBLE_DIMINISH -3
#define DIMINISH -2
#define MINOR -1
#define PERFECT 0
#define MAJOR 1
#define AUGMENT 2
#define DOUBLE_AUGMENT 3

#define P_SERIES_NUM 5
#define M_SERIES_NUM 6

#define P_DIM_IDX 1
#define P_PER_IDX 2
#define P_AUG_IDX 3
#define M_MIN_IDX 2
#define M_MAJ_IDX 3


void parse(int*, const char*);
void calc(const int*, const int*, int*);
int calc_series_natural(const int, const int);
void print(const int*);



int main(int argc, char *argv[])
{
    int lower_tone[2], higher_tone[2];
    int result[2];

    if( argc != 3 ){
        printf("Invalid arguments.\n");
        return -1;
    }

    parse(lower_tone, argv[1]);
    parse(higher_tone, argv[2]);
    calc(lower_tone, higher_tone, result);
    print(result);

    return 0;
}


void parse(int *tone_num, const char *tone_name)
{
    int i;

    // natural tone
    switch( tone_name[0] ){
        case 'C' : tone_num[NATURAL_TONE] = NC; break;
        case 'D' : tone_num[NATURAL_TONE] = ND; break;
        case 'E' : tone_num[NATURAL_TONE] = NE; break;
        case 'F' : tone_num[NATURAL_TONE] = NF; break;
        case 'G' : tone_num[NATURAL_TONE] = NG; break;
        case 'A' : tone_num[NATURAL_TONE] = NA; break;
        case 'H' : tone_num[NATURAL_TONE] = NH; break;
        default :
            printf("Invalid natural tone.\n");
            exit(EXIT_FAILURE);
    }

    // accidental
    tone_num[ACCIDENTAL] = 0;
    for( i = 1; tone_name[i] != '\0'; i++ ){
        switch( tone_name[i] ){
            case '+' : tone_num[ACCIDENTAL]++; break;
            case '-' : tone_num[ACCIDENTAL]--; break;
            default :
                printf("Invalid accidental.\n");
                exit(EXIT_FAILURE);
        }
    }

    return ;
}


void calc(const int *lower, const int *higher, int *res)
{
    const int perfect_series[P_SERIES_NUM] = {DOUBLE_DIMINISH, DIMINISH, PERFECT, AUGMENT, DOUBLE_AUGMENT};
    const int major_minor_series[M_SERIES_NUM] = {DOUBLE_DIMINISH, DIMINISH, MINOR, MAJOR, AUGMENT, DOUBLE_AUGMENT};

    int is_perfect_series, series_index;

    // interval of natural tone
    res[NATURAL_TONE] = higher[NATURAL_TONE] - lower[NATURAL_TONE] + 1;
    if( res[NATURAL_TONE] < 0 ){
        res[NATURAL_TONE] += 7;
    }

    // series
    switch( res[NATURAL_TONE] ){
        case 1 :
        case 4 :
        case 5 :
        case 8 :
            is_perfect_series = True;
            break;
        default :
            is_perfect_series = False;
    }

    series_index = calc_series_natural(lower[NATURAL_TONE], res[NATURAL_TONE]);
    series_index += higher[ACCIDENTAL] - lower[ACCIDENTAL];

    if( series_index < 0 ){
        printf("Invalid accidental.\n");
        exit(EXIT_FAILURE);
    }else if( is_perfect_series && series_index >= P_SERIES_NUM ){
        printf("Invalid accidental.\n");
        exit(EXIT_FAILURE);
    }else if( !is_perfect_series && series_index >= M_SERIES_NUM ){
        printf("Invalid accidental.\n");
        exit(EXIT_FAILURE);
    }
    res[ACCIDENTAL] = ( is_perfect_series )? perfect_series[series_index] : major_minor_series[series_index];

    return ;
}


int calc_series_natural(const int low, const int natural_interval)
{
    int init_series_index;

    switch( natural_interval ){
        case 1 :
            init_series_index = P_PER_IDX;
            break;
        case 2 :
            switch( low ){
                case NE :
                case NH :
                    init_series_index = M_MIN_IDX;
                    break;
                default :
                    init_series_index = M_MAJ_IDX;
            }
            break;
        case 3 :
            switch( low ){
                case NC :
                case NF :
                case NG :
                    init_series_index = M_MAJ_IDX;
                    break;
                default :
                    init_series_index = M_MIN_IDX;
            }
            break;
        case 4 :
            switch( low ){
                case NF : init_series_index = P_AUG_IDX; break;
                default : init_series_index = P_PER_IDX;
            }
            break;
        case 5 :
            switch( low ){
                case NH : init_series_index = P_DIM_IDX; break;
                default : init_series_index = P_PER_IDX;
            }
            break;
        case 6 :
            switch( low ){
                case NE :
                case NA :
                case NH :
                    init_series_index = M_MIN_IDX;
                    break;
                default :
                    init_series_index = M_MAJ_IDX;
            }
            break;
        case 7 :
            switch( low ){
                case NC :
                case NF :
                    init_series_index = M_MAJ_IDX;
                    break;
                default :
                    init_series_index = M_MIN_IDX;
            }
            break;
        default :
            init_series_index = -9999;
    }

    return init_series_index;
}


void print(const int *res)
{
    switch( res[ACCIDENTAL] ){
        case DOUBLE_DIMINISH : printf("Double Diminish"); break;
        case DIMINISH :        printf("Diminish");        break;
        case MINOR :           printf("Minor");           break;
        case PERFECT :         printf("Perfect");         break;
        case MAJOR :           printf("Major");           break;
        case AUGMENT :         printf("Augment");         break;
        case DOUBLE_AUGMENT :  printf("Double Augment");  break;
    }
    printf(" ");

    switch( res[NATURAL_TONE] ){
        case 1 : printf("Unizon");  break;
        case 2 : printf("Second");  break;
        case 3 : printf("Third");   break;
        case 4 : printf("Fourth");  break;
        case 5 : printf("Fifth");   break;
        case 6 : printf("Sixth");   break;
        case 7 : printf("Seventh"); break;
    }
    printf("\n");

    return ;
}
