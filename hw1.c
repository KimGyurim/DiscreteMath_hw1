#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// M: y, N: x

void receive_input(FILE* file, int *N, int *M, int input[][52]);
void print_input(int input[][52], int n,int m);
int parse( int n, int m);

int main(){

  char fileName[128] = {};

  printf("Please input file name: ");
  scanf("%s",fileName);

  FILE * input_f = fopen(fileName, "r");

  int x, y, c;
  int input[51][52];
  int N = 0, M = 0;

  //receive input
  receive_input(input_f, &N, &M, input);
  fclose(input_f);

  //open formula file
  FILE * fp = fopen("formula_hw1", "w");


  // Q0
  //make variables
  //0 은 white, 1은 black
  fprintf(fp,"\n; Q0\n");
  for(y = 1 ; y < M ; y++){//y
    for(x = 1 ; x < N ; x++){//x
      for(c = 0; c <= 1 ; c++){
        if(input[y][x] > 9) {
          printf("(Error) input range is wrong\n");
          return 0;
        }
        fprintf(fp,"(declare-const p_%d_%d_%d Int)\n", y, x, c);
      }
    }
  }


  //Q1
  //p_y_x_c = 0 or input[y][x];
  //p_y_x_c는 0이나 n을 값으로 가진다
  fprintf(fp,"\n; Q1\n");
  for (y = 1 ; y < M ; y++)
    for (x = 1 ; x < N ; x++)
      for(c = 0 ; c <= 1; c++)
        fprintf(fp,"(assert (or (= p_%d_%d_%d 0) (= p_%d_%d_%d %d)))\n", y, x, c, y, x, c, input[y][x]) ;


  //Q2
  //p_y_x_0 + p_y_x_1 = inpt[y][x]
  //하나가 참이면 하나는 꼭 거짓이여야 한다
  fprintf(fp,"\n;Q2\n");
  for (y = 1 ; y < M ; y++)
  {
    for (x = 1 ; x < N ; x++)
    {
          fprintf(fp,"(assert(= (+ ") ;
          for(c = 0 ; c <= 1 ; c++)
            fprintf(fp,"p_%d_%d_%d ", y, x, c) ;
          fprintf(fp,") %d))\n", input[y][x]) ;
    }
  }


  //Q3
  //Label of col =  sum of black
  fprintf(fp,"\n;Q3\n\n");
  for (x = 1 ; x < N ; x++)
  {
    fprintf(fp,"(assert(= (+ ") ;

    for (y = 1 ; y < M ; y++)
    {
      fprintf(fp,"p_%d_%d_1 ", y, x) ;
    }
    fprintf(fp,") %d))\n", input[0][x]) ;
  }


  //Q4
  //Label of row =  sum of white
  fprintf(fp,"\n;Q4\n\n");
  for (y = 1 ; y < M ; y++)
  {
    fprintf(fp,"(assert (= (+ ") ;
    for (x = 1 ; x < N ; x++)
    {
      fprintf(fp,"p_%d_%d_0 ", y, x) ;
    }
    fprintf(fp,") %d))\n", input[y][N]) ;
  }

  fprintf(fp,"(check-sat)\n(get-model)\n") ;
  fclose(fp);

  //print input test
  //print_input(input, N, M);

  //output
  FILE * fin = popen("z3 formula_hw1 > solution_hw1", "r");
  pclose(fin);

  parse(N, M);
  return 0;
}



//function


void receive_input(FILE* file, int *N, int *M, int input[][52])
{
  char t[500];
  int size_t = sizeof(t) / sizeof(char);
  int i = 0;

  while(fgets(t, size_t, file) != NULL){
    char *tp = strtok(t, " ");
    // printf("while loop\n");
    // printf("tp: %s\n", tp);

    for(i = 1; tp != NULL ; i++){
      input[(*M)][i] = atoi(tp);
      tp = strtok(NULL, " ");
    }

    (*M)++;
  }
  (*N) = i - 1;
}



int parse(int n, int m)
{
  FILE * output = fopen("solution_hw1", "r");

  char a[128] ;
  char b[128] ;
  char s[128] ;
  char t[128] ;
  int i, j, c;

  int board[m][n];

  fscanf(output,"%s %s", a, b) ;
  if(strcmp(a, "unsat") == 0){
    printf("No solution\n");
    fclose(output);
    return 0;
  }

  while (!feof(output)) {
    fscanf(output,"%s %s %s %s %s", b, s, b, b, t) ;

    // i = s[2] - '0' ;
    // j = s[4] - '0' ;
    // c = s[6] - '0' ;
    sscanf(s, "p_%d_%d_%d", &i, &j, &c);
    if (strcmp(t, "0)") != 0) {
       board[i][j] = c ;
    }
 }

 for (i = 1 ; i < m ; i++) {
    for (j = 1 ; j < n ; j++) {
       printf("%d ", board[i][j]) ;
    }
    printf("\n") ;
 }

 fclose(output);
 return 0;
}



void print_input(int input[][52], int n, int m){
  printf("M X N = %d X %d\n", m, n);
  for(int i = 0 ; i < m ; i++){//y
    for(int j = 1 ; j <= n ; j++){//x
      printf("%d ", input[i][j]);
    }
    printf("\n");
  }
}
