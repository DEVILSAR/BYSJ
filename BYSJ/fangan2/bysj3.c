#include <stdio.h>
#include <stdlib.h>

int G[100] = {2,4,8,16,32,64,128,256};
int g = 2;
int res = 1;
int M = 1010011010;
//定义乘法循环群G和生成元g
int T = 100;//定义流行度阈值T


int Enc(int, int)//对称加密算法AES
{
    return M;
}
int E(int, int)//非对称加密算法RSA
{
     return M;
}
int bloom_filter(int t)//定义布隆过滤器查询函数
{   
     int res = 0;
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
     int y = 59;
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
     else if(res == 1&&y >= T)
     {   
      
          int K = H(u);//用户随机选取参数u，并生成加密密钥K
          int CM = E(K,M);//利用非对称加密算法RSA计算数据密文，并发送至CSS存储
          int Kc = s ^ K;//TTS随机选取参数s, 生成密钥密文Kc,并发送至CSS存储
          printf("该数据为流行数据\n用户数据上传成功!\n云服务器去重成功!\n物理指针创建成功!\n");
          printf("指针p地址为:%d", &CM);
     }
     else if(res == 1&&y < T)
     {
          int K = H(u);//用户随机选取参数u，并生成加密密钥K
          int CM = Enc(K,M);//利用对称加密算法AES计算数据密文，并发送至CSS存储
          int Kc = s ^ K;//TTS随机选取参数s, 生成密钥密文Kc,并发送至CSS存储
            
          printf("该数据为非流行数据\n用户数据上传成功!\n云服务器去重成功!\n物理指针创建成功!\n");
          printf("指针p地址为:%d", &CM);
     }
     return 0;

}