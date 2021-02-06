
#ifdef TRANSDHMDLL_EXPORTS
#define TRANSDHMDLL_API __declspec(dllexport)
#else
#define TRANSDHMDLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
TRANSDHMDLL_API bool InitTransDHM(char *RefImage,double dx=0,double dy=0); //API初始化。RefImage:背景图像文件名,x、y方向采样间隔，默认为1
TRANSDHMDLL_API bool SetParameter(int outwidth,int outheight,void *reversed=NULL);
//设置处理参数。输出结果宽度、高度；reversed保留参数，扩展用，目前为NULL
TRANSDHMDLL_API bool SystemCalib(char *RefImage);//系统校准，RefImage:背景图像文件名
TRANSDHMDLL_API void ReleaseTransDHM();
//释放API
TRANSDHMDLL_API bool HoloPHase(unsigned char *hologram,double *HoloPHase,double *ImagePh=NULL);
//求解相位。全息图、输出相位、输出强度
#ifdef __cplusplus
}
#endif