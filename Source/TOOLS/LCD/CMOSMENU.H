#define clenID    3
#define clenIE    6
#define clenIR    2
#define clenKT    3
#define clenLA    4
#define clenLE    4
#define clenLN    4
#define clenLP    2
#define clenLS    2
#define clenPR    2
#define clenPU    2
#define clenTV    2
#define clenTL    2
#define clenTM    2
#define clenTR    2
#define clenCMOS    16
const char * const clabID[] = { 0,clab0003,clab0004 };
const char * const clabIE[] = { 0,clab0001,clab0013,clab0012,clab0010,clab0011 };
const char * const clabIR[] = { 0,clab0002 };
const char * const clabKT[] = { 0,clab0005,clab0005 };
const char * const clabLA[] = { 0,clab0014,clab000D,clab000C };
const char * const clabLE[] = { 0,clab000B,clab0015,clab000A };
const char * const clabLN[] = { 0,clab0017,clab0016,clab0009 };
const char * const clabLP[] = { 0,clab0006 };
const char * const clabLS[] = { 0,clab000F };
const char * const clabPR[] = { 0,clab0000 };
const char * const clabPU[] = { 0,clab0016 };
const char * const clabTV[] = { 0,clab0008 };
const char * const clabTL[] = { 0,clab0014 };
const char * const clabTM[] = { 0,clab000E };
const char * const clabTR[] = { 0,clab0007 };
const char * const citemID[] = { "#$","��1","��2" };
const char * const citemIE[] = { "#$","��1","��10","��11","��3","��4" };
const char * const citemIR[] = { "#$","��2" };
const char * const citemKT[] = { "#$","��1","��3" };
const char * const citemLA[] = { "#$","��7","��8","��9" };
const char * const citemLE[] = { "#$","��10","��5","��6" };
const char * const citemLN[] = { "#$","��1","��2","��3" };
const char * const citemLP[] = { "#$","��12" };
const char * const citemLS[] = { "#$","��2" };
const char * const citemPR[] = { "#$","��1" };
const char * const citemPU[] = { "#$","��7" };
const char * const citemTV[] = { "#$","��1" };
const char * const citemTL[] = { "#$","��1" };
const char * const citemTM[] = { "#$","��2" };
const char * const citemTR[] = { "#$","��2" };
const MENU cmnuID = { citemID, 0x80, clenID, (MENU **) clabID };
const MENU cmnuIE = { citemIE, 0x80, clenIE, (MENU **) clabIE };
const MENU cmnuIR = { citemIR, 0x80, clenIR, (MENU **) clabIR };
const MENU cmnuKT = { citemKT, 0x80, clenKT, (MENU **) clabKT };
const MENU cmnuLA = { citemLA, 0x80, clenLA, (MENU **) clabLA };
const MENU cmnuLE = { citemLE, 0x80, clenLE, (MENU **) clabLE };
const MENU cmnuLN = { citemLN, 0x80, clenLN, (MENU **) clabLN };
const MENU cmnuLP = { citemLP, 0x80, clenLP, (MENU **) clabLP };
const MENU cmnuLS = { citemLS, 0x80, clenLS, (MENU **) clabLS };
const MENU cmnuPR = { citemPR, 0x80, clenPR, (MENU **) clabPR };
const MENU cmnuPU = { citemPU, 0x80, clenPU, (MENU **) clabPU };
const MENU cmnuTV = { citemTV, 0x80, clenTV, (MENU **) clabTV };
const MENU cmnuTL = { citemTL, 0x80, clenTL, (MENU **) clabTL };
const MENU cmnuTM = { citemTM, 0x80, clenTM, (MENU **) clabTM };
const MENU cmnuTR = { citemTR, 0x80, clenTR, (MENU **) clabTR };
const char * const citemCMOS[] = { "#$","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��"};
const MENU * const clabCMOS[] = { 0,&cmnuID,&cmnuIE,&cmnuIR,&cmnuKT,&cmnuLA,&cmnuLE,&cmnuLN,&cmnuLP,&cmnuLS,&cmnuPR,&cmnuPU,&cmnuTV,&cmnuTL,&cmnuTM,&cmnuTR};
const MENU mnuCMOS = { citemCMOS, 0, clenCMOS, clabCMOS  };
