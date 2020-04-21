//
//  main.c
//  homework_NN2
//
//  Created by 末廣勇祐 on 2020/04/16.
//  Copyright © 2020 末廣勇祐. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define COUNT_SIZE 20000

void input_data(int *num);      //データの入力をする関数
void output_w(int innum, int outnum, int mid_width, int mid_height, double ***w);
//重みwを出力する関数
void free_array(int innum, int outnum, int mid_width, int mid_height, double ***w);
void free_array_sum(int innum, int outnum, int datanum, int mid_width, int mid_height, double ***w);
//多次元配列wを解放する関数
double sigmoid(double s);
void fill_v(double ***array, int i, int j, int k, double num);



int main(int argc, const char * argv[]) {
    
    int innum=0, outnum=0, datanum=0, data2num, mid_height=0, mid_width=0;
    FILE *fp;
    int **data, *result;
    double ***sum_z, ***sum_u;
    int para1=0, flag=0;
    int i=0, j=0;
    double learning=0.01;
    
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
    printf("一括(1)、逐次(2)を");      //中間層の層数を入力
    input_data(&flag);
    
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
    sum_z=malloc((datanum+data2num)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum+data2num;i++) {
        sum_z[i]=malloc((mid_width+1)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum+data2num;i++) {
        for (int j=0;j<mid_width+1;j++) {
            sum_z[i][j]=malloc((mid_height+1)*sizeof(double *));     //入力数分、領域確保
        }
    }
    sum_u=malloc((datanum+data2num)*sizeof(double *));     //データセット分、領域確保
    for (int i=0;i<datanum+data2num;i++) {
        sum_u[i]=malloc((mid_width+2)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum+data2num;i++) {
        for (int j=0;j<mid_width+2;j++) {
            sum_u[i][j]=malloc((mid_height+1)*sizeof(double *));     //入力数分、領域確保
        }
    }
    
    double ***mid, ***w, *error;
    
    mid=malloc((datanum+data2num)*sizeof(double *));     //中間層、領域確保
    for (int i=0;i<datanum+data2num;i++) {
        mid[i]=malloc((mid_width+2)*sizeof(double *));     //入力数分、領域確保
    }
    for (int i=0;i<datanum+data2num;i++) {
        for (int j=0;j<mid_width+2;j++) {
            mid[i][j]=malloc((mid_height+1)*sizeof(double *));     //入力数分、領域確保
        }
    }
    error=malloc((datanum)*sizeof(double *));     //誤差算数、領域確保
    
    int random=0;
    srand((int)time(0));
    
    w=malloc((mid_width+1)*sizeof(double *));     //重み、領域確保
    w[0]=malloc((innum+1)*sizeof(double *));     //重み、領域確保
    for (int i=0;i<innum+1;i++) {
        w[0][i]=malloc((mid_height+1)*sizeof(double *));     //重み、領域確保
        for (int j=0;j<mid_height+1;j++) {                //重みwを-1~1の乱数に初期化
            w[0][i][j]=(double)rand()/(double)RAND_MAX;
            random=rand();
            if (random % 2 ==0) {
                w[0][i][j]=-1*w[0][i][j];
            }
        }
    }
    for (int k=1;k<mid_width+1;k++) {
        w[k]=malloc((mid_height+1)*sizeof(double *));     //重み、領域確保
        for (int i=0;i<mid_height+1;i++) {
            w[k][i]=malloc((mid_height+1)*sizeof(double *));     //重み、領域確保
            for (int j=0;j<mid_height+1;j++) {                //重みwを-1~1の乱数に初期化
                w[k][i][j]=(double)rand()/(double)RAND_MAX;
                random=rand();
                if (random % 2 ==0) {
                    w[k][i][j]=-1*w[k][i][j];
                }
            }
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
    
    if (flag==1) {      //一括学習
        printf("計算中...\n\n");
        
        
        for (int cnt=0;cnt<COUNT_SIZE;cnt++) {
            int count=0;
            double eor=0;
            
            //初期化
            for (int num=0;num<datanum+data2num;num++) {
                for (int i=0;i<mid_width+1;i++) {
                    for (int j=0;j<mid_height+1;j++) {
                        
                        sum_z[num][i][j]=0;
                    }
                }
            }
            for (int num=0;num<datanum+data2num;num++) {
                for (int i=0;i<mid_width+2;i++) {
                    for (int j=0;j<mid_height+1;j++) {
                        sum_u[num][i][j]=0;
                        if (j==0) {
                            if (i<mid_width+1) {
                                mid[num][i][j]=1;
                            }
                            else {
                                mid[num][i][j]=0;
                            }
                        }
                        else {
                            mid[num][i][j]=0;
                        }
                    }
                }
            }
            
            
            //順方向伝搬
            for (int num=0;num<datanum;num++) {
                for (int layer=0;layer<mid_width+2;layer++) {
                    if (layer==0) {
                        for (int now=0;now<innum+1;now++) {
                            mid[num][layer][now]=(double)data[num][now];
                            
                        }
                        for (int now=0;now<innum+1;now++) {
                            for (int next=1;next<mid_height+1;next++) {
                                sum_u[num][layer+1][next]+=mid[num][layer][now]*w[layer][now][next];
                            }
                        }
                    }
                    else {
                        if (layer < mid_width+1 ) {
                            for (int now=1;now<mid_height+1;now++) {
                                mid[num][layer][now]=sigmoid(sum_u[num][layer][now]);
                            }
                            for (int now=0;now<mid_height+1;now++) {
                                for (int next=1;next<mid_height+1;next++) {
                                    sum_u[num][layer+1][next]+=mid[num][layer][now]*w[layer][now][next];
                                }
                            }
                        }
                        else {
                            mid[num][layer][1]=sigmoid(sum_u[num][layer][1]);
                            eor+=(mid[num][mid_width+1][1]-result[num])*(mid[num][mid_width+1][1]-result[num]);
                            
                        }
                    }
                }
            }
                //printf("cnt %d : %lf\n",cnt,eor);
            
                for (int num=0;num<datanum;num++) {
                    for (int layer=0;layer<mid_width+2;layer++) {
                    
                    
                    if (eor < 0.001) {
                        count+=1;
                        if (count==datanum) {
                            
                            cnt=COUNT_SIZE;
                        }
                    }
                    
                    //逆伝搬
                    for (int layer=mid_width+1;layer>0;layer--) {
                        if (layer==mid_width+1) {
                            for (int next=0;next<mid_height+1;next++) {
                                //sum_z[num][layer-1][next]+=(mid[num][layer][1]-result[num])*mid[num][layer][1]*(1-mid[num][layer][1])*w[layer-1][next][1];
                                sum_z[num][layer-1][next]+=(mid[num][layer][1]-result[num])*mid[num][layer][1]*(1-mid[num][layer][1])*w[layer-1][next][1];
                            }
                            for (int next=0;next<mid_height+1;next++) {
                                double tmp=0;
                                
                                tmp+=(mid[num][layer][1]-result[num])*mid[num][layer-1][next]*mid[num][layer][1]*(1-mid[num][layer][1]);
                                
                                w[layer-1][next][1]=w[layer-1][next][1]-2*learning*tmp;
                            }
                        }
                        else {
                            if (layer == 1) {
                                for (int now=1;now<mid_height+1;now++) {
                                    for (int next=0;next<innum+1;next++) {
                                        sum_z[num][layer-1][next]+=sum_z[num][layer][now]*w[layer-1][next][now]*mid[num][layer][now]*(1-mid[num][layer][now]);
                                    }
                                }
                                for (int now=1;now<mid_height+1;now++) {
                                    for (int next=0;next<innum+1;next++) {
                                        double tmp=0;
                                        
                                        tmp+=sum_z[num][layer][now]*mid[num][layer-1][next]*mid[num][layer][now]*(1-mid[num][layer][now]);
                                        w[layer-1][next][now]=w[layer-1][next][now]-learning*tmp;
                                    }
                                }
                            }
                            else {
                                for (int now=1;now<mid_height+1;now++) {
                                    for (int next=0;next<mid_height+1;next++) {
                                        sum_z[num][layer-1][next]+=sum_z[num][layer][now]*w[layer-1][next][now]*mid[num][layer][now]*(1-mid[num][layer][now]);
                                    }
                                }
                                for (int now=1;now<mid_height+1;now++) {
                                    for (int next=0;next<innum+1;next++) {
                                        double tmp=0;
                                        tmp+=sum_z[num][layer][now]*mid[num][layer-1][next]*mid[num][layer][now]*(1-mid[num][layer][now]);
                                        w[layer-1][next][now]=w[layer-1][next][now]-learning*tmp;
                                    }
                                }
                            }
                        }
                            
                    }
                }
                }
        }
        
        
        printf ("計算が終了しました\n\n");
        printf("一括学習 結果\n\n");
        
        
        //初期化
        for (int num=0;num<datanum+data2num;num++) {
            for (int i=0;i<mid_width+1;i++) {
                for (int j=0;j<mid_height+1;j++) {
                    
                    sum_z[num][i][j]=0;
                }
            }
        }
        for (int num=0;num<datanum+data2num;num++) {
            for (int i=0;i<mid_width+2;i++) {
                for (int j=0;j<mid_height+1;j++) {
                    sum_u[num][i][j]=0;
                    if (j==0) {
                        if (i<mid_width+1) {
                            mid[num][i][j]=1;
                        }
                        else {
                            mid[num][i][j]=0;
                        }
                    }
                    else {
                        mid[num][i][j]=0;
                    }
                }
            }
        }
        
        for (int num=0;num<datanum+data2num;num++) {
            for (int layer=0;layer<mid_width+2;layer++) {
                if (layer==0) {
                    for (int now=0;now<innum+1;now++) {
                        mid[num][layer][now]=(double)data[num][now];
                    }
                    for (int now=0;now<innum+1;now++) {
                        for (int next=1;next<mid_height+1;next++) {
                            sum_u[num][layer+1][next]+=mid[num][layer][now]*w[layer][now][next];
                        }
                    }
                }
                else {
                    if (layer < mid_width+1 ) {
                        for (int now=1;now<mid_height+1;now++) {
                            mid[num][layer][now]=sigmoid(sum_u[num][layer][now]);
                        }
                        for (int now=0;now<mid_height+1;now++) {
                            for (int next=1;next<mid_height+1;next++) {
                                sum_u[num][layer+1][next]+=mid[num][layer][now]*w[layer][now][next];
                            }
                        }
                    }
                    else {
                        mid[num][layer][1]=sigmoid(sum_u[num][layer][1]);
                        if (num < datanum) {
                            printf("教師データ 出力　 %d :%lf\n",num+1,mid[num][layer][1]);
                            printf("教師データ データ %d :%d\n\n",num+1,result[num]);
                        }
                        else {
                            printf("未学習データ 出力　 %d :%lf\n",num-datanum+1,mid[num][layer][1]);
                            printf("未学習データ データ %d :%d\n\n",num-datanum+1,result[num]);
                        }
                    }
            }
        }
        }
}
        else  {         //逐次学習
            printf("計算中...\n\n");
            
            
            for (int cnt=0;cnt<COUNT_SIZE;cnt++) {
                
                int count=0;
                double eor=0;
                
                //初期化
                for (int num=0;num<datanum+data2num;num++) {
                    for (int i=0;i<mid_width+1;i++) {
                        for (int j=0;j<mid_height+1;j++) {
                            
                            sum_z[num][i][j]=0;
                        }
                    }
                }
                for (int num=0;num<datanum+data2num;num++) {
                    for (int i=0;i<mid_width+2;i++) {
                        for (int j=0;j<mid_height+1;j++) {
                            sum_u[num][i][j]=0;
                            if (j==0) {
                                if (i<mid_width+1) {
                                    mid[num][i][j]=1;
                                }
                                else {
                                    mid[num][i][j]=0;
                                }
                            }
                            else {
                                mid[num][i][j]=0;
                            }
                        }
                    }
                }
                
                
                //順方向伝搬
                for (int num=0;num<datanum;num++) {
                    for (int layer=0;layer<mid_width+2;layer++) {
                        if (layer==0) {
                            for (int now=0;now<innum+1;now++) {
                                mid[num][layer][now]=(double)data[num][now];
                                
                            }
                            for (int now=0;now<innum+1;now++) {
                                for (int next=1;next<mid_height+1;next++) {
                                    sum_u[num][layer+1][next]+=mid[num][layer][now]*w[layer][now][next];
                                }
                            }
                        }
                        else {
                            if (layer < mid_width+1 ) {
                                for (int now=1;now<mid_height+1;now++) {
                                    mid[num][layer][now]=sigmoid(sum_u[num][layer][now]);
                                }
                                for (int now=0;now<mid_height+1;now++) {
                                    for (int next=1;next<mid_height+1;next++) {
                                        sum_u[num][layer+1][next]+=mid[num][layer][now]*w[layer][now][next];
                                    }
                                }
                            }
                            else {
                                mid[num][layer][1]=sigmoid(sum_u[num][layer][1]);
                                eor+=(mid[num][mid_width+1][1]-result[num])*(mid[num][mid_width+1][1]-result[num]);
                                
                            }
                        }
                    }
                
                    //printf("cnt %d : %lf\n",cnt,eor);
                
                    
                        for (int layer=0;layer<mid_width+2;layer++) {
                        
                        
                        if (eor < 0.001) {
                            count+=1;
                            
                        }
                        
                        //逆伝搬
                        for (int layer=mid_width+1;layer>0;layer--) {
                            if (layer==mid_width+1) {
                                for (int next=0;next<mid_height+1;next++) {
                                    //sum_z[num][layer-1][next]+=(mid[num][layer][1]-result[num])*mid[num][layer][1]*(1-mid[num][layer][1])*w[layer-1][next][1];
                                    sum_z[num][layer-1][next]+=(mid[num][layer][1]-result[num])*mid[num][layer][1]*(1-mid[num][layer][1])*w[layer-1][next][1];
                                }
                                for (int next=0;next<mid_height+1;next++) {
                                    double tmp=0;
                                    
                                    tmp+=(mid[num][layer][1]-result[num])*mid[num][layer-1][next]*mid[num][layer][1]*(1-mid[num][layer][1]);
                                    
                                    w[layer-1][next][1]=w[layer-1][next][1]-2*learning*tmp;
                                }
                            }
                            else {
                                if (layer == 1) {
                                    for (int now=1;now<mid_height+1;now++) {
                                        for (int next=0;next<innum+1;next++) {
                                            sum_z[num][layer-1][next]+=sum_z[num][layer][now]*w[layer-1][next][now]*mid[num][layer][now]*(1-mid[num][layer][now]);
                                        }
                                    }
                                    for (int now=1;now<mid_height+1;now++) {
                                        for (int next=0;next<innum+1;next++) {
                                            double tmp=0;
                                            
                                            tmp+=sum_z[num][layer][now]*mid[num][layer-1][next]*mid[num][layer][now]*(1-mid[num][layer][now]);
                                            w[layer-1][next][now]=w[layer-1][next][now]-learning*tmp;
                                        }
                                    }
                                }
                                else {
                                    for (int now=1;now<mid_height+1;now++) {
                                        for (int next=0;next<mid_height+1;next++) {
                                            sum_z[num][layer-1][next]+=sum_z[num][layer][now]*w[layer-1][next][now]*mid[num][layer][now]*(1-mid[num][layer][now]);
                                        }
                                    }
                                    for (int now=1;now<mid_height+1;now++) {
                                        for (int next=0;next<innum+1;next++) {
                                            double tmp=0;
                                            tmp+=sum_z[num][layer][now]*mid[num][layer-1][next]*mid[num][layer][now]*(1-mid[num][layer][now]);
                                            w[layer-1][next][now]=w[layer-1][next][now]-learning*tmp;
                                        }
                                    }
                                }
                            }
                                
                        }
                    }
                    }
               // printf("cnt %d : %lf\n",cnt,eor);
                if (count==datanum) {
                    cnt=COUNT_SIZE;
                }
            }
            
            
            printf ("計算が終了しました\n\n");
            printf("逐次学習 結果\n\n");
            
            
            //初期化
            for (int num=0;num<datanum+data2num;num++) {
                for (int i=0;i<mid_width+1;i++) {
                    for (int j=0;j<mid_height+1;j++) {
                        
                        sum_z[num][i][j]=0;
                    }
                }
            }
            for (int num=0;num<datanum+data2num;num++) {
                for (int i=0;i<mid_width+2;i++) {
                    for (int j=0;j<mid_height+1;j++) {
                        sum_u[num][i][j]=0;
                        if (j==0) {
                            if (i<mid_width+1) {
                                mid[num][i][j]=1;
                            }
                            else {
                                mid[num][i][j]=0;
                            }
                        }
                        else {
                            mid[num][i][j]=0;
                        }
                    }
                }
            }
            
            for (int num=0;num<datanum+data2num;num++) {
                for (int layer=0;layer<mid_width+2;layer++) {
                    if (layer==0) {
                        for (int now=0;now<innum+1;now++) {
                            mid[num][layer][now]=(double)data[num][now];
                        }
                        for (int now=0;now<innum+1;now++) {
                            for (int next=1;next<mid_height+1;next++) {
                                sum_u[num][layer+1][next]+=mid[num][layer][now]*w[layer][now][next];
                            }
                        }
                    }
                    else {
                        if (layer < mid_width+1 ) {
                            for (int now=1;now<mid_height+1;now++) {
                                mid[num][layer][now]=sigmoid(sum_u[num][layer][now]);
                            }
                            for (int now=0;now<mid_height+1;now++) {
                                for (int next=1;next<mid_height+1;next++) {
                                    sum_u[num][layer+1][next]+=mid[num][layer][now]*w[layer][now][next];
                                }
                            }
                        }
                        else {
                            mid[num][layer][1]=sigmoid(sum_u[num][layer][1]);
                            if (num < datanum) {
                                printf("教師データ 出力　 %d :%lf\n",num+1,mid[num][layer][1]);
                                printf("教師データ データ %d :%d\n\n",num+1,result[num]);
                            }
                            else {
                                printf("未学習データ 出力　 %d :%lf\n",num-datanum+1,mid[num][layer][1]);
                                printf("未学習データ データ %d :%d\n\n",num-datanum+1,result[num]);
                            }
                        }
                }
            }
            }
    }


        
        
        
        
        
        
        
        
        
        
        
        
        
        
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    free_array(innum, outnum, mid_width, mid_height, w);
    free_array_sum(innum, outnum, datanum, mid_width, mid_height, sum_z);
    free_array_sum(innum, outnum, datanum+data2num, mid_width, mid_height, mid);
    
    free(data);
    free(result);
    free(mid);
    free(w);
    free(error);
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
    return 1.0/(1.0+exp(-1*s));
}

void fill_v(double ***array, int i, int j, int k, double num) {
    for (int x=0;x<i;x++) {
        for (int y=0;y<j;y++) {
            for (int z=0;z<k;z++) {
                array[x][y][z]=num;
            }
        }
    }
}
