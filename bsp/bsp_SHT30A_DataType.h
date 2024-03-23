


#ifndef __DATATYPE_H
#define __DATATYPE_H

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

/*********************************************************************************************************
  ͨ�ú궨��
*********************************************************************************************************/
  /*
#ifndef TRUE
#define TRUE     1ul
#endif                                                               

#ifndef FALSE
#define FALSE    0ul
#endif                                                                 

#ifndef NULL
#define NULL     0ul
#endif 
 */ 
  
#ifndef BIT
#define BIT(x)	(1 << (x))
#endif

/*********************************************************************************************************
  ������������޹ص���������
*********************************************************************************************************/
#define BOOLEAN unsigned char                                            /*  ��������                    */
#define INT8U   unsigned char                                            /*  �޷���8λ���ͱ���           */
#define INT8S   signed   char                                            /*  �з���8λ���ͱ���           */
#define INT16U  unsigned short                                             /*  �޷���16λ���ͱ���          */
#define INT16S  signed   short                                             /*  �з���16λ���ͱ���          */
#define INT32U  unsigned int                                            /*  �޷���32λ���ͱ���          */
#define INT32S  signed   int                                            /*  �з���32λ���ͱ���          */
#define FP32    float                                                    /*  �����ȸ�������32λ���ȣ�    */
#define FP64    double                                                   /*  ˫���ȸ�������64λ���ȣ�    */

#define vid     void                                                     /*  ��                          */
//#define uint32  unsigned int                                            /*  �޷���32λ���ͱ���          */
#define int32s  signed   int                                            /*  �з���32λ���ͱ���          */
#define uint16  unsigned short                                             /*  �޷���16λ���ͱ���          */
#define uint16s signed   short                                             /*  �޷���8λ���ͱ���           */
#define uint8   unsigned char                                            /*  �޷���8λ���ͱ���           */
#define uint8s  signed   char                                            /*  �޷���8λ���ͱ���           */
#define fp32    float                                                    /*  �����ȸ�������32λ���ȣ�    */
#define fp64    double                                                   /*  ˫���ȸ�������64λ���ȣ�    */
	
//#define u32  unsigned int                                            /*  �޷���32λ���ͱ���          */
//#define u32s  signed  int                                            /*  �з���32λ���ͱ���          */
//#define u16  unsigned short                                             /*  �޷���16λ���ͱ���          */
//#define u16s signed   short                                             /*  �޷���8λ���ͱ���           */
//#define u8   unsigned char                                            /*  �޷���8λ���ͱ���           */
//#define u8s  signed   char                                            /*  �޷���8λ���ͱ���           */

/*********************************************************************************************************
  �̼��ṩ�Ĺ��������ͷ�ļ�
*********************************************************************************************************/

/*********************************************************************************************************
  ������Ҫ�����ͷ�ļ�
*********************************************************************************************************/

#ifdef __cplusplus
}
#endif 


#endif