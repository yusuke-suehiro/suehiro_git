//
//  main.c
//  homework_LLGMN_TA
//
//  Created by 末廣勇祐 on 2020/04/20.
//  Copyright © 2020 末廣勇祐. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define COUNT_SIZE 1000

void input_data(int *num);      //データの入力をする関数
void free_data(double **array, int datanum);    //領域を解放する関数
void free_w(double ***array, int datanum, int clas);    //領域を解放する関数
void initialize1(double *array, int first);     //初期化する関数
void initialize2(double **array, int first, int second);    //初期化する関数
void initialize3(double ***array, int first, int second, int third);    //初期化する関数


int main(int argc, const char * argv[]) {
    
    FILE *fp;
    int compo=0, dim=0, dim_h=0, clas=0, datanum=0;
    double **data_lea, **data_dis, **data_lea_T, **data_dis_T, **trans_data;
    double para1=0, para2=0, para3=0, para4=0;
    int i=0, j=0, flag=0;
    double ***w;
    double **in_0, ***in_1, **in_2, **out_0, ***out_1, *sum;
    int random=0;
    srand((int)time(0));
    double learning=0, b=0.9, r=0, J1=0, n=0;

    printf("コンポーネント数を");
    input_data(&compo);
    printf("入力次元数を");
    input_data(&dim);
    printf("非線形変換後の次元数を");
    input_data(&dim_h);
    printf("クラス数を");
    input_data(&clas);
    printf("データ数を");
    input_data(&datanum);
    printf("一括(1)か逐次(2)を");
    input_data(&flag);
    
    //教師データと未学習データを配列に読み込む
    data_lea=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        data_lea[i]=(double *)malloc((dim)*sizeof(double *));     //入力数分、領域確保
    }
    fp=fopen("lea_sig.csv","r");
    if (fp == NULL) {
        printf("ファイルがオープンできません\n");
        return 0;
    }
    while (fscanf(fp,"%lf,%lf", &para1, &para2)!=EOF) {
        data_lea[i][j]=para1;
        j+=1;
        data_lea[i][j]=para2;
        i+=1;
        j=0;
    }
    fclose(fp);
    data_dis=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        data_dis[i]=(double *)malloc((dim)*sizeof(double *));     //入力数分、領域確保
    }
    fp=fopen("dis_sig.csv","r");
    
    if (fp == NULL) {
        printf("ファイルがオープンできません\n");
        return 0;
    }
    i=0;
    j=0;
    while (fscanf(fp,"%lf,%lf", &para1, &para2)!=EOF) {
        data_dis[i][j]=para1;
        j+=1;
        data_dis[i][j]=para2;
        i+=1;
        j=0;
    }
    fclose(fp);
    data_lea_T=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        data_lea_T[i]=(double *)malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    fp=fopen("lea_T_sig.csv","r");
    if (fp == NULL) {
        printf("ファイルがオープンできません\n");
        return 0;
    }
    i=0;
    j=0;
    while (fscanf(fp,"%lf,%lf,%lf,%lf", &para1, &para2, &para3, &para4)!=EOF) {
        data_lea_T[i][j]=para1;
        j+=1;
        data_lea_T[i][j]=para2;
        j+=1;
        data_lea_T[i][j]=para3;
        j+=1;
        data_lea_T[i][j]=para4;
        i+=1;
        j=0;
    }
    fclose(fp);
    data_dis_T=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        data_dis_T[i]=(double *)malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    fp=fopen("dis_T_sig.csv","r");
    if (fp == NULL) {
        printf("ファイルがオープンできません\n");
        return 0;
    }
    i=0;
    j=0;
    while (fscanf(fp,"%lf,%lf,%lf,%lf", &para1, &para2, &para3, &para4)!=EOF) {
        data_dis_T[i][j]=para1;
        j+=1;
        data_dis_T[i][j]=para2;
        j+=1;
        data_dis_T[i][j]=para3;
        j+=1;
        data_dis_T[i][j]=para4;
        i+=1;
        j=0;
    }
    fclose(fp);
    
    //各パラメータの配列の設定
    w=malloc((dim_h)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<dim_h;i++) {
        w[i]=malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<dim_h;i++) {
        for (int j=0;j<clas;j++) {
            w[i][j]=malloc((compo)*sizeof(double *));     //入力数分、領域確保
        }
    }
    for (int i=0;i<dim_h;i++) {
        for (int j=0;j<clas;j++) {
            for (int k=0;k<compo;k++) {
                if (j == clas-1 && k == compo-1) {
                    w[i][j][k]=0;
                }
                else {
                    w[i][j][k]=(double)rand()/(double)RAND_MAX;
                    random=rand();
                    if (random % 2 ==0) {
                        w[i][j][k]=-1*w[i][j][k];
                    }
                }
            }
        }
    }
    in_0=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        in_0[i]=(double *)malloc((dim_h)*sizeof(double *));     //入力数分、領域確保
    }
    in_2=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        in_2[i]=(double *)malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    out_0=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        out_0[i]=(double *)malloc((dim_h)*sizeof(double *));     //入力数分、領域確保
    }
    initialize2(in_0, datanum, dim_h);
    initialize2(in_2, datanum, clas);
    initialize2(out_0, datanum, dim_h);
    in_1=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        in_1[i]=malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum;i++) {
        for (int j=0;j<clas;j++) {
            in_1[i][j]=malloc((compo)*sizeof(double *));     //入力数分、領域確保
        }
    }
    out_1=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        out_1[i]=malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum;i++) {
        for (int j=0;j<clas;j++) {
            out_1[i][j]=malloc((compo)*sizeof(double *));     //入力数分、領域確保
        }
    }
    initialize3(in_1, datanum, clas, compo);
    initialize3(out_1, datanum, clas, compo);
    double ***tmp;
    tmp=malloc((dim_h)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<dim_h;i++) {
        tmp[i]=malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<dim_h;i++) {
        for (int j=0;j<clas;j++) {
            tmp[i][j]=malloc((compo)*sizeof(double *));     //入力数分、領域確保
        }
    }
    sum=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    
    //非線形変換
    trans_data=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        trans_data[i]=(double *)malloc((dim_h)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum;i++) {
        for (int j=0;j<dim_h;j++) {
            switch (j) {
                case 0:
                    trans_data[i][j]=1;
                    break;
                case 1:
                    trans_data[i][j]=data_lea[i][0];
                    break;
                case 2:
                    trans_data[i][j]=data_lea[i][1];
                    break;
                case 3:
                    trans_data[i][j]=data_lea[i][0]*data_lea[i][0];
                    break;
                case 4:
                    trans_data[i][j]=data_lea[i][0]*data_lea[i][1];
                    break;
                case 5:
                    trans_data[i][j]=data_lea[i][1]*data_lea[i][1];
                    break;
            }
        }
    }
    if (flag == 1) {        // 一括学習
        printf("一括学習計算中...\n\n");

        for (int count=0;count<COUNT_SIZE;count++) {
            //第１層
            for (int num=0;num<datanum;num++) {
                for (int i=0;i<dim_h;i++) {
                    in_0[num][i]=trans_data[num][i];
                    out_0[num][i]=in_0[num][i];
                }
            }
            //第２層
            initialize1(sum,datanum);
            initialize3(in_1,datanum,clas,compo);
            initialize3(out_1,datanum,clas,compo);
            for (int num=0;num<datanum;num++) {
                for (int bef=0;bef<dim_h;bef++) {
                    for (int now_class=0;now_class<clas;now_class++) {
                        for (int now_compo=0;now_compo<compo;now_compo++) {
                            in_1[num][now_class][now_compo]+=out_0[num][bef]*w[bef][now_class][now_compo];
                        }
                    }
                }
            }
            for (int num=0;num<datanum;num++) {
                for (int now_class=0;now_class<clas;now_class++) {
                    for (int now_compo=0;now_compo<compo;now_compo++) {
                            sum[num]+=exp(in_1[num][now_class][now_compo]);
                    }
                }
            }
            for (int num=0;num<datanum;num++) {
                for (int now_class=0;now_class<clas;now_class++) {
                    for (int now_compo=0;now_compo<compo;now_compo++) {
                        out_1[num][now_class][now_compo]=exp(in_1[num][now_class][now_compo])/sum[num];
                    }
                }
            }
            //第３層
            initialize2(in_2,datanum,clas);
            for (int num=0;num<datanum;num++) {
                for (int now_class=0;now_class<clas;now_class++) {
                    for (int bef_compo=0;bef_compo<compo;bef_compo++) {
                        in_2[num][now_class]+=out_1[num][now_class][bef_compo];
                    }
                }
            }
            //評価関数
            double J=0;
            for (int num=0;num<datanum;num++) {
                for (int now=0;now<clas;now++) {
                    J+=-1*data_lea_T[num][now]*log(in_2[num][now]);
                }
            }
            if (count == 0) {       //評価関数の初期値
                J1=J;
            }
            printf("count %d  誤差関数%lf\n",count, J);
            //wを更新
            initialize3(tmp,dim_h,clas,compo);
            learning=0;
            for (int h=0;h<dim_h;h++) {
                for (int num=0;num<datanum;num++) {
                    for (int k=0;k<clas;k++) {
                        for (int m=0;m<compo;m++) {
                            tmp[h][k][m]+=(in_2[num][k]-data_lea_T[num][k])*out_1[num][k][m]*trans_data[num][h]/in_2[num][k];
                        }
                     }
                }
            }
            for (int h=0;h<dim_h;h++) {
                for (int k=0;k<clas;k++) {
                    for (int m=0;m<compo;m++) {
                            learning+=tmp[h][k][m]*tmp[h][k][m];
                        }
                }
            }
            n=pow(J1, 1-b)/((count+1)*(1-b));
            for (int h=0;h<dim_h;h++) {
                for (int k=0;k<clas;k++) {
                    for (int m=0;m<compo;m++) {
                        if (k == 3 && m == 1) {
                            w[h][k][m]=0;
                        }
                        else {
                            r=pow(J,b)/learning;
                            w[h][k][m]=w[h][k][m]-n*r*tmp[h][k][m];
                        }
                    }
                }
            }
        }
        printf("計算が終了しました\n\n");
    }
    
    else  {        // 逐次学習
          printf("逐次学習計算中...\n\n");

          for (int count=0;count<COUNT_SIZE;count++) {
              //第１層
              for (int num=0;num<datanum;num++) {
                  for (int i=0;i<dim_h;i++) {
                      in_0[num][i]=trans_data[num][i];
                      out_0[num][i]=in_0[num][i];
                  }
              }
              //第２層
              initialize1(sum,datanum);
              initialize3(in_1,datanum,clas,compo);
              initialize3(out_1,datanum,clas,compo);
              for (int num=0;num<datanum;num++) {
                  for (int bef=0;bef<dim_h;bef++) {
                      for (int now_class=0;now_class<clas;now_class++) {
                          for (int now_compo=0;now_compo<compo;now_compo++) {
                              in_1[num][now_class][now_compo]+=out_0[num][bef]*w[bef][now_class][now_compo];
                          }
                      }
                  }
              }
              for (int num=0;num<datanum;num++) {
                  for (int now_class=0;now_class<clas;now_class++) {
                      for (int now_compo=0;now_compo<compo;now_compo++) {
                              sum[num]+=exp(in_1[num][now_class][now_compo]);
                      }
                  }
              }
              for (int num=0;num<datanum;num++) {
                  for (int now_class=0;now_class<clas;now_class++) {
                      for (int now_compo=0;now_compo<compo;now_compo++) {
                          out_1[num][now_class][now_compo]=exp(in_1[num][now_class][now_compo])/sum[num];
                      }
                  }
              }
              //第３層
              initialize2(in_2,datanum,clas);
              for (int num=0;num<datanum;num++) {
                  for (int now_class=0;now_class<clas;now_class++) {
                      for (int bef_compo=0;bef_compo<compo;bef_compo++) {
                          in_2[num][now_class]+=out_1[num][now_class][bef_compo];
                      }
                  }
              }
              //評価関数
              double J=0;
              for (int num=0;num<datanum;num++) {
                  for (int now=0;now<clas;now++) {
                      J+=-1*data_lea_T[num][now]*log(in_2[num][now]);
                  }
              }
              if (count == 0) {       //評価関数の初期値
                  J1=J;
              }
              printf("count %d  誤差関数%lf\n",count, J);
              //wを更新
              initialize3(tmp,dim_h,clas,compo);
              learning=0;
              n=pow(J1, 1-b)/((count+1)*(1-b));
              for (int h=0;h<dim_h;h++) {
                  for (int num=0;num<datanum;num++) {
                      for (int k=0;k<clas;k++) {
                          for (int m=0;m<compo;m++) {
                              tmp[h][k][m]=(in_2[num][k]-data_lea_T[num][k])*out_1[num][k][m]*trans_data[num][h]/in_2[num][k];
                          }
                      }
                  }
              }
              for (int h=0;h<dim_h;h++) {
                  for (int k=0;k<clas;k++) {
                      for (int m=0;m<compo;m++) {
                          learning+=tmp[h][k][m]*tmp[h][k][m];
                      }
                  }
              }
              for (int h=0;h<dim_h;h++) {
                  for (int num=0;num<datanum;num++) {
                      for (int k=0;k<clas;k++) {
                          for (int m=0;m<compo;m++) {
                              tmp[h][k][m]=(in_2[num][k]-data_lea_T[num][k])*out_1[num][k][m]*trans_data[num][h]/in_2[num][k];
                              
                              if (k == 3 && m == 1) {
                                  w[h][k][m]=0;
                              }
                              else {
                                  r=pow(J,b)/learning;
                                  w[h][k][m]=w[h][k][m]-n*r*tmp[h][k][m];
                              }
                          }
                       }
                  }
              }
          }
          printf("計算が終了しました\n\n");
      }
    
    //未学習データを識別する
    for (int i=0;i<datanum;i++) {
        for (int j=0;j<dim_h;j++) {
            switch (j) {
                case 0:
                    trans_data[i][j]=1;
                    break;
                case 1:
                    trans_data[i][j]=data_dis[i][0];
                    break;
                case 2:
                    trans_data[i][j]=data_dis[i][1];
                    break;
                case 3:
                    trans_data[i][j]=data_dis[i][0]*data_dis[i][0];
                    break;
                case 4:
                    trans_data[i][j]=data_dis[i][0]*data_dis[i][1];
                    break;
                case 5:
                    trans_data[i][j]=data_dis[i][1]*data_dis[i][1];
                    break;
            }
        }
    }
    //第１層
    for (int num=0;num<datanum;num++) {
        for (int i=0;i<dim_h;i++) {
            in_0[num][i]=trans_data[num][i];
            out_0[num][i]=in_0[num][i];
        }
    }
    //第２層
    initialize1(sum,datanum);
    initialize3(in_1,datanum,clas,compo);
    initialize3(out_1,datanum,clas,compo);
    for (int num=0;num<datanum;num++) {
        for (int bef=0;bef<dim_h;bef++) {
            for (int now_class=0;now_class<clas;now_class++) {
                for (int now_compo=0;now_compo<compo;now_compo++) {
                    in_1[num][now_class][now_compo]+=out_0[num][bef]*w[bef][now_class][now_compo];
                }
            }
        }
    }
    for (int num=0;num<datanum;num++) {
        for (int now_class=0;now_class<clas;now_class++) {
            for (int now_compo=0;now_compo<compo;now_compo++) {
                    sum[num]+=exp(in_1[num][now_class][now_compo]);
            }
        }
    }
    for (int num=0;num<datanum;num++) {
        for (int now_class=0;now_class<clas;now_class++) {
            for (int now_compo=0;now_compo<compo;now_compo++) {
                out_1[num][now_class][now_compo]=exp(in_1[num][now_class][now_compo])/sum[num];
            }
        }
    }
    //第３層
    initialize2(in_2,datanum,clas);
    for (int num=0;num<datanum;num++) {
        for (int now_class=0;now_class<clas;now_class++) {
            for (int bef_compo=0;bef_compo<compo;bef_compo++) {
                in_2[num][now_class]+=out_1[num][now_class][bef_compo];
            }
        }
    }
    for (int num=0;num<datanum;num++) {
        printf("データ%d  正解ベクトル:   ",num);
        for (int now=0;now<clas;now++){
            printf("%.4lf   ",data_dis_T[num][now]);
        }
        printf("\n");
        printf("データ%d  出力　　　　:   ",num);
        for (int now=0;now<clas;now++){
            printf("%.4lf   ",in_2[num][now]);
        }
        printf("\n\n");
    }
    
    
    
    
        
        
    free(sum);
    free_data(data_lea, datanum);
    free_data(data_dis, datanum);
    free_data(trans_data, datanum);
    free_data(data_lea_T, datanum);
    free_data(data_dis_T, datanum);
    free_data(in_0, datanum);
    free_data(in_2, datanum);
    free_data(out_0, datanum);
    
    free_w(w, dim_h, clas);
    free_w(in_1, datanum, clas);
    free_w(out_1, datanum, clas);
    free_w(tmp, dim_h, clas);
    
    
    return 0;
}

void input_data(int *num) {
    printf("入力してください:");
    scanf("%d",num);
}

void free_data(double **array, int datanum) {
    for (int i=0;i<datanum;i++) {
        free(array[i]);
    }
    free(array);
}

void free_w(double ***array, int datanum, int clas) {
    for (int i=0;i<datanum;i++) {
        for (int j=0;j<clas;j++) {
            free(array[i][j]);
        }
        free(array[i]);
    }
    free(array);
}
void initialize1(double *array, int first){
    for (int i=0;i<first;i++) {
        array[i]=0;
    }
}
void initialize2(double **array, int first, int second){
    for (int i=0;i<first;i++) {
        for (int j=0;j<second;j++) {
            array[i][j]=0;
        }
    }
}
void initialize3(double ***array, int first, int second, int third){
    for (int i=0;i<first;i++) {
        for (int j=0;j<second;j++) {
            for (int k=0;k<third;k++) {
                array[i][j][k]=0;
            }
        }
    }
}



