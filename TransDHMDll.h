
#ifdef TRANSDHMDLL_EXPORTS
#define TRANSDHMDLL_API __declspec(dllexport)
#else
#define TRANSDHMDLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
TRANSDHMDLL_API bool InitTransDHM(char *RefImage,double dx=0,double dy=0); //API��ʼ����RefImage:����ͼ���ļ���,x��y������������Ĭ��Ϊ1
TRANSDHMDLL_API bool SetParameter(int outwidth,int outheight,void *reversed=NULL);
//���ô����������������ȡ��߶ȣ�reversed������������չ�ã�ĿǰΪNULL
TRANSDHMDLL_API bool SystemCalib(char *RefImage);//ϵͳУ׼��RefImage:����ͼ���ļ���
TRANSDHMDLL_API void ReleaseTransDHM();
//�ͷ�API
TRANSDHMDLL_API bool HoloPHase(unsigned char *hologram,double *HoloPHase,double *ImagePh=NULL);
//�����λ��ȫϢͼ�������λ�����ǿ��
#ifdef __cplusplus
}
#endif