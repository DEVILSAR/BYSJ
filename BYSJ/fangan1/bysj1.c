#include <stdio.h>
#include <stdlib.h>

int G[100] = {2,4,8,16,32,64,128,256};
int g = 2;
int res = 0;
int M = 1010011010;
//定义乘法循环群G和生成元g

int Enc(int, int)//对称加密算法AES
{
     return M;
}
int bloom_filter(int t)//定义布隆过滤器查询函数
{   
     return(res);
}
int H(int M)//定义抗碰撞的SHA-256哈希函数
{
     return M;
}

int main()
{
     printf("请上传txt文件:");
     
     int M;
     
     scanf("%d", &M);
     
     int CM;
     int s = rand();
     int u = rand();
     int t = g^H(M);//用户上传数据明文M前，首先计算TM去重检测标签
     
     bloom_filter(t);//将TM送至TTS后计算特征值t并用布隆过滤器查询标签对应值

     if(res == 0)
     {
        printf("这是第一次上传该文件\n");
        
        int K = H(u);//用户随机选取参数u，并生成加密密钥K
        int CM = Enc(K,M);//利用对称加密算法AES计算数据密文，并发送至CSS存储
        int Kc = s ^ K;//TTS随机选取参数s, 生成密钥密文Kc,并发送至CSS存储
        
        printf("用户上传数据成功!\n");
     }
     else
     {   
        printf("该数据为重复数据\n用户上传数据成功!\n云服务器去重成功!\n物理指针创建成功!\n");
        printf("指针p地址为:%d", &CM);
     }

     return 0;
}