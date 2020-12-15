#ifndef __MD5_H__
#define __MD5_H__ 1
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MD5_CTX
{
  unsigned long  state[4];        
  unsigned long  count[2];        
  unsigned char  data[64];      
} MD5_CTX;

int Init_MD5(MD5_CTX *MD5_ctx);
int Update_MD5(MD5_CTX *MD5_ctx,unsigned char *buffer,unsigned long count);
int Final_MD5(MD5_CTX *MD5_ctx);

#ifdef __cplusplus
}
#endif 
#endif 

