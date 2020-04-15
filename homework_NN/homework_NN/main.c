//
//  main.c
//  homework_NN
//
//  Created by 末廣勇祐 on 2020/04/13.
//  Copyright © 2020 末廣勇祐. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define COUNT_SIZE 1000000

void input_data(int *num);      //データの入力をする関数
void output_w(int innum, int outnum, int mid_width, int mid_height, double ***w);
//重みwを出力する関数
void free_array(int innum, int outnum, int mid_width, int mid_height, double ***w);
void free_array_sum(int innum, int outnum, int datanum, int mid_width, int mid_height, double ***w);
//多次元配列wを解放する関数
double sigmoid(double s);

int main(int argc, const char * argv[]) {
    
    int innum=0, outnum=0, datanum=0, data2num, mid_height=0, mid_width=0;
    FILE *fp;
    int **data, *result;
    double **out, *sum_bef;
    double ***sum_z;
    int para1=0;
    int i=0, j=0;
    double learning=0.0001;
    

    
    printf("入力データ数を");          //入力数を入力
    input_data(&innum);
    printf("出力データ数を");          //出力数を入力
    input_data(&outnum);
    printf("教師データ数を");     //教師データ数を入力
    input_data(&datanum);
    printf("未学習データ数を");     //教師データ数を入力
    input_data(&data2num);
    printf("中間層の素子数を");    //中間層の素子数を入力
    input_data(&mid_height);
    printf("中間層の層数を");      //中間層の層数を入力
    input_data(&mid_width);
    
    fp=fopen("data.txt","r");
    
    if (fp == NULL) {
        printf("ファイルがオープンできません\n");
        return 0;
    }

    data=malloc((datanum+data2num)*sizeof(int *));     //データセット分、領域確保
    for (int i=0;i<datanum+data2num;i++) {
        data[i]=(int *)malloc((innum+1)*sizeof(int *));     //入力数分、領域確保
    }
    result=malloc((datanum+data2num)*sizeof(int *));     //データセット分、領域確保
    out=malloc((datanum+data2num)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum+data2num;i++) {
        out[i]=malloc((outnum)*sizeof(double *));     //入力数分、領域確保
    }
    sum_bef=malloc((innum+1)*sizeof(double *));     //データセット分、領域確保
    sum_z=malloc((datanum)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum;i++) {
        sum_z[i]=malloc((mid_width)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum;i++) {
        for (int j=0;j<mid_width;j++) {
            sum_z[i][j]=malloc((mid_height)*sizeof(double *));     //入力数分、領域確保
        }
    }
    
    
    while (fscanf(fp,"%d", &para1)!=EOF) {
        if (j >= innum+1) {
            result[i]=para1;
            i+=1;
            j=0;
        }
        else {
            if (j==0 ) {
                data[i][j]=1;
                j+=1;
            }
            data[i][j]=para1;
            j+=1;
        }
    }
    
    for (int i=0;i<datanum+data2num;i++) {
        for (int j=0;j<innum+1;j++) {
            printf("%d ",data[i][j]);
        }
        printf("\n");
    }

    //data[0-5][0-3]には教師データ、data[6-7][0-3]には未学習データが格納
    //result[0-5]には教師データ出力、result[6-7]には未学習データ出力が格納
    
    
    double ***mid, ***w, *error;
    mid=malloc((datanum+data2num)*sizeof(double *));     //中間層、領域確保
    for (int i=0;i<datanum+data2num;i++) {
        mid[i]=malloc(mid_width*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum+data2num;i++) {
        for (int j=0;j<mid_width;j++) {
            mid[i][j]=malloc(mid_height*sizeof(double *));     //入力数分、領域確保
        }
    }
    error=malloc((datanum)*sizeof(double *));     //誤差算数、領域確保
    
    int random=0;
    srand((int)time(0));        //乱数を初期化
    
    
    
    w=malloc((mid_width+1)*sizeof(double *));     //重み、領域確保
    w[0]=malloc((innum+1)*sizeof(double *));     //重み、領域確保
    for (int i=0;i<innum+1;i++) {
        w[0][i]=malloc(mid_height*sizeof(double *));     //重み、領域確保
        for (int j=0;j<mid_height;j++) {                //重みwを-1~1の乱数に初期化
            w[0][i][j]=(double)rand()/(double)RAND_MAX;
            random=rand();
            if (random % 2 ==0) {
                w[0][i][j]=-1*w[0][i][j];
            }
        }
    }
    for (int k=1;k<mid_width;k++) {
        w[k]=malloc(mid_height*sizeof(double *));     //重み、領域確保
        for (int i=0;i<mid_height;i++) {
            w[k][i]=malloc(mid_height*sizeof(double *));     //重み、領域確保
            for (int j=0;j<mid_height;j++) {                //重みwを-1~1の乱数に初期化
                w[k][i][j]=(double)rand()/(double)RAND_MAX;
                random=rand();
                if (random % 2 ==0) {
                    w[k][i][j]=-1*w[k][i][j];
                }
            }
        }
    }
    w[mid_width]=malloc(mid_height*sizeof(double *));     //重み、領域確保
    for (int i=0;i<mid_height;i++) {
        w[mid_width][i]=malloc(outnum*sizeof(double *));     //重み、領域確保
        for (int j=0;j<outnum;j++) {                //重みwを-1~1の乱数に初期化
            w[mid_width][i][j]=(double)rand()/(double)RAND_MAX;
            random=rand();
            if (random % 2 ==0) {
                w[mid_width][i][j]=-1*w[mid_width][i][j];
            }
        }
    }
    //w[どのブロックか][どのmidか][どの方向か]
  
    printf("計算中...\n\n");
    
    for (int cnt=0;cnt<COUNT_SIZE;cnt++) {
        int count=0;
        
        //出力Yを求め、誤差関数を求める。
        for (int num=0;num<datanum;num++) {
            for (int k=0;k<mid_width+1;k++) {
                if (k == 0) {
                    for (int i=0;i<mid_width;i++) {     //midを初期化
                        for (int j=0;j<mid_height;j++){
                            mid[num][i][j]=0;
                        }
                    }
                    for (int i=0;i<outnum;i++) {        //outを初期化
                        out[num][i]=0;
                    }
                    for (int i=0;i<innum+1;i++) {
                        for (int j=0;j<mid_height;j++) {
                            mid[num][k][j]+=data[num][i]*w[k][i][j];
                        }
                    }
                }
                else if (k == mid_width) {
                    for (int i=0;i<mid_height;i++) {
                        for (int j=0;j<outnum;j++) {
                            out[num][j]+=mid[num][k-1][i]*w[k][i][j];
                        }
                    }
                    //printf("%d番目のデータ: %lf\n",num,sigmoid(out[num][0]));
                    error[num]=(result[num]-sigmoid(out[num][j]))*(result[num]-sigmoid(out[num][j]));
                }
                else {
                    for (int i=0;i<mid_height;i++) {
                        for (int j=0;j<mid_height;j++) {
                            mid[num][k][j]+=mid[num][k-1][i]*w[k][i][j];
                        }
                    }
                }
            }
            
            if (error[num] < 0.001) {             //収束条件：誤差関数が0.001以下になったら
                count+=1;
                
                if (count == datanum) {
                    count=COUNT_SIZE;
                    break;
                }
                //countに1たす。これが入力数と一致したら収束条件を満たしたことになる。
            }
            
          // else {      //収束条件を満たさない時、重みを更新
                for (int k=mid_width;k>=0;k--) {
                    if (k == mid_width) {
                        
                        for (int i=0;i<mid_height;i++) {
                            sum_z[num][k-1][i]=0;
                        }
                        for (int i=0;i<mid_height;i++) {
                            for (int j=0;j<outnum;j++) {
                                
                                w[k][i][j]=w[k][i][j]-2*learning*sigmoid(mid[num][k-1][i])*sigmoid(out[num][j])*(sigmoid(out[num][j])-result[num])*(1-sigmoid(out[num][j]));
                            
                            }
                        }
                        for (int i=0;i<mid_height;i++) {
                            for (int j=0;j<outnum;j++) {
                                sum_z[num][k-1][i]+=(sigmoid(out[num][j])-result[num])*sigmoid(out[num][j])*(1-sigmoid(out[num][j]))*w[k][i][j];
                                
                                
                            }
                        }

                        
                    }
                    else if (k == 0) {
                        for (int i=0;i<innum+1;i++) {
                            for (int j=0;j<mid_height;j++) {
                                w[k][i][j]=w[k][i][j]-learning*data[num][i]*sigmoid(mid[num][k][j])*(1-sigmoid(mid[num][k][j]))*sum_z[num][k][j];
                                
                        }
                    }
                    }
                    
                    else  {
                        
                        for (int i=0;i<mid_height;i++) {
                            sum_z[num][k-1][i]=0;
                        }
                        
                       
                        for (int i=0;i<mid_height;i++) {
                            for (int j=0;j<mid_height;j++) {
                                w[k][i][j]=w[k][i][j]-learning*sigmoid(mid[num][k-1][i])*sigmoid(mid[num][k][j])*(1-sigmoid(mid[num][k][j]))*sum_z[num][k][j];
                        }
                    }
                        for (int i=0;i<mid_height;i++) {
                            for (int j=0;j<mid_height;j++) {
                                sum_z[num][k-1][i]+=sum_z[num][k][j]*w[k][i][j]*sigmoid(mid[num][k][j])*(1-sigmoid(mid[num][k][j]));
                                
                            }
                        }
                        
                    }
            }
            
        }
    }
 //   }
    
    printf("計算が終了しました。\n");
    
    for (int num=0;num<datanum+data2num;num++) {
        for (int i=0;i<outnum;i++) {        //outを初期化
            out[num][i]=0;
        }
        for (int k=0;k<mid_width+1;k++) {
            if (k == 0) {
                
                for (int i=0;i<mid_width;i++) {     //midを初期化
                    for (int j=0;j<mid_height;j++){
                        mid[num][i][j]=0;
                    }
                }
                
                for (int i=0;i<innum+1;i++) {
                    for (int j=0;j<mid_height;j++) {
                        mid[num][k][j]+=data[num][i]*w[k][i][j];
                    }
                }
            }
            else if (k == mid_width) {
                for (int i=0;i<mid_height;i++) {
                    for (int j=0;j<outnum;j++) {
                        out[num][j]+=mid[num][k-1][i]*w[k][i][j];
                    }
                }
                if (num < datanum) {
                    printf("教師データ %d 出力: %lf\n",num+1,sigmoid(out[num][0]));
                    printf("教師データ %d データ: %d\n\n",num+1,result[num]);
                }
                else {
                    printf("未学習データ %d 出力: %lf\n",num+1-datanum,sigmoid(out[num][0]));
                    printf("未学習データ %d データ: %d\n\n",num+1-datanum,result[num]);
                }
            }
            else {
                for (int i=0;i<mid_height;i++) {
                    for (int j=0;j<mid_height;j++) {
                        mid[num][k][j]+=mid[num][k-1][i]*w[k][i][j];
                    }
                }
            }
        }
    }
    
    
    
    
    
    free_array(innum, outnum, mid_width, mid_height, w);
    free_array_sum(innum, outnum, datanum, mid_width, mid_height, sum_z);
    free_array_sum(innum, outnum, datanum+data2num, mid_width, mid_height, mid);
    
    for (int num=0;num<datanum;num++) {
        free(out[num]);
    }
    
    free(data);
    free(result);
    free(mid);
    free(w);
    free(out);
    free(error);
    free(sum_bef);
    free(sum_z);
    
    fclose(fp);
    
    return 0;
}







void input_data(int *num) {
    printf("入力してください:");
    scanf("%d",num);
}

void output_w(int innum, int outnum, int mid_width, int mid_height, double ***w) {
    for (int k=0;k<mid_width+1;k++) {
        if (k==0) {
            for (int i=0;i<innum+1;i++) {
                for (int j=0;j<mid_height;j++) {
                    printf("%lf ",w[k][i][j]);
                }
                printf("\n");
            }
        }
        else if (k==mid_width) {
            for (int i=0;i<mid_height;i++) {
                for (int j=0;j<outnum;j++) {
                    printf("%lf ",w[k][i][j]);
                }
                printf("\n");
            }
        }
        else {
            for (int i=0;i<mid_height;i++) {
                for (int j=0;j<mid_height;j++) {
                    printf("%lf ",w[k][i][j]);
                }
                printf("\n");
            }
        }
        printf("\n");
    }
}

void free_array(int innum, int outnum, int mid_width, int mid_height, double ***w) {
    for (int i=0;i<innum+1;i++) {
        free(w[0][i]);
    }
    free(w[0]);
    for (int k=1;k<mid_width;k++) {
        for (int i=0;i<mid_height;i++) {
            free(w[k][i]);
        }
        free(w[k]);
    }
    for (int i=0;i<mid_height;i++) {
        free(w[mid_width][i]);
    }
    free(w[mid_width]);

}

void free_array_sum(int innum, int outnum, int datanum, int mid_width, int mid_height, double ***w) {
    
    for (int num=0;num<datanum;num++) {
        for (int i=0;i<mid_width;i++) {
                free(w[num][i]);
        }
         free(w[num]);
    }
    
}

double sigmoid(double s) {
    return 1/(1+exp(-1*s));
}

