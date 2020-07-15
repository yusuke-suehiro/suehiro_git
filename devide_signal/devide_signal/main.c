//
//  main.c
//  homework_LLGMN
//
//  Created by 末廣勇祐 on 2020/04/20.
//  Copyright © 2020 末廣勇祐. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define COUNT_SIZE 500

void input_data(int *num);      //データの入力をする関数
void free_data(double **array, int datanum);    //領域を解放する関数
void free_w(double ***array, int datanum, int clas);    //領域を解放する関数
void initialize1(double *array, int first);     //初期化する関数
void initialize2(double **array, int first, int second);    //初期化する関数
void initialize3(double ***array, int first, int second, int third);    //初期化する関数



int main(int argc, const char * argv[]) {
    
    FILE *fp;
    int compo=0, dim=0, dim_h=0, clas=0, datanum=0, datanum2=0;;
    double **data_lea, **data_dis, **data_lea_T, **data_dis_T, **trans_data, **trans_data2;
    double para1=0, para2=0, para3=0, para4=0;
    int i=0, j=0, flag=0, *sum_flag;
    double ***w;
    double **in_0, ***in_1, **in_2, **out_0, ***out_1, *sum;
    double **in_02, ***in_12, **in_22, **out_02, ***out_12, *sum2;
    int random=0;
    srand((int)time(0));
    double learning=0.01;

    printf("コンポーネント数を");
    input_data(&compo);
    printf("入力次元数を");
    input_data(&dim);
    dim_h=1+dim*(dim+3)/2;
    printf("クラス数を");
    input_data(&clas);
    printf("教師データ数を");
    input_data(&datanum);
    printf("未学習データ数を");
    input_data(&datanum2);
    printf("一括(1)か逐次(2)を");
    input_data(&flag);
    
    //教師データと未学習データを配列に読み込む
    data_lea=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        data_lea[i]=(double *)malloc((dim)*sizeof(double *));     //入力数分、領域確保
    }
    fp=fopen("遷移ありデータ_2.csv","r");      //教師，筋電
    if (fp == NULL) {
        printf("ファイルがオープンできません\n");
        return 0;
    }
    while (fscanf(fp,"%lf,%lf,%lf,%lf", &para1, &para2, &para3, &para4)!=EOF) {
        data_lea[i][j]=para1;
        j+=1;
        data_lea[i][j]=para2;
        j+=1;
        data_lea[i][j]=para3;
        j+=1;
        data_lea[i][j]=para4;
        i+=1;
        j=0;
    }
    fclose(fp);
    data_lea_T=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        data_lea_T[i]=(double *)malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    fp=fopen("遷移あり未学習ありラベル_2.csv","r");        //教師，ラベル
    if (fp == NULL) {
        printf("ファイルがオープンできません\n");
        return 0;
    }
    i=0;
    j=0;
    //while (fscanf(fp,"%lf,%lf,%lf,%lf", &para1, &para2, &para3, &para4)!=EOF) {
    while (fscanf(fp,"%lf,%lf,%lf", &para1, &para2, &para3)!=EOF) {
        data_lea_T[i][j]=para1;
        j+=1;
        data_lea_T[i][j]=para2;
        j+=1;
        data_lea_T[i][j]=para3;
        /*j+=1;
        data_lea_T[i][j]=para4;*/
        i+=1;
        j=0;
    }
    fclose(fp);
    data_dis=malloc((datanum2)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum2;i++) {
        data_dis[i]=(double *)malloc((dim)*sizeof(double *));     //入力数分、領域確保
    }
    fp=fopen("遷移ありデータ_1.csv","r");      //テスト，筋電
    
    if (fp == NULL) {
        printf("ファイルがオープンできません\n");
        return 0;
    }
    i=0;
    j=0;
    while (fscanf(fp,"%lf,%lf,%lf,%lf", &para1, &para2, &para3, &para4)!=EOF) {
        data_dis[i][j]=para1;
        j+=1;
        data_dis[i][j]=para2;
        j+=1;
        data_dis[i][j]=para3;
        j+=1;
        data_dis[i][j]=para4;
        i+=1;
        j=0;
    }
    fclose(fp);
    
    data_dis_T=malloc((datanum2)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum2;i++) {
        data_dis_T[i]=(double *)malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    fp=fopen("遷移あり未学習ありラベル_1.csv","r");        //テスト，ラベル
    if (fp == NULL) {
        printf("ファイルがオープンできません\n");
        return 0;
    }
    i=0;
    j=0;
    //while (fscanf(fp,"%lf,%lf,%lf,%lf", &para1, &para2, &para3, &para4)!=EOF) {
    while (fscanf(fp,"%lf,%lf,%lf", &para1, &para2, &para3)!=EOF) {
        data_dis_T[i][j]=para1;
        j+=1;
        data_dis_T[i][j]=para2;
        j+=1;
        data_dis_T[i][j]=para3;
        /*j+=1;
        data_dis_T[i][j]=para4;*/
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
    sum_flag=malloc((datanum)*sizeof(int *));     //データセット分、領域確保
    
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
                    trans_data[i][j]=data_lea[i][2];
                    break;
                case 4:
                    trans_data[i][j]=data_lea[i][3];
                    break;
                case 5:
                    trans_data[i][j]=data_lea[i][0]*data_lea[i][0];
                    break;
                case 6:
                    trans_data[i][j]=data_lea[i][0]*data_lea[i][1];
                    break;
                case 7:
                    trans_data[i][j]=data_lea[i][0]*data_lea[i][2];
                    break;
                case 8:
                    trans_data[i][j]=data_lea[i][0]*data_lea[i][3];
                    break;
                case 9:
                    trans_data[i][j]=data_lea[i][1]*data_lea[i][1];
                    break;
                case 10:
                    trans_data[i][j]=data_lea[i][1]*data_lea[i][2];
                    break;
                case 11:
                    trans_data[i][j]=data_lea[i][1]*data_lea[i][3];
                    break;
                case 12:
                    trans_data[i][j]=data_lea[i][2]*data_lea[i][2];
                    break;
                case 13:
                    trans_data[i][j]=data_lea[i][2]*data_lea[i][3];
                    break;
                case 14:
                    trans_data[i][j]=data_lea[i][3]*data_lea[i][3];
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
            printf("count %d  誤差関数%lf\n",count, J);
            //wを更新
            initialize3(tmp,dim_h,clas,compo);
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
                        if (k == 3 && m == 1) {
                            w[h][k][m]=0;
                        }
                        else {
                            w[h][k][m]=w[h][k][m]-learning*tmp[h][k][m];
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
              printf("count %d  誤差関数%lf\n",count, J);
              if (J < 15){
                  count=COUNT_SIZE;
              }
              //wを更新
              for (int num=0;num<datanum;num++) {
                  sum_flag[num]=0;
              }
              int ran=0;
              initialize3(tmp,dim_h,clas,compo);
              for (int h=0;h<dim_h;h++) {
                  for (int num=0;num<datanum;num++) {
                      for (int k=0;k<clas;k++) {
                          for (int m=0;m<compo;m++) {
                              
                              ran=(int)rand()%(int)datanum;
                              /*do {
                                  ran+=1;
                                  if (ran == datanum) {
                                      ran=0;
                                  }
                              } while (sum_flag[ran]==1);*/
                              
                              
                              tmp[h][k][m]=(in_2[ran][k]-data_lea_T[ran][k])*out_1[ran][k][m]*trans_data[ran][h]/in_2[ran][k];
                              
                              if (k == 3 && m == 1) {
                                  w[h][k][m]=0;
                              }
                              else {
                                  w[h][k][m]=w[h][k][m]-learning*tmp[h][k][m];
                              }
                          }
                       }
                  }
              }
          }
          printf("計算が終了しました\n\n");
      }
    
    //未学習データを識別する
    in_02=malloc((datanum2)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum2;i++) {
        in_02[i]=(double *)malloc((dim_h)*sizeof(double *));     //入力数分、領域確保
    }
    in_22=malloc((datanum2)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum2;i++) {
        in_22[i]=(double *)malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    out_02=malloc((datanum2)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum2;i++) {
        out_02[i]=(double *)malloc((dim_h)*sizeof(double *));     //入力数分、領域確保
    }
    initialize2(in_02, datanum2, dim_h);
    initialize2(in_22, datanum2, clas);
    initialize2(out_02, datanum2, dim_h);
    in_12=malloc((datanum2)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum2;i++) {
        in_12[i]=malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum2;i++) {
        for (int j=0;j<clas;j++) {
            in_12[i][j]=malloc((compo)*sizeof(double *));     //入力数分、領域確保
        }
    }
    out_12=malloc((datanum2)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum2;i++) {
        out_12[i]=malloc((clas)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum2;i++) {
        for (int j=0;j<clas;j++) {
            out_12[i][j]=malloc((compo)*sizeof(double *));     //入力数分、領域確保
        }
    }
    initialize3(in_12, datanum2, clas, compo);
    initialize3(out_12, datanum2, clas, compo);
    sum2=malloc((datanum2)*sizeof(double *));     //データセット分、領域確保
    trans_data2=malloc((datanum2)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum2;i++) {
        trans_data2[i]=(double *)malloc((dim_h)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum2;i++) {
        for (int j=0;j<dim_h;j++) {
            switch (j) {
                case 0:
                    trans_data2[i][j]=1;
                    break;
                case 1:
                    trans_data2[i][j]=data_dis[i][0];
                    break;
                case 2:
                    trans_data2[i][j]=data_dis[i][1];
                    break;
                case 3:
                    trans_data2[i][j]=data_dis[i][2];
                    break;
                case 4:
                    trans_data2[i][j]=data_dis[i][3];
                    break;
                case 5:
                    trans_data2[i][j]=data_dis[i][0]*data_dis[i][0];
                    break;
                case 6:
                    trans_data2[i][j]=data_dis[i][0]*data_dis[i][1];
                    break;
                case 7:
                    trans_data2[i][j]=data_dis[i][0]*data_dis[i][2];
                    break;
                case 8:
                    trans_data2[i][j]=data_dis[i][0]*data_dis[i][3];
                    break;
                case 9:
                    trans_data2[i][j]=data_dis[i][1]*data_dis[i][1];
                    break;
                case 10:
                    trans_data2[i][j]=data_dis[i][1]*data_dis[i][2];
                    break;
                case 11:
                    trans_data2[i][j]=data_dis[i][1]*data_dis[i][3];
                    break;
                case 12:
                    trans_data2[i][j]=data_dis[i][2]*data_dis[i][2];
                    break;
                case 13:
                    trans_data2[i][j]=data_dis[i][2]*data_dis[i][3];
                    break;
                case 14:
                    trans_data2[i][j]=data_dis[i][3]*data_dis[i][3];
                    break;
            }
        }
    }
    //第１層
    for (int num=0;num<datanum2;num++) {
        for (int i=0;i<dim_h;i++) {
            in_02[num][i]=trans_data2[num][i];
            out_02[num][i]=in_02[num][i];
        }
    }
    //第２層
    initialize1(sum2,datanum2);
    initialize3(in_12,datanum2,clas,compo);
    initialize3(out_12,datanum2,clas,compo);
    for (int num=0;num<datanum2;num++) {
        for (int bef=0;bef<dim_h;bef++) {
            for (int now_class=0;now_class<clas;now_class++) {
                for (int now_compo=0;now_compo<compo;now_compo++) {
                    in_12[num][now_class][now_compo]+=out_02[num][bef]*w[bef][now_class][now_compo];
                }
            }
        }
    }
    for (int num=0;num<datanum2;num++) {
        for (int now_class=0;now_class<clas;now_class++) {
            for (int now_compo=0;now_compo<compo;now_compo++) {
                    sum2[num]+=exp(in_12[num][now_class][now_compo]);
            }
        }
    }
    for (int num=0;num<datanum2;num++) {
        for (int now_class=0;now_class<clas;now_class++) {
            for (int now_compo=0;now_compo<compo;now_compo++) {
                out_12[num][now_class][now_compo]=exp(in_12[num][now_class][now_compo])/sum2[num];
            }
        }
    }
    //第３層
    initialize2(in_22,datanum2,clas);
    for (int num=0;num<datanum2;num++) {
        for (int now_class=0;now_class<clas;now_class++) {
            for (int bef_compo=0;bef_compo<compo;bef_compo++) {
                in_22[num][now_class]+=out_12[num][now_class][bef_compo];
            }
        }
    }
    double per=0;
    fp=fopen("result_LL.csv","w");
    fprintf(fp,"%s,%s,%s,%s,%s,%s,%s","","answer","","","","","output\n");
    fprintf(fp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s","","ch1","ch2","ch3","ch4","","ch1","ch2","ch3","ch4\n");
    for (int num=0;num<datanum2;num++) {
        int max_num=0;
        double max=0;
        fprintf(fp,"%d,",num);
        for (int now=0;now<clas;now++){
            fprintf(fp,"%.4lf,",data_dis_T[num][now]);
        }
        fprintf(fp,"%s,","");
        for (int now=0;now<clas;now++){
            fprintf(fp,"%.4lf,",in_22[num][now]);
        }
        fprintf(fp,"%s","\n");
        for (int now=0;now<clas;now++) {
            if (max < in_22[num][now]) {
                max=in_22[num][now];
                max_num=now;
            }
        }
        if (data_dis_T[num][max_num] == 1) {
            per+=1;
        }
    }
    
    printf("識別率 : %.2lf %%\n",per/datanum2*100);
    
    fclose(fp);
    
    
        
        
    free(sum);
    free_data(data_lea, datanum);
    free_data(data_dis, datanum2);
    free_data(trans_data, datanum);
    free_data(trans_data2, datanum2);
    free_data(data_lea_T, datanum);
    free_data(data_dis_T, datanum2);
    free_data(in_02, datanum2);
    free_data(in_22, datanum2);
    free_data(out_02, datanum2);
    free_data(in_0, datanum);
    free_data(in_2, datanum);
    free_data(out_0, datanum);
    
    free_w(w, dim_h, clas);
    free_w(in_1, datanum, clas);
    free_w(out_1, datanum, clas);
    free_w(in_12, datanum2, clas);
    free_w(out_12, datanum2, clas);
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



